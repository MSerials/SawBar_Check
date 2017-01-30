// BottomView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageVision.h"
#include "BottomView.h"


// CBottomView

IMPLEMENT_DYNCREATE(CBottomView, CView)

CBottomView * pBview = NULL;

CBottomView::CBottomView()
{
	pBview = this;
}

CBottomView::~CBottomView()
{
}

BEGIN_MESSAGE_MAP(CBottomView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CBottomView ��ͼ

void CBottomView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  �ڴ���ӻ��ƴ���
}


// CBottomView ���

#ifdef _DEBUG
void CBottomView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CBottomView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBottomView ��Ϣ�������


void CBottomView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (g.Controller.ROI_rect)
	{
	case BROI_RECT: break;
	case BCHAR_RECT:break;
	default:
		break;
	}
	/*
	if (g.isRect)
	{
		g.isRect = false;
		get_mfc_roi();
		g.delay_display = true;
		return;
	}
	*/
	CView::OnLButtonDown(nFlags, point);
}


void CBottomView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (NO_RECT != g.Controller.ROI_rect) return;
#ifdef _HALCON
	if (IDYES != MessageBox(L"�Ƿ񱣴���ʾ���ͼƬ", L"", MB_YESNO | MB_ICONQUESTION)) return;

	CString str;
	SYSTEMTIME st;
	CString strDate, strTime;
	GetLocalTime(&st);
	strDate.Format(_T("%4d-%d-%d_"), st.wYear, st.wMonth, st.wDay);
	strTime.Format(_T("%d-%d-%d"), st.wHour, st.wMinute, st.wSecond);
	str = strDate + strTime;

	//��ȡ·��
	CString AppPath;
	::GetModuleFileName(GetModuleHandle(NULL), AppPath.GetBuffer(300), 300);
	AppPath.ReleaseBuffer();
	AppPath = AppPath.Left(AppPath.ReverseFind('\\'));
	AppPath = AppPath + L"\\"+ g.prjini.m_Prj_Name+ L"\\"+ L"Image";
	CreateDirectory(AppPath, NULL);
	str = AppPath + L"\\" + str + L".bmp";
	HException::InstallHHandler(&CPPExpDefaultExceptionHandler);
	try{
		Halcon::write_image(g.hRawImage1, "bmp", NULL, (char*)(LPCSTR)(CStringA)str);
	}
	catch (HException &CPPExpDefaultExceptionHandler)
	{
	}
#endif
	CView::OnRButtonDown(nFlags, point);
	ClientToScreen(&point);
	OnContextMenu(this, point);

}


void CBottomView::OnPaint()
{
	CPaintDC dc(this);
	return;
	if ((GetTickCount() - g.RunningTick) < 50000) CPaintDC dc(this);
	CRect rt;
	GetClientRect(&rt);
	g.show_image.open_window_extend1(rt);
	 // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CView::OnPaint()
}


void CBottomView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CView::OnRButtonUp(nFlags, point);

	ClientToScreen(&point);
	OnContextMenu(this, point);
	//CView::OnRButtonUp(nFlags, point);
}
