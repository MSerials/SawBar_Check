// MainControl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageVision.h"
#include "MainControl.h"
#include "afxdialogex.h"
#include "MotionCtrl.h"
#include "ImgControl.h"
#include "Tools.h"


// CMainControl �Ի���

IMPLEMENT_DYNAMIC(CMainControl, CDialogEx)

CMainControl::CMainControl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainControl::IDD, pParent)
{
	isOrigin = false;
}

CMainControl::~CMainControl()
{
}

void CMainControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBSELPRJ, m_ComBoxSel);
}


BEGIN_MESSAGE_MAP(CMainControl, CDialogEx)
//	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_IOMOTION, &CMainControl::OnBnClickedButtonIomotion)
	ON_BN_CLICKED(IDC_BUTTON_IMGSET, &CMainControl::OnBnClickedButtonImgset)
	ON_BN_CLICKED(IDC_BUTTON_TO_ORIGIN, &CMainControl::OnBnClickedButtonToOrigin)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBSELPRJ, &CMainControl::OnSelchangeCombselprj)
	ON_BN_CLICKED(IDC_BUTTON_ADDPRJ, &CMainControl::OnBnClickedButtonAddprj)
END_MESSAGE_MAP()


// CMainControl ��Ϣ�������


BOOL CMainControl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COMBSELPRJ);// ->AddString(L"1");
	win_tool wt;
	vector<CString> prj = wt.SplitCString(g.prjini.m_Prj_Vector, L",");
	for (size_t i = 0; i < prj.size(); i++)
	{
		pBox->AddString(prj[i]);
	}
	pBox->SetCurSel(g.prjini.m_Prj_index);


#if 0
	if (port.InitPort(this, "COM11"))
		isPortOK = true;
	else
		isPortOK = false;

	if (port1.InitPort(this, "COM12"))
		isPortOK1 = true;
	else
		isPortOK1 = false;
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}





void CMainControl::OnBnClickedButtonIomotion()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
	CMotionCtrl dlg;
	dlg.DoModal();
}


void CMainControl::OnBnClickedButtonImgset()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
	CImgControl cdlg;
	cdlg.DoModal();
	return;
}


void CMainControl::OnBnClickedButtonToOrigin()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (g.mc.CylHaveProduct()) { AfxMessageBox(L"��������������!"); return; }
	g.evtEmgencyStop.ResetEvent();
	int SysState = SYSRUNNING | FST_MOTOR_NOT_ALREADY | SND_MOTOR_NOT_ALREADY | SYSEMGENCY;//Ӱ���ԭ�������
	SysState = SysState&g.Controller.SysState;
	if (SysState && (WAIT_OBJECT_0 == ::WaitForSingleObject(g.evt_procedure.evt, 8))) return; //���������Ҳ���ܻ�ԭ�㣻
	if (IDYES != MessageBox(L"ȷ��", L"ϵͳ��ԭ��", MB_YESNO | MB_ICONQUESTION)) return;
	RunOrigin();
}


void CMainControl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (ORIGIN == nIDEvent)
	{
		KillTimer(ORIGIN);
		ToOrigin();
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CMainControl::ToOrigin()
{
	

}


bool CMainControl::RunOrigin()
{
	if (isOrigin) return false;
	pRunOrigin = AfxBeginThread(pRunOriginThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	return true;
}

UINT CMainControl::pRunOriginThread(LPVOID lp)
{
	CMainControl * pApp = (CMainControl*)lp;
	pApp->isOrigin = true;
	int res = g.mc.SysToOrigin();
	res &= ~TOPCAMERROR;
	res &= ~BOTTOMERROR;
	if (res)
	{
		g.Controller.SysState |= SYSNEEDORIGIN;
		AfxMessageBox(g.ErrorString(res));
	}		
	else
	{
		g.Controller.SysState &= ~SYSNEEDORIGIN;
		g.mc.clear_index_data();
		g.mc.clear_running_state();
		g.evt_procedure.SetEvent();
	}
	pApp->isOrigin = false;
	return 0;
}

void CMainControl::OnSelchangeCombselprj()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	win_tool wt;
	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COMBSELPRJ);
	int nSel = pBox->GetCurSel();
	// ����ѡ����������ȡ�����ַ���
	CString str;
	pBox->GetLBText(nSel, str);
	//bug 1
	g.prjini.m_Prj_index = nSel;
	g.prjini.m_Prj_Name = str;
	CString AppPath;
	::GetModuleFileName(GetModuleHandle(NULL), AppPath.GetBuffer(300), 300);
	AppPath.ReleaseBuffer();
	CString PrjPath = AppPath.Left(AppPath.ReverseFind('\\'));
	AppPath = PrjPath + L"\\" + L"Configuration";

	g.prjini.SaveParaFile(PARA_ALL);
	PrjPath += L"\\" + g.prjini.m_Prj_Name;
	g.ini.SetIniDir(PrjPath, false);
	if (0 == g.ini.SetIniFile(L"PrgParameter.ini"))
	{
		g.ini.InitData();
		g.ini.SaveParaFile(PARA_ALL);
	}
	g.ini.LoadParaFile(PARA_ALL);
}


void CMainControl::OnBnClickedButtonAddprj()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	win_tool  wt;
	CString str;
	GetDlgItem(IDC_EDIT_ADDPRJ)->GetWindowText(str);
	if (-1 != str.ReverseFind(','))
	{
		AfxMessageBox(L"���ܰ������·���: ,");
		return;
	}
	if (str.IsEmpty()) return;

	vector<CString> s = wt.SplitCString(g.prjini.m_Prj_Vector, L",");
	for (size_t i = 0; i < s.size(); i++)
	{
		if (!str.Compare(s[i]))
		{
			AfxMessageBox(L"�������б�������");
			return;
		}
	}
	g.prjini.m_Prj_Name = str;

	CString NewPrj = L"," + str;
	g.prjini.m_Prj_Vector += NewPrj;
	g.prjini.m_Prj_index = (int)s.size(); //�պõ����һ����Ŀ����
	g.prjini.SaveParaFile(PARA_ALL);

	CString AppPath;
	::GetModuleFileName(GetModuleHandle(NULL), AppPath.GetBuffer(300), 300);
	AppPath.ReleaseBuffer();
	CString PrjPath = AppPath.Left(AppPath.ReverseFind('\\'));
	AppPath = PrjPath + L"\\" + L"Configuration";

	PrjPath += L"\\" + g.prjini.m_Prj_Name;
	g.ini.SetIniDir(PrjPath, false);
	if (0 == g.ini.SetIniFile(L"PrgParameter.ini"))
	{
		g.ini.InitData();
		g.ini.SaveParaFile(PARA_ALL);
	}
	g.ini.LoadParaFile(PARA_ALL);
	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COMBSELPRJ);
	pBox->AddString(str);
	vector<CString> prj = wt.SplitCString(g.prjini.m_Prj_Vector, L",");
	pBox->SetCurSel(g.prjini.m_Prj_index);
}
