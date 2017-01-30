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

// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once

#include "Operation.h"
#include "BottomDlg.h"
#include "TopCamDlg.h"
#include "MainControl.h"
#include "TabControl.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg LRESULT OnResizeWin(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);


public:
	CSplitterWnd   m_wndSplitter; //������
	CSplitterWnd   m_childSplitter; //�Ӵ���
	CView* ImageVisionView; //= (CView*)MainFrame->m_wndSplitter1.GetPane(2, 0);
	CView* ControlView;
	CView* TopView;
	CView* BottomView;
	int ControlWidth;
	int ViewWidth;
	int ViewHeight;
	static BOOL ResizeWin(CMainFrame *pM);
	static BOOL ResizeWin1(CMainFrame *pM);
	void Quit();
	void Visible();


	//CTabCtrl		m_TabCtrl;
	CTabControl		m_TabCtrl;
	COperation		m_DockOperation;


	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


