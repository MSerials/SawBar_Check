// MotionCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageVision.h"
#include "MotionCtrl.h"
#include "afxdialogex.h"


// CMotionCtrl �Ի���

IMPLEMENT_DYNAMIC(CMotionCtrl, CDialogEx)

CMotionCtrl::CMotionCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMotionCtrl::IDD, pParent)
{
	isHomeThread = false;
	isHomeThread1 = false;
}

CMotionCtrl::~CMotionCtrl()
{

}

void CMotionCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMotionCtrl, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_COMFIRM, &CMotionCtrl::OnBnClickedButtonComfirm)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SAVEPOS, &CMotionCtrl::OnBnClickedButtonSavepos)
	ON_BN_CLICKED(IDC_BUTTON_SETZERO, &CMotionCtrl::OnBnClickedButtonSetzero)
	ON_BN_CLICKED(IDC_BUTTON_SAVEPOS2, &CMotionCtrl::OnBnClickedButtonSavepos2)
	ON_BN_CLICKED(IDC_BUTTON_SETZERO2, &CMotionCtrl::OnBnClickedButtonSetzero2)
	ON_BN_CLICKED(IDC_BUTTON_COMFIRM2, &CMotionCtrl::OnBnClickedButtonComfirm2)
	ON_BN_CLICKED(IDC_BUTTON_HOMERUN, &CMotionCtrl::OnBnClickedButtonHomerun)
	ON_BN_CLICKED(IDC_BUTTON_HOMERUN2, &CMotionCtrl::OnBnClickedButtonHomerun2)
	ON_BN_CLICKED(IDC_BUTTON_CYL, &CMotionCtrl::OnBnClickedButtonCyl)
	ON_BN_CLICKED(IDC_BUTTON_PRES, &CMotionCtrl::OnBnClickedButtonPres)
	ON_BN_CLICKED(IDC_BUTTON_CANCALERROR, &CMotionCtrl::OnBnClickedButtonCancalerror)
END_MESSAGE_MAP()


// CMotionCtrl ��Ϣ�������


void CMotionCtrl::OnBnClickedButtonComfirm()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	GetDlgItem(IDC_EDIT_INITSPD)->GetWindowText(str);
	g.ini.m_FstMtr.InitSpeed = _ttoi(str);
	GetDlgItem(IDC_EDIT_MAXSPD)->GetWindowText(str);
	g.ini.m_FstMtr.MaxSpeed= _ttoi(str);
	GetDlgItem(IDC_EDIT_WAVES)->GetWindowText(str);
	g.ini.m_FstMtr.Waves = _ttoi(str);
	GetDlgItem(IDC_EDIT_TACC)->GetWindowText(str);
	g.ini.m_FstMtr.Tacc= _ttof(str);
	GetDlgItem(IDC_EDIT_ORIGIN_CMPS)->GetWindowText(str);
	g.ini.m_FstMtr.compansate = _ttoi(str);
	g.ini.SaveParaFile(PARA_IO);
}



BOOL CMotionCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���

	if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_FORWARD)->m_hWnd)
		{
			int SysState = SYSRUNNING | FST_MOTOR_NOT_ALREADY | SYSEMGENCY;//Ӱ���ԭ�������
			SysState = SysState&g.Controller.SysState;
			if (SysState)
				AfxMessageBox(g.ErrorString(SysState));
			else
			g.mc.MoveForward(FIRST_MOTOR,2*g.ini.m_FstMtr.MaxSpeed);
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_BACKWORD)->m_hWnd) //����
		{
			int SysState = SYSRUNNING | FST_MOTOR_NOT_ALREADY | SYSEMGENCY;//Ӱ���ԭ�������
			SysState = SysState&g.Controller.SysState;
			if (SysState)
				AfxMessageBox(g.ErrorString(SysState));
			else
			g.mc.MoveBackward(FIRST_MOTOR, -1*2 * g.ini.m_FstMtr.MaxSpeed);
		}

		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_FORWARD2)->m_hWnd)
		{
			int SysState = SYSRUNNING |  SND_MOTOR_NOT_ALREADY | SYSEMGENCY;//Ӱ���ԭ�������
			SysState = SysState&g.Controller.SysState;
			if (SysState)
				AfxMessageBox(g.ErrorString(SysState));
			else
			g.mc.MoveForward(SECOND_MOTOR, 2 * g.ini.m_SndMtr.MaxSpeed);
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_BACKWORD2)->m_hWnd) //����
		{
			int SysState = SYSRUNNING  | SND_MOTOR_NOT_ALREADY |SYSEMGENCY;//Ӱ���ԭ�������
			SysState = SysState&g.Controller.SysState;
			if (SysState)
				AfxMessageBox(g.ErrorString(SysState));
			else
			g.mc.MoveBackward(SECOND_MOTOR,-1* 2 * g.ini.m_SndMtr.MaxSpeed);
		}
	}

	if (pMsg->message == WM_LBUTTONUP)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_FORWARD)->m_hWnd)
		{
			g.mc.Stop(FIRST_MOTOR);
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_BACKWORD)->m_hWnd)
		{
			g.mc.Stop(FIRST_MOTOR);
		}

		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_FORWARD2)->m_hWnd)
		{
			g.mc.Stop(SECOND_MOTOR);
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_BACKWORD2)->m_hWnd)
		{
			g.mc.Stop(SECOND_MOTOR);
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMotionCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (1 == nIDEvent)
	{
		CString str;
		str.Format(_T("%d"), g.mc.Get_Position(FIRST_MOTOR));
		GetDlgItem(IDC_EDIT_POS)->SetWindowText(str);
		str.Format(_T("%d"), g.mc.Get_Position(SECOND_MOTOR));
		GetDlgItem(IDC_EDIT_POS2)->SetWindowText(str);
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CMotionCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetTimer(1, 33, NULL);
	CString str;
	str.Format(_T("%d"), g.ini.m_FstMtr.InitSpeed);
	GetDlgItem(IDC_EDIT_INITSPD)->SetWindowText(str);
	str.Format(_T("%d"), g.ini.m_FstMtr.MaxSpeed);
	GetDlgItem(IDC_EDIT_MAXSPD)->SetWindowText(str);
	str.Format(_T("%d"), g.ini.m_FstMtr.Waves);
	GetDlgItem(IDC_EDIT_WAVES)->SetWindowText(str);
	str.Format(_T("%f"), g.ini.m_FstMtr.Tacc);
	GetDlgItem(IDC_EDIT_TACC)->SetWindowText(str);
	str.Format(_T("%i"), g.ini.m_FstMtr.compansate);
	GetDlgItem(IDC_EDIT_ORIGIN_CMPS)->SetWindowText(str);

	str.Format(_T("%d"), g.ini.m_SndMtr.InitSpeed);
	GetDlgItem(IDC_EDIT_INITSPD2)->SetWindowText(str);
	str.Format(_T("%d"), g.ini.m_SndMtr.MaxSpeed);
	GetDlgItem(IDC_EDIT_MAXSPD2)->SetWindowText(str);
	str.Format(_T("%d"), g.ini.m_SndMtr.Waves);
	GetDlgItem(IDC_EDIT_WAVES2)->SetWindowText(str);
	str.Format(_T("%f"), g.ini.m_SndMtr.Tacc);
	GetDlgItem(IDC_EDIT_TACC2)->SetWindowText(str);
	str.Format(_T("%i"), g.ini.m_SndMtr.compansate);
	GetDlgItem(IDC_EDIT_ORIGIN_CMPS2)->SetWindowText(str);

	if (g.mc.Read_Output(OUT_CYL))
	{
		GetDlgItem(IDC_BUTTON_CYL)->SetWindowText(L"��������");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_CYL)->SetWindowText(L"�����½�");
	}

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (g.mc.Read_Output(OUT_PRESSURE))
	{
		GetDlgItem(IDC_BUTTON_PRES)->SetWindowText(L"������");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PRES)->SetWindowText(L"����");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CMotionCtrl::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	KillTimer(1);
	CDialogEx::OnClose();
}


void CMotionCtrl::OnBnClickedButtonSavepos()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	g.ini.m_FstMtr.Waves = g.mc.Get_Position(FIRST_MOTOR);
	str.Format(_T("%d"), g.ini.m_FstMtr.Waves);
	g.ini.SaveParaFile(PARA_IO);
	GetDlgItem(IDC_EDIT_WAVES)->SetWindowText(str);
}


void CMotionCtrl::OnBnClickedButtonSetzero()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g.mc.SetMotorZero(FIRST_MOTOR);
	OnBnClickedButtonSavepos();
}



void CMotionCtrl::OnBnClickedButtonSavepos2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	g.ini.m_SndMtr.Waves = g.mc.Get_Position(SECOND_MOTOR);
	str.Format(_T("%d"), g.ini.m_SndMtr.Waves);
	g.ini.SaveParaFile(PARA_IO);
	GetDlgItem(IDC_EDIT_WAVES2)->SetWindowText(str);
}


void CMotionCtrl::OnBnClickedButtonSetzero2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g.mc.SetMotorZero(SECOND_MOTOR);
	OnBnClickedButtonSavepos2();
}


void CMotionCtrl::OnBnClickedButtonComfirm2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	GetDlgItem(IDC_EDIT_INITSPD2)->GetWindowText(str);
	g.ini.m_SndMtr.InitSpeed = _ttoi(str);
	GetDlgItem(IDC_EDIT_MAXSPD2)->GetWindowText(str);
	g.ini.m_SndMtr.MaxSpeed = _ttoi(str);
	GetDlgItem(IDC_EDIT_WAVES2)->GetWindowText(str);
	g.ini.m_SndMtr.Waves = _ttoi(str);
	GetDlgItem(IDC_EDIT_TACC2)->GetWindowText(str);
	g.ini.m_SndMtr.Tacc = _ttof(str);
	GetDlgItem(IDC_EDIT_ORIGIN_CMPS2)->GetWindowText(str);
	g.ini.m_SndMtr.compansate = _ttoi(str);
	g.ini.SaveParaFile(PARA_IO);
}


void CMotionCtrl::OnBnClickedButtonHomerun()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (SYSEMGENCY == (SYSEMGENCY&g.Controller.SysState)) return;
	if (isHomeThread) return;
	pHomeThread = AfxBeginThread(HomeThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	return ;
}


UINT CMotionCtrl::HomeThread(LPVOID lp)
{
	CMotionCtrl *p = (CMotionCtrl*)lp;
	p->isHomeThread = true;
	g.mc.BackToOrigin(FIRST_MOTOR, g.ini.m_FstMtr.InitSpeed, g.ini.m_FstMtr.MaxSpeed, g.ini.m_FstMtr.Tacc, g.ini.m_FstMtr.compansate);
	p->isHomeThread = false;
	return 0;
}

void CMotionCtrl::OnBnClickedButtonHomerun2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (SYSEMGENCY == (SYSEMGENCY&g.Controller.SysState)) return;
	if (isHomeThread1) return;
	pHomeThread = AfxBeginThread(HomeThread1, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	return;
}


UINT CMotionCtrl::HomeThread1(LPVOID lp)
{
	CMotionCtrl *p = (CMotionCtrl*)lp;
	p->isHomeThread1 = true;
	g.mc.BackToOrigin(SECOND_MOTOR, g.ini.m_SndMtr.InitSpeed, g.ini.m_SndMtr.MaxSpeed, g.ini.m_SndMtr.Tacc, g.ini.m_SndMtr.compansate);
	p->isHomeThread1 = false;
	return 0;
}

void CMotionCtrl::OnBnClickedButtonCyl()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (!g.mc.Read_Output(OUT_CYL))
	{ 
		g.mc.Write_Output(OUT_CYL, ON);
		GetDlgItem(IDC_BUTTON_CYL)->SetWindowText(L"��������");
	}
	else
	{
		g.mc.Write_Output(OUT_CYL, OFF);
		GetDlgItem(IDC_BUTTON_CYL)->SetWindowText(L"�����½�");
	}
}


void CMotionCtrl::OnBnClickedButtonPres()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (!g.mc.Read_Output(OUT_PRESSURE))
	{
		g.mc.Write_Output(OUT_PRESSURE, ON);
		GetDlgItem(IDC_BUTTON_PRES)->SetWindowText(L"������");
	}
	else
	{
		g.mc.Write_Output(OUT_PRESSURE, OFF);
		GetDlgItem(IDC_BUTTON_PRES)->SetWindowText(L"����");
	}
}


void CMotionCtrl::OnBnClickedButtonCancalerror()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g.evtEmgencyStop.ResetEvent();
}
