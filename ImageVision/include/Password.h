// Password.h: interface for the CPassword class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_PASSWORD_H__)
#define _PASSWORD_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPassword  
{
public:
	BOOL GetHDSerial(CStringA *HDSerialNo);
	BOOL PwdCompare();
	BOOL GetSerialNo(CStringA *password);
	CPassword();
	virtual ~CPassword();

};

#endif // !defined(_PASSWORD_H__
