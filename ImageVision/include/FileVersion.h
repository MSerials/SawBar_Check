/* FileVersion.h --  version 0.1.0, 2005-2-19

  Copyright (C) 2010

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.  

  ʹ�÷���:
  ����Ҫȡ��D:\IPCSERVICE.DLL�İ汾��Ϣ��
  CString szRet;
  CFileVersion fv("D:\\IPCSERVICE.DLL");
  szRet = fv.GetFullVersion();
  //...
  szRet = fv.GetFileVersion();
  //...

*/

#ifndef _FILE_VERSION_H_
#define _FILE_VERSION_H_

#pragma comment(lib, "Version.lib")

class CFileVersion
{
public:
	CFileVersion(CString szPath);
	~CFileVersion();

    //ȡ�����е��ļ��汾��Ϣ������Ϣ����\n�ָ�
	CString GetFullVersion();

	//ȡ�ñ�ע
	CString GetComments();
	//ȡ���ڲ�����
	CString GetInternalName();
	//ȡ�ò�Ʒ����
	CString GetProductName();
	//ȡ�ù�˾����
	CString GetCompanyName();
	//ȡ�ð�Ȩ
	CString GetLegalCopyright();
	//ȡ�ò�Ʒ�汾
	CString GetProductVersion();
	//ȡ���ļ�����
	CString GetFileDescription();
	//ȡ�úϷ��̱�
	CString GetLegalTrademarks();
	//ȡ�ø������ڲ��汾˵��
	CString GetPrivateBuild();
	//ȡ���ļ��汾
	CString GetFileVersion();
	//ȡ��Դ�ļ���
	CString GetOriginalFilename();
	//ȡ�������ڲ��汾˵��
	CString GetSpecialBuild();

private:
	CString GetVer(CString szSubBlock);

	CString m_szPath;
};


#endif

