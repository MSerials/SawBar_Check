#ifndef __LANGUAGETEXT_H__
#define __LANGUAGETEXT_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----------------------------------------------------------------------------------
/**
\brief  获取时间差类
*/
//----------------------------------------------------------------------------------
class CStopWatch
{ 
public: 

	/// 构造函数 
	CStopWatch()
	{ 
		Start(); 
	} 

	/// 开始计时
	void Start() 
	{ 
		QueryPerformanceCounter(&m_StartTime);
	} 

	/// 停止计时,ms
	double Stop() 
	{
		QueryPerformanceCounter(&m_StopTime);
		double theElapsedTime = ElapsedTime();

		m_StartTime = m_StopTime; 
		return theElapsedTime;
	} 

	/// 获取时间差 
	double ElapsedTime() 
	{ 
		LARGE_INTEGER timerFrequency; QueryPerformanceFrequency(&timerFrequency); 
		__int64 oldTicks = ((__int64)m_StartTime.HighPart << 32) + (__int64)m_StartTime.LowPart;
		__int64 newTicks = ((__int64)m_StopTime.HighPart << 32) + (__int64)m_StopTime.LowPart; 
		long double timeDifference = (long double) (newTicks - oldTicks); 
		long double ticksPerSecond = (long double) (((__int64)timerFrequency.HighPart << 32) + (__int64)timerFrequency.LowPart); 
		return (double)(timeDifference * 1000/ ticksPerSecond); 
	}
protected: 
	LARGE_INTEGER m_StartTime;   ///< 开始时间
	LARGE_INTEGER m_StopTime;    ///< 停止时间 
};

class CLanguageText
{
public:
	CString GetText (LPCTSTR strSection, LPCTSTR strKey) ;
	CString GetText (LPCTSTR strSection, int nKey) ;

	void InitMenuLanguageText (HMENU hRootMenu) ;
	CString GetMenuPureText (int nMenuCommand) ;
	void CreateLanguageFile() ;

private:
	CString   m_language_file ;

private:
	
};

inline 	CString CLanguageText::GetText (LPCTSTR strSection, LPCTSTR strKey) 
{
	CString   s ;
	FCFileEx::INIRead (m_language_file, strKey, s, strSection) ;
	s.Replace (L"\\n", L"\n") ;
	return s ;
}
inline CString CLanguageText::GetText (LPCTSTR strSection, int nKey) 
{
	CString   s ;
	s.Format (L"%d", nKey) ;
	return GetText (strSection, s) ;
}

inline void CLanguageText::InitMenuLanguageText (HMENU hRootMenu) 
{
	std::deque<CMenu*>   ls ;
	ls.push_back (CMenu::FromHandle(hRootMenu)) ;

	while (ls.size())
	{
		CMenu   * pMenu = ls.back() ;
		ls.pop_back() ;

		for (UINT i=0 ; i < pMenu->GetMenuItemCount() ; i++)
		{
			CMenu     * pSub = pMenu->GetSubMenu(i) ;
			UINT      nID = pMenu->GetMenuItemID(i) ;
			CString   strNew ;
			if (pSub)
			{
				// pop menu, no ID
				CString   s ;
				pMenu->GetMenuString(i, s, MF_BYPOSITION) ;
				strNew = GetText(L"MENU_TEXT", s) ;
				ls.push_back(pSub) ;
			}
			else
			{
				strNew = GetText(L"MENU_TEXT", nID) ;
				strNew.Replace (L"\\t", L"\t") ;
			}

			if (strNew.GetLength())
			{
				pMenu->ModifyMenu(i, MF_STRING|MF_BYPOSITION, nID, strNew) ;
			}
		}
	}
}
inline CString CLanguageText::GetMenuPureText (int nMenuCommand) 
{
	CString   s = GetText(L"MENU_TEXT", nMenuCommand) ;

	int   n = s.Find(L"(&") ;
	if (n != -1)
	{
		s = s.Left(n) ;
	}

	n = s.Find(L"\\t") ;
	if (n != -1)
	{
		s = s.Left(n) ;
	}

	s.Replace (L"...", L"") ;
	s.Replace (L"&", L"") ;
	return s ;
}

inline void CLanguageText::CreateLanguageFile() 
{
	HMODULE  hModule = ::GetModuleHandle(NULL) ;
//	HRSRC    hRes = ::FindResource (hModule, MAKEINTRESOURCE(IDR_LANGUAGE_FILE), L"INI") ;
//	DWORD    nSize = ::SizeofResource (hModule, hRes) ;
//	void     * pData = ::LockResource (::LoadResource (hModule, hRes)) ;
	
	//m_language_file = FCFileEx::QueryTempFilePath() ;
	//FCFileEx::Write (m_language_file, pData, nSize) ;
	m_language_file=FCFileEx::GetModuleFolder(hModule)+L"lang.ini";
}

inline std::wstring A_to_W(const char* p)
{
	std::wstring ws;
	if(p)
	{
		//setlocale(LC_CTYPE,"");
		int requiredSize;
		requiredSize=strlen(p);
		std::vector<wchar_t> buf(requiredSize+1,0);
		mbstowcs(&buf[0],p,buf.size()-1);
		ws=&buf[0];
	}
	return ws;
}

inline std::string W_to_A(const wchar_t* p)
{
	std::string s;
	if(p)
	{
		int requiredSize;
		requiredSize=wcslen(p);
		//setlocale(LC_CTYPE,L"");
		std::vector<char> buf(requiredSize+1,0);
		wcstombs(&buf[0],p,buf.size()-1);
		s=&buf[0];
	}
	return s;
}
#endif