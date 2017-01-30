// Password.cpp: implementation of the CPassword class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../include/Password.h"
#include "../include/GetHDSerial.h"
#include "../include/Md5A.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPassword::CPassword()
{

}

CPassword::~CPassword()
{

}

BOOL CPassword::GetSerialNo(CStringA *serialno)
{
	///////////////////////////////////////get HDD
	char* temp;
	CStringA strHDSerial;
	CGetHDSerial HDSerial;  // 创建实例   
	temp=HDSerial.GetHDSerial(); // 得到硬盘序列号
	strHDSerial.Format("%s",temp);
	strHDSerial.TrimLeft();
	strHDSerial.TrimRight();
	
	/////////////////////////////////////MD5

	CMd5A md5;
	char* cResultString;//cTempString,
	char cTempChar[33]="\0",cResultChar[33]="\0";
	int length=strHDSerial.GetLength();
	if(length==0)
	{
		AfxMessageBox(L"error");
		return false;
	}
	for(int i=0;i<length;i++)
	{
		cTempChar[i]=strHDSerial.GetAt(i);
	}
	cResultString=md5.MDString(cTempChar);
	cResultString=md5.MDString(cResultString);
	*serialno=(CString)cResultString;
	//////////////////////////////////////////
	return 1;
}


BOOL CPassword::PwdCompare()
{
	CStringA password;
	CStringA serialno;
	GetSerialNo(&serialno);
	HKEY hKey;
	RegOpenKey(HKEY_CURRENT_USER,L"Software\\Image processer",&hKey);
	//查询有关的数据 (SNbuffer)
	LPBYTE SNbuffer=new BYTE[255];
	DWORD type_1=REG_SZ;
	DWORD cbData_1=255;
	//hKEY为刚才RegOpenKeyEx()函数所打开的键的句柄，"SNbuffer"。
	//表示要查询的键值名，type_1表示查询数据的类型，SNbuffer保存所。
	//查询的数据，cbData_1表示预设置的数据长度。
	long ret1=::RegQueryValueEx(hKey,L"SN", NULL,&type_1,SNbuffer,&cbData_1);
	password=CStringA(SNbuffer);
	delete[] SNbuffer;
	RegCloseKey(hKey);	
	int result=password.Compare(serialno);	
	return result;
}

BOOL CPassword::GetHDSerial(CStringA *HDSerialNo)
{
	///////////////////////////////////////get HDD
	char* temp;
	CStringA strHDSerial;
	CGetHDSerial HDSerial;  // 创建实例   
	temp=HDSerial.GetHDSerial(); // 得到硬盘序列号
	strHDSerial.Format("%s",temp);
	strHDSerial.TrimLeft();
	strHDSerial.TrimRight();
	

	/////////////////////////////////////MD5
	CMd5A md5;
	char* cResultString;//cTempString,

	char cTempChar[33]="\0",cResultChar[33]="\0";
	int length=strHDSerial.GetLength();
	if(length==0)
	{
		AfxMessageBox(L"error");
		return false;
	}
	for(int i=0;i<length;i++)
	{
		cTempChar[i]=strHDSerial.GetAt(i);
	}
	*HDSerialNo=cTempChar;
	if(strHDSerial=="")
		return 0;
	return 1;
	cResultString=md5.MDString(cTempChar);
	*HDSerialNo=cResultString;

	if(strHDSerial=="")
		return 0;
	return 1;

}
