#pragma once
#include "CSocket.h"
#pragma comment(lib,"WS2_32.lib") 
#pragma comment(lib,"User32.lib")
namespace NSCSocket{
	string CSocket::getAddress(){
		return localAddress;
	}
	u_short CSocket::getUPort(){
		return localUnicastPort;
	}
	u_short CSocket::getBPort(){
		return localBroadcastPort;
	}

	void CSocket::setAddress(string _address){
		this->localAddress = _address;
	}
	void CSocket::setUPort(u_short _uport){
		this->localUnicastPort = _uport;
	}

	void CSocket::setBPort(u_short _bport){
		this->localBroadcastPort = _bport;
	}

	void CSocket::showError(LPTSTR errorMessage){
		DWORD _errorid = GetLastError();
		wstring weM = wstring(errorMessage);
		int _eid = _errorid;
		wchar_t _rem[1];
		wmemset(_rem, (wchar_t)L"\0", 1);
		wsprintf(_rem, L"%d", _eid);
		weM += L"\nGetLastError:";
		weM += _rem;
		MessageBox(NULL, weM.c_str(), L"ERROR", MB_OK);
	}
	void CSocket::showWSAError(LPTSTR errorMessage){
		DWORD _errorid = WSAGetLastError();
		int asdf = 0;
		MessageBox(NULL, errorMessage, L"WSAERROR", MB_OK);
	}
}