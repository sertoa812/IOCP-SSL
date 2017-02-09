#pragma once
#include "CSocketListenWithCompletionPort.h"
#pragma comment(lib,"WS2_32.lib") 
#pragma comment(lib,"User32.lib")
#pragma comment(lib,"Iphlpapi.lib")
//ssl数组标示
namespace NSCSocket{
	using namespace NSCSocketInfoQueue;
	HANDLE CSocketListenWithCompletionPort::hCompletionPort;
	DWORD CSocketListenWithCompletionPort::iCompletionPort(SOCKET s){
		//获得消息队列的指针

		//监听线程
		hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

		//根据CPU数量建立线程
		SYSTEM_INFO mSystemInfo;
		GetSystemInfo(&mSystemInfo);
		for (int i = 0; i < mSystemInfo.dwNumberOfProcessors * 2; i++){
			HANDLE ThreadHandle;
			DWORD ThreadID;
			if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, hCompletionPort, 0, &ThreadID)) == NULL){
				this->showError(L"Init CompletionProt Thread failed");
				return -1;
			}
			CloseHandle(ThreadHandle);
		}

		//建立监听线程
		LThreadParameter* listenParameter;
		listenParameter = new LThreadParameter;
		listenParameter->bindSocket = s;
		HANDLE ListenHANDLE = CreateThread(NULL,0,ListenThread,listenParameter,0,NULL);

		return 0;
	}
	DWORD WINAPI CSocketListenWithCompletionPort::ListenThread(LPVOID _p){
		LThreadParameter* lp = (LThreadParameter*)_p;
		SOCKET bindSocket = (SOCKET)lp->bindSocket;
		SOCKET Acceptsocket;

		int re = listen(bindSocket, 1);
		if (re == SOCKET_ERROR){
			return -1;
		}
		//监听这个端口打开，在这里循环，一有socket脸上，WSAAccept就创建一个socket，这个socket已经和完成端口相关联
		while (1){
			//等待客户连接
			//为每一个连接的客户端建立一个单IO数据和单IO句柄，直至socket断开时释放
			LPPER_HANDLE_DATA mPerHandleData;
			LPPER_IO_DATA mPerIoData;
			DWORD Flags;
			DWORD RecvBytes;
			sockaddr_in inAddr;
			char * strAddr;
			int addrsize = sizeof(inAddr);
			Acceptsocket = WSAAccept(bindSocket, (sockaddr*)&inAddr, &addrsize, NULL, 0);

			strAddr = getAddrIn(&inAddr);
			if (Acceptsocket == SOCKET_ERROR){
				closesocket(Acceptsocket);
				showError(L"WSAAcept failed");
				return -1;
			}

			initPerHandleData(mPerHandleData, string(strAddr), Acceptsocket);
			initPerIoData(mPerIoData);

			//将与客户端进行通信的套接字socket与完成端口hCompletion相关联
			if (CreateIoCompletionPort((HANDLE)Acceptsocket, hCompletionPort, (ULONG_PTR)mPerHandleData, 0) == NULL){
				showError(L"createIOCompletionPort failed");
				return -1;
			}
			Flags = 0;

			//接收数据，放到PerIoData中
			//perIoData又通过工作线程中的ServerWorkerThread函数取出
			//使用AcceptEx的方式
			if (WSARecv(Acceptsocket, &(mPerIoData->DataBuf), 1, &RecvBytes, &Flags, &(mPerIoData->Overlapped), NULL) == SOCKET_ERROR){
				if (WSAGetLastError() != ERROR_IO_PENDING){
					showError(L"WSARecv failed");
					return -1;
				}
			}
		}
	}
	DWORD WINAPI CSocketListenWithCompletionPort::ServerWorkerThread(LPVOID CompletionPortID)
	{
		HANDLE CompletionPort = (HANDLE)CompletionPortID;
		DWORD BytesTransferred;
		LPOVERLAPPED Overlapped;
		LPPER_HANDLE_DATA PerHandleData;
		LPPER_IO_DATA PerIoData;
		DWORD SendBytes, RecvBytes;
		DWORD Flags;

		while (TRUE)
		{

			if (GetQueuedCompletionStatus(CompletionPort, &BytesTransferred, (PULONG_PTR)&PerHandleData, (LPOVERLAPPED *)&PerIoData, INFINITE) == 0)
			{
				showError(L"GetQueuedCompletionStatus Failed");
				return 0;
			}

			//赋值给SInformation
			string addr = PerHandleData->Addr;
			string wmessage = string(PerIoData->DataBuf.buf);
				
			while (1){
				//存入消息队列中
				PASS_DATA mPassData = PASS_DATA();
				mPassData.addr = PerHandleData->Addr;
				mPassData.info = wmessage;
				mPassData.type = PerHandleData->type;
				MQueue::CreateInstance()->Push(mPassData);

				closesocket(PerHandleData->Socket);
				initPerIoData(PerIoData);
				WSARecv(PerHandleData->Socket, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags, &(PerIoData->Overlapped), NULL);
				GlobalFree(PerHandleData);
				GlobalFree(PerIoData);
			}
		}
	}
	int CSocketListenWithCompletionPort::waitTime(){
		int a = 300;
		int b = 800;
		return (rand() % (b - a)) + a;
	}
	void CSocketListenWithCompletionPort::initPerIoData(LPPER_IO_DATA m){
		if ((m = (LPPER_IO_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_DATA))) == NULL){
			showError(L"mPerIoData Global failed");
			return;
		}
		ZeroMemory(&(m->Overlapped), sizeof(OVERLAPPED));
		m->ByteSend = 0;
		m->ByteRecv = 0;
		m->DataBuf.len = DATA_BUFSIZE;
		m->DataBuf.buf = m->Buffer;
	}
	void CSocketListenWithCompletionPort::initPerHandleData(LPPER_HANDLE_DATA mPerHandleData, string _addr, SOCKET socket){
		if ((mPerHandleData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA))) == NULL){
			showError(L"mPerHandleData Global failed");
			return;
		}
		SecureZeroMemory(mPerHandleData, sizeof(PER_HANDLE_DATA));
		mPerHandleData->Socket = socket;
		mPerHandleData->Addr = _addr;
	}
	char* CSocketListenWithCompletionPort::getAddrIn(sockaddr_in* _in){
		return inet_ntoa(_in->sin_addr);
	}
	void CSocketListenWithCompletionPort::bindCompletionPort(SOCKET& mSocket,string addr){
		LPPER_HANDLE_DATA mPerHandleData;
		LPPER_IO_DATA mPerIoData;
		DWORD Flags;
		DWORD RecvBytes;

		initPerHandleData(mPerHandleData, addr, mSocket);
		initPerIoData(mPerIoData);

		//将与客户端进行通信的套接字socket与完成端口hCompletion相关联
		if (CreateIoCompletionPort((HANDLE)mSocket, hCompletionPort, (ULONG_PTR)mPerHandleData, 0) == NULL){
			showError(L"createIOCompletionPort failed");
		}
		Flags = 0;
		//接收数据，放到PerIoData中
		//perIoData又通过工作线程中的ServerWorkerThread函数取出
		//使用AcceptEx的方式
		if (WSARecv(mSocket, &(mPerIoData->DataBuf), 1, &RecvBytes, &Flags, &(mPerIoData->Overlapped), NULL) == SOCKET_ERROR){
			if (WSAGetLastError() != ERROR_IO_PENDING){
				showError(L"WSARecv failed");
			}
		}
	}
}