#pragma once 
#include "CSocketSSL.h"
#pragma comment(lib,"WS2_32.lib") 
#pragma comment(lib,"User32.lib")
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"libeay32.lib")
#pragma comment(lib,"ssleay32.lib")
namespace NSCSocket{
	CSocketSSL::CSocketSSL(){
		iSSLServer();
		iSSLClient();
	}
	void CSocketSSL::SSLError(){
		unsigned long ulErr = ERR_get_error(); // 获取错误号
		char szErrMsg[1024] = { 0 };
		char *pTmp = NULL;
		ERR_load_crypto_strings();
		pTmp = ERR_error_string(ulErr, szErrMsg); // 格式：error:errId:库:函数:原因
		const char* re = ERR_reason_error_string(ulErr);
		if (re != NULL){
			string sre = string(pTmp);
			DWORD _errorid = GetLastError();
			wstring weM = wstring((LPTSTR)ConvertClass::CConvert::toWstring(sre).c_str());
			int _eid = _errorid;
			wchar_t _rem[1];
			wmemset(_rem, (wchar_t)L"\0", 1);
			wsprintf(_rem, L"%d", _eid);
			weM += L"\nGetLastError:";
			weM += _rem;
			MessageBox(NULL, weM.c_str(), L"ERROR", MB_OK);
		}
	}

	DWORD CSocketSSL::iSSLServer(){
		//初始化SSL环境
		SSL_library_init();//SSL库的初始化
		OpenSSL_add_ssl_algorithms();//对SSL进行初始化
		SSL_load_error_strings();//加载SSL错误信息
		ERR_load_BIO_strings();
		const SSL_METHOD* method = TLSv1_server_method();//服务器创建本次会话所使用的方式，TLSv1
		sslCTX = SSL_CTX_new(method);//申请SSL会话的环境CTX（使用不同的协议进行会话，其环境也是不同的）
		CHK_NULL(sslCTX);

		//加载服务器端证书

		//指定证书验证方式
		SSL_CTX_set_verify(sslCTX,SSL_VERIFY_PEER,NULL);//第三个参数是验证处理验证的回调函数，无特殊请求时为NULL
		//加载CA证书
		//第二个参数为CA证书文件名，第三个参数是CA证书的路径，可在第二个参数中给出
		int tmpre = SSL_CTX_load_verify_locations(sslCTX,CACertFile,NULL);
		//为SSL会话加载用户证书
		if (SSL_CTX_use_certificate_file(sslCTX, ServerCertFile, X509_FILETYPE_PEM) <= 0){
			this->SSLError();
		}
		//加载私钥文件
		if (SSL_CTX_use_PrivateKey_file(sslCTX, ServerKeyFile, SSL_FILETYPE_PEM) <= 0){
			this->SSLError();
		}
		//加载证书与私钥后，便可验证证书与私钥是否相符
		if (!SSL_CTX_check_private_key(sslCTX)){
			this->SSLError();
		}
		//设置加密列表
		SSL_CTX_set_cipher_list(sslCTX,"RC4-MD5");
	}
	DWORD CSocketSSL::iSSLClient(){
		//初始化SSL环境
		SSL_library_init();//SSL库的初始化
		OpenSSL_add_ssl_algorithms();//对SSL进行初始化
		SSL_load_error_strings();//加载SSL错误信息
		const SSL_METHOD* method = TLSv1_client_method();//服务器创建本次会话所使用的方式，TLSv1
		cslCTX = SSL_CTX_new(method);//申请SSL会话的环境CTX（使用不同的协议进行会话，其环境也是不同的）
		CHK_NULL(sslCTX);

		//加载服务器端证书

		//指定证书验证方式
		SSL_CTX_set_verify(cslCTX, SSL_VERIFY_PEER, NULL);//第三个参数是验证处理验证的回调函数，无特殊请求时为NULL
		//加载CA证书
		//第二个参数为CA证书文件名，第三个参数是CA证书的路径，可在第二个参数中给出
		int tmpre = SSL_CTX_load_verify_locations(cslCTX, CACertFile, NULL);
		//为SSL会话加载用户证书
		if (SSL_CTX_use_certificate_file(cslCTX, ClientCertFile, X509_FILETYPE_PEM) <= 0){
			this->SSLError();
		}
		//加载私钥文件
		if (SSL_CTX_use_PrivateKey_file(cslCTX, ClientKeyFile, SSL_FILETYPE_PEM) <= 0){
			this->SSLError();
		}
		//加载证书与私钥后，便可验证证书与私钥是否相符
		if (!SSL_CTX_check_private_key(cslCTX)){
			this->SSLError();
		}
		//设置加密列表
		SSL_CTX_set_cipher_list(cslCTX, "RC4-MD5");
		return 0;
	}

	void CSocketSSL::initSslData(SSL_DATA& mData){
		if (mData.type == CLIENTFLAG){
			mData.ssl = SSL_new(cslCTX);
			SSL_set_connect_state(mData.ssl);
		}
		else{
			mData.ssl = SSL_new(sslCTX);
			SSL_set_accept_state(mData.ssl);
		}

		mData.bio[INPUT] = BIO_new(BIO_s_mem());
		mData.bio[OUTPUT] = BIO_new(BIO_s_mem());
		SSL_set_bio(mData.ssl, mData.bio[INPUT], mData.bio[OUTPUT]);
	}

	string CSocketSSL::SendHandShake(SSL_DATA& mData){
		SSL_do_handshake(mData.ssl);
		int pending = BIO_ctrl_pending(mData.bio[OUTPUT]);
		char buf[4096];
		string ssre;
		if (pending > 0){
			int sre = BIO_read(mData.bio[OUTPUT], buf, sizeof(buf));
			ssre = c2s(buf, sre);	
		}
		return ssre;
	}
	string CSocketSSL::RecvHandShake(SSL_DATA& mData, string info){
		int size;
		char buf[DEFAULT_SIZE];
		s2c(OtherComponents::split(info, "|"), buf, size);
		SSLError();
		int wsize = BIO_write(mData.bio[INPUT], buf, size);
		SSLError();
		int ipending = BIO_ctrl_pending(mData.bio[INPUT]);
		SSLError();
		if (SSL_is_init_finished(mData.ssl)){
			newinfo = SendHandShake(mData);
		}
		else newinfo = "1";
		return newinfo;
	}

	string CSocketSSL::InfoPack(string _sinput,SSL_DATA& mData){
		char buf[DEFAULT_SIZE];
		SSL_write(mData.ssl, _sinput.c_str(), _sinput.length());
		int len = BIO_read(mData.bio[OUTPUT], buf, sizeof(buf));
		string sendre = c2s(buf, len);
		return sendre;
	}
	string CSocketSSL::InfoUnPack(string _input,SSL_DATA& mData ){
		char enbuf[DEFAULT_SIZE];
		char buf[DEFAULT_SIZE];
		int size;
		s2c(OtherComponents::split(_input, "|"), enbuf, size);
		int wsize = BIO_write(mData.bio[INPUT], enbuf, size);
		SSL_read(mData.ssl, buf, wsize);
		string re = string(buf);
		return re;
	}

	void CSocketSSL::s2c(vector<string> _input, char *st, int& size){
		size = _input.size() - 1;
		for (int i = 0; i < size + 1; i++){
			string tmp = _input[i];
			int itmp = atoi(tmp.c_str());
			st[i] = itmp;
		}
	}
	string CSocketSSL::c2s(char* st, int len){
		int itmp[10000];
		string sresult;
		for (int i = 0; i < len; i++){
			itmp[i] = st[i];
			char temp[10];
			memset(temp, '\0', 10);
			sprintf(temp, "%d", itmp[i]);
			sresult += temp;
			sresult += "|";
		}
		return sresult;
	}
	std::vector<std::string> CSocketSSL::split(std::string str, std::string pattern)
	{
		std::string::size_type pos;
		std::vector<std::string> result;
		str += pattern;//扩展字符串以方便操作
		int size = str.size();

		for (int i = 0; i<size; i++)
		{
			pos = str.find(pattern, i);
			if (pos<size)
			{
				std::string s = str.substr(i, pos - i);
				result.push_back(s);
				i = pos + pattern.size() - 1;
			}
		}
		return result;
	}
}