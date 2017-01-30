// Preferences.cpp: implementation of the CPreferences class.
//
//////////////////////////////////////////////////////////////////////

#include "..//stdafx.h"
//#include "djiajiao.h"
#include "../include/Preferences.h"
#include <fstream>

#pragma warning(disable:4996)

using namespace std;
CPreferences *ini = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPreferences::CPreferences()
{
	path = L"";
	dir = L"";
	Reset();
}

CPreferences::~CPreferences()
{
	
}

//constructor, can specify pathname here instead of using SetPath later
void CPreferences::CreateIniFile(CString fileName)
{       
	CString strCurDir;	 
	strCurDir  = dir + L"\\" + fileName; 
	HANDLE hf = CreateFile(strCurDir, GENERIC_WRITE, FILE_SHARE_READ, NULL,CREATE_NEW, NULL, NULL);
	CloseHandle(hf);
	path = strCurDir;
}

void CPreferences::DeleteIniFile(CString fileName)
{
	CString strCurDir;	 
	strCurDir  = dir + "\\" + fileName; 
	DeleteFile(strCurDir);
}

void CPreferences::CopyIniFile(CString srcName, CString dstName)
{
	CString strOldPath,strNewPath;	 
	strOldPath  = dir + "\\" + srcName; 
	strNewPath  = dir + "\\" + dstName; 
	CopyFile(strOldPath,strNewPath,FALSE);
}

int CPreferences::SetIniFile(CString fileName)
{
	
	if (dir == "")
	{
		AfxMessageBox(L"请先设置文件目录!");
		return -1;
	}
	else
	{
		path = dir + L"\\" + fileName;
	}
	if (!PathFileExists(path))
	{
		HANDLE hf = CreateFile(path, GENERIC_WRITE, FILE_SHARE_READ, NULL,CREATE_NEW, NULL, NULL);
		CloseHandle(hf);
		
		return 0;
	}
	return 1;
}

CString CPreferences::GetAllFileName(CString suffix, int &nCnt, TCHAR separ)
{
	CString sFileName;
	CFileFind finder;	
	CString rStr;
	CString strWildcard;
	rStr.Empty();
	
	nCnt=0;
	strWildcard = dir + L"\\*" + suffix;		
	BOOL bWorking = finder.FindFile(strWildcard);
	
	if (dir == L"")
	{
		AfxMessageBox(L"请先设置文件目录!");
		return L"";
	}
	
	while (bWorking)
	{
		sFileName.Empty();		
		bWorking = finder.FindNextFile();
		if (finder.IsDots()) continue;
		sFileName = finder.GetFileName();
		sFileName = sFileName.Left(sFileName.ReverseFind('.')) + separ;
		rStr += sFileName;
		nCnt++;
	}
	rStr.TrimRight(separ);
	
	return rStr;
}

CString CPreferences::SplitString(CString srcStr, int nIndex, TCHAR separ/* = */)
{
	int i, len =0;
	CString strSplit;
	srcStr.TrimRight(separ);
	for(i=0; i<srcStr.GetLength(); i++)
	{
		if(srcStr[i] == separ)  len++;
	}
	
	if ((nIndex>len) || (srcStr==L"")) return L"";
	AfxExtractSubString(strSplit, srcStr, nIndex, separ);
	return strSplit;
}

/////////////////////////////////////////////////////////////////////
// Public Functions
/////////////////////////////////////////////////////////////////////

//sets path of ini file to read and write from
bool CPreferences::SetIniDir(CString filedir, bool bmodule/* =true */)
{
	if (bmodule)
		dir = GetModulePath() + "\\" + filedir;
	else
		dir = filedir;
	
	return CreateMultipleDirectory(dir);
}

//reads ini file specified using CIniFile::SetPath()
//returns true if successful, false otherwise
bool CPreferences::ReadFile()
{
	CFile file;
	CFileStatus status;
	if (!file.GetStatus(path,status))
		return 0;
	ifstream inifile;
	CString readinfo;
	inifile.open(path);
	int curkey = -1, curval = -1;
	if (inifile.fail())
	{
		error = "打开文件出错.";
		return 0;
	}
	CString keyname, valuename, _value;
	CString temp;
	while (getline(inifile,readinfo))
	{
		if (readinfo != "")
		{
			if (readinfo[0] == '[' && readinfo[readinfo.GetLength()-1] == ']') //if a section heading
			{
				//去掉[]
				keyname = readinfo;
				keyname.TrimLeft('[');
				keyname.TrimRight(']');
			}
			else //if a value
			{
				valuename = readinfo.Left(readinfo.Find(L"="));
				_value = readinfo.Right(readinfo.GetLength()-valuename.GetLength()-1);
				SetValue(keyname,valuename,_value);
			}
		}
	}
	inifile.close();
	return 1;
}

//writes data stored in class to ini file
void CPreferences::WriteFile()
{
	FILE *fp;
	fp=_tfopen(path,L"wt");
	if(fp==(FILE*)NULL)
	{
		return ;
	}
	for (int keynum = 0; keynum <= names.GetUpperBound(); keynum++)
	{
		if (keys[keynum].names.GetSize() != 0)
		{
			_ftprintf(fp,_T("[%s]\n"),names[keynum]);
			for (int valuenum = 0; valuenum <= keys[keynum].names.GetUpperBound(); valuenum++)
			{
				CString strtemp=keys[keynum].names[valuenum];
				CString strtemp1=keys[keynum].values[valuenum];
				_ftprintf(fp,_T("%s=%s"),strtemp,strtemp1);
				
				if (valuenum != keys[keynum].names.GetUpperBound())
					_ftprintf(fp,_T("\n"));
				else
					if (keynum < names.GetSize())
						_ftprintf(fp,_T("\n"));
			}
			if (keynum < names.GetSize())
				_ftprintf(fp,_T("\n"));
		}
	}
	fclose(fp);
}

//deletes all stored ini data
void CPreferences::Reset()
{
	keys.SetSize(0);
	names.SetSize(0);
}

//returns number of keys currently in the ini
int CPreferences::GetNumKeys()
{
	return keys.GetSize();
}

//returns number of values stored for specified key, or -1 if key found
int CPreferences::GetNumValues(CString keyname)
{
	int keynum = FindKey(keyname);
	if (keynum == -1)
		return -1;
	else
		return keys[keynum].names.GetSize();
}

//gets value of [keyname] valuename = 
//overloaded to return CString, int, and double
CString CPreferences::GetValue(CString keyname, CString valuename)
{
	int keynum = FindKey(keyname), valuenum = FindValue(keynum,valuename);
	
	if (keynum == -1)
	{
		error = L"没有此项"+keyname;
		return L"";
	}
	
	if (valuenum == -1)
	{
		error = L"没有此项"+valuename;
		return L"";
	}
	return keys[keynum].values[valuenum];
}

//gets value of [keyname] valuename = 
//overloaded to return CString, int, and double
int CPreferences::GetValueI(CString keyname, CString valuename)
{
	return _tstoi(GetValue(keyname,valuename));
}

//gets value of [keyname] valuename = 
//overloaded to return CString, int, and double
double CPreferences::GetValueF(CString keyname, CString valuename)
{
	return _tstof(GetValue(keyname, valuename));
}

CString CPreferences::GetValueStr(CString keyname, CString valuename)
{
	return GetValue(keyname, valuename);
}
bool CPreferences::GetValuebool(CString keyname,CString valuename)
{
	return (GetValue(keyname,valuename)==L"0")?false:true;
}


//sets value of [keyname] valuename =.
//specify the optional paramter as false (0) if you do not want it to create
//the key if it doesn't exist. Returns true if data entered, false otherwise
//overloaded to accept CString, int, and double
bool CPreferences::SetValue(CString keyname, CString valuename, CString value, bool create)
{
	int keynum = FindKey(keyname), valuenum = 0;
	//find key
	if (keynum == -1) //if key doesn't exist
	{
		if (!create) //and user does not want to create it,
			return 0; //stop entering this key
		names.SetSize(names.GetSize()+1);
		keys.SetSize(keys.GetSize()+1);
		keynum = names.GetSize()-1;
		names[keynum] = keyname;
	}
	
	//find value
	valuenum = FindValue(keynum,valuename);
	if (valuenum == -1)
	{
		if (!create)
			return 0;
		keys[keynum].names.SetSize(keys[keynum].names.GetSize()+1);
		keys[keynum].values.SetSize(keys[keynum].names.GetSize()+1);
		valuenum = keys[keynum].names.GetSize()-1;
		keys[keynum].names[valuenum] = valuename;
	}
	keys[keynum].values[valuenum] = value;
	return 1;
}

//sets value of [keyname] valuename =.
//specify the optional paramter as false (0) if you do not want it to create
//the key if it doesn't exist. Returns true if data entered, false otherwise
//overloaded to accept CString, int, and double
bool CPreferences::SetValueI(CString keyname, CString valuename, int value, bool create)
{
	CString temp;
	temp.Format(L"%d",value);
	return SetValue(keyname, valuename, temp, create);
}

//sets value of [keyname] valuename =.
//specify the optional paramter as false (0) if you do not want it to create
//the key if it doesn't exist. Returns true if data entered, false otherwise
//overloaded to accept CString, int, and double
bool CPreferences::SetValueF(CString keyname, CString valuename, double value, bool create)
{
	CString temp;
	temp.Format(L"%e",value);
	return SetValue(keyname, valuename, temp, create);
}

bool CPreferences::SetValueStr(CString keyname, CString valuename,CString value,bool create)
{
	return SetValue(keyname, valuename, value, create);
}
bool CPreferences::SetValuebool(CString keyname,CString valuename,bool value,bool create)
{
	CString temp;
	if(value==false)
		temp=L"0";
	else
		temp=L"1";
	return SetValue(keyname, valuename, temp, create);
}

/////////////////////////////////////////////////////////////////////
// Private Functions
/////////////////////////////////////////////////////////////////////

//returns index of specified key, or -1 if not found
int CPreferences::FindKey(CString keyname)
{
	int keynum = 0;
	while ( keynum < keys.GetSize() && names[keynum] != keyname)
		keynum++;
	if (keynum == keys.GetSize())
		return -1;
	return keynum;
}

//returns index of specified value, in the specified key, or -1 if not found
int CPreferences::FindValue(int keynum, CString valuename)
{
	if (keynum == -1)
		return -1;
	int valuenum = 0;
	while (valuenum < keys[keynum].names.GetSize() && keys[keynum].names[valuenum] != valuename)
		valuenum++;
	if (valuenum == keys[keynum].names.GetSize())
		return -1;
	return valuenum;
}

//overloaded from original getline to take CString
istream & CPreferences:: getline(istream & is, CString & str)
{
    char buf[2048];
    is.getline(buf,2048);
    str = buf;
    return is;
}


void CPreferences::OpenIniFile()
{
	::ShellExecute(NULL,L"open",path,NULL,NULL,SW_SHOW);
}

CString CPreferences::GetFilePath()
{
	return path;
}

CString CPreferences::GetFileDir()
{
	return dir;
}

CString CPreferences::GetModulePath()
{
	TCHAR szModulePath[MAX_PATH] = {0};        
	CString strCurDir;
	
	::GetModuleFileName(NULL,szModulePath,_MAX_PATH);  	
	strCurDir  = szModulePath;  
	strCurDir  = strCurDir.Left(strCurDir.ReverseFind(TEXT('\\')));
	return strCurDir;
}

bool CPreferences::CreateMultipleDirectory(CString szPath)
{
	if (szPath == _T("")) return false;
	
    CString strDir(szPath);
	
    if (strDir.GetAt(strDir.GetLength()-1)!=_T('\\'))
    {
        strDir += _T('\\');
    }
    std::vector<CString> vPath;
    CString strTemp;
    bool bSuccess = false;
	
    for (int i=0;i<strDir.GetLength();++i)
    {
        if (strDir.GetAt(i) != _T('\\')) 
        {
            strTemp += strDir.GetAt(i);
        }
        else 
        {
            vPath.push_back(strTemp);
            strTemp += _T('\\');
			
        }
    }
	
    std::vector<CString>::const_iterator vIter;
    for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
    {
        bSuccess = CreateDirectory(*vIter, NULL) ? true : false;    
    }	
    return bSuccess;
}


int CPreferences::SelFolder(CWnd *pWnd, CString &strFolder)
{
	HWND hParent = pWnd->m_hWnd;
    LPMALLOC lpMalloc;
    strFolder.Empty();	
	
    if (::SHGetMalloc(&lpMalloc) != NOERROR) return 0;
	
    TCHAR szDisplayName[_MAX_PATH];
    TCHAR szBuffer[_MAX_PATH];
    BROWSEINFO browseInfo;
    browseInfo.hwndOwner = hParent;
    browseInfo.pidlRoot = NULL; // set root at Desktop
    browseInfo.pszDisplayName = szDisplayName;
    browseInfo.lpszTitle = L"Select a folder";
    browseInfo.ulFlags = BIF_RETURNFSANCESTORS|BIF_RETURNONLYFSDIRS;
    browseInfo.lpfn = NULL;
    browseInfo.lParam = 0;
	
    LPITEMIDLIST lpItemIDList;
    if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
    {
        // Get the path of the selected folder from the    item ID list.
        if (::SHGetPathFromIDList(lpItemIDList, szBuffer))
        {
            // At this point, szBuffer contains the path the user chose.
            if (szBuffer[0] == '\0') return 0;
			
            // We have a path in szBuffer! Return it.
            strFolder = szBuffer;
            return 1;
        }
        else return 1; // strResult is empty
		
        lpMalloc->Free(lpItemIDList);
        lpMalloc->Release();
    }
    
	return 1;
}
CString CPreferences::Encrypt(CString Source, WORD Key) // 加密函数
{
	CString Result,str;
	int i,j;
	
	Result=Source; // 初始化结果字符串
	for(i=0; i<Source.GetLength(); i++) // 依次对字符串中各字符进行操作
	{
		Result.SetAt(i, Source.GetAt(i)^(Key>>8)); // 将密钥移位后与字符异或
		Key = ((BYTE)Result.GetAt(i)+Key)*C1+C2; // 产生下一个密钥
	}
	Source=Result; // 保存结果
	Result.Empty(); // 清除结果
	for(i=0; i<Source.GetLength(); i++) // 对加密结果进行转换
	{
		j=(BYTE)Source.GetAt(i); // 提取字符
		// 将字符转换为两个字母保存
		str=L"12"; // 设置str长度为2
		str.SetAt(0, SHIFT+j/26);//这里将65改大点的数例如256，密文就会变乱码，效果更好，相应的，解密处要改为相同的数
		str.SetAt(1, SHIFT+j%26);
		Result += str;
	}
	return Result;
}


CString CPreferences::Decrypt(CString Source, WORD Key) // 解密函数
{
	CString Result,str;
	int i,j;
	
	Result.Empty(); // 清除结果
	for(i=0; i < Source.GetLength()/2; i++) // 将字符串两个字母一组进行处理
	{
		j = ((BYTE)Source.GetAt(2*i)-SHIFT)*26;//相应的，解密处要改为相同的数		
		j += (BYTE)Source.GetAt(2*i+1)-SHIFT;
		str="1"; // 设置str长度为1
		str.SetAt(0, j);
		Result+=str; // 追加字符，还原字符串
	}
	Source=Result; // 保存中间结果
	for(i=0; i<Source.GetLength(); i++) // 依次对字符串中各字符进行操作
	{
		Result.SetAt(i, (BYTE)Source.GetAt(i)^(Key>>8)); // 将密钥移位后与字符异或
		Key = ((BYTE)Source.GetAt(i)+Key)*C1+C2; // 产生下一个密钥
	}
	return Result;
}

void CPreferences::LoadParaFile(UINT item)
{
	UINT mask = item;



	if (ReadFile())
	{
		////////////////////////////////////
		if (PARA_PRJ == (mask&PARA_PRJ))
		{
			//m_strCurrentProjectName = GetValue(L"projectSetting",L"CurrentProject");
			//m_strCurrentProjectName = GetValue(L"projectSetting",L"CurrentProject");
			m_Prj_Name = GetValue(L"projectSetting", L"PRJ");
			m_Prj_Vector = GetValue(L"projectSetting", L"PRJVEC");
			m_Prj_index = GetValueI(L"projectSetting", L"PRJIDX");
			m_TopCamConfig = GetValue(L"projectSetting", L"TOPCAMCFG");
			m_BottomCamConfig = GetValue(L"projectSetting", L"BOTCAMCFG");
			m_password = GetValue(L"projectSetting", L"PWD");

			m_TopSnapWait=GetValueI(L"projectSetting", L"TWAIT");
			m_BottomWait = GetValueI(L"projectSetting", L"BWAIT");

			m_TopCamIndex = GetValueI(L"projectSetting", L"TCAMINDEX");
			m_BottomCamIndex = GetValueI(L"projectSetting", L"BCAMINDEX");

			m_TopShutter = GetValueI(L"projectSetting", L"TSHUTR");
			m_BottomShutter= GetValueI(L"projectSetting", L"BSHUTR");

			m_isTopSnap = GetValueI(L"projectSetting", L"TISSNAP");
			m_isBottomSnap = GetValueI(L"projectSetting", L"BISSNAP");
		}
		if (PARA_IMAGE == (mask&PARA_IMAGE))
		{
			TROI_x = GetValueF(L"IMAGE_SETTING", L"TROI_x");
			TROI_y = GetValueF(L"IMAGE_SETTING", L"TROI_y");

			TROI_w = GetValueF(L"IMAGE_SETTING", L"TROI_w");
			TROI_h = GetValueF(L"IMAGE_SETTING", L"TROI_h");

			BROI_x = GetValueF(L"IMAGE_SETTING", L"BROI_x");
			BROI_y = GetValueF(L"IMAGE_SETTING", L"BROI_y");

			BROI_w = GetValueF(L"IMAGE_SETTING", L"BROI_w");
			BROI_h = GetValueF(L"IMAGE_SETTING", L"BROI_h");

			top_threshold = GetValueF(L"IMAGE_SETTING", L"TTHRES");
			top_train_score= GetValueF(L"IMAGE_SETTING", L"TTRAINSCORE");
			top_match_score= GetValueF(L"IMAGE_SETTING", L"TMATCHSCORE");
			bottom_threshold = GetValueF(L"IMAGE_SETTING", L"BTHRES");
			bottom_train_score = GetValueF(L"IMAGE_SETTING", L"BTRAINSCOR");
			bottom_match_score = GetValueF(L"IMAGE_SETTING", L"BMATCHSCORE");

			m_topwScaleTo = GetValueI(L"IMAGE_SETTING", L"TWSCALETO");
			m_tophScaleTo = GetValueI(L"IMAGE_SETTING", L"THSCALETO");

			m_botwScaleTo = GetValueI(L"IMAGE_SETTING", L"BWSCALETO");
			m_bothScaleTo = GetValueI(L"IMAGE_SETTING", L"BHSCALETO");

			m_TopModel = GetValueI(L"IMAGE_SETTING", L"TMODEL");
			m_BottomModel = GetValueI(L"IMAGE_SETTING", L"BMODEL");

			m_TopRatio = GetValueF(L"IMAGE_SETTING", L"TWHRATIO");
			m_BottomRatio = GetValueF(L"IMAGE_SETTING", L"BWHRATIO");

			isTopScale = GetValueI(L"IMAGE_SETTING", L"ISTSCALE");
			isBottomScale = GetValueI(L"IMAGE_SETTING", L"ISBSCALE");
		}



		if (PARA_IO == (mask&PARA_IO))
		{
			m_startDetect = GetValueI(L"IOCARD_SETTING", L"INDETECT");
			m_SrvEnable = GetValueI(L"IOCARD_SETTING", L"INSRVEN");
			m_void = GetValueI(L"IOCARD_SETTING", L"OUTVOID");
			m_OK = GetValueI(L"IOCARD_SETTING", L"OUTOK");
			m_NG = GetValueI(L"IOCARD_SETTING", L"OUTNG");


			m_FstMtr.No = GetValueI(L"IOCARD_SETTING", L"FSTMOTR");
			m_FstMtr.InitSpeed = GetValueI(L"IOCARD_SETTING", L"FSTMOTRISPD");
			m_FstMtr.MaxSpeed = GetValueI(L"IOCARD_SETTING", L"FSTMOTRSPD");
			m_FstMtr.Waves = GetValueI(L"IOCARD_SETTING", L"FSTMOTRWAV");
			m_FstMtr.Tacc = GetValueF(L"IOCARD_SETTING", L"FSTMOTRTCC");
			m_FstMtr.compansate = GetValueI(L"IOCARD_SETTING", L"FSTCMSATE");

			m_SndMtr.No = GetValueI(L"IOCARD_SETTING", L"SNDMOTR");
			m_SndMtr.InitSpeed = GetValueI(L"IOCARD_SETTING", L"SNDMOTRISPD");
			m_SndMtr.MaxSpeed = GetValueI(L"IOCARD_SETTING", L"SNDMOTRSPD");
			m_SndMtr.Waves = GetValueI(L"IOCARD_SETTING", L"SNDMOTRWAV");
			m_SndMtr.Tacc = GetValueF(L"IOCARD_SETTING", L"SNDMOTRTCC");
			m_SndMtr.compansate = GetValueI(L"IOCARD_SETTING", L"SNDCMSATE");
		}
	
	}

}

void CPreferences::SaveParaFile(UINT item)
{
	UINT mask = item;
	////////////////////////////////////
	if (PARA_PRJ==(mask&PARA_PRJ))
	{
		//m_strCurrentProjectName = GetValue(L"projectSetting",L"CurrentProject");
		SetValue(L"projectSetting", L"PRJ", m_Prj_Name);
		SetValue(L"projectSetting", L"PRJVEC", m_Prj_Vector);
		SetValueI(L"projectSetting", L"PRJIDX", m_Prj_index);
		SetValue(L"projectSetting", L"TOPCAMCFG", m_TopCamConfig);
		SetValue(L"projectSetting", L"BOTCAMCFG", m_BottomCamConfig);		
		SetValue(L"projectSetting", L"PWD", m_password);

		SetValueI(L"projectSetting", L"TWAIT", m_TopSnapWait);
		SetValueI(L"projectSetting", L"BWAIT", m_BottomWait);

		SetValueI(L"projectSetting", L"TCAMINDEX", m_TopCamIndex);
		SetValueI(L"projectSetting", L"BCAMINDEX", m_BottomCamIndex);

		SetValueI(L"projectSetting", L"TSHUTR", m_TopShutter);
		SetValueI(L"projectSetting", L"BSHUTR", m_BottomShutter);

		SetValueI(L"projectSetting", L"TISSNAP", m_isTopSnap);
		SetValueI(L"projectSetting", L"BISSNAP", m_isBottomSnap);

	}

	if(PARA_IMAGE==(mask&PARA_IMAGE))
	{
		SetValueF(L"IMAGE_SETTING", L"TROI_x", TROI_x);
		SetValueF(L"IMAGE_SETTING", L"TROI_y", TROI_y);

		SetValueF(L"IMAGE_SETTING", L"TROI_w", TROI_w);
		SetValueF(L"IMAGE_SETTING", L"TROI_h", TROI_h);

		SetValueF(L"IMAGE_SETTING", L"BROI_x", BROI_x);
		SetValueF(L"IMAGE_SETTING", L"BROI_y", BROI_y);

		SetValueF(L"IMAGE_SETTING", L"BROI_w", BROI_w);
		SetValueF(L"IMAGE_SETTING", L"BROI_h", BROI_h);

		SetValueF(L"IMAGE_SETTING", L"TTHRES", top_threshold);
		SetValueF(L"IMAGE_SETTING", L"TTRAINSCORE", top_train_score);
		SetValueF(L"IMAGE_SETTING", L"TMATCHSCORE", top_match_score);
		SetValueF(L"IMAGE_SETTING", L"BTHRES", bottom_threshold);
		SetValueF(L"IMAGE_SETTING", L"BTRAINSCOR", bottom_train_score);
		SetValueF(L"IMAGE_SETTING", L"BMATCHSCORE", bottom_match_score);

		SetValueI(L"IMAGE_SETTING", L"TWSCALETO", m_topwScaleTo);
		SetValueI(L"IMAGE_SETTING", L"THSCALETO", m_tophScaleTo);

		SetValueI(L"IMAGE_SETTING", L"BWSCALETO", m_botwScaleTo);
		SetValueI(L"IMAGE_SETTING", L"BHSCALETO", m_bothScaleTo);

		SetValueI(L"IMAGE_SETTING", L"TMODEL", m_TopModel);
		SetValueI(L"IMAGE_SETTING", L"BMODEL", m_BottomModel);

		SetValueF(L"IMAGE_SETTING", L"TWHRATIO", m_TopRatio);
		SetValueF(L"IMAGE_SETTING", L"BWHRATIO", m_BottomRatio);

		SetValueI(L"IMAGE_SETTING", L"ISTSCALE", isTopScale);
		SetValueI(L"IMAGE_SETTING", L"ISBSCALE", isBottomScale);	
	}
	if(PARA_IO==(mask&PARA_IO))
	{
		SetValueI(L"IOCARD_SETTING", L"INDETECT", m_startDetect);
		SetValueI(L"IOCARD_SETTING", L"INSRVEN", m_SrvEnable);
		SetValueI(L"IOCARD_SETTING", L"OUTVOID", m_void);
		SetValueI(L"IOCARD_SETTING", L"OUTOK", m_OK);
		SetValueI(L"IOCARD_SETTING", L"OUTNG", m_NG);


		SetValueI(L"IOCARD_SETTING", L"FSTMOTR", m_FstMtr.No);
		SetValueI(L"IOCARD_SETTING", L"FSTMOTRISPD", m_FstMtr.InitSpeed);
		SetValueI(L"IOCARD_SETTING", L"FSTMOTRSPD", m_FstMtr.MaxSpeed);
		SetValueI(L"IOCARD_SETTING", L"FSTMOTRWAV", m_FstMtr.Waves);
		SetValueF(L"IOCARD_SETTING", L"FSTMOTRTCC", m_FstMtr.Tacc);
		SetValueI(L"IOCARD_SETTING", L"FSTCMSATE", m_FstMtr.compansate);

		SetValueI(L"IOCARD_SETTING", L"SNDMOTR", m_SndMtr.No);
		SetValueI(L"IOCARD_SETTING", L"SNDMOTRISPD", m_SndMtr.InitSpeed);
		SetValueI(L"IOCARD_SETTING", L"SNDMOTRSPD", m_SndMtr.MaxSpeed);
		SetValueI(L"IOCARD_SETTING", L"SNDMOTRWAV", m_SndMtr.Waves);
		SetValueF(L"IOCARD_SETTING", L"SNDMOTRTCC", m_SndMtr.Tacc);
		SetValueI(L"IOCARD_SETTING", L"SNDCMSATE", m_SndMtr.compansate);
	}
	
	//if(PARA_SN==(mask&PARA_SN))
	//{
	//	SetValue(L"PRJ_SN",L"SN",m_password); 
	//}
	WriteFile();
}


void CPreferences::InitData()
{

	//IMAGE #define PARA_IMAGE			(0x1<<1)
	m_Prj_Name = L"1";
	m_Prj_Vector = m_Prj_Name;
	m_Prj_index = 0;
	m_TopCamConfig = L"";
	m_BottomCamConfig= L"";

	m_TopSnapWait = 15000;
	m_BottomWait = 15000;

	m_TopCamIndex = 0;
	m_BottomCamIndex = 0;

	m_TopShutter = 800;
	m_BottomShutter = 800;

	m_isTopSnap = 0;
	m_isBottomSnap = 0;
	CString m_password;			//密码


	//图像的ROI //注意这不会根据显示器不同而被换算，程序中会自动根据显示器换算
	TROI_x =  TROI_y = TROI_w = TROI_h = 0;
	BROI_x = BROI_y = BROI_w = BROI_h = 0;

	//原图W方向缩放尺寸
	m_topwScaleTo = 3000; m_tophScaleTo = 3000;
	m_botwScaleTo = 3000; m_bothScaleTo = 3000;

	//检测使用的模板，掩码计算，低位为第一个模板，意味着只支持到32个模板
	m_TopModel = 0;
	m_BottomModel = 0;

	isTopScale = 1;
	isBottomScale = 1;

	//图像计算用的匹配精度
	top_threshold = 110.0f;
	top_train_score = 0.95f; 
	top_match_score = 0.95f;
	bottom_threshold = 110.0f; 
	bottom_train_score = 0.95f; 
	bottom_match_score = 0.95f;

	//缩放比例,这个需要计算，但是却不用
	m_TopRatio = -1.0f;
	m_BottomRatio = -1.0f;

	m_SrvEnable = 4;
	m_startDetect = 7;
	m_void = 9;
	m_NG = 10;
	m_OK = 11;

	//IO卡 #define PARA_IO				(0x1<<2)

	m_FstMtr.No = 0;
	m_FstMtr.InitSpeed = 100;
	m_FstMtr.MaxSpeed = 5000;
	m_FstMtr.Waves = 8000;
	m_FstMtr.Tacc = 0.05;
	m_FstMtr.compansate = 0;
	m_SndMtr = m_FstMtr;

	m_SndMtr.No = 1;
//	m_SndMtr.InitSpeed = 100;
//	m_SndMtr.MaxSpeed = 5000;
//	m_SndMtr.Waves = 8000;
//	m_SndMtr.Tacc = 0.05;
}