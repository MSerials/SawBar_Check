//工具

//
#include "stdafx.h"
#include <windows.h> //不适合64位程序
#include <tlhelp32.h>


#ifndef __TOOLS__
#define __TOOLS__


class win_tool
{
public:
	win_tool(){};
	virtual ~win_tool(){};
	int GetProcessIDByName(PWSTR pname)
	{
		HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		SHFILEINFO shSmall;
		BOOL Status = FALSE;
		PROCESSENTRY32 ProcessInfo;//声明进程信息变量
		DWORD pid = 0;
		if (SnapShot == NULL)
		{
			return -1;
		}


		ProcessInfo.dwSize = sizeof(ProcessInfo);//设置ProcessInfo的大小
		//返回系统中第一个进程的信息
		Status = Process32First(SnapShot, &ProcessInfo);

		while (Status)
		{
			//获取进程文件信息
			SHGetFileInfo(ProcessInfo.szExeFile, 0, &shSmall,
				sizeof(shSmall), SHGFI_ICON | SHGFI_SMALLICON);
			//在列表控件中添加映像名称
			if (StrStrI(ProcessInfo.szExeFile, pname) != NULL)
			{
				pid = ProcessInfo.th32ProcessID;
				break;
			}
			//获取下一个进程的信息
			Status = Process32Next(SnapShot, &ProcessInfo);

		}
		return pid;
	}

	CString AppPath()
	{
		CString AppPath;
		::GetModuleFileName(GetModuleHandle(NULL), AppPath.GetBuffer(300), 300);
		AppPath.ReleaseBuffer();
		AppPath = AppPath.Left(AppPath.ReverseFind('\\'));
		return AppPath;
	}


	CString Get_Date()
	{
		SYSTEMTIME st;
		CString strDate;
		GetLocalTime(&st);
		strDate.Format(_T("%4d-%d-%d"), st.wYear, st.wMonth, st.wDay);
		return strDate;	
	}

	CString Get_Time()
	{
		SYSTEMTIME st;
		CString strTime;
		GetLocalTime(&st);
		strTime.Format(_T("%d-%d-%d"), st.wHour, st.wMinute, st.wSecond);
		return  strTime;	
	}

	CString Get_Date_Time()
	{
		CString str;
		SYSTEMTIME st;
		CString strDate, strTime;
		GetLocalTime(&st);
		strDate.Format(_T("%4d-%d-%d_"), st.wYear, st.wMonth, st.wDay);
		strTime.Format(_T("%d-%d-%d"), st.wHour, st.wMinute, st.wSecond);
		str = strDate + strTime;
		return str;
	}

	vector<CString> SplitCString(CString strSource, CString ch)
	{
		vector <CString> vecString;
		int iPos = 0;
		CString strTmp;
		strTmp = strSource.Tokenize(ch, iPos);
		while (strTmp.Trim() != _T(""))
		{
			vecString.push_back(strTmp);
			strTmp = strSource.Tokenize(ch, iPos);
		}
		return vecString;
	}



};


#endif