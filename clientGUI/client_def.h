#include "def_header.h"
#include <iostream>
#include <Winsock2.h>
#include <windows.h>
#pragma comment(lib,"Ws2_32.lib")
#define DEBUG
#define MAXBLK 65535
#define MAXRD 64
namespace tftp_client
{
	typedef struct RVFILEBUFF
	{
		char *blk[MAXBLK*MAXRD+1];
	}RVFILEBUFF;
	struct tranState
	{
		unsigned long tSize = 0; //目前已传输大小
		float lossRate = 0.0;	 //丢包率
		bool over = false;		 //传输是否结束:用于感知异常中断等
	};
	extern tranState* pkt;
	extern bool tsinit(tranState* buff);
	extern bool socketinit(int& err_code);
	extern bool createSocket(std::string sip, USHORT sport, sockaddr_in& saddr, SOCKET& csock, long timeout, int& err_code);
	extern bool sendFile(SOCKET& csock, sockaddr_in& saddr, std::string& filePath, int fileType, int model, long timeout, 
						 int tryCNT, int& err_code);
	extern bool recvFile(SOCKET& csock, sockaddr_in& saddr, std::string& fileName, int fileType, int model, long timeout,
					     int tryCNT, std::string& fileRecvPath, int& err_code);
	extern void path2Name(const char* path, char* name);
	extern int getTypeByPath(std::string& filePath);
};// namepsace tftp_client