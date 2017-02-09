#pragma once
#include "CSocket.h"
#include <iphlpapi.h>
#include <WS2tcpip.h>
namespace NSCSocket{
	class CSocketInit :virtual public CSocket{
		private:
			SOCKET uSocket;
			SOCKET bSocket;
		public :
			CSocketInit(u_short uport , u_short bport);
			DWORD iWSA();
			DWORD iAddr();
			DWORD iBind();

			SOCKET getUSocket();
			SOCKET getBSocket();
	};
}