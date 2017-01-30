// TabControl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageVision.h"
#include "TabControl.h"


// CTabControl

IMPLEMENT_DYNAMIC(CTabControl, CTabCtrl)

CTabControl::CTabControl()
{

}

CTabControl::~CTabControl()
{
}


BEGIN_MESSAGE_MAP(CTabControl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CTabControl::OnTcnSelchange)
END_MESSAGE_MAP()



// CTabControl ��Ϣ�������




void CTabControl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	switch (GetCurSel())
	{
	case 0:
		m_MainCtrl.ShowWindow(SW_SHOW);
		m_TopDlg.ShowWindow(SW_HIDE);
		m_BottomDlg.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_MainCtrl.ShowWindow(SW_HIDE);
		m_BottomDlg.ShowWindow(SW_HIDE);
		m_TopDlg.ShowWindow(SW_SHOW);
		break;
	case 2:
		m_MainCtrl.ShowWindow(SW_HIDE);
		m_TopDlg.ShowWindow(SW_HIDE);
		m_BottomDlg.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
	*pResult = 0;
	return;
}


void CTabControl::InitData()
{
	InsertItem(0, _T("��Ŀ"));
	InsertItem(1, _T("����"));
	InsertItem(2, _T("�ײ�"));
	CRect rt;
	GetClientRect(&rt);	
	rt.top += 25;
	rt.bottom -= 1;
	rt.left += 1;
	rt.right -= 1;
	CTabControl* t = this;
	m_MainCtrl.Create(IDD_DIALOG_MAINCONTROL,t);
	m_TopDlg.Create(IDD_DIALOG_TOPCAM, t);
	m_BottomDlg.Create(IDD_DIALOG_BOTTOMCAM, t);
	m_MainCtrl.MoveWindow(rt);
	m_TopDlg.MoveWindow(rt);
	m_BottomDlg.MoveWindow(rt);
	m_MainCtrl.ShowWindow(SW_SHOW);
	SetCurSel(0);
}