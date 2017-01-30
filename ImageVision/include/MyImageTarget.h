#pragma once
#include "..\image\include\imgpro.h"
#include "ImageProc.h"
#include "SudBitmap.h"
//#include "..\..\BlobImage\BlobImage\include\SudBitmap.h"

#ifdef _DEBUG
#pragma comment(lib,"BlobImage.lib")
#else
#pragma comment(lib,"BlobImage.lib")
#endif

typedef struct lineproperty
{
	int xpos;
	int ypos;
	UINT length;
	lineproperty()
	{
		xpos=-1;
		ypos=-1;
		length=0;
	}
	int orders;
}LINEEDGEPOINT;


typedef struct LINE_STRUCT
{
	//y=ax+b
	LINE_STRUCT()
	{
		a=0;
		b=0;
	}

	double a;
	double b;
}MYLINE;


class CMyImageTarget :
	public CImgPro
{
public:
	CMyImageTarget(LPBYTE lpBits,LONG lWidth,LONG lHeight);
	~CMyImageTarget(void);


public:
	int m_nESCPixel;
	int m_idOfThreshold;
	int m_iThreshNoiseLow,m_iThreshNoiseHi;
	int m_iFrontPixelMargin,m_iBackPixelMargin,m_iObjectPixelHiMargin;
	int m_idThreshNoise;

	LINEEDGEPOINT *edgePt;
	LINEEDGEPOINT *edgePtHW2B;
	LINEEDGEPOINT *edgePtVW2B;
	LINEEDGEPOINT *edgePtVB2W;

	MYLINE linetop,linebottm,lineleft,lineright;
public:
	int GetImageResult(int &Pixel,unsigned char iWin=250,unsigned char iThreshold=0);
	int FindObjectRect(SRect_I srect,int i_Step,int DifferRange);
	int GetLinePointFromBin(Bin linedataH1,Bin linedataH2,Bin linedataV1,Bin linedataV2);
	BOOL linearReg_Y_a0_a1X(double &a0, double &a1,LINEEDGEPOINT *linepoint,int size);
	BOOL linearReg_X_a0_a1Y(double &a0, double &a1,LINEEDGEPOINT *linepoint,int size);
	BOOL FilterFitLine(POINT *pInput, long lDataSize, 
						 double *dStartY, double *dEndY);
	BOOL LAx_B(double &a0, double &a1,POINT *linepoint,int size);
	int DrawLine(MYLINE line);
	void SetFlag(MYLINE _linetop,MYLINE _linebottm,MYLINE _lineleft,MYLINE _lineright);
};

