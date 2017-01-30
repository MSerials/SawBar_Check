// CircleSearch.h: interface for the CCircleSearch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIRCLESEARCH_H__6A828299_D152_435C_897E_34488FD6C99F__INCLUDED_)
#define AFX_CIRCLESEARCH_H__6A828299_D152_435C_897E_34488FD6C99F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageProc.h"

class CCircleSearch : public CImageProc  
{
public:
	CCircleSearch();
	virtual ~CCircleSearch();

public:
	void TwoValueImage(unsigned char* ImageP=NULL);
	CRect GetTemplateWnd();
	CRect GetSearchWnd();
	void SetTemplate(unsigned char *ImageArray=NULL);
	int GetThreshold();
	void SetThreshold(int nThreshold=100);
	void SetCircleParameter(int nXc=0,int nYc=0,int nInnerR=5,int nOuterR=10);
	void SetCircleFitParameters(int nDif=10,int nTotalDif=100);
	void SetTempWnd(int x1=0,int y1=0,int x2=50,int y2=50);
	void SetTempWnd(CRect rc);
	void SetSearchWnd(int x1=0,int y1=0,int x2=100,int y2=100);
	void SetSearchWnd(CRect rc);
	double GetActualMatchRate();
	BOOL SetTargetMatchRate(double dRate=0.9);
	BOOL MeasureCircle(SCircle_F& matchPoint,unsigned char* ImagePointer=NULL);
	BOOL MeasureTemplate(SPoint_I& matchpnt,unsigned char*  ImagePointer=NULL);
public:
	double m_dTargetMatchRate;
	double m_dActualMatchRate;
	SRect_I m_SearchWnd;
	SRect_I m_TempWnd;
	Cirque m_Cirque;
	int m_nNoise;
	int m_nArea;
	int m_nLowhigh;
	float m_fAreaSpec;
	int m_nTotalRange;
public:
	void ShowMeasureResult(SPoint_I& Point,CDC* pTargetDC=NULL,int nXPos=0,int nYPos=0);
	void ShowMeasureResult(SCircle_F& matchPoint,CDC* pTargetDC=NULL,int nXPos=0,int nYPos=0);
	unsigned char m_TempArray[ImageHeight][ImageWidth];
private:
	int m_nTempHeight;
	int m_nTempWidth;
	int m_nSearchHeight;
	int m_nSearchWidth;
};

#endif // !defined(AFX_CIRCLESEARCH_H__6A828299_D152_435C_897E_34488FD6C99F__INCLUDED_)
