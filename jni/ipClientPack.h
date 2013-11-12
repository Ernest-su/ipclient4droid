#include "MD5.h"
#include <stdio.h>
#include <iostream>
using namespace std;
#include <vector>
#include <stdlib.h>
#include <sys/time.h>
#define IPCLIENT_SERVER_IP "172.16.12.11"

#define CONSTANT_1 0x0a
#define CONSTANT_2 0x0d10
#define CONSTANT_3 0x05
#define CONSTANT_4 0x05dc
#define CONSTANT_5 0x0f
#define CONSTANT_6 0x2382

#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
typedef unsigned long       DWORD;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef char CHAR;
typedef short SHORT;
typedef long LONG;
typedef long long __int64 ;
typedef unsigned char UCHAR;
typedef double LONGLONG;

typedef union _LARGE_INTEGER {
struct {
DWORD LowPart;
LONG HighPart;
};
struct {
DWORD LowPart;
LONG HighPart;
} u;
LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

struct SendData
{
	DWORD DataLen;
	char Data[30];
};
struct ipClientPackHead
{
	WORD sign;//ipclient包的标识, 值为0x82 0x23
	BYTE number;//数据包序号，每次自加1
	char unknow[8];
};
class ipClientPack
{
public:
	char *current;
	char Data[500];
	ipClientPack();
	ipClientPack(char* userName,char* password);
	ipClientPack(char* userName,char* password,unsigned short* key);
	void AddDate(void* ,int );
	void SetPoint(int);
	void Stuff_0xff();
	void Reset();
};



int MD5(unsigned char *out, unsigned char *in);
int Hex2Str(char *str,char *hex,int len);
int GenerateMD5String(char *str);
/********************连接服务器的函数**********************************/
void Send_0x05(ipClientPack *pack);

char* Recv_0x06(ipClientPack *pack);

/********************开放IP的函数们**********************************/
void Send_0x1F(ipClientPack *pack);
void Recv_0x20(ipClientPack *pack);
void Send_0x21(ipClientPack *pack);
int Recv_0x22(ipClientPack *pack);

/********************心跳包函数**********************************/
void Send_0x1e(ipClientPack *pack);
void Recv_0x1f(ipClientPack *pack);

/********************关闭IP的函数们**********************************/
void Send_0x14(ipClientPack *pack);
void Recv_0x15(ipClientPack *pack);
void Send_0x16(ipClientPack *pack);
void Recv_0x17(ipClientPack *pack);
