#pragma once
#include "CSocketFactoryIOCP.h"
#pragma comment(lib,"WS2_32.lib") 
#pragma comment(lib,"User32.lib")
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"libeay32.lib")
#pragma comment(lib,"ssleay32.lib")
namespace NSCSocket{
	using namespace NSCSocketInfoQueue;
	using namespace ConvertClass;

	CSocketSSL CSocketFactoryIOCP::mSocketSSL;
	HANDLE CSocketFactoryIOCP::mWorkThread;
	map<string, Iocp_Data> CSocketFactoryIOCP::RecvHandle;	//接收的保存每一对连接
	map<string, Iocp_Data> CSocketFactoryIOCP::SendHandle;	//发送的保存每一对连接
	CSocketSend CSocketFactoryIOCP::mSocketSend;
	CSocketListenWithCompletionPort CSocketFactoryIOCP::mIOCP;

	CSocketFactoryIOCP::CSocketFactoryIOCP(){
		mInit.iAddr();
		localAddress = mInit.getAddress();

		mSocketSSL = CSocketSSL();
		mSocketSend = CSocketSend();

		this->WorkListen();

		//建立消息处理线程
		LPVOID lpvoid = NULL;
		mWorkThread = CreateThread(NULL, 0, this->GetInfo, lpvoid, 0, NULL);

		this->StartInfo();
	}
	int CSocketFactoryIOCP::WorkListen(){
		SOCKET bindSocket = mInit.getBSocket();
		mIOCP.iCompletionPort(bindSocket);//初始化完成端口，建立监听线程和工作线程

		return 0;
	}
	void CSocketFactoryIOCP::StartInfo(){
		Iocp_Data mIocp_Data = Iocp_Data();
		mIocp_Data.mSSL_DATA.type = CLIENTFLAG;
		mSocketSSL.initSslData(mIocp_Data.mSSL_DATA);

		mIocp_Data.mPerHandleData.Addr = localAddress;
		mIocp_Data.mPerHandleData.type = CLIENTFLAG;

		string tmpInfo = mSocketSSL.SendHandShake(mIocp_Data.mSSL_DATA);
		mSocketSend.sendInfo(localAddress.c_str(), tmpInfo);

		SendHandle.insert(map<string, Iocp_Data>::value_type(localAddress, mIocp_Data));
	}
	DWORD WINAPI CSocketFactoryIOCP::GetInfo(LPVOID _p){
		while (1){
			if (!MQueue::CreateInstance()->Empty()){
				PASS_DATA _data = MQueue::CreateInstance()->Pop();

				map<string, Iocp_Data>* pHandle = (_data.type == SERVERFLAG ? &RecvHandle : &SendHandle);

				if (pHandle->find(_data.addr) == RecvHandle.end()){//没有找到，新建数据
					Iocp_Data mIocp_Data = Iocp_Data();

					mIocp_Data.mSSL_DATA.type = SERVERFLAG;
					mSocketSSL.initSslData(mIocp_Data.mSSL_DATA);

					mIocp_Data.mPerHandleData.type = SERVERFLAG;
					mIocp_Data.mPerHandleData.Addr = _data.addr;

					pHandle->insert(map<string, Iocp_Data>::value_type(_data.addr, mIocp_Data));
				}

				string tmpinfo = mSocketSSL.RecvHandShake((*pHandle)[_data.addr].mSSL_DATA, _data.info);
				if (tmpinfo == "1"){
					//握手完成
					mSocketSend.showError(L"complete");
				}

				//继续握手
				else mSocketSend.sendInfo(_data.addr.c_str(), tmpinfo);
			}
		}

		return 0;
	}
}