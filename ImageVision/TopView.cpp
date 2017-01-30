// TopView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageVision.h"
#include "TopView.h"
#include "Tools.h"

// CTopView

IMPLEMENT_DYNCREATE(CTopView, CView)


CTopView* pTview = NULL;

CTopView::CTopView()
{
	pTview = this;
}

CTopView::~CTopView()
{
}

BEGIN_MESSAGE_MAP(CTopView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CTopView ��ͼ

void CTopView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  �ڴ���ӻ��ƴ���
}


// CTopView ���

#ifdef _DEBUG
void CTopView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CTopView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTopView ��Ϣ�������


void CTopView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (g.Controller.ROI_rect)
	{
	case TROI_RECT:break;
	case TCHAR_RECT:break;
	default:break;
	}
		
		
	CView::OnLButtonDown(nFlags, point);
}


void CTopView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (NO_RECT != g.Controller.ROI_rect) return;
#ifdef _HALCON
	if (IDYES != MessageBox(L"�Ƿ񱣴���ʾ��һͼƬ", L"", MB_YESNO | MB_ICONQUESTION)) return;
	win_tool wt;
	CString fileName = wt.Get_Date_Time();
	CString AppPath = wt.AppPath();
	AppPath = AppPath + L"\\" + g.prjini.m_Prj_Name + L"\\" + L"Image";
	CreateDirectory(AppPath, NULL);
	CString str = AppPath + L"\\" + fileName + L".bmp";

	HException::InstallHHandler(&CPPExpDefaultExceptionHandler);
	try{
		set_check("~give_error");
		Halcon::write_image(g.hRawImage, "bmp", NULL, (char*)(LPCSTR)(CStringA)str);
		g.show_image.Dis_Obj(g.hRawImage);
	}
	catch (HException &CPPExpDefaultExceptionHandler)
	{
		set_check("~give_error");
		AfxMessageBox(L"����ʧ��");
		TRACE(CPPExpDefaultExceptionHandler);
	}
#endif
	CView::OnRButtonDown(nFlags, point);
	ClientToScreen(&point);
	OnContextMenu(this, point);
}


void CTopView::OnPaint()
{
	CPaintDC dc(this);
	return;
	if ((GetTickCount() - g.RunningTick) < 5000) CPaintDC dc(this);
	CRect rt; 
	GetClientRect(&rt);
	g.show_image.open_window_extend(rt);
	 // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	
	// ��Ϊ��ͼ��Ϣ���� CView::OnPaint()
}


void CTopView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CView::OnRButtonUp(nFlags, point);

	ClientToScreen(&point);
	OnContextMenu(this, point);
	//CView::OnRButtonUp(nFlags, point);
}
