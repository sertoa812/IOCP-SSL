#pragma once
#include "CSocketSend.h"
namespace NSCSocket{
	int CSocketSend::sendInfo(LPCSTR _address, string _info){
		//在信息中添加自己的IP地址
		SOCKADDR_IN addr;
		WSABUF DataBuf;
		DWORD SendBytes;
		//分配并设置socket句柄结构体

		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(_address);
		addr.sin_port = htons(this->localUnicastPort);

		SOCKET sendSocket = socket(AF_INET,//IPv4
			SOCK_STREAM,
			IPPROTO_TCP
			);

		if (connect(sendSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR){
			this->showError(L"connect failed");
		}

		if (sendSocket == NULL){
			this->showError(L"not connected");
			return -1;
		}

		int sendResult = send((sendSocket), _info.c_str(), _info.length(), 0);

		closesocket(sendSocket);
		return 0;
	}
}