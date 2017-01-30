// CircleSearch.cpp: implementation of the CCircleSearch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "../include/CircleSearch.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCircleSearch::CCircleSearch()
{
	m_SearchWnd.point1.x=0;
	m_SearchWnd.point1.y=0;
	m_SearchWnd.point2.x=0;
	m_SearchWnd.point2.y=0;
	m_TempWnd.point1.x=0;
	m_TempWnd.point1.y=0;
	m_TempWnd.point2.x=0;
	m_TempWnd.point2.y=0;
	m_nTempHeight=0;
	m_nTempWidth=0;
	m_nSearchHeight=0;
	m_nSearchWidth=0;
	m_Cirque.DeltaX=0;
	m_Cirque.DeltaY=0;
	m_Cirque.InnerR=0;
	m_Cirque.OuterR=0;
	m_nNoise=0;
	m_nArea=0;
	m_nLowhigh=0;
	m_fAreaSpec=0.0;
	m_nTotalRange=0;
	m_dTargetMatchRate=0.9;
	m_dActualMatchRate=0.0;
	for(int i=0;i<ImageHeight;i++)
	{
		for(int j=0;j<ImageWidth;j++)
			m_TempArray[i][j]=0;
	}
}

CCircleSearch::~CCircleSearch()
{

}

BOOL CCircleSearch::MeasureTemplate(SPoint_I& matchpnt,unsigned char *ImagePointer)
{
	matchpnt.x=0;
	matchpnt.y=0;
	double dMatchNum=0.0;
	if(ImagePointer==NULL)
	{
		return FALSE;
	}
	matchpnt=matchGravityValueImage(ImagePointer,m_TempArray,
		     m_SearchWnd,m_TempWnd,m_nLowhigh,&dMatchNum);
	m_dActualMatchRate=1-dMatchNum;
	if((matchpnt.x>639)||(matchpnt.x<=0)||(matchpnt.y>479)||(matchpnt.y<=0)||(m_dActualMatchRate<m_dTargetMatchRate))
		{
			matchpnt.x=0;
			matchpnt.y=0;
			return FALSE;
		}
	return TRUE;
}

BOOL CCircleSearch::MeasureCircle(SCircle_F& matchPoint,unsigned char *ImagePointer)
{
	double dMatchNum=0.0;
	float fTargetMatchRate=(float)m_dTargetMatchRate;
    SPoint_I matchpnt;
	SPoint_I Actpnt;
    
	matchPoint.a=0;
	matchPoint.b=0;
	matchPoint.r=0;
	matchPoint.key=false;
    float ActArea=0;
	matchpnt=matchGravityValueImage(ImagePointer,m_TempArray,
		     m_SearchWnd,m_TempWnd,m_nLowhigh,&dMatchNum);
	m_dActualMatchRate=1-dMatchNum;
	if((matchpnt.x>639)||(matchpnt.x<=0)||(matchpnt.y>479)||(matchpnt.y<=0)||(m_dActualMatchRate<m_dTargetMatchRate))
	{
		matchPoint.key=false;
		matchPoint.a=0;
		matchPoint.b=0;
		matchPoint.r=0;
		return FALSE;
	}
	Actpnt.x=matchpnt.x+m_Cirque.DeltaX;
	Actpnt.y=matchpnt.y+m_Cirque.DeltaY;
	if((Actpnt.x+m_Cirque.OuterR)>639 || (Actpnt.y+m_Cirque.OuterR)>479 ||
		(Actpnt.x-m_Cirque.OuterR)<1 || (Actpnt.y-m_Cirque.OuterR)<1 )
	{
		matchPoint.key=false;
		matchPoint.a=0;
		matchPoint.b=0;
		matchPoint.r=0;
		return FALSE;
	}
	else
	{
		matchPoint=CircleFitting(ImagePointer,Actpnt,0,360,
			m_Cirque.InnerR,m_Cirque.OuterR,m_nNoise,
			m_nTotalRange,fTargetMatchRate,m_dActualMatchRate);

		if((matchPoint.a==0.0) && (matchPoint.b==0.0) && (matchPoint.key==false))
		{
			return FALSE;
		}
		else
		{
			ActArea=(float)((matchPoint.r*matchPoint.r)*PI);
			matchPoint.key=true;
		}
	}
	return TRUE;
}

BOOL CCircleSearch::SetTargetMatchRate(double dRate)
{
	if((dRate<=0)||(dRate>1))
		return FALSE;
	m_dTargetMatchRate=dRate;
	return TRUE;
}

double CCircleSearch::GetActualMatchRate()
{
	return m_dActualMatchRate;
}

void CCircleSearch::SetSearchWnd(int x1, int y1, int x2, int y2)
{
	if((x1>=x2) || (y1>=y2))
	{
		AfxMessageBox(L"Parameters Error!,Can't Set");
		return;
	}
	m_SearchWnd.point1.x=x1;
	m_SearchWnd.point1.y=y1;
	m_SearchWnd.point2.x=x2;
	m_SearchWnd.point2.y=y2;
	m_nSearchWidth=x2-x1;
	m_nSearchHeight=y2-y1;
}

void CCircleSearch::SetSearchWnd(CRect rc)
{
	m_SearchWnd.point1.x=rc.left;
	m_SearchWnd.point1.y=rc.top;
	m_SearchWnd.point2.x=rc.right;
	m_SearchWnd.point2.y=rc.bottom;
	m_nSearchWidth=rc.Width();
    m_nSearchHeight=rc.Height();
}

void CCircleSearch::SetTempWnd(int x1, int y1, int x2, int y2)
{
	if((x1>=x2) || (y1>=y2))
	{
		AfxMessageBox(L"Parameters Error!");
		return;
	}
	m_TempWnd.point1.x=x1;
	m_TempWnd.point1.y=y1;
	m_TempWnd.point2.x=x2;
	m_TempWnd.point2.y=y2;
	m_nTempWidth=x2-x1;
	m_nTempHeight=y2-y1;
}
void CCircleSearch::SetTempWnd(CRect rc)
{
	m_TempWnd.point1.x=rc.left;
	m_TempWnd.point1.y=rc.top;
	m_TempWnd.point2.x=rc.right;
	m_TempWnd.point2.y=rc.bottom;
	m_nTempWidth=rc.Width();
	m_nTempHeight=rc.Height();
}

void CCircleSearch::SetCircleFitParameters(int nDif,int nTotalDif)
{
	m_nNoise=nDif;
	m_nTotalRange=nTotalDif;
}

void CCircleSearch::SetCircleParameter(int nXc, int nYc, int nInnerR, int nOuterR)
{
	m_Cirque.InnerR=nInnerR;
	m_Cirque.OuterR=nOuterR;
	m_Cirque.DeltaX=nXc-m_TempWnd.point1.x;
	m_Cirque.DeltaY=nYc-m_TempWnd.point1.y;
}

void CCircleSearch::SetThreshold(int nThreshold)
{
	if((nThreshold>=0)&&(nThreshold<=255))
		m_nLowhigh=nThreshold;
	else
		AfxMessageBox(L"Value error!");
}

int CCircleSearch::GetThreshold()
{
	return m_nLowhigh;
}

void CCircleSearch::SetTemplate(unsigned char *ImageArray)
{
	if(ImageArray==NULL)
		return;
	CopyMemory(m_TempArray,ImageArray,307200);
}

CRect CCircleSearch::GetSearchWnd()
{
	CRect rc;
	rc.left=m_SearchWnd.point1.x;
	rc.top=m_SearchWnd.point1.y;
	rc.right=m_SearchWnd.point2.x;
	rc.bottom=m_SearchWnd.point2.y;
	return rc;
}

CRect CCircleSearch::GetTemplateWnd()
{
	CRect rc;
	rc.left=m_TempWnd.point1.x;
	rc.top=m_TempWnd.point1.y;
	rc.right=m_TempWnd.point2.x;
	rc.bottom=m_TempWnd.point2.y;
	return rc;
}

void CCircleSearch::TwoValueImage(unsigned char* ImageP)
{
	if(ImageP==NULL)
		return;
	SetLowhigh(ImageP,m_nLowhigh);
}

void CCircleSearch::ShowMeasureResult(SPoint_I& Point,CDC *pTargetDC,int nXPos,int nYPos)
{
	if(pTargetDC==NULL)
		return;
	CString szResult;
	szResult.Format(L"x=%d,y=%d,matchrate=%.3f",Point.x,Point.y,m_dActualMatchRate);
	pTargetDC->TextOut(nXPos,nYPos,szResult);
	CPen NewPen(PS_SOLID,0,RGB(0,255,0));
	CPen* oldPen=pTargetDC->SelectObject(&NewPen);
	SRect_I rect;
	rect.point1.x=Point.x;
	rect.point1.y=Point.y;
	rect.point2.x=Point.x+m_nTempWidth;
	rect.point2.y=Point.y+m_nTempHeight;
	pTargetDC->MoveTo(rect.point1.x,rect.point1.y);
	pTargetDC->LineTo(rect.point2.x,rect.point1.y);
	pTargetDC->LineTo(rect.point2.x,rect.point2.y);
	pTargetDC->LineTo(rect.point1.x,rect.point2.y);
	pTargetDC->LineTo(rect.point1.x,rect.point1.y);
	pTargetDC->SelectObject(oldPen);
}

void CCircleSearch::ShowMeasureResult(SCircle_F& matchPoint,CDC* pTargetDC,int nXPos,int nYPos)
{
	if(pTargetDC==NULL)
		return;
	CString szResult;
	szResult.Format(L"x=%.3f,y=%.3f,matchrate=%.3f",matchPoint.a,matchPoint.b,m_dActualMatchRate);
	pTargetDC->TextOut(nXPos,nYPos,szResult);
	CPen NewPen(PS_SOLID,0,RGB(0,255,0));
	CPen* oldPen=pTargetDC->SelectObject(&NewPen);
	pTargetDC->SetBkMode(TRANSPARENT);
	CBrush newBrush;
	LOGBRUSH logbrush;
	logbrush.lbColor=RGB(255,0,0);
	logbrush.lbStyle=BS_NULL;
	newBrush.CreateBrushIndirect(&logbrush);
	CBrush* oldbrush=pTargetDC->SelectObject(&newBrush);
	pTargetDC->MoveTo(matchPoint.a-5,matchPoint.b);
	pTargetDC->LineTo(matchPoint.a+5,matchPoint.b);
	pTargetDC->MoveTo(matchPoint.a,matchPoint.b-5);
	pTargetDC->LineTo(matchPoint.a,matchPoint.b+5);
	pTargetDC->Ellipse(matchPoint.a-matchPoint.r,matchPoint.b-matchPoint.r,matchPoint.a+matchPoint.r,matchPoint.b+matchPoint.r);
	pTargetDC->SelectObject(oldPen);
	pTargetDC->SelectObject(oldbrush);
}