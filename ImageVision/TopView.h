#pragma once


// CTopView ��ͼ

class CTopView : public CView
{
	DECLARE_DYNCREATE(CTopView)

protected:
	CTopView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CTopView();

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	HWND get_handle(){ return this->m_hWnd; }
	CRect get_rect(){ CRect rect; 
	GetClientRect(&rect); return rect; }
#ifdef _HALCON
	
#endif
	afx_msg void OnPaint();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

extern CTopView* pTview;

