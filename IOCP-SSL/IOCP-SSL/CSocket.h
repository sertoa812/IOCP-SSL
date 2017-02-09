#pragma once
#include "winsock2.h"
#include "Windows.h"
#include "Winuser.h"
#include <string>
#include <queue>
#include <memory>
namespace NSCSocket{
	using namespace std;
	using namespace System;

	class CSocket{
		public:
			WORD mRequested;
			WSADATA wsaData;

			string localAddress;

			u_short localUnicastPort;
			u_short localBroadcastPort;

			string getAddress();
			void setAddress(string _address);

			u_short getUPort();
			void setUPort(u_short _uport);

			void setBPort(u_short _bport);
			u_short getBPort();

			static void showError(LPTSTR errorMessage);
			static void showWSAError(LPTSTR errorMessage);
	};
}