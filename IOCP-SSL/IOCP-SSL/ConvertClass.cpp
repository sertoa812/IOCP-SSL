#pragma once
#include "ConvertClass.h">

#pragma comment(lib, "comsuppw.lib")
namespace ConvertClass{
	using namespace std;
	using namespace System;
	using namespace msclr::interop;
	LPSTR CConvert::toLPSTR(LPTSTR InWchar){
		char *OutStr = new char[1000];
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, InWchar, -1, NULL, 0, NULL, FALSE) + 1;
		WideCharToMultiByte(CP_OEMCP, NULL, InWchar, wcslen(InWchar), OutStr, dwNum, NULL, FALSE);
		return OutStr;
	}
	LPSTR CConvert::toLPSTR(string _input){
		stringstream sstr;
		sstr.clear();
		char * ch = new char[1024];
		sstr << _input;
		sstr >> ch;
		return ch;
	}
	void CConvert::toLPSTR(int _input, char*_output){
		if (_input % 10 >= 10){
			toLPSTR(_input / 10, _output);
		}
		char tmp = _input % 10 - '0';
		_output += tmp;
	}
	char CConvert::iTostr(int _input){
		char result = _input % 10 - '0';
		return result;
	}
	LPTSTR CConvert::toLPTSTR(LPSTR _input){
		TCHAR * outWchar = new TCHAR[1000];
		SecureZeroMemory(outWchar, 0);
		int nLen = strlen(_input) + 1;
		int nwLen = MultiByteToWideChar(CP_ACP, 0, _input, nLen, NULL, 0);

		TCHAR   *wString;
		wString = new TCHAR[nwLen];


		MultiByteToWideChar(CP_ACP, 0, _input, nLen, wString, nwLen);
		_tcscpy(outWchar, wString);//   wcscpy(outWchar,wString);


		delete[] wString;
		return outWchar;
	}

	LPTSTR CConvert::toLPTSTR(DWORD _input){
		int middle = (int)_input;
		char * middleLPSTR = new char[100];
		toLPSTR(middle, middleLPSTR);
		return toLPTSTR(middleLPSTR);
	}

	LPCWSTR CConvert::toLPCWSTR(string _input){
		size_t origsize = _input.length() + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(_input.length() - 1));
		mbstowcs_s(&convertedChars, wcstring, origsize, _input.c_str(), _TRUNCATE);
		std::wstring wstrResult(wcstring);
		free(wcstring);
		return wstrResult.c_str();
	}

	String^ CConvert::toStringDelegate(string _input){
		String^ st = marshal_as<String^>(_input);
		return st;
	}
	String^ CConvert::toStringDelegate(wstring _input){
		return marshal_as<String^>(_input);
	}
	LPCSTR CConvert::toLPCSTR(string _input){
		return _input.c_str();
	}

	string CConvert::toString(String^ _input){
		wstring wstr = marshal_as<std::wstring>(_input);
		
		std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
		setlocale(LC_ALL, "chs");
		const wchar_t* _Source = wstr.c_str();
		size_t _Dsize = 2 * wstr.size() + 1;
		char *_Dest = new char[_Dsize];
		memset(_Dest, 0, _Dsize);
		wcstombs(_Dest, _Source, _Dsize);
		std::string result =string(_Dest);
		delete[]_Dest;
		setlocale(LC_ALL, curLocale.c_str());
		cout << result;
		return result;

		return marshal_as<std::string>(_input);
	}

	string CConvert::toString(wstring _input){
		std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
		setlocale(LC_ALL, "chs");
		const wchar_t* _Source = _input.c_str();
		size_t _Dsize = 2 * _input.size() + 1;
		char *_Dest = new char[_Dsize];
		memset(_Dest, 0, _Dsize);
		wcstombs(_Dest, _Source, _Dsize);
		std::string result = string(_Dest);
		delete[]_Dest;
		setlocale(LC_ALL, curLocale.c_str());
		cout << result;
		return result;
	}

	wstring CConvert::toWstring(String^ _input){
		return marshal_as<std::wstring>(_input);
		return NULL;
	}
	wstring CConvert::toWstring(string _input){
		setlocale(LC_ALL, "chs");
		const char* _Source = _input.c_str();
		size_t _Dsize = _input.size() + 1;
		wchar_t *_Dest = new wchar_t[_Dsize];
		wmemset(_Dest, 0, _Dsize);
		mbstowcs(_Dest, _Source, _Dsize);
		std::wstring result = wstring(_Dest);
		delete[]_Dest;
		setlocale(LC_ALL, "C");
		return result;
	}
	wstring CConvert::toWstring(char* __input){
		string _input = string(__input);
		setlocale(LC_ALL, "chs");
		const char* _Source = _input.c_str();
		size_t _Dsize = _input.size() + 1;
		wchar_t *_Dest = new wchar_t[_Dsize];
		wmemset(_Dest, 0, _Dsize);
		mbstowcs(_Dest, _Source, _Dsize);
		std::wstring result = wstring(_Dest);
		delete[]_Dest;
		setlocale(LC_ALL, "C");
		return result;
	}

	string CConvert::toString(char *_input){
		string output;
		strcpy(_input, output.c_str());
		return output;
	}
	string CConvert::toString(int _input){
		std::stringstream ss;
		std::string str;
		ss << _input;
		ss >> str;
		return str;
	}
}