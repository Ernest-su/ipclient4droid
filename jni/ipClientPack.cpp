#include "ipClientPack.h"


unsigned char MAC[6] = {0x00,0x50,0x56,0xc0,0x00,0x08};
const char Sentence[] = {"今天天气真好"};

char UserName[32] ;/*{"1000360117"};*/
char Password[32] ;/*{"20917"};*/

unsigned short key;
unsigned int counter;


ipClientPack::ipClientPack()
{
	memset(Data,0,300);
	current = Data;
}

ipClientPack::ipClientPack(char* userName,char* password)
{


	memcpy(UserName,userName,32);
	memcpy(Password,password,32);
	memset(Data,0,300);
	current = Data;
}

ipClientPack::ipClientPack(char* userName,char* password,unsigned short* k)
{

	memcpy(UserName,userName,32);
	memcpy(Password,password,32);
	memcpy(&key,k,sizeof(unsigned short));
	memset(Data,0,300);
	current = Data;
}

void ipClientPack::AddDate(void* buffer,int len)
{
	memcpy(current,buffer,len);
	current+=len;
}
void ipClientPack::SetPoint(int move)
{
	current += move;
}
void ipClientPack::Stuff_0xff()
{
	while(&Data[499] != current)
	{
		*current = (char)0xff;
		current++;
	}
	*current = (char)0xff;
}
void ipClientPack::Reset()
{
	memset(Data,0,500);
	current = Data;
}



int MD5(unsigned char *out, unsigned char *in)
{
	MD5_CTX mctx;
	
	MD5Init(&mctx);
	MD5Update(&mctx, in, strlen((char*)in));
	MD5Final(&mctx, out);
	
	return 0;
}
int Hex2Str(char *str,char *hex,int len)
{
	int i;
	
	for(i = 0;i < len;i++)
		sprintf(str + i*2, "%02X", (unsigned char)hex[i]);
	
	return 0;
}
int GenerateMD5String(char *str)
{
	char szStr[64];
	char szMD5[64];
	
	memset(szStr, 0x00, sizeof(szStr));
	sprintf(szStr, "%d", key - CONSTANT_2);
	
	strcat(szStr, Password);
	
	memset(szMD5, 0x00, sizeof(szMD5));
	MD5((unsigned char *)szMD5, (unsigned char *)szStr);
	memset(szStr, 0x00, sizeof(szStr));
	Hex2Str(szStr, szMD5, 0x10);
	
	memset(szMD5, 0x00, sizeof(szMD5));
	strcpy(szMD5, szStr);
	memset(szStr, 0x00, sizeof(szStr));
	strncpy(szStr, szMD5, CONSTANT_3);
	strcat(szStr, UserName);
	
	MD5((unsigned char *)szMD5, (unsigned char *)szStr);
	memset(szStr, 0x00, sizeof(szStr));
	Hex2Str(szStr, szMD5, 0x10);
	
	memcpy(str, szStr, strlen(szStr));
	return 0;
}
/********************连接服务器的函数**********************************/
void Send_0x05(ipClientPack *pack)
{
	int i;
	ipClientPackHead head;
	SendData senddata;
	char buffer[0x40];
	head.sign = MAKEWORD(0x82,0x23);
	head.number = 0x05;
	for(i=0;i<3;i++)
		head.unknow[i] = (char)rand()%0x100;
	memset(&head.unknow[3],0,5);
	pack->AddDate(&head,10);
	///////////////////////////////////////////
	memset(&senddata,0,sizeof(senddata));
	for(i=1;i<4;i++)
		senddata.Data[i] = (char)rand()%0x100;
	pack->AddDate(&senddata,8);
	///////////////////////////////////////////
	memset(buffer,0,0x9);
	buffer[0] = buffer[8] = 0x41;
	for(i=5;i<8;i++)
		buffer[i] = (char)rand()%0x100;
	pack->AddDate(&buffer,0x09);
	///////////////////////////////////////////
	senddata.DataLen = 0x01;
	senddata.DataLen<<24;
	senddata.Data[0x00] = 0x61;
	pack->AddDate(&senddata,4+1);
	///////////////////////////////////////////
	senddata.DataLen = 0x01;
	senddata.DataLen<<24;
	senddata.Data[0x00] = 0x62;
	pack->AddDate(&senddata,4+1);
	///////////////////////////////////////////
	memset(buffer,0,0x31);
	buffer[0] = 0x29;
	for(i=0x04;i<0x2a;i++)
		buffer[i] = rand()%100;
	buffer[0x2a] = 0x20;
	buffer[0x2b] = 0x27;
	pack->AddDate(&buffer,0x31);
	///////////////////////////////////////////
	pack->Stuff_0xff();
}
char* Recv_0x06(ipClientPack *pack)
{
	char *p = pack->Data;
	p += 0x02;
	char buffer[0x100] = {0};
	if (0x06 != *p)
	{
		//buffer = "连接服务器失败";
		//return;
	}
	UCHAR len;
	p += 0x23;

	char IPaddr[0x10] = {0};
	len = *p;
	p += 4;
	memcpy(buffer,p,(int)len);
//	char  buffer[100];

	//SetWindowText(GetDlgItem(m_hwnd, IDC_EDIT_MESSG), buffer);

	cout<<buffer<<endl;
	p += len;
	len = *p;
	p += 4;
	memcpy(IPaddr,p,(int)len);
//	cout<<"本机IP:"<<IPaddr<<endl;

	//SetWindowText(GetDlgItem(m_hwnd, IDC_EDIT_IP), IPaddr);
	char* ptr;
	memcpy(p,buffer,0x100);
	ptr=ptr+0x100;
	memcpy(p,IPaddr,0x10);
	return ptr;
}
/********************开放IP的函数们**********************************/
void Send_0x1F(ipClientPack *pack)
{
	int i;
	ipClientPackHead head;
	head.sign = MAKEWORD(0x82,0x23);
	head.number = 0x1f;
	memset(&head.unknow,0,8);
	pack->AddDate(&head,11);
	////////////////////////////////////////////
	SendData senddata;
	senddata.DataLen = strlen(UserName);
	senddata.DataLen<<24;	
	for (i=0;i<strlen(UserName);i++)
		senddata.Data[i] = UserName[i] - CONSTANT_1;
	pack->AddDate(&senddata,4+strlen(UserName));
	////////////////////////////////////////////
	senddata.DataLen = 0x13;
	senddata.DataLen<<24;
	//LARGE_INTEGER li;
	timeval li;
	gettimeofday(&li,NULL);
	counter = *((unsigned int *)&li);
	for(i = 0;i < 0x12;i++)/************************此处值为0x10还是0x12有待考证***/
	{
		counter = counter*0x08088405 + 1;
		senddata.Data[i] = (unsigned char)((((__int64)counter*(__int64)0x71)>>32) + 0x10);
	}
// 	senddata.Data[0x10] = 0x20;
// 	senddata.Data[0x11] = 0x00;
	senddata.Data[0x12] = 0x00;
	pack->AddDate(&senddata,4+0x13);
	///////////////////////////////////////////
	senddata.DataLen = 0x01;
	senddata.DataLen<<24;
	senddata.Data[0x00] = 0x61;
	pack->AddDate(&senddata,4+1);
	///////////////////////////////////////////
	counter = counter*0x08088405 + 1;
	unsigned int rd = counter*0x054C5638 + 0x00A98AC7;
	pack->AddDate(&rd,4);
	///////////////////////////////////////////
	pack->SetPoint(6);
	WORD dw = MAKEWORD(0xf0,0x3f);
	pack->AddDate(&dw,2);
	///////////////////////////////////////////
	senddata.DataLen = 0x01;
	senddata.DataLen<<24;
	senddata.Data[0x00] = 0x62;
	pack->AddDate(&senddata,4+1);
	///////////////////////////////////////////
	pack->Stuff_0xff();
}
void Recv_0x20(ipClientPack *pack)
{
	char *p = pack->Data;
	p += 0x0b;
	UCHAR len;
	while (p <= &pack->Data[299])
	{
		len = *p;
		p += 4;
		if (0x01 == len && 0x61 == *p)
		{
			key = *((unsigned short *)(p + 1));
			break;
		}
		p += len;
	}
}
void Send_0x21(ipClientPack *pack)
{
	int i;
	ipClientPackHead head;
	SendData senddata;
	head.sign = MAKEWORD(0x82,0x23);
	head.number = 0x21;
	memset(&head.unknow,0,8);
	pack->AddDate(&head,11);
	////////////////////////////////////////////
	senddata.DataLen = 0x13;
	senddata.DataLen<<24;
	//LARGE_INTEGER li;
	timeval li;
	gettimeofday(&li,NULL);
	counter = *((unsigned int *)&li);
	for(i = 0;i<0x12;i++)/************************此处值为0x10还是0x12有待考证***/
	{
		counter = counter*0x08088405 + 1;
		senddata.Data[i] = (unsigned char)((((__int64)counter*(__int64)0x71)>>32) + 0x10);
	}
	senddata.Data[0x12] = 0x00;
	pack->AddDate(&senddata,4+0x13);
	////////////////////////////////////////////
	senddata.DataLen = 0x1e;
	senddata.DataLen<<24;
	pack->AddDate(&senddata.DataLen,0x04);
	char md5[32];
	GenerateMD5String(md5);
	pack->AddDate(md5,0x1e);
	////////////////////////////////////////////
	senddata.DataLen = 0x11;
	senddata.DataLen<<24;
	sprintf(senddata.Data, "%02X-%02X-%02X-%02X-%02X-%02X", MAC[0], MAC[1], MAC[2], MAC[3], MAC[4], MAC[5]);
	pack->AddDate(&senddata,4+0x11);
	////////////////////////////////////////////
	counter = counter*0x08088405 + 1;
	unsigned int rd = counter*0x054C5638 + 0x00A98AC7;
	pack->AddDate(&rd,4);
	////////////////////////////////////////////
	pack->SetPoint(6);
	WORD dw = MAKEWORD(0xf0,0x3f);
	pack->AddDate(&dw,2);
	///////////////////////////////////////////
	senddata.DataLen = 0x01;
	senddata.DataLen<<24;
	senddata.Data[0x00] = 0x62;
	pack->AddDate(&senddata,4+1);
	///////////////////////////////////////////
	pack->Stuff_0xff();
}
int Recv_0x22(ipClientPack *pack)
{
	char *p = pack->Data;
	p += 0x02;
	if (0x22 != *p)
	{


		return -1;
	}
	p += 0x09;
	UCHAR len;
	while (p <= &pack->Data[299])
	{
		len = *p;
		p += 4;
		p += len;
		if (0x11 == len)
			break;
	}
	p += 0x0c;
	len = *p;
	p += 0x04;
	char Str[64] = {0};
	for (int i=0;i<len;i++,p++)
	{
		Str[i] = *p;
	}

//	cout<<Str<<endl;
	// SetWindowText(GetDlgItem(m_hwnd, IDC_EDIT_MESSG), Str);
	p = pack->Data + 0x03;
	if (0x00 != *p)
	{

		return 0;
	}
	else{

		return 1;
	}

	
}
/********************心跳包函数**********************************/
/*
 * 对于下一字段长度似乎有问题
 * void Send_0x1e(ipClientPack *pack)
{
	int i;
	ipClientPackHead head;
	unsigned short Identi;
	double number;
	SendData senddata;
	head.sign = MAKEWORD(0x82,0x23);
	head.number = 0x1e;
	Identi = key - CONSTANT_2 + CONSTANT_4; 
	memcpy(head.unknow,&Identi,2);
	memset(&head.unknow[2],0,6);
	pack->AddDate(&head,11);
	///////////////////////////////////////////////////////////
	counter = counter*0x08088405 + 1;
	number = (double)(((((__int64)counter*(__int64)0x000D9038))>>32) + 0x0001B207);
	pack->AddDate(&number,8);
	///////////////////////////////////////////////////////////
	counter = counter*0x08088405 + 1;
	number = (double)(((((__int64)counter*(__int64)0x000D9038))>>32) + 0x0001B207);
	pack->AddDate(&number,8);
	///////////////////////////////////////////////////////////
	senddata.DataLen = 0x1e;
	senddata.DataLen<<24;
	for (i=0;i<strlen(UserName);i++)
		senddata.Data[i] = UserName[i];
	pack->AddDate(&senddata,4+strlen(UserName));
	///////////////////////////////////////////////////////////
	senddata.DataLen = 0x01;
	senddata.DataLen<<24;
	senddata.Data[0x00] = 0x62;
	pack->AddDate(&senddata,4+1);
	///////////////////////////////////////////////////////////
	senddata.DataLen = 0x0a;
	senddata.DataLen<<24;
	for (i=0;i<strlen(Sentence);i++)
		senddata.Data[i] = Sentence[i];
	pack->AddDate(&senddata,4+strlen(Sentence));
	///////////////////////////////////////////////////////////
	senddata.DataLen = 0x01;
	senddata.DataLen<<24;
	senddata.Data[0x00] = 0x20;
	pack->AddDate(&senddata,4+1);
	///////////////////////////////////////////////////////////
	pack->Stuff_0xff();
}*/
/*
 * 更改如下
 * senddata.DataLen = 0x0a;
 * senddata.DataLen = 0x0c;
 */
void Send_0x1e(ipClientPack *pack)
{
	int i;
	ipClientPackHead head;
	unsigned short Identi;
	double number;
	SendData senddata;
	head.sign = MAKEWORD(0x82,0x23);
	head.number = 0x1e;
	Identi = key - CONSTANT_2 + CONSTANT_4;
	memcpy(head.unknow,&Identi,2);
	memset(&head.unknow[2],0,6);
	pack->AddDate(&head,11);
	///////////////////////////////////////////////////////////
	counter = counter*0x08088405 + 1;
	number = (double)(((((__int64)counter*(__int64)0x000D9038))>>32) + 0x0001B207);
	pack->AddDate(&number,8);
	///////////////////////////////////////////////////////////
	counter = counter*0x08088405 + 1;
	number = (double)(((((__int64)counter*(__int64)0x000D9038))>>32) + 0x0001B207);
	pack->AddDate(&number,8);
	///////////////////////////////////////////////////////////
	senddata.DataLen = 0x0a;
	senddata.DataLen<<24;
	for (i=0;i<strlen(UserName);i++)
		senddata.Data[i] = UserName[i];
	pack->AddDate(&senddata,4+strlen(UserName));
	///////////////////////////////////////////////////////////
	senddata.DataLen = 0x01;
	senddata.DataLen<<24;
	senddata.Data[0x00] = 0x62;
	pack->AddDate(&senddata,4+1);
	///////////////////////////////////////////////////////////
	senddata.DataLen = 0x0c;
	senddata.DataLen<<24;
	for (i=0;i<strlen(Sentence);i++)
		senddata.Data[i] = Sentence[i];
	pack->AddDate(&senddata,4+strlen(Sentence));
	///////////////////////////////////////////////////////////
	senddata.DataLen = 0x01;
	senddata.DataLen<<24;
	senddata.Data[0x00] = 0x20;
	pack->AddDate(&senddata,4+1);
	///////////////////////////////////////////////////////////
	pack->Stuff_0xff();
}
void Recv_0x1f(ipClientPack *pack)
{
	double flow = 0;//保存流量
	double balance = 0;//保存剩余金额
	char *p = pack->Data;
	p += 0x02;
	if (0x1f != *p)
		return;
	p += 0x09;
	memcpy(&flow,p,0x08);
	p += 0x08;
	memcpy(&balance,p,0x08);
	p += 0x08;
	char  buffer[100];
	sprintf(buffer,"当前流量: %4.2lf K",flow);
	//SetWindowText(GetDlgItem(m_hwnd, IDC_STATIC_USE), buffer);
	sprintf(buffer,"剩余金额: %f 元",balance);
   //	SetWindowText(GetDlgItem(m_hwnd, IDC_STATIC_MONEY), buffer);

}
/********************关闭IP的函数们**********************************/
void Send_0x14(ipClientPack *pack)
{
	int i;
	ipClientPackHead head;
	SendData senddata;
	head.sign = MAKEWORD(0x82,0x23);
	head.number = 0x14;
	memset(&head.unknow,0,0x08);
	pack->AddDate(&head,11);
	////////////////////////////////////////////////////
	senddata.DataLen = strlen(UserName);
	senddata.DataLen<<24;	
	for (i=0;i<strlen(UserName);i++)
		senddata.Data[i] = UserName[i] - CONSTANT_5;
	pack->AddDate(&senddata,4+strlen(UserName));
	////////////////////////////////////////////////////
	senddata.DataLen = 0x13;
	senddata.DataLen<<24;
	for(i = 0;i < 0x12;i++)
	{
		counter = counter*0x08088405 + 1;
		senddata.Data[i] = (unsigned char)((((__int64)counter*(__int64)0x71)>>32) + 0x10);
	}
	senddata.Data[0x12] = 0x00;
	pack->AddDate(&senddata,4+0x13);
	/////////////////////////////////////////////////////
	senddata.DataLen = 0x01;
	senddata.DataLen<<24;
	senddata.Data[0x00] = 0x61;
	pack->AddDate(&senddata,4+1);
	/////////////////////////////////////////////////////
	counter = counter*0x08088405 + 1;
	unsigned int rd = counter*0x054C5638 + 0x00A98AC7;
	pack->AddDate(&rd,4);
	/////////////////////////////////////////////////////
	pack->SetPoint(6);
	WORD dw = MAKEWORD(0xf0,0x3f);
	pack->AddDate(&dw,2);
	/////////////////////////////////////////////////////
	senddata.DataLen = 0x01;
	senddata.DataLen<<24;
	senddata.Data[0x00] = 0x61;
	pack->AddDate(&senddata,4+1);
	/////////////////////////////////////////////////////
	pack->Stuff_0xff();
}
void Recv_0x15(ipClientPack *pack)
{
	char *p = pack->Data;
	p += 0x02;
	if (0x15 != *p)
		return;
	p += 0x09;
	UCHAR len;
	while (p <= &pack->Data[299])
	{
		len = *p;
		p += 4;
		if (0x01 == len && 0x61 == *p)
		{
			key = *((unsigned short *)(p + 1));
			break;
		}
		p += len;
	}
}
void Send_0x16(ipClientPack *pack )
{
	int i;
	int len;
	ipClientPackHead head;
	SendData senddata;
	head.sign = MAKEWORD(0x82,0x23);
	head.number = 0x16;
	memset(&head.unknow,0,0x08);
	pack->AddDate(&head,11);
	/////////////////////////////////////////
	senddata.DataLen = 0x13;
	senddata.DataLen<<24;
	for(i = 0;i < 0x12;i++)
	{
		counter = counter*0x08088405 + 1;
		senddata.Data[i] = (unsigned char)((((__int64)counter*(__int64)0x71)>>32) + 0x10);
	}
	senddata.Data[0x12] = 0x00;
	pack->AddDate(&senddata,4+0x13);
	/////////////////////////////////////////////////////
	len = strlen(Password);
	senddata.DataLen = len;
	memcpy(senddata.Data,Password,len);
	unsigned short k = key - CONSTANT_6;
	for(i = 0;i < len;i++)
	{
		senddata.Data[i] = ((unsigned char)(k>>0x08))^senddata.Data[i];
		k = (k + (unsigned char)senddata.Data[i])*(unsigned short)0xCE6D + (unsigned short)0x58BF;
	}
	pack->AddDate(&senddata,4+len);
	/////////////////////////////////////////////////////
	senddata.DataLen = 0x11;
	senddata.DataLen<<24;
	sprintf(senddata.Data, "%02X-%02X-%02X-%02X-%02X-%02X", MAC[0], MAC[1], MAC[2], MAC[3], MAC[4], MAC[5]);
	pack->AddDate(&senddata,4+0x11);
	/////////////////////////////////////////////////////
	unsigned int rd = ((key - 0x2382)%0x2710)*2 + len;
	pack->AddDate(&rd,4);
	/////////////////////////////////////////////////////
	pack->SetPoint(6);
	WORD dw = MAKEWORD(0xf0,0x3f);
	pack->AddDate(&dw,2);
	/////////////////////////////////////////////////////
	senddata.DataLen = 0x01;
	senddata.DataLen<<24;
	senddata.Data[0x00] = 0x61;
	pack->AddDate(&senddata,4+1);
	/////////////////////////////////////////////////////
	pack->Stuff_0xff();
}
void Recv_0x17(ipClientPack *pack)
{
	char *p = pack->Data;
	p += 0x02;
	if (0x17 != *p)
		return;
	p += 0x09;
	UCHAR len;
	while (p <= &pack->Data[299])
	{
		len = *p;
		p += 4;
		p += len;
		if (0x11 == len)
			break;
	}
	p += 0x0c;
	len = *p;
	p += 0x04;
	char Str[64] = {0};
	for (int i=0;i<len;i++,p++)
	{
		Str[i] = *p;
	}
 //SetWindowText(GetDlgItem(m_hwnd, IDC_EDIT_MESSG), Str);
//	cout<<Str<<endl;
}
