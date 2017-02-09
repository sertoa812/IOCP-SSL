#pragma once
#include "ConvertClass.h"
#include "CSocketListenWithCompletionPort.h"
#include "CSocketSend.h"
#include "CSocketInit.h"
#include "CSocketSSL.h"

namespace NSCSocket{
	using namespace NSCSocketInfoQueue;
	using namespace ConvertClass;
	using namespace std;

	typedef struct{
		int sslEnable;	//会话是否加密
		SSL_DATA mSSL_DATA;	//每个会话的ssl信息
		PER_HANDLE_DATA mPerHandleData;	//Socket信息
	}Iocp_Data;

	class CSocketFactoryIOCP{
	private:
		static CSocketSSL mSocketSSL;
		static HANDLE mWorkThread;
	public:
		static map<string, Iocp_Data> RecvHandle;	//接收的保存每一对连接
		static map<string, Iocp_Data> SendHandle;	//发送的保存每一对连接
		static CSocketSend mSocketSend;
		static CSocketListenWithCompletionPort mIOCP;
		CSocketInit mInit = CSocketInit((u_short)10000,(u_short)10001);

		string localAddress;

		CSocketFactoryIOCP();

		int WorkListen();
		void StartInfo();
		static DWORD WINAPI GetInfo(LPVOID);
	};
}
