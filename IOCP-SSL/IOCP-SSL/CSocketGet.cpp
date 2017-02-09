#pragma once
#include "CSocketGet.h"
#include "Components.h"
#include "ConvertClass.h"
namespace NSCSocket{
	using namespace ConvertClass;
	HANDLE CSocketGet::hGetInformationEvent;
	HANDLE CSocketGet::hGetAddressEvent;
	HANDLE CSocketGet::hUpdateEvent;

	string CSocketGet::SInformation;
	string CSocketGet::SAddress;

	CSocketGet::CSocketGet(){
		//初始化同步信号量
		hGetInformationEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		hGetAddressEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		hUpdateEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		SetEvent(hUpdateEvent);
	}
	HANDLE CSocketGet::getInformationEvent(){
		return this->hGetInformationEvent;
	}
	HANDLE CSocketGet::getAddressInfoEvent(){
		return this->hGetAddressEvent;
	}
	HANDLE CSocketGet::getUpdateEvent(){
		return this->hUpdateEvent;
	}

	void CSocketGet::setInformationEvent(){
		SetEvent(this->hGetInformationEvent);
	}
	void CSocketGet::setAddressInforEvent(){
		SetEvent(this->hGetAddressEvent);
	}

	string CSocketGet::gInformation(){
		return SInformation;
	}
	string CSocketGet::gAddress(){
		WaitForSingleObject(hGetAddressEvent, INFINITE);
		infoRecord[SAddress] = L"";
		ResetEvent(hGetAddressEvent);
		return SAddress;
	}
	BOOL CSocketGet::ifGetAddress(char * addr){
		for (int i = 0; i < addressCount; i++){
			if (addresslist[i] == addr){
				return TRUE;
			}
		}
		return FALSE;
	}
}