#pragma once
#include "CSocketInfoQueue.h"
namespace NSCSocketInfoQueue{
	using namespace NSCSocket;
	using namespace std;

	CRITICAL_SECTION MQueue::oSection = CRITICAL_SECTION();
	queue<PASS_DATA> MQueue::mQueue;
	MQueue::MQueue(){
		::InitializeCriticalSection(&oSection);
	}

	MQueue::~MQueue(){
		::DeleteCriticalSection(&oSection);
	}

	void MQueue::Push(PASS_DATA t){
		::EnterCriticalSection(&oSection);
		mQueue.push(t);
		::LeaveCriticalSection(&oSection);
	}

	PASS_DATA MQueue::Pop(){
		::EnterCriticalSection(&oSection);
		PASS_DATA t = mQueue.front();
		mQueue.pop();
		::LeaveCriticalSection(&oSection);
		return t;
	}

	bool MQueue::Empty(){
		if (mQueue.empty())
			return true;
		else return false;
	}
}