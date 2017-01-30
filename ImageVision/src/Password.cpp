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
	CGetHDSerial HDSerial;  // ����ʵ��   
	temp=HDSerial.GetHDSerial(); // �õ�Ӳ�����к�
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
	//��ѯ�йص����� (SNbuffer)
	LPBYTE SNbuffer=new BYTE[255];
	DWORD type_1=REG_SZ;
	DWORD cbData_1=255;
	//hKEYΪ�ղ�RegOpenKeyEx()�������򿪵ļ��ľ����"SNbuffer"��
	//��ʾҪ��ѯ�ļ�ֵ����type_1��ʾ��ѯ���ݵ����ͣ�SNbuffer��������
	//��ѯ�����ݣ�cbData_1��ʾԤ���õ����ݳ��ȡ�
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
	CGetHDSerial HDSerial;  // ����ʵ��   
	temp=HDSerial.GetHDSerial(); // �õ�Ӳ�����к�
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
