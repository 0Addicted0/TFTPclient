#define BUFF_SIZE 1024
#define SERVER_PORT 69
#define SYNC_MODEL 0
#define ASYNC_MODEL 1

#define _DATA_HEADER_LEN 4
#define _RRQ 1
#define _WRQ 2
#define _DATA 3
#define _ACK 4
#define _ERROR 5
#define _ENDL 0
#define STR_FILE_TYPE 1
#define BIN_FILE_TYPE 2
#define STR_FILE "netascii"
#define BIN_FILE "octet"

#define ERR_UNKNOWN 10
#define ERR_UNKNOWN_FILE 11
#define ERR_ACCESS_VIOLATION 12
#define ERR_DISK_FULL 13
#define ERR_ILLOP 14
#define ERR_UNKNOWN_TID 15
#define ERR_FILE_EXIST 16
#define ERR_UNKNOWN_USER 17

#define ERR_DEST_IP  20//invalid ipv4
#define ERR_SOCK 21//SOCKET ERROR
#define ERR_FILETYPE 22//wrong fileType
#define ERR_SENDTO 23//fail to sendto
#define ERR_BUFF 24//fail to access buffer
#define ERR_NO_ANS 25// NO ANSWER
#define ERR_READFILE_SHORT 26//读取文件长度小于预期
#define ERR_TIMEOUT 27
#define ERR_WRTIEFILE_SHORT 28
#define ERR_MSG 29
#define ERR_CFILEF 30//createFile failed

#define ERR_QT_WFILE 40
#define ERR_QT_WFOLDER 41
#define ERR_QT_WREMOTEFILE 42
#define ERR_QT_WSHAREDM 43

#define _PULL 0
#define _PUSH 1
#define _RETRY 2

#pragma once
