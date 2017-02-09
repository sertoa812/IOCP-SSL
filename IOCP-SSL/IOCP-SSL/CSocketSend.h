#pragma once
#include "CSocket.h"
namespace NSCSocket{
	class CSocketSend :public CSocket{
	public:
		CSocketSend(){}
		~CSocketSend(){}
		int sendInfo( LPCSTR _address = "127.0.0.1", string _info = "init");
	};
}
