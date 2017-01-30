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
	//��ȡ�������λ��
	CString text;
	GetWindowText (text);
	
	if (isdigit(nChar)) 
	//������������ͣ��Ͳ���Ҫ������д�����
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	else if (nChar == '-')
	//���ڡ�-����Ҫ�������֣�
	//���ڡ�-�������ڵ�һ��λ�ã���Ϊ�Ǹ������������룻
	//���ڡ�-�������ڡ�e�����ߡ�E���ĺ��棬��Ϊ��ָ���������룻
	//����������������롣
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
	//���ڡ�.��ҲҪ�������֣�
	//����ֻ����һ�Σ���Ϊ��С�����������룻
	//���ֶ�Σ�����Ϊ���Ϸ������������룻
	//���ڡ�.�������ڡ�e�����ߡ�E���ĺ��棬��Ϊ���Ϸ������������롣
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
