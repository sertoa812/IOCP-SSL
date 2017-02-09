#pragma once
#include "CSocket.h"
#include "ConvertClass.h"
#include "Components.h"
//ssl相关
#include "openssl/ssl.h"
#include "openssl/x509.h"
#include "openssl/rand.h"
#include "openssl/err.h"

#define ServerCertFile "server.crt"
#define ServerKeyFile "server.key"
#define CACertFile "ca.crt"
#define ClientCertFile "client.crt"
#define ClientKeyFile "client.key"
//ssl的bio数组标示
#define INPUT 0
#define OUTPUT 1
//ssl数组用于标识客户端和服务端
#define CLIENTFLAG 0
#define SERVERFLAG 1
//ssl握手信息
#define HANDNOTSUCCESS 0
#define HANDSUCCESS 1
#define HANDERROR 2
//页面大小
#define DEFAULT_SIZE 8192
//ssl处理的宏
#define CHK_NULL(param) if((param)==NULL){ERR_print_errors_fp(stdout);getchar();exit(1);}
#define CHK_ERR(err,msg) if((err)==1){perror(msg);getchar();exit(1);}
#define CHK_SSL(err) if((err)==1){ERR_print_errors_fp(stderr);getchar();exit(2);}
namespace NSCSocket{

	typedef struct{
		int type;	//标志是客户端还是服务器端
		int complete;	//标志握手是否完成
		SSL* ssl;
		BIO* bio[2];
	}SSL_DATA,*LPSSL_DATA;

	class CSocketSSL{
		SSL_CTX* sslCTX;
		SSL_CTX* cslCTX;

		string newinfo;
	public:
		CSocketSSL();//构造函数加载服务器端和客户端证书

		void SSLError();	//错误信息

		DWORD iSSLServer();	//作为服务器端加载证书
		DWORD iSSLClient();	//作为客户端加载证书

		void initSslData(SSL_DATA&);	//初始化SSL_DATA

		string SendHandShake(SSL_DATA&);	//握手判断
		string RecvHandShake(SSL_DATA&,string);

		void bioInput(SSL_DATA&, char*);//读入BIO内存中
		char* bioOutput();//从BIO内从中读出

		string InfoPack(string,SSL_DATA&);	//数据加密
		string InfoUnPack(string,SSL_DATA&);	//数据解密

		void s2c(vector<string> _input, char *st, int& size);
		string c2s(char* st, int len);
		std::vector<std::string> split(std::string str, std::string pattern);
	};
}