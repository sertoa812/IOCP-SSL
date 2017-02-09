#pragma once
#include "CSocketFactoryIOCP.h"
#include <iostream>
namespace NSCSocket{
	using namespace NSCSocketInfoQueue;
	int main(){
		CSocketFactoryIOCP mIOCP = CSocketFactoryIOCP();
		cout << "Success";
		return 0;
	}
}