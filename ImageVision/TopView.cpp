// TopView.cpp : 实现文件
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


// CTopView 绘图

void CTopView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
}


// CTopView 诊断

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


// CTopView 消息处理程序


void CTopView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
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
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (NO_RECT != g.Controller.ROI_rect) return;
#ifdef _HALCON
	if (IDYES != MessageBox(L"是否保存显示框一图片", L"", MB_YESNO | MB_ICONQUESTION)) return;
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
		AfxMessageBox(L"保存失败");
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
	// TODO:  在此处添加消息处理程序代码
	
	// 不为绘图消息调用 CView::OnPaint()
}


void CTopView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CView::OnRButtonUp(nFlags, point);

	ClientToScreen(&point);
	OnContextMenu(this, point);
	//CView::OnRButtonUp(nFlags, point);
}
