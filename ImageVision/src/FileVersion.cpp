#include "stdafx.h"
#include "FileVersion.h"

CFileVersion::CFileVersion(CString szPath)
{
	m_szPath = szPath;
}

CFileVersion::~CFileVersion()
{
}

//ȡ�����е��ļ��汾��Ϣ������Ϣ����\n�ָ�
CString CFileVersion::GetFullVersion()
{
	CString szRet;
	szRet = "�ļ��汾: " + GetFileVersion() + "\n"; 
	szRet += "˵��: " + GetFileDescription() +"\n";
	szRet += "��Ȩ: " + GetLegalCopyright() +"\n";
	szRet += "��ע: " + GetComments() + "\n";
	szRet += "��Ʒ�汾: " + GetProductVersion() +"\n";
	szRet += "��Ʒ����: " + GetProductName() +"\n";
	szRet += "�������ڲ��汾˵��: " + GetPrivateBuild() +"\n";
	szRet += "��˾����: " + GetCompanyName() +"\n";
	szRet += "�Ϸ��̱�: " + GetLegalTrademarks() +"\n";
	szRet += "�ڲ�����: " + GetInternalName() +"\n";
	szRet += "�����ڲ��汾˵��: " + GetSpecialBuild() +"\n";
	szRet += "Դ�ļ���: " + GetOriginalFilename() +"\n";	
	return szRet;
}

//ȡ�ñ�ע
CString CFileVersion::GetComments()
{
	return GetVer("Comments");
}

//ȡ���ڲ�����
CString CFileVersion::GetInternalName()
{
	return GetVer("InternalName");
}

//ȡ�ò�Ʒ����
CString CFileVersion::GetProductName()
{
	return GetVer("ProductName");
}

//ȡ�ù�˾����
CString CFileVersion::GetCompanyName()
{
	return GetVer("CompanyName");
}

//ȡ�ð�Ȩ
CString CFileVersion::GetLegalCopyright()
{
	return GetVer("LegalCopyright");
}

//ȡ�ò�Ʒ�汾
CString CFileVersion::GetProductVersion()
{
	return GetVer("ProductVersion");
}

//ȡ���ļ�����
CString CFileVersion::GetFileDescription()
{
	return GetVer("FileDescription");
}

//ȡ�úϷ��̱�
CString CFileVersion::GetLegalTrademarks()
{
	return GetVer("LegalTrademarks");
}

//ȡ�ø����ڲ��汾˵��
CString CFileVersion::GetPrivateBuild()
{
	return GetVer("PrivateBuild");
}

//ȡ���ļ��汾
CString CFileVersion::GetFileVersion()
{
	return GetVer("FileVersion");
}

//ȡ��Դ�ļ���
CString CFileVersion::GetOriginalFilename()
{
	return GetVer("OriginalFilename");
}

//ȡ�������ڲ��汾˵��
CString CFileVersion::GetSpecialBuild()
{
	return GetVer("SpecialBuild");
}

CString CFileVersion::GetVer(CString szSubBlock)
{
	CString szRet;

	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	} *lpTranslate;

	DWORD dwHandle;
	DWORD dwInfoSize;

	dwInfoSize = GetFileVersionInfoSize((char*)(LPCTSTR)m_szPath, &dwHandle);

	if( dwInfoSize != 0 )
	{
		char* lpData = new char[dwInfoSize];
		if( GetFileVersionInfo((char*)(LPCTSTR)m_szPath, 0, dwInfoSize, lpData) )
		{
			UINT cbTranslate = 0;
			VerQueryValue(lpData, 
              TEXT("\\VarFileInfo\\Translation"),
              (LPVOID*)&lpTranslate,
              &cbTranslate);

			char SubBlock[256];
			char* lpBuffer;
			UINT dwBytes = 0;
			for(int  i=0; i <(int) (cbTranslate/sizeof(struct LANGANDCODEPAGE)); i++ )
			{
				wsprintf( SubBlock, 
					TEXT("\\StringFileInfo\\%04x%04x\\%s"),
					lpTranslate[i].wLanguage,
					lpTranslate[i].wCodePage,
					szSubBlock);		
				
				VerQueryValue(lpData, 
					SubBlock, 
					(LPVOID*)&lpBuffer, 
					&dwBytes); 
				
				szRet.Format("%s", lpBuffer);

				break; //only retrieve the first language and code page
			}

		}
		delete [] lpData;
		lpData = NULL;
	}

	return szRet;
}