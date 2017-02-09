#pragma once
#include "CSocket.h"
namespace NSCSocketInfoQueue{
	using namespace std;
	using namespace NSCSocket;

	//消息传递结构
	typedef struct{
		string addr;
		string info;
		int type;//服务端还是客户端
	}PASS_DATA;

	//单例类
	template <typename T>
	class Singleton{
	public:
		static T* CreateInstance();
		static void Release();
		static CRITICAL_SECTION newSection;
	protected:
		Singleton(){}
		virtual ~Singleton(){}
	private:
		Singleton(const Singleton&){}
		Singleton& operator=(const Singleton&){}
		static T* mT;
	};

	template<typename T>
	T* Singleton<T>::mT = NULL;

	template<typename T>
	CRITICAL_SECTION Singleton<T>::newSection = CRITICAL_SECTION();

	template<typename T>
	T* Singleton<T>::CreateInstance(){
		if (mT == NULL){
			::InitializeCriticalSection(&newSection);
			::EnterCriticalSection(&newSection);
			if (mT == NULL)
				mT = new T;
			::LeaveCriticalSection(&newSection);
			::DeleteCriticalSection(&newSection);
			
		}
		return mT;
	}

	template<typename T>
	void Singleton<T>::Release(){
		if (mT == NULL){
			delete mT;
			mT = NULL;
		}
	}

	//加入同步机制后的模板类
	class MQueue:public Singleton<MQueue>{

		friend class Singleton<MQueue>;
	private:
		MQueue();
		virtual ~MQueue();
		static queue<PASS_DATA> mQueue;
		static CRITICAL_SECTION oSection;
	public:
		static void Push(PASS_DATA t);
		static PASS_DATA Pop();
		static bool Empty();
	};
}
