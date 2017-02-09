#pragma once
#include "CSocket.h"
#include <iostream>
#include <comutil.h>
#include <sstream>
#include <msclr\marshal.h>
#include <msclr\marshal_windows.h>
#include <msclr\marshal_cppstd.h>
#include <msclr\marshal_atl.h>
namespace ConvertClass{
	using namespace std;
	using namespace System;
	class CConvert{
	private:
		int nothing;
	protected:
	public:
		static LPSTR toLPSTR(LPTSTR InWchar);
		static LPSTR toLPSTR(string _input);
		static void toLPSTR(int _input, char*_output);
		static char iTostr(int _input);
		static LPTSTR toLPTSTR(LPSTR _input);

		static LPTSTR toLPTSTR(DWORD _input);

		static LPCWSTR toLPCWSTR(string _input);

		static String^ toStringDelegate(string _input);
		static String^ toStringDelegate(wstring _input);

		static LPCSTR toLPCSTR(string _input);

		static string toString(String^ _input);
		static string toString(wstring _input);
		static string toString(char *_input);
		static string toString(int _input);
		static wstring toWstring(String^);
		static wstring toWstring(string _input);
		static wstring toWstring(char* __input);
	};
}