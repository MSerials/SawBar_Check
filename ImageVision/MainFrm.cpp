// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "ImageVision.h"
#include "MainFrm.h"
#include "control.h"
#include "TopView.h"
#include "BottomView.h"
#include "Tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
//	ON_MLRESULT OnResizeWin(WPARAM wParam, LPARAM lParam)S
	ON_MESSAGE(WMMSG_RESIZEWIN, &CMainFrame::OnResizeWin)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO:  �ڴ���ӳ�Ա��ʼ������



//��ȡini����ʼ����������ȵ�
	win_tool wt;
	CString AppPath;
	::GetModuleFileName(GetModuleHandle(NULL), AppPath.GetBuffer(300), 300);
	AppPath.ReleaseBuffer();
	CString PrjPath = AppPath.Left(AppPath.ReverseFind('\\'));
	AppPath = PrjPath + L"\\" + L"Configuration";
	//AfxMessageBox(AppPath);
	g.prjini.SetIniDir(AppPath, false);
	if (0 == g.prjini.SetIniFile(L"PrgParameter.ini"))
	{
		g.prjini.InitData();
		g.prjini.SaveParaFile(PARA_ALL);
	}
	g.prjini.LoadParaFile(PARA_ALL);

	if (g.prjini.m_Prj_Vector.IsEmpty()) { g.prjini.m_Prj_Vector = L"1"; g.prjini.SaveParaFile(PARA_PRJ); }
	vector<CString> strvec = wt.SplitCString(g.prjini.m_Prj_Vector,L",");

	if (g.prjini.m_Prj_index >= strvec.size()) { g.prjini.m_Prj_index = 0; g.prjini.SaveParaFile(PARA_PRJ); }
	g.prjini.m_Prj_Name = strvec[g.prjini.m_Prj_index];

	PrjPath += L"\\" + g.prjini.m_Prj_Name;
	g.ini.SetIniDir(PrjPath, false);
	if (0 == g.ini.SetIniFile(L"PrgParameter.ini"))
	{
		g.ini.InitData();
		g.ini.SaveParaFile(PARA_ALL);
	}
	g.ini.LoadParaFile(PARA_ALL);


	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
	
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;


	BOOL bNameValid;

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
	ModifyStyleEx(WS_EX_CLIENTEDGE, NULL);
	ModifyStyleEx(WS_CAPTION, NULL);

	//���ȳ�ʼ����ȡ��������Ϊ��Ժ���öԻ����ʼ������Ӱ��



	if (!m_DockOperation.Create(_T("�������"), this, CRect(0, 0, 400, 1080), TRUE, 1001, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI
		, AFX_CBRS_REGULAR_TABS, AFX_CBRS_CLOSE | AFX_CBRS_FLOAT
		))
	{
		TRACE0("Failed to create LeftPane\n");
		return -1;
	}

	m_DockOperation.LoadState(FALSE);
	m_DockOperation.SetAutoHideMode(FALSE,NULL,NULL,NULL);
	m_DockOperation.EnableDocking(CBRS_ALIGN_RIGHT|CBRS_ALIGN_LEFT);
	DockPane(&m_DockOperation);
	m_TabCtrl.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 1920, 1080), &m_DockOperation, 125);
	m_TabCtrl.InitData();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	m_strTitle = L"�㽭���������ܼ������޹�˾";
	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO:  �ڴ����ר�ô����/����û���
	CRect rect;
	GetWindowRect(&rect);
	ControlWidth = 0;
	const int TotalWidth = rect.right;
	ViewWidth = (TotalWidth - ControlWidth) / 2;
	ViewHeight = rect.Height();
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;
 
	//if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(control), CSize(rect.Width() / 4, rect.Height()), pContext))	return FALSE;
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CTopView), CSize(200, 300), pContext)) return FALSE;
	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CBottomView), CSize(200, 300), pContext)) return FALSE;
	
	m_wndSplitter.SetRowInfo(0, ViewHeight, 0);
	m_wndSplitter.SetRowInfo(1, ViewHeight, 0);
//	m_wndSplitter.SetRowInfo(2, ViewHeight, 0);

//	m_wndSplitter.SetColumnInfo(0, ControlWidth, 80);
	m_wndSplitter.SetColumnInfo(0, ViewWidth, 80);
	m_wndSplitter.SetColumnInfo(1, ViewWidth, 80);

	//ControlView = (CView*)this->m_wndSplitter.GetPane(0, 0);
	TopView = (CView*)this->m_wndSplitter.GetPane(0, 0);
	BottomView = (CView*)this->m_wndSplitter.GetPane(0, 1);
	
	g.show_image.set_wnd(pTview->m_hWnd);
	g.show_image.set_wnd1(pBview->m_hWnd);

	return TRUE;
	return CFrameWndEx::OnCreateClient(lpcs, pContext);
}

LRESULT CMainFrame::OnResizeWin(WPARAM wParam, LPARAM lParam)
{
	ResizeWin1(this);
	return 0;
}

BOOL CMainFrame::ResizeWin1(CMainFrame *pM)
{
	pM->DefWindowProc(WM_SYSCOMMAND, SC_MAXIMIZE, NULL);
	CRect rect;
	
	if (!pM->m_DockOperation.isVisible())
		pM->ControlWidth = 0;
	else if (pM->m_DockOperation.IsDocked() && pM->m_DockOperation.isVisible())
	{
		CSize csize = pM->m_DockOperation.CalcFixedLayout(NULL, NULL);
		pM->ControlWidth = 18 + csize.cx;
	}
	else
		pM->ControlWidth = 0;

	pM->GetWindowRect(&rect);
	const int TotalWidth = rect.right;
	pM->ViewWidth = (TotalWidth - pM->ControlWidth) / 2;
	pM->ViewHeight = rect.Height();
	pM->m_wndSplitter.SetRowInfo(0, pM->ViewHeight, 0);
	pM->m_wndSplitter.SetRowInfo(1, pM->ViewHeight, 0);
	pM->m_wndSplitter.SetColumnInfo(0, pM->ViewWidth, 80);
	pM->m_wndSplitter.SetColumnInfo(1, pM->ViewWidth, 80);
	pM->m_wndSplitter.RecalcLayout();

	g.show_image.open_window_extend(pTview->get_rect());
	g.show_image.open_window_extend1(pBview->get_rect());

	return TRUE;
}

BOOL CMainFrame::ResizeWin(CMainFrame *pM)
{
	pM->DefWindowProc(WM_SYSCOMMAND, SC_MAXIMIZE, NULL);
	pM->Visible();
	CRect rect;
	
	if (!pM->m_DockOperation.isVisible())
		pM->ControlWidth = 0;
	else if (pM->m_DockOperation.IsDocked() && pM->m_DockOperation.isVisible())
	{
		CSize csize = pM->m_DockOperation.CalcFixedLayout(NULL, NULL);
		pM->ControlWidth = 18 + csize.cx;
	}
	else
		pM->ControlWidth = 0;

	pM->GetWindowRect(&rect);
	const int TotalWidth = rect.right;
	pM->ViewWidth = (TotalWidth - pM->ControlWidth) / 2;
	pM->ViewHeight = rect.Height();
	pM->m_wndSplitter.SetRowInfo(0, pM->ViewHeight, 0);
	pM->m_wndSplitter.SetRowInfo(1, pM->ViewHeight, 0);
	pM->m_wndSplitter.SetColumnInfo(0, pM->ViewWidth, 80);
	pM->m_wndSplitter.SetColumnInfo(1, pM->ViewWidth, 80);
	pM->m_wndSplitter.RecalcLayout();

	g.show_image.open_window_extend(pTview->get_rect());
	g.show_image.open_window_extend1(pBview->get_rect());
	return TRUE;
}

void CMainFrame::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (IDYES != MessageBox(L"ȷ���˳�", L"", MB_YESNO | MB_ICONQUESTION)) return; 
	Quit();
	Sleep(300);
	CFrameWndEx::OnClose();
}

void CMainFrame::Quit()
{
	g.show_image.close();
	g.mc.close();
}

void CMainFrame::Visible()
{
	//m_DockOperation.GetParentFrame

	m_DockOperation.Show();
	return;

}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_SPACE)
		{
			//PostMessage(WM_KEYUP, VK_SPACE, 0);
			int state = SYSRUNNING | SYSSTOP;
			state = ~state & g.Controller.SysState;
			if (!state)
			{
				if (!g.evt_procedure.EventState())
					g.evt_procedure.SetEvent();
				else
					g.evt_procedure.ResetEvent();
			}
			else
			{
				AfxMessageBox(g.ErrorString(state));
			}
			return TRUE;
		}
	}
	return CFrameWndEx::PreTranslateMessage(pMsg);
}
