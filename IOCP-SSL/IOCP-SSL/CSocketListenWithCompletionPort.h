#pragma once
#include "CSocketGet.h"
#include "CSocketInfoQueue.h"
#define DATA_BUFSIZE 8192
namespace NSCSocket{
	//用于传送递绑定的Socket
	typedef struct LThreadParameter{
		SOCKET bindSocket;
	}LThreadParameter;
	//用于保存与客户端进行通信的套接字
	typedef struct {
		SOCKET Socket;
		string Addr;
		DWORD Flags;
		DWORD RecvBytes;
		int type;//服务端,客户端
	}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;
	//用于保存I/O操作（这里指发送和接收数据）的相关数据
	typedef struct {
		OVERLAPPED Overlapped;
		WSABUF DataBuf;
		CHAR Buffer[DATA_BUFSIZE];
		DWORD ByteSend;
		DWORD ByteRecv;
	}PER_IO_DATA, *LPPER_IO_DATA;

	class CSocketListenWithCompletionPort :public CSocketGet{
	public:
		CSocketListenWithCompletionPort(){}
		static HANDLE hCompletionPort;
		DWORD iCompletionPort(SOCKET s);
		static char* getAddrIn(sockaddr_in* _in);
		static DWORD WINAPI ListenThread(LPVOID);
		static DWORD WINAPI ServerWorkerThread(LPVOID);
		static int waitTime();
		static void initPerIoData(LPPER_IO_DATA);
		static void initPerHandleData(LPPER_HANDLE_DATA mPerHandleData,string _addr, SOCKET socket);
		static void bindCompletionPort(SOCKET&,string);
	};
}