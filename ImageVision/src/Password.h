// Password.h: interface for the CPassword class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PASSWORD_H__73D77DFC_DB50_4514_827C_66CC376F6A41__INCLUDED_)
#define AFX_PASSWORD_H__73D77DFC_DB50_4514_827C_66CC376F6A41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPassword  
{
public:
	BOOL GetHDSerial(CString *HDSerialNo);
	BOOL PwdCompare();
	BOOL GetSerialNo(CString *password);
	CPassword();
	virtual ~CPassword();

};

#endif // !defined(AFX_PASSWORD_H__73D77DFC_DB50_4514_827C_66CC376F6A41__INCLUDED_)
