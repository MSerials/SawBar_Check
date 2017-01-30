// NumEdit.cpp : implementation file
//
// By Mike Scanlon, 8/26/98

#include "stdafx.h"
#include "../include/NumEdit.h"



/////////////////////////////////////////////////////////////////////////////
// CNumEdit

CNumEdit::CNumEdit()
{
}

CNumEdit::~CNumEdit()
{
}

BEGIN_MESSAGE_MAP(CNumEdit, CEdit)
	//{{AFX_MSG_MAP(CNumEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNumEdit message handlers

void CNumEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == 8)
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	
	POINT caret;
	::GetCaretPos (&caret);
	caret.x = LOWORD (CharFromPos (caret));
	//获取输入光标的位置
	CString text;
	GetWindowText (text);
	
	if (isdigit(nChar)) 
	//如果是数字类型，就不需要额外进行处理了
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	else if (nChar == '-')
	//对于“-”，要进行区分：
	//对于“-”出现在第一个位置，认为是负数，允许输入；
	//对于“-”出现在“e”或者“E”的后面，认为是指数允许输入；
	//其它情况不允许输入。
	{
		if (!caret.x)
		{
			if (((text.GetLength() > 0) && (text[0]!='-')) || (text.GetLength()==0))
				CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		else
		{
			if ((text [caret.x-1] == 'e') || (text [caret.x-1] == 'E'))
				CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	}

	else if ((nChar == 'e') || (nChar == 'E'))
	{
		if ((caret.x == 1) && (text[0] == '-'))
			return ;

		if (caret.x)
		{
			for (int i=0; i<text.GetLength(); i++)
			{
				if ((text[i] == 'e') ||(text[i] == 'E'))
					return ;
			}
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	}

	else if (nChar == '.')
	//对于“.”也要进行区分：
	//对于只出现一次，认为是小数，允许输入；
	//出现多次，则认为不合法，不允许输入；
	//对于“.”出现在“e”或者“E”的后面，认为不合法，不允许输入。
	{
		int i=0;
		for (i=0; i<text.GetLength(); i++)
		{
			if (text[i] == '.')
				return ;
		}

		for (i=0; i<text.GetLength(); i++)
		{
			if (((text[i] == 'e') ||(text[i]=='E')) && (caret.x > i))
				return ;
		}

		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}


void CNumEdit::SetValue(double val)
{
	CString tmp;
	tmp.Format (L"%G",val);
	SetWindowText (tmp);
}

double CNumEdit::GetValue()
{
	CString tmp;
	GetWindowText (tmp);
	return _tcstod (tmp,NULL);
}
