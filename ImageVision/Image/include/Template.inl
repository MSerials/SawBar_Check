// Template.cpp: implementation of the CTemplate class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "ImgPro.h"
inline CTemplate::CTemplate()
{
	m_hParent               = NULL;
	m_hTmp                  = NULL;
	m_pTmp                  = NULL;
	m_nRowLength            = 0;
	m_nWidth                = 0;
	m_nHeight               = 0;
	m_hSubsample            = NULL;
	m_nStepSize             = 3;
	m_nSampleCount          = 0;
	m_pSample               = NULL;
	m_maxContrast           = 0;
	m_minContrast           = 255 * 255;
	m_upperMeanGrayScale    = 0;
	m_lowerMeanGrayScale    = 255;
	m_meanGrayScale         = 0;
	m_nAngularStepSize      = 3;
	m_pExtraction           = NULL;
    m_nHaltonSamplesCount   = 0;
}

inline CTemplate::~CTemplate()
{
    Destroy();
}

inline BOOL CTemplate::Create(HBITMAP hParent, const CRect rect, BOOL bInitialize)
{
	Destroy();
	if(!hParent)
		return FALSE;

	CopyParent(hParent);

	if(!m_hParent)
		return FALSE;

    return PrivateCreate(rect, bInitialize);
}

inline void CTemplate::Destroy()
{
	if(m_hTmp)
	{
		::DeleteObject(m_hTmp);
		m_hTmp = NULL;
        m_pTmp = NULL;
	}

	if(m_hSubsample)
	{
		::GlobalFree(m_hSubsample);
		m_hSubsample = NULL;
	}

	if(m_pSample)
	{
		delete [] m_pSample;
		m_pSample=NULL;
		m_nSampleCount=0;
	}

	if(m_hParent)
	{
		::DeleteObject(m_hParent);
		m_hParent    = NULL;
	}

	if(m_pExtraction)
	{
		delete [] m_pExtraction;
        m_pExtraction = NULL;
	}

	m_nRowLength            = 0;
	m_nWidth                = 0;
	m_nHeight               = 0;
	m_nStepSize             = 3;
	m_maxContrast           = 0;
	m_minContrast           = 255 * 255;
	m_upperMeanGrayScale    = 0;
	m_lowerMeanGrayScale    = 255;
	m_meanGrayScale         = 0;
	m_nAngularStepSize      = 3;
    m_nHaltonSamplesCount   = 0;
}

inline BOOL CTemplate::Rotate(const double alpha)
{
	if(!m_hParent || !m_hTmp)
		return FALSE;

	int nWidthImg;
	int nHeightImg;
	BYTE* pByteImg;
	int nRowBytesImg;

	CBitmap cbm;
	BITMAP bm;
	cbm.Attach(m_hParent);
	cbm.GetBitmap(&bm);
	nWidthImg    = bm.bmWidth;
	nHeightImg   = bm.bmHeight;
	pByteImg     = (BYTE*)bm.bmBits;//原图像
	nRowBytesImg = ((bm.bmWidth * bm.bmBitsPixel + 31) & ~31)>>3;;
	cbm.Detach();

	//
	//Template window legality check.
	//
	double halfDiagonal  = sqrt(pow(m_nWidth/2.0,2)+pow(m_nHeight/2.0,2));
	double leftFarX      = m_rectTmp.left + m_nWidth/2 - halfDiagonal;
	double rightFarX     = m_rectTmp.left + m_nWidth/2 + halfDiagonal;
	double topFarY       = m_rectTmp.top + m_nHeight/2 - halfDiagonal;
	double bottomFarY    = m_rectTmp.top + m_nHeight/2 + halfDiagonal;
	if(leftFarX < 0 || rightFarX > nWidthImg - 1 || topFarY < 0 || bottomFarY > nHeightImg -1)
		return FALSE;

	int xA,yA;
	double xAs,yAs;
	CPoint pointA;
	double r,theta;
	double alfa= alpha*PI/180;
	int left,right,top,bottom;
	double a,b;
	//
	for(yA=0;yA<m_nHeight;yA++)
		for(xA=0;xA<m_nWidth;xA++)
		{
			pointA.x=xA-m_nWidth/2;
			pointA.y=yA-m_nHeight/2;
			r=sqrt(pow((float)pointA.x,2)+pow((float)pointA.y,2));
			theta=atan2((float)pointA.y,(float)pointA.x);
			xAs=r*cos(theta-alfa)+m_rectTmp.left+m_nWidth/2;
			yAs=r*sin(theta-alfa)+m_nHeight/2+m_rectTmp.top;
			left=(int)floor(xAs);
			right=(int)ceil(xAs);
			bottom=(int)floor(yAs);
			top=(int)ceil(yAs);
			a=top-yAs;
			b=xAs-left;
			*(m_pTmp+yA*m_nRowLength+xA)=(BYTE)(a*b*(*(pByteImg+bottom*nRowBytesImg+right))+\
				b*(1-a)*(*(pByteImg+top*nRowBytesImg+right))+\
				a*(1-b)*(*(pByteImg+bottom*nRowBytesImg+left))+\
				(1-a)*(1-b)*(*(pByteImg+top*nRowBytesImg+left))+0.5);
		}

	return TRUE;
}

inline BOOL CTemplate::SubSample(int nSampleCount)
{
	if(m_hSubsample)
	{
		::GlobalFree(m_hSubsample);
		m_hSubsample = NULL;
	}
	//特征点小于60不处理
	if(nSampleCount<60)
		return FALSE;
	//模板图像不存在
	if(!m_hTmp)
		return FALSE;
	
	m_hSubsample = ::GlobalAlloc(GPTR,sizeof(HaltonSample)+(nSampleCount-1)*sizeof(HaltonPoint));
	if(!m_hSubsample)
		return FALSE;
	
	int nWidth = m_nWidth;
	int nHeight = m_nHeight;
	HaltonSample* pHaltonSample = (HaltonSample*)m_hSubsample;
	pHaltonSample->hsSize             = sizeof(HaltonSample);
	pHaltonSample->hsWidth            = nWidth;
	pHaltonSample->hsHeight           = nHeight;
	pHaltonSample->hsSamplePointCount = nSampleCount;
	
	int digit = 0;
	double halton;
	int i,j,n;
	for(n = 1 ;n <= nSampleCount ; n++)
	{
		double half = 0.5;
		double thirds = 1.0/3.0;
		halton = 0.0;
		i = j = n;
		
		while(i)
		{
			digit = i % 2;
			halton += digit*half;
			i = (i-digit) / 2;
			half /= 2.0;
		}
		pHaltonSample->hsHaltonPoints[n-1].x = (int) (halton*(nWidth-1)+0.5);
		
		halton = 0.0;
		while(j)
		{
			digit = j % 3;
			halton += digit*thirds;
			j = (j-digit) / 3;
			thirds /= 3.0;
		}
		pHaltonSample->hsHaltonPoints[n-1].y = (int) (halton*(nHeight-1)+0.5);
		pHaltonSample->hsHaltonPoints[n-1].nStableSize = 1;
	}
	
	BOOL* pbHit=new BOOL[nWidth*nHeight];
	for(j=0;j<nHeight;j++)
		for(i=0;i<nWidth;i++)
			*(pbHit+j*nWidth+i)=FALSE;
		//标志被选择点
	for(n = 0 ;n < nSampleCount ; n++)
		*(pbHit+pHaltonSample->hsHaltonPoints[n].y*nWidth+\
			pHaltonSample->hsHaltonPoints[n].x) = TRUE;
		
	n=0;
	for(j=0;j<nHeight;j++)
		for(i=0;i<nWidth;i++)
			if(*(pbHit+j*nWidth+i))
			{
				pHaltonSample->hsHaltonPoints[n].x = i;
				pHaltonSample->hsHaltonPoints[n].y = j;
				n++;
				//记录坐标
			}
				
	delete []pbHit;
				
	return TRUE;
}

inline HGLOBAL CTemplate::GetSubSample()
{
	return m_hSubsample;
}

inline BOOL CTemplate::Learn(const LearnPatternOptions& learnoption)
{
	if(m_pSample)
	{
		delete [] m_pSample;
		m_pSample=NULL;
		m_nSampleCount=0;
	}
	int nUpper = 0;
	int nLower = 0;
	int nMiddle = 0;
	//进位截取
	nUpper = (int)ceil((double)learnoption.maxAngular/(double)learnoption.finalAngularAccuracy);
	//去尾截取
	nLower = (int)floor((double)learnoption.minAngular/(double)learnoption.finalAngularAccuracy);
	m_nAngularStepSize = learnoption.initialAngularAccuracy / learnoption.finalAngularAccuracy;
	nMiddle = (nUpper+nLower)/2;
	m_nSampleCount = nUpper-nLower+1;//角度分度数

	//大小为角度范围*每个角度方向特征点数量
	m_pSample = new double[m_nSampleCount*(learnoption.initialSampleSize+1)];

	m_pExtraction = new double[learnoption.initialSampleSize];//一次特征点数据
	ASSERT(m_pSample);

	m_nHaltonSamplesCount = learnoption.initialSampleSize;//特征点数
	if(SubSample(m_nHaltonSamplesCount)==FALSE)//特征点采样
	{
		return FALSE;
	};
	OptimizeStepSize();//优化步长大小

	HaltonSample* pHaltonSample = (HaltonSample*)m_hSubsample;//模板数据
	ASSERT(pHaltonSample);
	ASSERT(m_pTmp);

	int i = nMiddle+1;
	int j = nLower;
	int k = 0;//角度统计值

	while(j<=nMiddle-1)
	{
		Rotate(j*learnoption.finalAngularAccuracy);//模板旋转
		*(m_pSample+(learnoption.initialSampleSize+1)*k)=j*learnoption.finalAngularAccuracy;
		
		for(int n = 0;n < pHaltonSample->hsSamplePointCount;n++)
			*(m_pSample+(learnoption.initialSampleSize+1)*k+n+1)=\
			*(m_pTmp+m_nRowLength*pHaltonSample->hsHaltonPoints[n].y+\
			pHaltonSample->hsHaltonPoints[n].x);
		j++;
		k++;
	}

	Rotate(nMiddle*learnoption.finalAngularAccuracy);
	*(m_pSample+(learnoption.initialSampleSize+1)*k) = nMiddle*learnoption.finalAngularAccuracy;
	int n;
	for(n = 0;n < pHaltonSample->hsSamplePointCount;n++)
		*(m_pSample+(learnoption.initialSampleSize+1)*k+n+1)=\
		*(m_pTmp+m_nRowLength*pHaltonSample->hsHaltonPoints[n].y+\
		pHaltonSample->hsHaltonPoints[n].x);
	k++;

	while(i<=nUpper)
	{
		Rotate(i*learnoption.finalAngularAccuracy);
		*(m_pSample+(learnoption.initialSampleSize+1)*k)=i*learnoption.finalAngularAccuracy;
		for(int n = 0;n < pHaltonSample->hsSamplePointCount;n++)
			*(m_pSample+(learnoption.initialSampleSize+1)*k+n+1)=\
			*(m_pTmp+m_nRowLength*pHaltonSample->hsHaltonPoints[n].y+\
			pHaltonSample->hsHaltonPoints[n].x);
		i++;
		k++;	
	}

	int contrast            = 0;
	int mean                = 0;
	m_maxContrast           = 0;
	m_minContrast           = 255 * 255;
	m_upperMeanGrayScale    = 0;
	m_lowerMeanGrayScale    = 255;
	m_meanGrayScale         = 0;
	for(n = 0; n < m_nSampleCount; n++)
	{
		imageNormalize(m_pSample+n*(learnoption.initialSampleSize+1)+1,\
		learnoption.initialSampleSize , &contrast , &mean);
		m_meanGrayScale += mean;
		if(mean > m_upperMeanGrayScale)
			m_upperMeanGrayScale = mean;
		if(mean < m_lowerMeanGrayScale)
			m_lowerMeanGrayScale = mean;
		if(contrast > m_maxContrast)
			m_maxContrast = contrast;
		if(contrast < m_minContrast)
			m_minContrast = contrast;
	}
    m_meanGrayScale /= m_nSampleCount;

	return TRUE;
}

inline BOOL CTemplate::DrawSample(CDC *pDC)
{
	if(!pDC || !m_hSubsample)
		return FALSE;
	int left,top,right,bottom;
	CPen bluePen;
	CBrush blueBrush;
	blueBrush.CreateSolidBrush(RGB(0,255,0));
	bluePen.CreatePen(PS_SOLID,1,RGB(0,255,0));
	CPen* pOldPen =(CPen*)pDC->SelectObject(&bluePen);
	CBrush* pOldBrush =(CBrush*)pDC->SelectObject(&blueBrush);
	HaltonSample* pHaltonSample = (HaltonSample*)m_hSubsample;
	for(int i = 0;i<pHaltonSample->hsSamplePointCount;i++)
	{
		left = m_rectTmp.left+pHaltonSample->hsHaltonPoints[i].x-2;
		top =  m_rectTmp.bottom-pHaltonSample->hsHaltonPoints[i].y-2;
		right = m_rectTmp.left+pHaltonSample->hsHaltonPoints[i].x+2;
		bottom = m_rectTmp.bottom-pHaltonSample->hsHaltonPoints[i].y+2;
		pDC->Ellipse(left,top,right,bottom);
	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	return TRUE;
}

inline BOOL imageNormalize(double *pVector, const int nCount , int* const pContrast , int* const pMean)
{
	ASSERT(pVector && nCount>0);
	double sumVector   = 0.0;
	double sumsqVector = 0.0;
	double temp        = 0.0;
	int i=0;

	for(i=0;i<nCount;i++)
	{
		temp = *(pVector+i);
		sumVector += temp;
		sumsqVector += temp*temp;
	}

	sumVector /= nCount;
	if(pMean)
		*pMean = (int)sumVector;
	sumsqVector -= (nCount*sumVector*sumVector);
	sumsqVector = sqrt(sumsqVector);
	if(pContrast)
		*pContrast = (int)sumsqVector;

	if(sumsqVector == 0.0)
	{
		for(i=0;i<nCount;i++)
			*(pVector+i) = 0;
	}
	else
	{
		for(i=0;i<nCount;i++)
			*(pVector+i) = (*(pVector+i) - sumVector)/sumsqVector;
	}

	return TRUE;
}

inline int CTemplate::GetStepSize()
{
	return m_nStepSize;
}

inline int CTemplate::GetAngularStepSize()
{
	return m_nAngularStepSize;
}

inline void CTemplate::OptimizeStepSize()
{
	HaltonSample* pHaltonSample = (HaltonSample*)m_hSubsample;
	int nSampleCount = pHaltonSample->hsSamplePointCount;
	HaltonSample* pHaltonSampleCopy = (HaltonSample*)::GlobalAlloc(GPTR,\
		sizeof(HaltonSample)+(nSampleCount-1)*sizeof(HaltonPoint));

	int minX = pHaltonSample->hsWidth - 1;
	int maxX = 0;
	int minY = pHaltonSample->hsHeight - 1;
	int maxY = 0;
	int n    = 0;

	for(n = 0 ; n < nSampleCount ; n++)
	{
		if(pHaltonSample->hsHaltonPoints[n].x < minX)
			minX = pHaltonSample->hsHaltonPoints[n].x;
		if(pHaltonSample->hsHaltonPoints[n].x > maxX)
			maxX = pHaltonSample->hsHaltonPoints[n].x;
		if(pHaltonSample->hsHaltonPoints[n].y < minY)
			minY = pHaltonSample->hsHaltonPoints[n].y;
		if(pHaltonSample->hsHaltonPoints[n].y > maxY)
			maxY = pHaltonSample->hsHaltonPoints[n].y;
	}

	int lowerOffsetX = 0 - minX;
	int upperOffsetX = pHaltonSample->hsWidth - 1 - maxX;
	int lowerOffsetY = 0 - minY;
	int upperOffsetY = pHaltonSample->hsHeight - 1 - maxY;

	int mostStableXOffset = 0;
	int mostStableYOffset = 0;
	int nMaxStepSize = 1;
	int nStepSize = 1;

	for(int j = lowerOffsetY ; j <=upperOffsetY ; j++)
		for(int i = lowerOffsetX ; i <= upperOffsetX ; i++)
		{
			::CopyMemory(pHaltonSampleCopy , pHaltonSample , \
				sizeof(HaltonSample)+(nSampleCount-1)*sizeof(HaltonPoint));		
			OffsetSample(pHaltonSampleCopy , i , j);
			CalcStableSize(pHaltonSampleCopy , 10);
			nStepSize = CalcStepSize(pHaltonSampleCopy);
			if(nStepSize > nMaxStepSize){
				nMaxStepSize = nStepSize;
				mostStableXOffset = i;
				mostStableYOffset = j;
			}
		}

	if(nMaxStepSize >= 3){
		m_nStepSize = nMaxStepSize;
		OffsetSample(pHaltonSample , mostStableXOffset , mostStableYOffset);
		CalcStableSize(pHaltonSample , 10);
	}

	if(pHaltonSampleCopy)
		::GlobalFree(pHaltonSampleCopy);
}

inline void CTemplate::CalcStableSize(HaltonSample * const pHaltonSample, int nNoiseLevel)
{
	//
	//Get information of the image. 
	//
	CBitmap cbm;
	BITMAP  bm;
	cbm.Attach(m_hParent);
	cbm.GetBitmap(&bm);
	int   nImageWidth     = bm.bmWidth;
	int   nImageHeight    = bm.bmHeight;
	int   nImageRowLength = ((bm.bmWidth * bm.bmBitsPixel + 31) & ~31)>>3;
	BYTE* pImage          = (BYTE*)bm.bmBits;
	cbm.Detach();

	//
	//Calculate template bottom left coordinates
	//
	int left   = m_rectTmp.left; 
	int bottom = nImageHeight - m_rectTmp.bottom - 1;

	int n         = 0;
	int xCenter   = 0;
	int yCenter   = 0;
	int xStart    = 0;
	int yStart    = 0;
	int xEnd      = 0;
	int yEnd      = 0;
	int nMaxNoise = 0;
	int nNoise    = 0;
	int x         = 0;
	int y         = 0;

	//
	//Calculate the stable size in 3X3,5X5 and 7X7 neighborhood
	//
	for(n = 0 ; n < pHaltonSample->hsSamplePointCount ; n++)
	{
		xCenter = left + pHaltonSample->hsHaltonPoints[n].x;
		yCenter = bottom + pHaltonSample->hsHaltonPoints[n].y;

		//
		//Check if the pixel intensity value is stable in 3X3 neighborhood.
		//
		xStart  = max(xCenter - 1 , 0);
		yStart  = max(yCenter - 1 , 0);
		xEnd    = min(xCenter + 1 , nImageWidth - 1);
		yEnd    = min(yCenter + 1 , nImageHeight - 1);

		//
		//Calculate pixel's (xCenter , yCenter) maximum noise in 3X3 neighborhood.
		//
		nMaxNoise = 0;
		for(y = yStart ; y <= yEnd ; y++)
			for(x = xStart ; x <= xEnd ; x++)
			{
				nNoise = abs(pImage[y * nImageRowLength + x] -\
					pImage[yCenter * nImageRowLength + xCenter]);
				if(nNoise > nMaxNoise)
					nMaxNoise = nNoise;
			}

		if(nMaxNoise < nNoiseLevel){
			pHaltonSample->hsHaltonPoints[n].nStableSize = 3;
			}
		else continue;

		//
		//Check if the pixel intensity value is stable in 5X5 neighborhood.
		//
		xStart  = max(xCenter - 2 , 0);
		yStart  = max(yCenter - 2 , 0);
		xEnd    = min(xCenter + 2 , nImageWidth - 1);
		yEnd    = min(yCenter + 2 , nImageHeight - 1);

		//
		//Calculate pixel's (xCenter , yCenter) maximum noise in 5X5 neighborhood.
		//
		nMaxNoise = 0;
		for(y = yStart ; y <= yEnd ; y++)
			for(x = xStart ; x <= xEnd ; x++)
			{
				nNoise = abs(pImage[y * nImageRowLength + x] -\
					pImage[yCenter * nImageRowLength + xCenter]);
				if(nNoise > nMaxNoise)
					nMaxNoise = nNoise;
			}

		if(nMaxNoise < nNoiseLevel){
			pHaltonSample->hsHaltonPoints[n].nStableSize = 5;
			}
		else continue;

		//
		//Check if the pixel intensity value is stable in 7X7 neighborhood.
		//
		xStart  = max(xCenter - 3 , 0);
		yStart  = max(yCenter - 3 , 0);
		xEnd    = min(xCenter + 3 , nImageWidth - 1);
		yEnd    = min(yCenter + 3 , nImageHeight - 1);

		//
		//Calculate pixel's (xCenter , yCenter) maximum noise in 7X7 neighborhood.
		//
		nMaxNoise = 0;
		for(y = yStart ; y <= yEnd ; y++)
			for(x = xStart ; x <= xEnd ; x++)
			{
				nNoise = abs(pImage[y * nImageRowLength + x] -\
					pImage[yCenter * nImageRowLength + xCenter]);
				if(nNoise > nMaxNoise)
					nMaxNoise = nNoise;
			}

		if(nMaxNoise < nNoiseLevel){
			pHaltonSample->hsHaltonPoints[n].nStableSize = 7;
			}
		else continue;
	}
}

inline int CTemplate::CalcStepSize(HaltonSample *pHaltonSample)
{
	int n1Stable = 0;
	int n3Stable = 0;
	int n5Stable = 0;
	int n7Stable = 0;
	int nUnknown = 0;

	for(int n = 0 ; n < pHaltonSample->hsSamplePointCount ; n++)
	{
		switch(pHaltonSample->hsHaltonPoints[n].nStableSize)
		{
			case 7:
				n7Stable++;
				break;
			case 5:
				n5Stable++;
				break;
			case 3:
				n3Stable++;
				break;
			case 1:
				n1Stable++;
				break;
				
			default:
				nUnknown++;
		}
	}

	ASSERT(nUnknown == 0);

	float rate = (float)n7Stable / (float)pHaltonSample->hsSamplePointCount;
	if(rate > 0.8)
		return 7;

	rate = (float)n5Stable / (float)pHaltonSample->hsSamplePointCount;
	if(rate > 0.8)
		return 5;

	rate = (float)n3Stable / (float)pHaltonSample->hsSamplePointCount;
	if(rate > 0.8)
		return 3;

	return 1;
}

inline void CTemplate::OffsetSample(HaltonSample* const pHaltonSample , const int xOffset , const int yOffset)
{
	for(int n = 0 ; n < pHaltonSample->hsSamplePointCount ; n++)
	{
		pHaltonSample->hsHaltonPoints[n].x += xOffset;
		pHaltonSample->hsHaltonPoints[n].y += yOffset;		
	}
}

inline void CTemplate::CopyParent(HBITMAP hParentBitmap)
{
	if(m_hParent)
	{
		::DeleteObject(m_hParent);
		m_hParent = NULL;
	}

	ASSERT(hParentBitmap);

	CBitmap cbm;
	BITMAP bm;
	cbm.Attach(hParentBitmap);
	cbm.GetBitmap(&bm);
	cbm.Detach();
	int nParentWidth     = bm.bmWidth;
	int nParentHeight    = bm.bmHeight;
	int nParentRowLength = ((bm.bmWidth * bm.bmBitsPixel + 31) & ~31)>>3;
	int nParentBitCount  = bm.bmBitsPixel;
	BYTE* pParent        = (BYTE*)bm.bmBits;

	BITMAPINFO* pParentBmi = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];

	pParentBmi->bmiHeader.biSize           = sizeof(BITMAPINFOHEADER);
	pParentBmi->bmiHeader.biWidth          = nParentWidth;
	pParentBmi->bmiHeader.biHeight         = nParentHeight;
	pParentBmi->bmiHeader.biPlanes         = 1;
	pParentBmi->bmiHeader.biBitCount       = nParentBitCount;
	pParentBmi->bmiHeader.biCompression    = BI_RGB;
	pParentBmi->bmiHeader.biSizeImage      = nParentRowLength * nParentHeight;
	pParentBmi->bmiHeader.biXPelsPerMeter  = 0;
	pParentBmi->bmiHeader.biYPelsPerMeter  = 0;
	pParentBmi->bmiHeader.biClrUsed        = 0;
	pParentBmi->bmiHeader.biClrImportant   = 0;

	for(int i = 0 ;i < 256 ; i++)
	{
	  pParentBmi->bmiColors[i].rgbBlue     = i;
	  pParentBmi->bmiColors[i].rgbGreen    = i;
	  pParentBmi->bmiColors[i].rgbRed      = i;
	  pParentBmi->bmiColors[i].rgbReserved = 0;
	}

	BYTE* pParentCopy;
	m_hParent = ::CreateDIBSection(NULL,pParentBmi,DIB_RGB_COLORS,(void**)&pParentCopy,NULL,0);

	if(m_hParent)
		::CopyMemory(pParentCopy , pParent , pParentBmi->bmiHeader.biSizeImage);

	delete [] pParentBmi;
}

inline void CTemplate::CopyParent(BYTE *pParent, int nWidth, int nHeight, int nBitCount, int nRowLength)
{
	if(m_hParent)
	{
		::DeleteObject(m_hParent);
		m_hParent = NULL;
	}

	ASSERT(pParent);

	int nParentWidth     = nWidth;
	int nParentHeight    = nHeight;
	int nParentRowLength = ((nWidth * nBitCount + 31) & ~31)>>3;
	int nParentBitCount  = nBitCount;

	BITMAPINFO* pParentBmi = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];

	pParentBmi->bmiHeader.biSize           = sizeof(BITMAPINFOHEADER);
	pParentBmi->bmiHeader.biWidth          = nParentWidth;
	pParentBmi->bmiHeader.biHeight         = nParentHeight;
	pParentBmi->bmiHeader.biPlanes         = 1;
	pParentBmi->bmiHeader.biBitCount       = nParentBitCount;
	pParentBmi->bmiHeader.biCompression    = BI_RGB;
	pParentBmi->bmiHeader.biSizeImage      = nParentRowLength * nParentHeight;
	pParentBmi->bmiHeader.biXPelsPerMeter  = 0;
	pParentBmi->bmiHeader.biYPelsPerMeter  = 0;
	pParentBmi->bmiHeader.biClrUsed        = 0;
	pParentBmi->bmiHeader.biClrImportant   = 0;

	for(int i = 0 ;i < 256 ; i++)
	{
	  pParentBmi->bmiColors[i].rgbBlue     = i;
	  pParentBmi->bmiColors[i].rgbGreen    = i;
	  pParentBmi->bmiColors[i].rgbRed      = i;
	  pParentBmi->bmiColors[i].rgbReserved = 0;
	}

	BYTE* pParentCopy;
	m_hParent = ::CreateDIBSection(NULL,pParentBmi,DIB_RGB_COLORS,(void**)&pParentCopy,NULL,0);

	if(m_hParent)
	{
		for(int n = 0 ; n < nParentHeight ; n++)
			::CopyMemory(pParentCopy + n * nParentRowLength , pParent + n * nRowLength , nRowLength);
	}

	delete [] pParentBmi;
}

inline BOOL CTemplate::Create(BYTE *pParent, int nWidth, int nHeight, int nBitCount, int nRowLength, CRect rect, BOOL bInitialize)
{
	Destroy();
	if(!pParent)
		return FALSE;

	CopyParent(pParent , nWidth , nHeight , nBitCount , nRowLength);

	if(!m_hParent)
		return FALSE;

    return PrivateCreate(rect, bInitialize);
}

inline BOOL CTemplate::PrivateCreate(const CRect rect, BOOL bInitialize)
{
	m_rectTmp = rect;

	m_nWidth  = rect.Width()+1;
	m_nHeight = rect.Height()+1;
	m_nRowLength = ((m_nWidth * 8 + 31) & ~31)>>3;
	BITMAPINFO* pTmpBmi = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];

	pTmpBmi->bmiHeader.biSize           = sizeof(BITMAPINFOHEADER);
	pTmpBmi->bmiHeader.biWidth          = m_nWidth;
	pTmpBmi->bmiHeader.biHeight         = m_nHeight;
	pTmpBmi->bmiHeader.biPlanes         = 1;
	pTmpBmi->bmiHeader.biBitCount       = 8;
	pTmpBmi->bmiHeader.biCompression    = BI_RGB;
	pTmpBmi->bmiHeader.biSizeImage      = m_nRowLength * m_nHeight;
	pTmpBmi->bmiHeader.biXPelsPerMeter  = 0;
	pTmpBmi->bmiHeader.biYPelsPerMeter  = 0;
	pTmpBmi->bmiHeader.biClrUsed        = 0;
	pTmpBmi->bmiHeader.biClrImportant   = 0;
	int i;
	for(i = 0 ;i < 256 ; i++)
	{
	  pTmpBmi->bmiColors[i].rgbBlue     = i;
	  pTmpBmi->bmiColors[i].rgbGreen    = i;
	  pTmpBmi->bmiColors[i].rgbRed      = i;
	  pTmpBmi->bmiColors[i].rgbReserved = 0;
	}

	m_hTmp = ::CreateDIBSection(NULL,pTmpBmi,DIB_RGB_COLORS,(void**)&m_pTmp,NULL,0);

	if(m_hTmp == NULL)
	{
	  delete [] pTmpBmi;
	  Destroy();
	  return FALSE;
	}

	CBitmap cbm;
	BITMAP bm;
	cbm.Attach(m_hParent);
	cbm.GetBitmap(&bm);
	int xStart = rect.left;
	int xEnd   = rect.right;
	int yStart = rect.top;
	int yEnd   = rect.bottom;

	if(xStart<0||xEnd>bm.bmWidth-1||yStart<0||yEnd>bm.bmHeight-1)
	{
		AfxMessageBox(_T("Template selection window out of image border"));
		delete [] pTmpBmi;
		Destroy();
		cbm.Detach();
		return FALSE;
	}

	int x,y,j;

	if(bInitialize)
	{
		BYTE* pBits = (BYTE*)bm.bmBits;
		long nRowLength = ((bm.bmWidth * bm.bmBitsPixel + 31) & ~31)>>3;
		for(y=yStart,j=0;y<=yEnd;y++,j++)
			for(x=xStart,i=0;x<=xEnd;x++,i++)
				*(m_pTmp+m_nRowLength*j+i)=*(pBits+y*nRowLength+x*(bm.bmBitsPixel/8));
	}

	delete [] pTmpBmi;
    cbm.Detach();
	return TRUE;
}

inline double* CTemplate::GetExtractionBuffer()
{
	return m_pExtraction;
}

inline BOOL CTemplate::Save(LPCTSTR lpszFileName)
{
	HaltonSample* pHaltonSample = (HaltonSample*)m_hSubsample;

	if(!m_pSample || !pHaltonSample || !m_hTmp || !m_hParent)
		return FALSE;

	CFile templateFile;
	CFileException ex;
	if(!templateFile.Open(lpszFileName , CFile::modeCreate | CFile::modeWrite |\
		CFile::modeNoInherit | CFile::shareExclusive , &ex))
	{
		CString strError;
        TCHAR szError[1024];
        ex.GetErrorMessage(szError, 1024);
		strError  = _T("Couldn't create template file: ");
		strError += szError;
		return FALSE;
	}

	//
	//Save template width.
	//
	templateFile.Write(&m_nWidth , sizeof(int));

	//
	//Save template height.
	//
	templateFile.Write(&m_nHeight , sizeof(int));

	//
	//Save template rect relative to source image.
	//
	templateFile.Write(&m_rectTmp.left , sizeof(int));
	templateFile.Write(&m_rectTmp.top , sizeof(int));
	templateFile.Write(&m_rectTmp.right , sizeof(int));
	templateFile.Write(&m_rectTmp.bottom , sizeof(int));

	//
	//Save template row length
	//
	templateFile.Write(&m_nRowLength , sizeof(int));

	//
	//Save template mean gray scale.
	//
	templateFile.Write(&m_meanGrayScale , sizeof(int));

	//
	//Save template lower mean gray scale.
	//
	templateFile.Write(&m_lowerMeanGrayScale , sizeof(int));

	//
	//Save template upper mean gray scale.
	//
	templateFile.Write(&m_upperMeanGrayScale , sizeof(int));

	//
	//Save template minimum contrast.
	//
	templateFile.Write(&m_minContrast , sizeof(int));

	//
	//Save template maximum contrast.
	//
	templateFile.Write(&m_maxContrast , sizeof(int));

	//
	//Save angular step size.
	//
	templateFile.Write(&m_nAngularStepSize , sizeof(int));

	//
	//Save translation step size.
	//
    templateFile.Write(&m_nStepSize , sizeof(int));

	//
	//Save Halton samples count.
	//
	templateFile.Write(&m_nHaltonSamplesCount , sizeof(int));

	//
	//Save Halton samples structure.
	//
    templateFile.Write(pHaltonSample , sizeof(HaltonSample) + \
		(m_nHaltonSamplesCount - 1) * sizeof(HaltonPoint));

	//
	//Save template samples population count
	//
	templateFile.Write(&m_nSampleCount , sizeof(int));

	//
	//Save template samples population
	//
    templateFile.Write(m_pSample , (m_nSampleCount * \
		(m_nHaltonSamplesCount + 1)) * sizeof(double));

	templateFile.Flush();
	templateFile.Close();

	return TRUE;
}

inline BOOL CTemplate::Load(LPCTSTR lpszFileName)
{
	Destroy();

	CFile templateFile;
	CFileException ex;
	if(!templateFile.Open(lpszFileName , CFile::modeRead |\
		CFile::modeNoInherit | CFile::shareExclusive , &ex))
	{
		CString strError;
        TCHAR szError[1024];
        ex.GetErrorMessage(szError, 1024);
		strError  = _T("Couldn't open template file: ");
		strError += szError;
		return FALSE;
	}

	//
	//Load template width.
	//
	templateFile.Read(&m_nWidth , sizeof(int));

	//
	//Load template height.
	//
	templateFile.Read(&m_nHeight , sizeof(int));

	//
	//Load template rect relative to source image.
	//
	templateFile.Read(&m_rectTmp.left , sizeof(int));
	templateFile.Read(&m_rectTmp.top , sizeof(int));
	templateFile.Read(&m_rectTmp.right , sizeof(int));
	templateFile.Read(&m_rectTmp.bottom , sizeof(int));

	//
	//Load template row length
	//
	templateFile.Read(&m_nRowLength , sizeof(int));

	//
	//Load template mean gray scale.
	//
	templateFile.Read(&m_meanGrayScale , sizeof(int));

	//
	//Load template lower mean gray scale.
	//
	templateFile.Read(&m_lowerMeanGrayScale , sizeof(int));

	//
	//Load template upper mean gray scale.
	//
	templateFile.Read(&m_upperMeanGrayScale , sizeof(int));

	//
	//Load template minimum contrast.
	//
	templateFile.Read(&m_minContrast , sizeof(int));

	//
	//Load template maximum contrast.
	//
	templateFile.Read(&m_maxContrast , sizeof(int));

	//
	//Load angular step size.
	//
	templateFile.Read(&m_nAngularStepSize , sizeof(int));

	//
	//Load translation step size.
	//
    templateFile.Read(&m_nStepSize , sizeof(int));

	//
	//Load Halton samples count.
	//
	templateFile.Read(&m_nHaltonSamplesCount , sizeof(int));

	//
	//Load Halton samples structure.
	//
	m_hSubsample = ::GlobalAlloc(GPTR,\
		sizeof(HaltonSample) + (m_nHaltonSamplesCount - 1) * sizeof(HaltonPoint));
	
	if(!m_hSubsample)
	{
		Destroy();
		return FALSE;
	}
	::ZeroMemory(m_hSubsample ,\
		sizeof(HaltonSample) + (m_nHaltonSamplesCount - 1) * sizeof(HaltonPoint));

	templateFile.Read(m_hSubsample ,\
		sizeof(HaltonSample) + (m_nHaltonSamplesCount - 1) * sizeof(HaltonPoint));

	//
	//Load template samples population count
	//
	templateFile.Read(&m_nSampleCount , sizeof(int));

	//
	//Load template samples population
	//
	m_pSample = new double[m_nSampleCount*(m_nHaltonSamplesCount+1)];
	m_pExtraction = new double[m_nHaltonSamplesCount];
	if(!m_pSample || !m_pExtraction)
	{
		Destroy();
		return FALSE;	
	}
	::ZeroMemory(m_pSample , \
		 (m_nSampleCount * (m_nHaltonSamplesCount + 1)) * sizeof(double));
    templateFile.Read(m_pSample , (m_nSampleCount * \
		(m_nHaltonSamplesCount + 1)) * sizeof(double));

	templateFile.Close();

	return TRUE;
}

inline void CTemplate::KillParent()
{
	if(m_hParent)
	{
		::DeleteObject(m_hParent);
		m_hParent = NULL;
	}
}

inline BOOL CTemplate::imageNormalize(double *pVector, const int nCount , int* const pContrast , int* const pMean)
{
	ASSERT(pVector && nCount>0);
	double sumVector   = 0.0;
	double sumsqVector = 0.0;
	double temp        = 0.0;
	int i=0;

	for(i=0;i<nCount;i++)
	{
		temp = *(pVector+i);
		sumVector += temp;
		sumsqVector += temp*temp;
	}
	//求和与平方和
	sumVector /= nCount;
	if(pMean)
		*pMean = (int)sumVector;//平均值
	sumsqVector -= (nCount*sumVector*sumVector);//平方和与均值平方和之差
	sumsqVector = sqrt(sumsqVector);//开方
	if(pContrast)
		*pContrast = (int)sumsqVector;

	if(sumsqVector == 0.0)
	{
		for(i=0;i<nCount;i++)
			*(pVector+i) = 0;//归一化
	}
	else
	{
		for(i=0;i<nCount;i++)
			*(pVector+i) = (*(pVector+i) - sumVector)/sumsqVector;//归一化
	}

	return TRUE;
}

inline BOOL SaveMatchPattern(LONG m_lHeight,LONG m_lWidth,LPBYTE m_lpBits,CRect rectTmp,int nCommand, CTemplate &m_objTemplate)
{
	CString szPath;
	CString szPathTemp;
	BOOL bSaveOK=TRUE;
	//获得当前程序的全路径
	GetModuleFileName(NULL,szPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	szPath.ReleaseBuffer();
	int nPos;
	nPos=szPath.ReverseFind ('\\');
	szPath=szPath.Left (nPos);
	szPath=szPath+L"\\";
	int nImageWidth=m_lWidth;
	int nImageHeight=m_lHeight;
	if(rectTmp.Width()%2)
		rectTmp.right += 1;
	if(rectTmp.Height()%2)
		rectTmp.bottom += 1;
	int imageRowLength = ((nImageWidth * 8 + 31) & ~31)>>3;

	if(m_objTemplate.Create(m_lpBits,nImageWidth,nImageHeight,8,imageRowLength,rectTmp,FALSE))
	{
		LearnPatternOptions lo;
		lo.bLearnForRotation      = TRUE;
		lo.finalAngularAccuracy   = 1;
		lo.initialAngularAccuracy = 3;
		lo.initialSampleSize      = 480;
		lo.finalSampleSize        =960;
		lo.initialStepSize        = 3;
		lo.maxAngular             = 180;
		lo.minAngular             = 0;
		if(m_objTemplate.Learn(lo))
		{
			AfxMessageBox(_T("Learn template successfully"));
		}
		else
		{
			AfxMessageBox(_T("Learn template failed"));
			bSaveOK=FALSE;
		}
	}
	else
	{
		AfxMessageBox(_T("Create template failed"));
		bSaveOK=FALSE;
	}
	szPathTemp.Format(L"Template%d.dat",nCommand);
	szPath=szPath+szPathTemp;
	if(m_objTemplate.Save(szPath))
		AfxMessageBox(L"Template Saved Successfully!",MB_OK);
	else
	{
		AfxMessageBox(L"Template Saved Failed!",MB_OK);
		bSaveOK=FALSE;
	}

	return bSaveOK;
}

inline BOOL FindMatchPattern(LONG m_lHeight,LONG m_lWidth,LPBYTE m_lpBits,int nCommand, CRect SearchRect, MatchPatternOptions m_options,PatternMatch &m_PatternMatch)
{
	CString szPath;
	CString szPathTemp;
	BOOL bFindMatchPattern=FALSE;
	double dMatchRate=0;//pDoc->m_objParameter.list.list0.match.m_iMatchRate;
	double dRate=0.0;
	CTemplate m_objTemplate;
	CImgPro m_objImageProc(m_lpBits,m_lWidth,m_lHeight);
	GetModuleFileName(NULL,szPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	szPath.ReleaseBuffer();
	int nPos;
	nPos=szPath.ReverseFind ('\\');
	szPath=szPath.Left (nPos);
	szPath=szPath+L"\\";
	szPathTemp.Format(L"Template%d.dat",nCommand);
	//读取模板数据

	BOOL bb=m_objTemplate.Load(szPath+szPathTemp);
	if(!bb)
	{
		AfxMessageBox(L"Load temp error!");
		return FALSE;
	}
	m_options.size = sizeof(MatchPatternOptions);
	m_options.initialStepSize = 0;
	m_options.minMatchScore   = -1;
	
	int nImageWidth=m_lWidth;
	int nImageHeight=m_lHeight;
	int   nImageRowLength = ((nImageWidth * 8 + 31) & ~31)>>3;

	bFindMatchPattern=m_objImageProc.imageMatchPattern(m_lpBits,
				nImageWidth,
				nImageHeight,
				nImageRowLength,
				m_objTemplate,
				m_options,
				SearchRect,
				&m_PatternMatch,
				NULL);
//	delete [] m_pImageTemp;
	return bFindMatchPattern;
}	
