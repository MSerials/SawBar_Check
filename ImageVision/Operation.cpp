// Operation.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageVision.h"
#include "Operation.h"
#include <iostream>
using namespace std;
//#include "MainFrm.h"

// COperation

IMPLEMENT_DYNAMIC(COperation, CDockablePane)

COperation::COperation()
{
	//pMainFrame = NULL;
	m_state = 0;
}

COperation::~COperation()
{
	
}


BEGIN_MESSAGE_MAP(COperation, CDockablePane)
	ON_WM_PAINT()
//	ON_WM_KILLFOCUS()
//ON_WM_CLOSE()
//ON_WM_DESTROY()
//ON_WM_SHOWWINDOW()
ON_WM_TIMER()
END_MESSAGE_MAP()



// COperation ��Ϣ�������




void COperation::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDockablePane::OnPaint()
	//CDockablePane::OnPaint();
	SetTimer(1, 2, NULL);
		return;
	CDockablePane::OnPaint();
}



void COperation::OnPressCloseButton()
{
	//return;
	Lock lck;
	m_state = 0;
	SetTimer(1, 1, NULL);
	return CDockablePane::OnPressCloseButton();
}

void COperation::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (1 == nIDEvent)
	{
		Lock lck;
		KillTimer(1);
		CWnd *pWnd = AfxGetApp()->m_pMainWnd;
		if (!pWnd) return;
		::SendMessage(pWnd->m_hWnd, WMMSG_RESIZEWIN, 1, 1);
	}
	CDockablePane::OnTimer(nIDEvent);
}
