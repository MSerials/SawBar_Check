// ImageProc.h: interface for the CImageProc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROC_H__7FE91610_9388_40F0_BA1B_E08D1AFC77BA__INCLUDED_)
#define AFX_IMAGEPROC_H__7FE91610_9388_40F0_BA1B_E08D1AFC77BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define ImageWidth   640
#define ImageHeight  480
//#define PI           3.1415926535

typedef struct
{
	float a;
	float b;
	float r;
	bool key;
}SCircle_F;

typedef struct 
{
	int InnerR;
	int OuterR;
	int DeltaX;
	int DeltaY;
}Cirque;

typedef struct 
{
	int x;
	int y;
}SPoint_I;

typedef struct 
{
	float x;
	float y;
}SPoint_F;

typedef struct 
{
	SPoint_I point1;
	SPoint_I point2;
}SRect_I;

typedef struct 
{
    SPoint_F point;
	float angle;
	bool Key;
}SX_Y_Angle;

typedef struct 
{
	SPoint_I point1;
	SPoint_I point2;
	float a;
	float b;
	bool Symbol;
	float Std;
}SLine_F;


class CImageProc  
{
public:
    // 模板匹配函数(no rotate/zoom)
     SPoint_I matchGravityValueImage(unsigned char *ImageArray,
		unsigned char TempArray[ImageHeight][ImageWidth],
		SRect_I SearchWnd,SRect_I TempWnd,
		int Lowhigh,double *dMatchPixelRate);
    // 找水平直线
     SLine_F FindHline(unsigned char *XXX,
				SRect_I srect, int DifferRange);
    //找垂直直线
	 SLine_F FindVline(unsigned char *ImageArray,
			    SRect_I srect,int DifferRange);

	SLine_F FindHlineW2B(unsigned char *XXX,
			                 SRect_I srect, 
							 int DifferRange);
	SLine_F FindHlineB2W(unsigned char *XXX,
			                 SRect_I srect, 
							 int DifferRange);
	 SLine_F FindVlineB2W(unsigned char *ImageArray,
			    SRect_I srect,int DifferRange);

	 SLine_F FindVlineW2B(unsigned char *ImageArray,
			    SRect_I srect,int DifferRange);



	//边缘提取-Sobel 算子
	void Sobel(unsigned char*ImagePointer,SRect_I HoughWnd);
	//细化
	void Thinning(unsigned char*ImagePointer,SRect_I HoughWnd);
	//Hough变换提取直线
	SLine_F HoughLine(unsigned char *ImagePointer,SRect_I HoughWnd,
		double *mtsin,double *mtcos);
    //拟和圆函数
    SCircle_F CircleFitting(unsigned char *ImageArray,
			    SPoint_I point,float StartAngle,
				float EndAngle,int InnerCR,
				int OuterCR,int GrayRange,
				int TotalDiffRange,
				float fTargetRate,double& dActualRate);
    SPoint_F FindDiagonalWtoB(unsigned char *ImageArray,
		        float Angle,int r,int R,
				SPoint_F CenterPos,int Range,
				int DiffRange);
    SCircle_F CircleFitToData(SPoint_F Points[],
			    int validnum,int FitNumber);

	SCircle_F CircleWeight(unsigned char *ImagePointer,
				int Lowhigh,SPoint_I CircleCenter,int OuterR);

	inline BOOL CheckRange(SRect_I wnd);
	inline BOOL CheckTempRange(SRect_I wnd);

public:
	CImageProc();
	virtual ~CImageProc();
	void SetLowhigh(unsigned char*ImagePointer,int lowhigh);

};

#endif // !defined(AFX_IMAGEPROC_H__7FE91610_9388_40F0_BA1B_E08D1AFC77BA__INCLUDED_)
