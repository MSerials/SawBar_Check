// ImgPro.cpp: implementation of the CImgPro class.
//
//////////////////////////////////////////////////////////////////////


//#include "math.h"
#ifndef _IMGPRO_INL__
#define _IMGPRO_INL__
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

inline CImgPro::CImgPro(LPBYTE lpBits,LONG lWidth,LONG lHeight)
{
	//初始化
	m_lpBits=lpBits;
	m_lWidth=lWidth;
	m_lHeight=lHeight;
	m_lpBitsObjectflag=new BYTE[m_lWidth*m_lHeight];
	memset(m_lpBitsObjectflag,1,m_lWidth*m_lHeight);
}

inline CImgPro::~CImgPro()
{
	if(m_lpBitsObjectflag!=NULL) delete m_lpBitsObjectflag;
}

inline void CImgPro::DrawPoint(POINT pt,byte gray,int linelength)
{
	int x=pt.x;
	int y=pt.y;
	int i,j;
	int startx=x-linelength;
	int endx=x+linelength;
	int starty=y-linelength;
	int endy=y+linelength;
	if(startx<0) startx=0;
	if(startx>m_lWidth) startx=m_lWidth;
	if(endx<0)endx=0;
	if(endx>m_lWidth)endx=m_lWidth;
	if(starty<0)starty=0;
	if(starty>m_lHeight)starty=m_lHeight;
	if(endy<0)endy=0;
	if(endy>m_lHeight)endy=m_lHeight;
	//X

	for(j=startx;j<endx;j++)
	{
		*(m_lpBits+pt.y*m_lWidth+j)=gray;
	}
	
	//Y	
	for(i=starty;i<endy;i++ )
	{
		*(m_lpBits+i*m_lWidth+pt.x)=gray;
	}
	return;
}
inline BOOL CImgPro::Roberts()
{
	//Robert边缘检测算法
	
	// 指向源图像的指针
	LPSTR lpSrc;
	
	// 指向缓存图像的指针
	LPSTR lpDst;
	
	// 指向缓存位图图像的指针
	LPSTR lpNewBmpBits;
	HLOCAL hNewBmpBits;

	//循环变量
	long i;
	long j;

	//像素值
	double result;
	unsigned char pixel[4];

	// 暂时分配内存，以保存新图像
	hNewBmpBits=LocalAlloc(LHND,m_lWidth*m_lHeight);
	
	if(hNewBmpBits==NULL)
	{
		//分配内存失败
		return FALSE;
	}
	// 锁定内存
	lpNewBmpBits=(char*)LocalLock(hNewBmpBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst=(char*)lpNewBmpBits;
	memset(lpDst,(BYTE)255,m_lWidth*m_lHeight);


	//使用水平方向的结构元素进行腐蚀
	for(j=m_lHeight-1;j>0;j--)
	{
		for(i=0;i<m_lWidth-1;i++)
		{
			//由于使用2×2的模板，为防止越界，所以不处理最下边和最右边的两列像素

			// 指向源图像第j行，第i个象素的指针			
			lpSrc = (char *)m_lpBits + m_lWidth * j + i;

			// 指向目标图像第j行，第i个象素的指针			
			lpDst = (char *)lpNewBmpBits + m_lWidth * j + i;

			//取得当前指针处2*2区域的像素值，注意要转换为unsigned char型
			pixel[0]=(unsigned char)*lpSrc;
			pixel[1]=(unsigned char)*(lpSrc+1);
			pixel[2]=(unsigned char)*(lpSrc-m_lWidth);
			pixel[3]=(unsigned char)*(lpSrc-m_lWidth+1);

			//计算目标图像中的当前点
			 result=sqrt((float)((pixel[0]-pixel[3])*(pixel[0]-pixel[3])+
				  (pixel[1]-pixel[2])*(pixel[1]-pixel[2])));
			*lpDst = (unsigned char)result;
		
		}
	}

	// 复制腐蚀后的图像
	memcpy(m_lpBits,lpNewBmpBits,m_lWidth*m_lHeight);

	// 释放内存
	LocalUnlock(hNewBmpBits);
	LocalFree(hNewBmpBits);

	// 返回
	return TRUE;
}

inline BOOL CImgPro::Template(LPSTR lpBits,int iTempH, int iTempW, int iTempMX, int iTempMY,
					 FLOAT * fpArray, FLOAT fCoef)
{
	// 指向复制图像的指针
	LPSTR lpNewBmpBits;
	HLOCAL hNewBmpBits;
	
	// 指向源图像的指针
	unsigned char*	lpSrc;
	
	// 指向要复制区域的指针
	unsigned char*	lpDst;
	
	// 循环变量
	LONG i;
	LONG j;
	LONG k;
	LONG l;
	
	// 计算结果
	FLOAT fResult;
		
	// 暂时分配内存，以保存新图像
	hNewBmpBits=LocalAlloc(LHND,m_lWidth*m_lHeight);
	
	// 判断是否内存分配失败
	if (hNewBmpBits==NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBmpBits=(char*)LocalLock(hNewBmpBits);
	
	// 初始化图像为原始图像
	memcpy(lpNewBmpBits,lpBits,m_lWidth* m_lHeight);
	
	// 行(除去边缘几行)
	for(i=iTempMY;i<m_lHeight-iTempH+iTempMY+1;i++)
	{
		// 列(除去边缘几列)
		for(j=iTempMX;j<m_lWidth-iTempW+iTempMX+1;j++)
		{
			// 指向新BMP第i行，第j个象素的指针
			lpDst=(unsigned char*)lpNewBmpBits+m_lWidth*(m_lHeight-1-i)+j;
			
			fResult=0;
			
			// 计算
			for(k=0;k<iTempH;k++)
			{
				for(l=0;l<iTempW;l++)
				{
					// 指向BMP第i - iTempMY + k行，第j - iTempMX + l个象素的指针
					lpSrc=(unsigned char*)lpBits+m_lWidth*
						(m_lHeight-1-i+iTempMY-k)+j-iTempMX+l;
					
					// 保存象素值
					fResult += (* lpSrc) * fpArray[k * iTempW + l];
				}
			}
			
			// 乘上系数
			fResult *= fCoef;
			
			// 取绝对值
			fResult = (FLOAT) fabs(fResult);
			
			// 判断是否超过255
			if(fResult > 255)
			{
				// 直接赋值为255
				* lpDst = 255;
			}
			else
			{
				// 赋值
				*lpDst=(unsigned char)(fResult+0.5);
			}
			
		}
	}
	
	// 复制变换后的图像
	memcpy(lpBits, lpNewBmpBits, m_lWidth * m_lHeight);
	
	// 释放内存
	LocalUnlock(hNewBmpBits);
	LocalFree(hNewBmpBits);
	
	// 返回
	return TRUE;
}

inline BOOL CImgPro::Sobel()
{
	//Sobel算子
	// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;
	
	// 指向缓存位图图像的指针
	LPSTR	lpNewBmpBits1;
	HLOCAL	hNewBmpBits1;
	LPSTR	lpNewBmpBits2;
	HLOCAL	hNewBmpBits2;

	//循环变量
	long i;
	long j;

	// 模板高度
	int	iTempH;
	
	// 模板宽度
	int	iTempW;
	
	// 模板系数
	FLOAT fTempC;
	
	// 模板中心元素X坐标
	int	iTempMX;
	
	// 模板中心元素Y坐标
	int	iTempMY;
	
	//模板数组
	FLOAT aTemplate[9];

	// 暂时分配内存，以保存新图像
	hNewBmpBits1 = LocalAlloc(LHND, m_lWidth * m_lHeight);

	if (hNewBmpBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBmpBits1 = (char * )LocalLock(hNewBmpBits1);

	// 暂时分配内存，以保存新图像
	hNewBmpBits2 = LocalAlloc(LHND, m_lWidth *m_lHeight);

	if (hNewBmpBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBmpBits2 = (char * )LocalLock(hNewBmpBits2);

	// 拷贝源图像到缓存图像中
	lpDst1 = (char *)lpNewBmpBits1;
	memcpy(lpNewBmpBits1, m_lpBits, m_lWidth * m_lHeight);
	lpDst2 = (char *)lpNewBmpBits2;
	memcpy(lpNewBmpBits2, m_lpBits, m_lWidth * m_lHeight);

	// 设置Sobel模板参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aTemplate[0] = -1.0;
	aTemplate[1] = -2.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 2.0;
	aTemplate[8] = 1.0;

	// 调用Template()函数
	if (!Template(lpNewBmpBits1,iTempH,iTempW,iTempMX,iTempMY,aTemplate,fTempC))
	{
		return FALSE;
	}

	// 设置Sobel模板参数
	aTemplate[0] = -1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = -2.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 2.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = 1.0;

	// 调用Template()函数
	if (!Template(lpNewBmpBits2,iTempH,iTempW,
		iTempMX,iTempMY,aTemplate,fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBmpBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBmpBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 复制经过模板运算后的图像到源图像
	memcpy(m_lpBits, lpNewBmpBits1, m_lWidth * m_lHeight);

	// 释放内存
	LocalUnlock(hNewBmpBits1);
	LocalFree(hNewBmpBits1);

	LocalUnlock(hNewBmpBits2);
	LocalFree(hNewBmpBits2);

	// 返回
	return TRUE;
}

inline BOOL CImgPro::Threshold(unsigned char iThresholdLow,unsigned char iThresholdHi,unsigned char iThreshold,int &sumPixl)
{
	//如果阈值小于0或大于255，返回FALSE
	if(iThreshold<0 || iThreshold>255)
		return FALSE;

	int i,j;        //循环变量
	unsigned cur;       //当前象素位置      
	int PixelSumCounter=0;
	for(i=0;i<m_lHeight;i++)
		for(j=0;j<m_lWidth;j++)
		{
			cur=i*m_lWidth+j;
			//如果小于阈值，变为黑色
			if(m_lpBits[cur]>=iThresholdLow&&m_lpBits[cur]<=iThresholdHi)
			{
				if(m_lpBits[cur]<=iThreshold)
				{
					m_lpBits[cur]=0;
					PixelSumCounter++;
				}
				else
				//如果大于阈值，变为白色
					m_lpBits[cur]=255;
			}
			else
				m_lpBits[cur]=255;

		}
		sumPixl=PixelSumCounter;
		return TRUE;
}

inline BOOL CImgPro::Threshold(unsigned char iThreshold,int &sumPixel)
{
	//如果阈值小于0或大于255，返回FALSE
	if(iThreshold<0 || iThreshold>255)
		return FALSE;
	
	int i,j;        //循环变量
	unsigned cur;       //当前象素位置      
	int PixelSumCounter=0;

	for(i=0;i<m_lHeight;i++)
		for(j=0;j<m_lWidth;j++)
		{
			cur=i*m_lWidth+j;
			//如果小于阈值，变为黑色
			if(m_lpBits[cur]<=iThreshold)
			{
				m_lpBits[cur]=0;
				PixelSumCounter++;
			}
			else
				//如果大于阈值，变为白色
				m_lpBits[cur]=255;
		}
	sumPixel=PixelSumCounter;
	return TRUE;
}

inline BOOL CImgPro::ThresholdA(unsigned char iThreshold,int &sumPixel)
{
	//如果阈值小于0或大于255，返回FALSE
	if(iThreshold<0 || iThreshold>255)
		return FALSE;

	int i,j;        //循环变量
	unsigned cur;       //当前象素位置      
	int PixelSumCounter=0;

	for(i=0;i<m_lHeight;i++)
		for(j=0;j<m_lWidth;j++)
		{
			cur=i*m_lWidth+j;
			//如果小于阈值，变为黑色
			if(m_lpBits[cur]<=iThreshold&&m_lpBitsObjectflag[cur]==0)
			{
				m_lpBits[cur]=0;
				PixelSumCounter++;
			}
			else
				//如果大于阈值，变为白色
				m_lpBits[cur]=255;
		}
		sumPixel=PixelSumCounter;
		return TRUE;
}
inline void CImgPro::Reserve()
{
	int i,j;                //循环变量     
	unsigned long cur;

	for(i=0;i<m_lHeight;i++)
		for(j=0;j<m_lWidth;j++)
		{
			cur=m_lWidth*i+j;
			//反色
			m_lpBits[cur]=255-m_lpBits[cur];
		}
}

inline BOOL CImgPro::FaceFit()
{
		//Sobel算子
	// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;
	
	// 指向缓存位图图像的指针
	LPSTR	lpNewBmpBits1;
	HLOCAL	hNewBmpBits1;
	LPSTR	lpNewBmpBits2;
	HLOCAL	hNewBmpBits2;

	//循环变量
	long i;
	long j;

	// 模板高度
	int	iTempH;
	
	// 模板宽度
	int	iTempW;
	
	// 模板系数
	FLOAT fTempC;
	
	// 模板中心元素X坐标
	int	iTempMX;
	
	// 模板中心元素Y坐标
	int	iTempMY;
	
	//模板数组
	FLOAT aTemplate[4];

	// 暂时分配内存，以保存新图像
	hNewBmpBits1 = LocalAlloc(LHND, m_lWidth * m_lHeight);

	if (hNewBmpBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBmpBits1 = (char * )LocalLock(hNewBmpBits1);

	// 暂时分配内存，以保存新图像
	hNewBmpBits2 = LocalAlloc(LHND, m_lWidth *m_lHeight);

	if (hNewBmpBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBmpBits2 = (char * )LocalLock(hNewBmpBits2);

	// 拷贝源图像到缓存图像中
	lpDst1 = (char *)lpNewBmpBits1;
	memcpy(lpNewBmpBits1, m_lpBits, m_lWidth * m_lHeight);
	lpDst2 = (char *)lpNewBmpBits2;
	memcpy(lpNewBmpBits2, m_lpBits, m_lWidth * m_lHeight);

	// 设置Sobel模板参数
	iTempW = 2;
	iTempH = 2;
	fTempC = 1.0;
	iTempMX = 0;
	iTempMY = 0;
	aTemplate[0] = -1.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = 1.0;
	aTemplate[3] =1.0; 

	// 调用Template()函数
	if (!Template(lpNewBmpBits1,iTempH,iTempW,iTempMX,iTempMY,aTemplate,fTempC))
	{
		return FALSE;
	}

	// 设置Sobel模板参数
	aTemplate[0] = -1.0;
	aTemplate[1] = 1.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 1.0;

	// 调用Template()函数
	if (!Template(lpNewBmpBits2,iTempH,iTempW,
		iTempMX,iTempMY,aTemplate,fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBmpBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBmpBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 复制经过模板运算后的图像到源图像
	memcpy(m_lpBits, lpNewBmpBits1, m_lWidth * m_lHeight);

	// 释放内存
	LocalUnlock(hNewBmpBits1);
	LocalFree(hNewBmpBits1);

	LocalUnlock(hNewBmpBits2);
	LocalFree(hNewBmpBits2);

	// 返回
	return TRUE;
}

inline unsigned char CImgPro::IterativeThreshold()
{
	// 指向源图像的指针
	LPSTR	lpSrc;

	//循环变量
	long i;
	long j;

	//像素值
	unsigned char pixel;

	//直方图数组
	long lHistogram[256];

	//阈值，最大灰度值与最小灰度值，两个区域的平均灰度值
	unsigned char iThreshold,iNewThreshold,iMaxGrayValue,
		iMinGrayValue,iMean1GrayValue,iMean2GrayValue;

	//用于计算区域灰度平均值的中间变量
	long lP1,lP2,lS1,lS2;

	//迭代次数
	int iIterationTimes;

	for (i = 0; i < 256;i++)
	{
		lHistogram[i]=0;
	}

	//获得直方图
	iMaxGrayValue = 0;
	iMinGrayValue = 255;
	for (i = 0;i <m_lWidth ;i++)
	{
		for(j = 0;j <m_lHeight ;j++)
		{
			// 指向源图像倒数第j行，第i个象素的指针	

				lpSrc = (char *)m_lpBits + m_lWidth * j + i;
	
				pixel = (unsigned char)*lpSrc;
			
				lHistogram[pixel]++;
				//修改最大，最小灰度值
				if(iMinGrayValue > pixel)
				{
					iMinGrayValue = pixel;
				}
				if(iMaxGrayValue < pixel)
				{
					iMaxGrayValue = pixel;
				}
			
		}
	}

	//迭代求最佳阈值
	iNewThreshold = (iMinGrayValue + iMaxGrayValue)/2;
	iThreshold = 0;
	
	for(iIterationTimes = 0; iThreshold != iNewThreshold && 
		iIterationTimes < 100;iIterationTimes ++)
	{
		iThreshold = iNewThreshold;
		lP1 =0;
		lP2 =0;
		lS1 = 0;
		lS2 = 0;
		//求两个区域的灰度平均值
		for (i = iMinGrayValue;i < iThreshold;i++)
		{
			lP1 += lHistogram[i]*i;
			lS1 += lHistogram[i];
		}
		if(lP1!=0 && lS1!=0)
		{
			iMean1GrayValue = (unsigned char)(lP1 / lS1);
		}
		else
		{
			iMean1GrayValue=(unsigned char)lP1;
		}
		for (i = iThreshold+1;i < iMaxGrayValue;i++)
		{
			lP2 += lHistogram[i]*i;
			lS2 += lHistogram[i];
		}
		if(lP2!=0 && lS2!=0)
		{
			iMean2GrayValue = (unsigned char)(lP2 / lS2);
		}
		else
		{
			iMean2GrayValue = (unsigned char)lP2;
		}
		iNewThreshold =  (iMean1GrayValue + iMean2GrayValue)/2;
	}

	return iThreshold;

}

inline BOOL CImgPro::Prewitt()
{

	// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;
	
	// 指向缓存位图图像的指针
	LPSTR	lpNewBits1;
	HLOCAL	hNewBits1;
	LPSTR	lpNewBits2;
	HLOCAL	hNewBits2;

	//循环变量
	long i;
	long j;

	// 模板高度
	int		iTempH;
	
	// 模板宽度
	int		iTempW;
	
	// 模板系数
	FLOAT	fTempC;
	
	// 模板中心元素X坐标
	int		iTempMX;
	
	// 模板中心元素Y坐标
	int		iTempMY;
	
	//模板数组
	FLOAT aTemplate[9];

	// 暂时分配内存，以保存新图像
	hNewBits1 = LocalAlloc(LHND, m_lWidth * m_lHeight);

	if (hNewBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBits1 = (char * )LocalLock(hNewBits1);

	// 暂时分配内存，以保存新图像
	hNewBits2 = LocalAlloc(LHND, m_lWidth * m_lHeight);

	if (hNewBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBits2 = (char * )LocalLock(hNewBits2);

	// 拷贝源图像到缓存图像中
	lpDst1 = (char *)lpNewBits1;
	memcpy(lpNewBits1, m_lpBits, m_lWidth * m_lHeight);
	lpDst2 = (char *)lpNewBits2;
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);


	// 设置Prewitt模板参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aTemplate[0] = -1.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 1.0;
	aTemplate[8] = 1.0;

	// 调用Template()函数
	if (!Template(lpNewBits1,iTempH, iTempW, iTempMX,
		iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 设置Prewitt模板参数
	aTemplate[0] = 1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 1.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = -1.0;

	// 调用Template()函数
	if (!Template(lpNewBits2,iTempH, iTempW, iTempMX,
		iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 复制经过模板运算后的图像到源图像
	memcpy(m_lpBits, lpNewBits1, m_lWidth * m_lHeight);

	// 释放内存
	LocalUnlock(hNewBits1);
	LocalFree(hNewBits1);

	LocalUnlock(hNewBits2);
	LocalFree(hNewBits2);

	// 返回
	return TRUE;
}

inline BOOL CImgPro::Kirsch()
{

	// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;
	
	// 指向缓存位图图像的指针
	LPSTR	lpNewBits1;
	HLOCAL	hNewBits1;
	LPSTR	lpNewBits2;
	HLOCAL	hNewBits2;

	//循环变量
	long i;
	long j;

	// 模板高度
	int		iTempH;
	
	// 模板宽度
	int		iTempW;
	
	// 模板系数
	FLOAT	fTempC;
	
	// 模板中心元素X坐标
	int		iTempMX;
	
	// 模板中心元素Y坐标
	int		iTempMY;
	
	//模板数组
	FLOAT aTemplate[9];

	// 暂时分配内存，以保存新图像
	hNewBits1 = LocalAlloc(LHND, m_lWidth * m_lHeight);

	if (hNewBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBits1 = (char * )LocalLock(hNewBits1);

	// 暂时分配内存，以保存新图像
	hNewBits2 = LocalAlloc(LHND, m_lWidth * m_lHeight);

	if (hNewBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBits2 = (char * )LocalLock(hNewBits2);

	// 拷贝源图像到缓存图像中
	lpDst1 = (char *)lpNewBits1;
	memcpy(lpNewBits1, m_lpBits, m_lWidth * m_lHeight);
	lpDst2 = (char *)lpNewBits2;
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);


	// 设置Kirsch模板1参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aTemplate[0] = 5.0;
	aTemplate[1] = 5.0;
	aTemplate[2] = 5.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	// 调用Template()函数
	if (!Template(lpNewBits1, iTempH, iTempW, iTempMX, 
		iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 设置Kirsch模板2参数
	aTemplate[0] = -3.0;
	aTemplate[1] = 5.0;
	aTemplate[2] = 5.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 5.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, 
		iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);

	// 设置Kirsch模板3参数
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = 5.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 5.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = 5.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, 
		iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	
	// 设置Kirsch模板4参数
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 5.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = 5.0;
	aTemplate[8] = 5.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, iTempMY, 
		aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	
	// 设置Kirsch模板5参数
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = 5.0;
	aTemplate[7] = 5.0;
	aTemplate[8] = 5.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	
	// 设置Kirsch模板6参数
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = 5.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = 5.0;
	aTemplate[7] = 5.0;
	aTemplate[8] = -3.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	
	// 设置Kirsch模板7参数
	aTemplate[0] = 5.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = 5.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = 5.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	
	// 设置Kirsch模板8参数
	aTemplate[0] = 5.0;
	aTemplate[1] = 5.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = 5.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 复制经过模板运算后的图像到源图像
	memcpy(m_lpBits, lpNewBits1, m_lWidth * m_lHeight);

	// 释放内存
	LocalUnlock(hNewBits1);
	LocalFree(hNewBits1);

	LocalUnlock(hNewBits2);
	LocalFree(hNewBits2);

	// 返回
	return TRUE;
}

inline BOOL CImgPro::Gauss()
{

	// 指向源图像的指针
	LPSTR lpSrc;

	// 指向缓存图像的指针
	LPSTR	lpDst;
	
	// 指向缓存位图图像的指针
	LPSTR	lpNewBits;
	HLOCAL  hNewBits;

	// 模板高度
	int		iTempH;
	
	// 模板宽度
	int		iTempW;
	
	// 模板系数
	FLOAT	fTempC;
	
	// 模板中心元素X坐标
	int		iTempMX;
	
	// 模板中心元素Y坐标
	int		iTempMY;
	
	//模板数组
	FLOAT aTemplate[25];

	//像素值
	double result=0.0;

	//循环变量
	LONG i;
	LONG j;
	LONG k;

	//过零点检测求最终边缘点时原始图像
	//的0和1区域灰度值之和与象素数量
	int iGray0=0;
	int iGray1=0;
	int iGray0Num=0;
	int iGray1Num=0;

	//0区域与1区域灰度均值
	float iGray0Av=0.0;
	float iGray1Av=0.0;

	//象素指针
	char *lpPixel[18];

	// 暂时分配内存，以保存新图像
	hNewBits = LocalAlloc(LHND, m_lWidth * m_lHeight);

	if (hNewBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBits = (char * )LocalLock(hNewBits);

	// 拷贝源图像到缓存图像中
	lpDst = (char *)lpNewBits;
	memcpy(lpNewBits, m_lpBits, m_lWidth * m_lHeight);

	// 设置Gauss模板参数
	iTempW = 5;
	iTempH = 5;
	fTempC = 1.0;
	iTempMX = 2;
	iTempMY = 2;
	aTemplate[0] = -2.0;
	aTemplate[1] = -4.0;
	aTemplate[2] = -4.0;
	aTemplate[3] = -4.0;
	aTemplate[4] = -2.0;
	aTemplate[5] = -4.0;
	aTemplate[6] = 0.0;
	aTemplate[7] = 8.0;
	aTemplate[8] = 0.0;
	aTemplate[9] = -4.0;
	aTemplate[10] = -4.0;
	aTemplate[11] = 8.0;
	aTemplate[12] = 24.0;
	aTemplate[13] = 8.0;
	aTemplate[14] = -4.0;
	aTemplate[15] = -4.0;
	aTemplate[16] = 0.0;
	aTemplate[17] = 8.0;
	aTemplate[18] = 0.0;
	aTemplate[19] = -4.0;
	aTemplate[20] = -2.0;
	aTemplate[21] = -4.0;
	aTemplate[22] = -4.0;
	aTemplate[23] = -4.0;
	aTemplate[24] = -2.0;

	// 调用Template()函数
	if (!Template(lpNewBits, iTempH, iTempW, iTempMX, iTempMY,
		aTemplate, fTempC))
	{
		return FALSE;
	}

	/*//过零点检测
	//除去面积小于t的区域
	for(i=1;i<m_lHeight-1;i++)
	{
		for(j=1;j<m_lWidth-1;j++)
		{
			//由于使用3×3的模板，为防止越界，所以不处理图像边缘一行（列）的像素

			// 指向二值化后的图像第i行，第j个象素的指针			
			lpDst = (char *)lpNewBits + m_lWidth * i + j;

		    if((unsigned char)(*lpDst)==255)
			{   
			//取得当前指针处3*3区域的像素的指针
			lpPixel[0]=lpDst+m_lWidth-1;
			lpPixel[1]=lpDst+m_lWidth;
			lpPixel[2]=lpDst+m_lWidth+1;
			lpPixel[3]=lpDst-1;
			lpPixel[4]=lpDst;
			lpPixel[5]=lpDst+1;
			lpPixel[6]=lpDst-m_lWidth-1;
			lpPixel[7]=lpDst-m_lWidth;
			lpPixel[8]=lpDst-m_lWidth+1;

			//计算当前区域中黑色区域的大小
			for(k=0;k<9;k++)
				if((unsigned char)*lpPixel[k]==0)
					result++;

			//如果面积小于阈值t，则除去它
			if(result<6)
				for(k=0;k<9;k++)
					*(lpPixel[k])=0;

			//result清零
			result=0.0;
			}
		}
	}

	//求最终边缘点
	for(i=1;i<m_lHeight-1;i++)
	{
		for(j=1;j<m_lWidth-1;j++)
		{
			//由于使用3×3的窗口，为防止越界，所以不处理图像边缘一行（列）的像素

			// 指向源图像第i行，第j个象素的指针			
			lpSrc = (char *)m_lpBits + m_lWidth * i + j;

			// 指向二值化后的图像第i行，第j个象素的指针			
			lpDst = (char *)lpNewBits + m_lWidth * i + j;

			//对候选边缘点进行选择
			if((unsigned char)(*lpDst)==255)
			{
				//取得源图像当前指针处3*3区域的像素的指针
				lpPixel[0]=lpSrc+m_lWidth-1;
				lpPixel[1]=lpSrc+m_lWidth;
				lpPixel[2]=lpSrc+m_lWidth+1;
				lpPixel[3]=lpSrc-1;
				lpPixel[4]=lpSrc;
				lpPixel[5]=lpSrc+1;
				lpPixel[6]=lpSrc-m_lWidth-1;
				lpPixel[7]=lpSrc-m_lWidth;
				lpPixel[8]=lpSrc-m_lWidth+1;

				//取得变换后图像当前指针处3*3区域的像素的指针
				lpPixel[9]=lpDst+m_lWidth-1;
				lpPixel[10]=lpDst+m_lWidth;
				lpPixel[11]=lpDst+m_lWidth+1;
				lpPixel[12]=lpDst-1;
				lpPixel[13]=lpDst;
				lpPixel[14]=lpDst+1;
				lpPixel[15]=lpDst-m_lWidth-1;
				lpPixel[16]=lpDst-m_lWidth;
				lpPixel[17]=lpDst-m_lWidth+1;
				
				//求二值化图像中1区域和0区域的两部分在原始图像中
				//的灰度值的均值之差作为该点的灰度梯度值
				for(k=9;k<18;k++)
				{
					if(((unsigned char)*(lpPixel[k]))==0)
					{
						//是0(黑)区域
						iGray0+=(unsigned char)*(lpPixel[k-9]);
						iGray0Num++;
					}
					else
					{
						//是1(白)区域
						iGray1+=(unsigned char)*(lpPixel[k-9]);
						iGray1Num++;
					}
				}

				//检查两个区域的数量是否为0
				if(iGray0Num!=0)
					iGray0Av=iGray0/(float)iGray0Num;
				else
					iGray0Av=0;

				if(iGray1Num!=0)
					iGray1Av=iGray1/(float)iGray1Num;
				else
					iGray1Av=0;
				//如果梯度值小于某一阈值，则不是最终边缘点
				if(fabs(iGray0Av-iGray1Av)<6)
				{
					*lpDst=(unsigned char)0;
				}

				//原始图像的0和1区域灰度值之和与象素数量清零
				iGray0=0;
				iGray1=0;
				iGray0Num=0;
				iGray1Num=0;
			}
		}
	}*/

	// 复制经过模板运算后的图像到源图像
	memcpy(m_lpBits, lpNewBits, m_lWidth * m_lHeight);

	// 释放内存
	LocalUnlock(hNewBits);
	LocalFree(hNewBits);

	// 返回
	return TRUE;
}

inline float CImgPro::Zadeh(unsigned char iGrayXY,unsigned char p,float fWin)
					 
{
	//标准S隶属函数

	if(iGrayXY<(p-fWin))
		return 0.0;
	else
		if((iGrayXY>=(p-fWin)) && (iGrayXY<=p))
		{
			return 2*(float)pow((iGrayXY-p+fWin)/(2*fWin),2);
		}
		else
			if((iGrayXY>p) && (iGrayXY<=(p+fWin)))
			{
				return 1-2*(float)pow((iGrayXY-p-fWin)/(2*fWin),2);
			}
			else 
			{
				return 1.0;
			}
}

inline unsigned char CImgPro::FuzzyThreshold(unsigned char iWin)
{
	//模糊阈值分割算法
	//iWin为隶属函数窗口宽度

	float fFuzzy;              //模糊率
	float fMinFuzzy;           //最小模糊率
	float s;                   //隶属函数返回值
	unsigned char iThreshold;  //模糊阈值
		
	LPSTR	lpSrc;             // 指向源图像的指针

	//循环变量
	long i;
	long j;
	
	//灰度循环变量
	int l;

	//窗口中心循环变量
	unsigned char q;

	//像素值
	unsigned char pixel;

	//直方图数组
	long lHistogram[256];

	for (i = 0; i < 256;i++)
	{
		lHistogram[i]=0;
	}

	for (i = 0;i <m_lHeight ;i++)
	{
		for(j = 0;j <m_lWidth ;j++)
		{
			// 指向源图像倒数第j行，第i个象素的指针	

				lpSrc = (char *)m_lpBits + m_lWidth * i + j;
				pixel = (unsigned char)*lpSrc;
				lHistogram[pixel]++;
			
		}
	}

	//模糊率赋初值
	fFuzzy=0.0;
	//最小模糊率赋初值
	fMinFuzzy=10.0;
	//隶属度赋初值
	s=0;

	//求模糊阈值
	for(q=iWin/2+1;q<256-iWin/2-1;q++)
	{
		for(l=0;l<256;l++)
		{	
			//隶属函数值
			s=Zadeh(l,q,iWin/(float)2);
			if(s>0.5)
				s=1-s;
			//求模糊率
			fFuzzy+=s*lHistogram[l];
		}

		fFuzzy=2*fFuzzy/(m_lWidth*m_lHeight);

		if(fFuzzy<fMinFuzzy)
		{
			//找使模糊率最小的q
			fMinFuzzy=fFuzzy;
			iThreshold=q;
		}

		fFuzzy=0.0;
	}

	//返回阈值
	return iThreshold;
}

inline BOOL CImgPro::Laplace()
{
	// 指向缓存图像的指针
	LPSTR	lpDst;

	// 指向缓存位图图像的指针
	LPSTR	lpNewBits;
	HLOCAL  hNewBits;

	// 模板高度
	int		iTempH;
	
	// 模板宽度
	int		iTempW;
	
	// 模板系数
	FLOAT	fTempC;
	
	// 模板中心元素X坐标
	int		iTempMX;
	
	// 模板中心元素Y坐标
	int		iTempMY;
	
	//模板数组
	FLOAT aTemplate[9];

	// 暂时分配内存，以保存新图像
	hNewBits = LocalAlloc(LHND, m_lWidth * m_lHeight);

	if (hNewBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBits = (char * )LocalLock(hNewBits);


	// 拷贝源图像到缓存图像中
	lpDst = (char *)lpNewBits;
	memcpy(lpNewBits, m_lpBits, m_lWidth * m_lHeight);

	// 设置Laplacian模板参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aTemplate[0] = 0.0;
	aTemplate[1] = 1.0;
	aTemplate[2] = 0.0;
	aTemplate[3] = 1.0;
	aTemplate[4] = -4.0;
	aTemplate[5] = 1.0;
	aTemplate[6] = 0.0;
	aTemplate[7] = 1.0;
	aTemplate[8] = 0.0;

	// 调用Template()函数
	if (!Template(lpNewBits, iTempH, iTempW, iTempMX, iTempMY,
		aTemplate, fTempC))
	{
		return FALSE;
	}

	// 复制经过模板运算后的图像到源图像
	memcpy(m_lpBits, lpNewBits, m_lWidth * m_lHeight);

	// 释放内存
	LocalUnlock(hNewBits);
	LocalFree(hNewBits);

	// 返回
	return TRUE;
}

inline BOOL CImgPro::ShenJun(float a)
{
	//沈俊边缘检测算法
	// 指向源图像的指针
	LPSTR lpSrc;

	// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;
	
	// 指向缓存位图图像的指针
	LPSTR	lpNewBits1;
	HLOCAL  hNewBits1;
	LPSTR	lpNewBits2;
	HLOCAL	hNewBits2;

	//循环变量
	LONG i;
	LONG j;
	LONG k;

	//像素值
	double result=0.0;
	unsigned char pixel[2];

	//过零点检测求最终边缘点时原始图像
	//的0和1区域灰度值之和与象素数量
	int iGray0=0;
	int iGray1=0;
	int iGray0Num=0;
	int iGray1Num=0;

	//0区域与1区域灰度均值
	float iGray0Av=0.0;
	float iGray1Av=0.0;

	//象素指针
	char *lpPixel[18];

	// 暂时分配内存，以保存新图像
	hNewBits1 = LocalAlloc(LHND, m_lWidth * m_lHeight);

	if (hNewBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBits1 = (char * )LocalLock(hNewBits1);

	// 暂时分配内存，以保存新图像
	hNewBits2 = LocalAlloc(LHND, m_lWidth * m_lHeight);

	if (hNewBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBits2 = (char * )LocalLock(hNewBits2);

	// 初始化新分配的内存，设定初始值为0
	lpDst1=(char*)lpNewBits1;
	memset(lpDst1,(BYTE)0,m_lWidth*m_lHeight);
	lpDst2=(char*)lpNewBits2;
	memset(lpDst2,(BYTE)0,m_lWidth*m_lHeight);

	//正向行递归滤波
	for(i = 0; i <m_lHeight; i++)
	{
		for(j = 1;j <m_lWidth; j++)
		{
			// 指向源图像第i行，第j个象素的指针			
			lpSrc = (char *)m_lpBits + m_lWidth * i + j;

			// 指向目标图像（缓存图像1）第i行，第j个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * i + j;

			//计算目标图像中的当前点
			pixel[0]=(unsigned char)*lpSrc;
			pixel[1]=(unsigned char)*((char *)lpNewBits1 + m_lWidth * i + j-1);
			result=pixel[1]+a*(pixel[0]-pixel[1]);
			*lpDst1 = (unsigned char)result;
		}
	}

	//反向行递归滤波
	for(i = 0; i <m_lHeight; i++)
	{
		for(j = m_lWidth-2;j >=0; j--)
		{
			// 指向缓存图像1第i行，第j个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * i + j;

			// 指向目标图像（缓存图像2）第i行，第j个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * i + j;

			//计算目标图像中的当前点
			pixel[0]=(unsigned char)*lpDst1;
			pixel[1]=(unsigned char)*((char *)lpNewBits2 + m_lWidth * i + j+1);
			result=pixel[1]+a*(pixel[0]-pixel[1]);
			*lpDst2 = (unsigned char)result;
		}
	}

	// 缓存图像1清0
	lpDst1=(char*)lpNewBits1;
	memset(lpDst1,(BYTE)0,m_lWidth*m_lHeight);

	//正向列递归滤波
	for(i = 1; i <m_lHeight; i++)
	{
		for(j = 0;j <m_lWidth; j++)
		{
			// 指向缓存图像2第i行，第j个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * i + j;

			// 指向目标图像（缓存图像1）第i行，第j个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * i + j;

			//计算目标图像中的当前点
			pixel[0]=(unsigned char)*lpDst2;
			pixel[1]=(unsigned char)*((char *)lpNewBits1 + m_lWidth * (i-1) + j);
			result=pixel[1]+a*(pixel[0]-pixel[1]);
			*lpDst1 = (unsigned char)result;
		}
	}

	// 缓存图像2清0
	lpDst2=(char*)lpNewBits1;
	memset(lpDst2,(BYTE)0,m_lWidth*m_lHeight);

	//反向列递归滤波
	for(i = m_lHeight-2; i >=0; i--)
	{
		for(j = 0;j >=m_lWidth; j++)
		{
			// 指向缓存图像1第i行，第j个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * i + j;

			// 指向目标图像第i行，第j个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * i + j;

			//计算目标图像中的当前点
			pixel[0]=(unsigned char)*lpDst1;
			pixel[1]=(unsigned char)*((char *)lpNewBits2 + m_lWidth * (i+1) + j);
			result=pixel[1]+a*(pixel[0]-pixel[1]);
			*lpDst2 = (unsigned char)result;
		}
	}

	//求滤波后的图像与原图像的差
	for(i = 0; i <m_lHeight; i++)
	{
		for(j = 0;j <m_lWidth; j++)
		{
			// 指向源图像第i行，第j个象素的指针			
			lpSrc = (char *)m_lpBits + m_lWidth * i + j;

			// 指向目标图像（缓存图像2）第i行，第j个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * i + j;

			//计算目标图像中的当前点
			pixel[0]=(unsigned char)*lpSrc;
			pixel[1]=(unsigned char)*lpDst2;
			result=pixel[1]-pixel[0];
			if(result>0)
				*lpDst2 =(unsigned char)255;
			else
				*lpDst2=(unsigned char)0;
		}
	}

	/*//过零点检测
	//除去面积小于t的区域
	for(i=1;i<m_lHeight-1;i++)
	{
		for(j=1;j<m_lWidth-1;j++)
		{
			//由于使用3×3的模板，为防止越界，所以不处理图像边缘一行（列）的像素

			// 指向二值化后的图像第i行，第j个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * i + j;

		    if((unsigned char)(*lpDst2)==255)
			{   
			//取得当前指针处3*3区域的像素的指针
			lpPixel[0]=lpDst2+m_lWidth-1;
			lpPixel[1]=lpDst2+m_lWidth;
			lpPixel[2]=lpDst2+m_lWidth+1;
			lpPixel[3]=lpDst2-1;
			lpPixel[4]=lpDst2;
			lpPixel[5]=lpDst2+1;
			lpPixel[6]=lpDst2-m_lWidth-1;
			lpPixel[7]=lpDst2-m_lWidth;
			lpPixel[8]=lpDst2-m_lWidth+1;

			//计算当前区域中白色区域的大小
			for(k=0;k<9;k++)
				if((unsigned char)*lpPixel[k]==0)
					result++;

			//如果面积小于阈值t，则除去它
			if(result<6)
				for(k=0;k<9;k++)
					*(lpPixel[k])=0;

			//result清零
			result=0.0;
			}
		}
	}

	//求最终边缘点
	for(i=1;i<m_lHeight-1;i++)
	{
		for(j=1;j<m_lWidth-1;j++)
		{
			//由于使用3×3的窗口，为防止越界，所以不处理图像边缘一行（列）的像素

			// 指向源图像第i行，第j个象素的指针			
			lpSrc = (char *)m_lpBits + m_lWidth * i + j;

			// 指向二值化后的图像第i行，第j个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * i + j;

			//对候选边缘点进行选择
			if((unsigned char)(*lpDst2)==255)
			{
				//取得源图像当前指针处3*3区域的像素的指针
				lpPixel[0]=lpSrc+m_lWidth-1;
				lpPixel[1]=lpSrc+m_lWidth;
				lpPixel[2]=lpSrc+m_lWidth+1;
				lpPixel[3]=lpSrc-1;
				lpPixel[4]=lpSrc;
				lpPixel[5]=lpSrc+1;
				lpPixel[6]=lpSrc-m_lWidth-1;
				lpPixel[7]=lpSrc-m_lWidth;
				lpPixel[8]=lpSrc-m_lWidth+1;

				//取得变换后图像当前指针处3*3区域的像素的指针
				lpPixel[9]=lpDst2+m_lWidth-1;
				lpPixel[10]=lpDst2+m_lWidth;
				lpPixel[11]=lpDst2+m_lWidth+1;
				lpPixel[12]=lpDst2-1;
				lpPixel[13]=lpDst2;
				lpPixel[14]=lpDst2+1;
				lpPixel[15]=lpDst2-m_lWidth-1;
				lpPixel[16]=lpDst2-m_lWidth;
				lpPixel[17]=lpDst2-m_lWidth+1;
				
				//求二值化图像中1区域和0区域的两部分在原始图像中
				//的灰度值的均值之差作为该点的灰度梯度值
				for(k=9;k<18;k++)
				{
					if(((unsigned char)*(lpPixel[k]))==0)
					{
						//是0(黑)区域
						iGray0+=(unsigned char)*(lpPixel[k-9]);
						iGray0Num++;
					}
					else
					{
						//是1(白)区域
						iGray1+=(unsigned char)*(lpPixel[k-9]);
						iGray1Num++;
					}
				}

				//检查两个区域的数量是否为0
				if(iGray0Num!=0)
					iGray0Av=iGray0/(float)iGray0Num;
				else
					iGray0Av=0;

				if(iGray1Num!=0)
					iGray1Av=iGray1/(float)iGray1Num;
				else
					iGray1Av=0;
				//如果梯度值小于某一阈值，则不是最终边缘点
				if(fabs(iGray0Av-iGray1Av)<6)
				{
					*lpDst2=(unsigned char)0;
				}

				//原始图像的0和1区域灰度值之和与象素数量清零
				iGray0=0;
				iGray1=0;
				iGray0Num=0;
				iGray1Num=0;
			}
		}
	}*/
	// 复制变换后的图像
	memcpy(m_lpBits, lpNewBits2, m_lWidth * m_lHeight);

	// 释放内存
	LocalUnlock(hNewBits1);
	LocalFree(hNewBits1);

	LocalUnlock(hNewBits2);
	LocalFree(hNewBits2);

	return TRUE;
}

inline BOOL CImgPro::ColOrth()
{
	//综合正交算子

		// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;
	
	// 指向缓存位图图像的指针
	LPSTR	lpNewBits1;
	HLOCAL	hNewBits1;
	LPSTR	lpNewBits2;
	HLOCAL	hNewBits2;

	//循环变量
	long i;
	long j;

	// 模板高度
	int		iTempH;
	
	// 模板宽度
	int		iTempW;
	
	// 模板系数
	FLOAT	fTempC;
	
	// 模板中心元素X坐标
	int		iTempMX;
	
	// 模板中心元素Y坐标
	int		iTempMY;
	
	//模板数组
	FLOAT aTemplate[9];

	// 暂时分配内存，以保存新图像
	hNewBits1 = LocalAlloc(LHND, m_lWidth * m_lHeight);

	if (hNewBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBits1 = (char * )LocalLock(hNewBits1);

	// 暂时分配内存，以保存新图像
	hNewBits2 = LocalAlloc(LHND, m_lWidth * m_lHeight);

	if (hNewBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBits2 = (char * )LocalLock(hNewBits2);

	// 拷贝源图像到缓存图像中
	lpDst1 = (char *)lpNewBits1;
	memcpy(lpNewBits1, m_lpBits, m_lWidth * m_lHeight);
	lpDst2 = (char *)lpNewBits2;
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);


	// 设置综合正交模板对称梯度1参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aTemplate[0] = 1.0;
	aTemplate[1] = 2.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = -2.0;
	aTemplate[8] = -1.0;

	// 调用Template()函数
	if (!Template(lpNewBits1, iTempH, iTempW, iTempMX, 
		iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 设置综合正交模板对称梯度2参数
	aTemplate[0] = 1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 2.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -2.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = -1.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, 
		iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);

	// 设置综合正交模板波纹1参数
	aTemplate[0] = 0.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = 2.0;
	aTemplate[3] = 1.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = -2.0;
	aTemplate[7] = 1.0;
	aTemplate[8] = 0.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, 
		iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	
	// 设置综合正交模板波纹2参数
	aTemplate[0] = 2.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = 0.0;
	aTemplate[3] = -1.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 1.0;
	aTemplate[6] = 0.0;
	aTemplate[7] = 1.0;
	aTemplate[8] = -2.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, iTempMY, 
		aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	
	// 设置综合正交模板直线模板1的参数
	aTemplate[0] = 0.0;
	aTemplate[1] = 1.0;
	aTemplate[2] = 0.0;
	aTemplate[3] = -1.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = 0.0;
	aTemplate[7] = 1.0;
	aTemplate[8] = 0.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	
	// 设置综合正交模板直线模板2参数
	aTemplate[0] = -1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = -1.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	
	// 设置综合正交模板Laplacian模板1的参数
	aTemplate[0] = 1.0;
	aTemplate[1] = -2.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = -2.0;
	aTemplate[4] = 4.0;
	aTemplate[5] = -2.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = -2.0;
	aTemplate[8] = 1.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	
	// 设置综合正交模板Laplacian模板2参数
	aTemplate[0] = -2.0;
	aTemplate[1] = 1.0;
	aTemplate[2] = -2.0;
	aTemplate[3] = 1.0;
	aTemplate[4] = 4.0;
	aTemplate[5] = 1.0;
	aTemplate[6] = -2.0;
	aTemplate[7] = 1.0;
	aTemplate[8] = -2.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	
	// 设置综合正交模板平均模板的参数
	fTempC = 9.0;
	aTemplate[0] = 1.0;
	aTemplate[1] = 1.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = 1.0;
	aTemplate[4] = 1.0;
	aTemplate[5] = 1.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 1.0;
	aTemplate[8] = 1.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 复制经过模板运算后的图像到源图像
	memcpy(m_lpBits, lpNewBits1, m_lWidth * m_lHeight);

	// 释放内存
	LocalUnlock(hNewBits1);
	LocalFree(hNewBits1);

	LocalUnlock(hNewBits2);
	LocalFree(hNewBits2);

	// 返回
	return TRUE;
}

inline BOOL CImgPro::EdgeMod()
{
	//边缘模板匹配

	// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;
	
	// 指向缓存位图图像的指针
	LPSTR	lpNewBits1;
	HLOCAL	hNewBits1;
	LPSTR	lpNewBits2;
	HLOCAL	hNewBits2;

	//循环变量
	long i;
	long j;

	// 模板高度
	int		iTempH;
	
	// 模板宽度
	int		iTempW;
	
	// 模板系数
	FLOAT	fTempC;
	
	// 模板中心元素X坐标
	int		iTempMX;
	
	// 模板中心元素Y坐标
	int		iTempMY;
	
	//模板数组
	FLOAT aTemplate[9];

	// 暂时分配内存，以保存新图像
	hNewBits1 = LocalAlloc(LHND, m_lWidth * m_lHeight);

	if (hNewBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBits1 = (char * )LocalLock(hNewBits1);

	// 暂时分配内存，以保存新图像
	hNewBits2 = LocalAlloc(LHND, m_lWidth * m_lHeight);

	if (hNewBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBits2 = (char * )LocalLock(hNewBits2);

	// 拷贝源图像到缓存图像中
	lpDst1 = (char *)lpNewBits1;
	memcpy(lpNewBits1, m_lpBits, m_lWidth * m_lHeight);
	lpDst2 = (char *)lpNewBits2;
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);


	// 设置边缘模板1参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aTemplate[0] = 1.0;
	aTemplate[1] = 1.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = -1.0;
	aTemplate[8] = -1.0;

	// 调用Template()函数
	if (!Template(lpNewBits1, iTempH, iTempW, iTempMX, 
		iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 设置边缘模板2参数
	aTemplate[0] = 1.0;
	aTemplate[1] = 1.0;
	aTemplate[2] = 0.0;
	aTemplate[3] = 1.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = 0.0;
	aTemplate[7] = -1.0;
	aTemplate[8] = -1.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, 
		iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);

	// 设置边缘模板3参数
	aTemplate[0] = 1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 1.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = -1.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, 
		iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	
	// 设置边缘模板4参数
	aTemplate[0] = 0.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 1.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 1.0;
	aTemplate[8] = 0.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, iTempMY, 
		aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	/*// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	
	// 设置边缘模板5参数
	aTemplate[0] = -1.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 1.0;
	aTemplate[8] = 1.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	
	// 设置边缘模板6参数
	aTemplate[0] = -1.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = 0.0;
	aTemplate[3] = -1.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 1.0;
	aTemplate[6] = 0.0;
	aTemplate[7] = 1.0;
	aTemplate[8] = 1.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	
	// 设置边缘模板7参数
	aTemplate[0] = -1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = -1.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 1.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = 1.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	
	// 设置边缘模板8参数
	aTemplate[0] = 0.0;
	aTemplate[1] = 1.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = -1.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 1.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = -1.0;
	aTemplate[8] = 0.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}*/

	// 复制经过模板运算后的图像到源图像
	memcpy(m_lpBits, lpNewBits1, m_lWidth * m_lHeight);

	// 释放内存
	LocalUnlock(hNewBits1);
	LocalFree(hNewBits1);

	LocalUnlock(hNewBits2);
	LocalFree(hNewBits2);

	// 返回
	return TRUE;
}

inline BOOL CImgPro::LineMod()
{
	//线模板匹配

	// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;
	
	// 指向缓存位图图像的指针
	LPSTR	lpNewBits1;
	HLOCAL	hNewBits1;
	LPSTR	lpNewBits2;
	HLOCAL	hNewBits2;

	//循环变量
	long i;
	long j;

	// 模板高度
	int		iTempH;
	
	// 模板宽度
	int		iTempW;
	
	// 模板系数
	FLOAT	fTempC;
	
	// 模板中心元素X坐标
	int		iTempMX;
	
	// 模板中心元素Y坐标
	int		iTempMY;
	
	//模板数组
	FLOAT aTemplate[9];

	// 暂时分配内存，以保存新图像
	hNewBits1 = LocalAlloc(LHND, m_lWidth * m_lHeight);

	if (hNewBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBits1 = (char * )LocalLock(hNewBits1);

	// 暂时分配内存，以保存新图像
	hNewBits2 = LocalAlloc(LHND, m_lWidth * m_lHeight);

	if (hNewBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBits2 = (char * )LocalLock(hNewBits2);

	// 拷贝源图像到缓存图像中
	lpDst1 = (char *)lpNewBits1;
	memcpy(lpNewBits1, m_lpBits, m_lWidth * m_lHeight);
	lpDst2 = (char *)lpNewBits2;
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);


	// 设置点模板1参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aTemplate[0] = -1.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 2.0;
	aTemplate[4] = 2.0;
	aTemplate[5] = 2.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = -1.0;
	aTemplate[8] = -1.0;

	// 调用Template()函数
	if (!Template(lpNewBits1, iTempH, iTempW, iTempMX, 
		iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 设置点模板2参数
	aTemplate[0] = -1.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = 2.0;
	aTemplate[3] = -1.0;
	aTemplate[4] = 2.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = 2.0;
	aTemplate[7] = -1.0;
	aTemplate[8] = -1.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, 
		iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);

	// 设置点模板3参数
	aTemplate[0] = -1.0;
	aTemplate[1] = 2.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = -1.0;
	aTemplate[4] = 2.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = 2.0;
	aTemplate[8] = -1.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, 
		iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewBits2, m_lpBits, m_lWidth * m_lHeight);
	
	// 设置边缘模板4参数
	aTemplate[0] = 2.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = -1.0;
	aTemplate[4] = 2.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = -1.0;
	aTemplate[8] = 2.0;

	// 调用Template()函数
	if (!Template(lpNewBits2, iTempH, iTempW, iTempMX, iTempMY, 
		aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBits1 + m_lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBits2 + m_lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 复制经过模板运算后的图像到源图像
	memcpy(m_lpBits, lpNewBits1, m_lWidth * m_lHeight);

	// 释放内存
	LocalUnlock(hNewBits1);
	LocalFree(hNewBits1);

	LocalUnlock(hNewBits2);
	LocalFree(hNewBits2);

	// 返回
	return TRUE;
}

inline unsigned char CImgPro::OstuThreshold()
{
	//最大类间方差法求最佳阈值

	// 指向源图像的指针
	LPSTR lpSrc;

	//循环变量
	long i;
	long j;
	long l;

	//最佳阈值
	unsigned char iThreshold;

	//象素值
	unsigned char pixel;

	//两个类的出现概率
	double fProb1;
	double fProb2;

	//两个类的灰度均值均值
	double fAvg1;
	double fAvg2;

	//整副图像的灰度均值
	double fAvg;

	//类间方差
	double fSqr;
	//最大类间方差
	double fMaxSqr;

	//第一类灰度累计值
	double iGray1;
	//直方图数组
	long   lHistogram[256];
	//归一化灰度直方图
	double fHistogram[256];

	//赋初值
	fMaxSqr=0.0;
	iThreshold=0;
	fProb1=0.0;
	fProb2=0.0;
	fAvg=0.0;
	fAvg1=0.0;
	fAvg2=0.0;
	iGray1=0.0;

	for (i = 0; i < 256;i++)
	{
		lHistogram[i]=0;
		fHistogram[i]=0.0;
	}


	//获取灰度直方图
	for (i = 0;i < m_lHeight;i++)
	{
		for(j = 0;j <m_lWidth ;j++)
		{
			// 指向源图像第i行，第j个象素的指针	

				lpSrc = (char *)m_lpBits + m_lWidth * i + j;
				pixel = (unsigned char)*lpSrc;
				lHistogram[pixel]++;
			
		}
	}

	//归一化灰度直方图
	unsigned long N=0;
	for(i = 0;i < 256 ;i++)
	{
		N+=lHistogram[i];
	}

	for(i = 0;i < 256 ;i++)
	{
		fHistogram[i]=lHistogram[i]/(float)N;
	}

	//求整副图像灰度累计值
	for(i = 0;i < 255 ;i++)
	{
		fAvg+=i*fHistogram[i];
	}

	for(l = 0;l < 255 ;l++)
	{
		//两个类的概率
		for(i=0;i<=l;i++)
		{
			fProb1+=fHistogram[i];
			iGray1+=i*fHistogram[i];
		}
		fProb2=1.0-fProb1;			

		//求两个类的灰度均值
		if(fProb1!=0.0)
			fAvg1=iGray1/fProb1;
		else
			fAvg1=0.0;

		if(fProb2!=0.0)
			fAvg2=fabs(fAvg-iGray1)/fProb2;
		else
			fAvg2=0.0;

		//求类间方差
		fSqr=fProb1*fProb2*(fAvg1-fAvg2)*(fAvg1-fAvg2);

		//求最佳阈值
		if(fSqr>fMaxSqr)
		{
			fMaxSqr=fSqr;
			iThreshold=(unsigned char)l;
		}
		
		//变量清零
		fProb1=0.0;
		fProb2=0.0;
		fAvg1=0.0;
		fAvg2=0.0;
		iGray1=0;
	}

	//返回阈值
	return iThreshold;
}

inline BOOL CImgPro::PalKing(float fd,unsigned char FuzzyNum)
{
	//Pal.King模糊边缘检测算法

	// 指向源图像的指针
	LPSTR lpSrc;
	
	// 指向缓存图像的指针
	LPSTR lpDst;
	
	// 指向缓存位图图像的指针
	LPSTR lpNewBits;
	HLOCAL hNewBits;

	//模糊矩阵元素指针
	double *pFuzzySet;

	//模糊矩阵头指针及缓存句柄
	double *lpFuzzy;
	HLOCAL hFuzzy;

	//循环变量
	long i;
	long j;
	int k;

	//象素值
	double result;
	unsigned char pixel;
	unsigned char pixels[9];

	//最大最小灰度级
	unsigned char MaxGray=0;
	//模糊矩阵元素最小值
	double minfuzzy;

	// 暂时分配内存，以保存新图像
	hNewBits=LocalAlloc(LHND,m_lWidth*m_lHeight);
	
	if(hNewBits==NULL)
	{
		//分配内存失败
		return FALSE;
	}
	// 锁定内存
	lpNewBits=(char*)LocalLock(hNewBits);

	// 拷贝源图像到缓存图像中
	lpDst = (char *)lpNewBits;
	memcpy(lpNewBits, m_lpBits, m_lWidth * m_lHeight);

	// 暂时分配内存，以保存模糊矩阵
	hFuzzy=LocalAlloc(LHND,m_lWidth*m_lHeight*sizeof(double));
	
	if(hFuzzy==NULL)
	{
		//分配内存失败
		return FALSE;
	}
	// 锁定内存
	lpFuzzy=(double *)LocalLock(hFuzzy);

	//求最大灰度级
	for(i = 0; i <m_lHeight; i++)
	{
		for(j = 0;j <m_lWidth; j++)
		{
			// 指向源图像第i行，第j个象素的指针			
			lpDst = (char *)lpNewBits + m_lWidth * i + j;
			//计算当前指针处象素值
			pixel=(unsigned char)*lpDst;

			//求最大灰度级
			if(pixel>MaxGray)
				MaxGray=pixel;
		}
	}

	//进行G变换
	for(i = 0; i <m_lHeight; i++)
	{
		for(j = 0;j <m_lWidth; j++)
		{	
			// 指向源图像第i行，第j个象素的指针			
			lpDst = (char *)lpNewBits + m_lWidth * i + j;
			//计算当前指针处象素值
			pixel=(unsigned char)*lpDst;

			//指向模糊矩阵的指针
			pFuzzySet=(double *)lpFuzzy + m_lWidth * i + j;

			//求模糊矩阵元素的值
			*pFuzzySet=(double)(1/((1+(MaxGray-pixel)/fd)*(1+(MaxGray-pixel)/fd)));
		}
	}

	//模糊增强

	//增强次数
	while(FuzzyNum>0)
	{
		for(i = 0; i <m_lHeight; i++)
		{
			for(j = 0;j <m_lWidth; j++)
			{
				//指向模糊矩阵的指针
				pFuzzySet=(double*)lpFuzzy + m_lWidth * i + j;
				result=(double)*pFuzzySet;

				if(result<=0.5)
					//若模糊度小于0.5，减弱。
					*pFuzzySet=(double)(2*result*result);
				else
					//模糊度大于0.5，增强。
					*pFuzzySet=(double)(1-2*(1-result)*(1-result));
			}
		}
		FuzzyNum--;
	}

	//求模糊矩阵元素最小值
	minfuzzy=(double)(1/((1+MaxGray/fd)*(1+MaxGray/fd)));

	//对模糊平面进行G的逆变换
	for(i = 0; i <m_lHeight; i++)
	{
		for(j = 0;j <m_lWidth; j++)
		{
			// 指向缓冲图像第i行，第j个象素的指针			
			lpDst = (char *)lpNewBits + m_lWidth * i + j;

			//指向模糊矩阵的指针
			pFuzzySet=(double*)lpFuzzy + m_lWidth * i + j;
			
			//计算当前指针模糊矩阵元素值
			result=(double)*pFuzzySet;
			//值小于最小的模糊矩阵元素值，赋为矩阵中最小值
			if(result<minfuzzy)
				result=minfuzzy;
				
			//得到模糊增强后的象素值
			*lpDst=(unsigned char)(MaxGray-fd*(1/sqrt(result)-1));
		}
	}

	//边缘提取
	for(i = 1; i <m_lHeight-1; i++)
	{
		for(j = 1;j <m_lWidth-1; j++)
		{
			// 指向源图像第i行，第j个象素的指针			
			lpSrc = (char *)m_lpBits + m_lWidth * i + j;
			
			// 指向缓冲源图像第i行，第j个象素的指针			
			lpDst = (char *)lpNewBits + m_lWidth * i + j;

			//取得当前指针处3*3区域的像素的值
			pixels[0]=(unsigned char)*(lpDst+m_lWidth-1);
			pixels[1]=(unsigned char)*(lpDst+m_lWidth);
			pixels[2]=(unsigned char)*(lpDst+m_lWidth+1);
			pixels[3]=(unsigned char)*(lpDst-1);
			pixels[4]=(unsigned char)*lpDst;
			pixels[5]=(unsigned char)*(lpDst+1);
			pixels[6]=(unsigned char)*(lpDst-m_lWidth-1);
			pixels[7]=(unsigned char)*(lpDst-m_lWidth);
			pixels[8]=(unsigned char)*(lpDst-m_lWidth+1);

			//取得最大象素值
			pixel=255;
			for(k=0;k<9;k++)
			{
				pixel=min(pixel,pixels[k]);
			}


			//获得边缘象素值
			*lpSrc=(unsigned char)fabs((float)(pixels[4]-pixel));
		}
	}

	// 复制变换后的图像
	/*memcpy(m_lpBits,lpNewBits,m_lWidth*m_lHeight);*/

	// 释放内存
	LocalUnlock(hNewBits);
	LocalFree(hNewBits);
	LocalUnlock(hFuzzy);
	LocalFree(hFuzzy);

	// 返回
	return TRUE;
}

inline BOOL CImgPro::ImpPalking(int FuzzyNum)
{
	//改进的Pal.King算法1
	
	// 指向源图像的指针
	LPSTR lpSrc;
	
	// 指向缓存图像的指针
	LPSTR lpDst;
	
	// 指向缓存位图图像的指针
	LPSTR lpNewBits;
	HLOCAL hNewBits;

	//Ostu法求得的阈值
	unsigned char iThreshold;

	//象素值
	unsigned char pixel;
	unsigned char pixels[9];

	//模糊增强表
	unsigned char table[256];

	//最大灰度级
	unsigned char MaxGray=0;

	//循环变量
	long i;
	long j;
	int k;

	// 暂时分配内存，以保存新图像
	hNewBits=LocalAlloc(LHND,m_lWidth*m_lHeight);
	
	if(hNewBits==NULL)
	{
		//分配内存失败
		return FALSE;
	}
	// 锁定内存
	lpNewBits=(char*)LocalLock(hNewBits);

	// 拷贝源图像到缓存图像中
	lpDst = (char *)lpNewBits;
	memcpy(lpNewBits, m_lpBits, m_lWidth * m_lHeight);

	//求最大灰度级
	for(i = 0; i <m_lHeight; i++)
	{
		for(j = 0;j <m_lWidth; j++)
		{
			// 指向源图像第i行，第j个象素的指针			
			lpDst = (char *)lpNewBits + m_lWidth * i + j;
			//计算当前指针处象素值
			pixel=(unsigned char)*lpDst;

			//求最大灰度级
			if(pixel>MaxGray)
				MaxGray=pixel;
		}
	}

	//求阈值
	iThreshold=OstuThreshold();

	//求模糊增强表
	for(i=0;i<256;i++)
	{
		if(i<=iThreshold)
			if(iThreshold!=0)
				table[i]=(unsigned char)(i*i/iThreshold);
			else
				table[i]=(unsigned char)0;
		else
		{
			if(i!=MaxGray)
				table[i]=(unsigned char)(MaxGray-(MaxGray-i)*(MaxGray-i)/(MaxGray-iThreshold)+0.5);
			else
				table[i]=(unsigned char)i;
		}
	}

	//图象增强
	while(FuzzyNum>0)
	{
		for(i = 0; i <m_lHeight; i++)
		{
			for(j = 0;j <m_lWidth; j++)
			{
				// 指向源图像第i行，第j个象素的指针			
				lpDst = (char *)lpNewBits + m_lWidth * i + j;
				//计算当前指针处象素值
				pixel=(unsigned char)*lpDst;
				
				//求增强图象象素的值
				*lpDst=table[pixel];
			}
		}
		FuzzyNum--;
	}

	//边缘提取
	for(i = 1; i <m_lHeight-1; i++)
	{
		for(j = 1;j <m_lWidth-1; j++)
		{
			// 指向源图像第i行，第j个象素的指针			
			lpSrc = (char *)m_lpBits + m_lWidth * i + j;
			
			// 指向缓冲源图像第i行，第j个象素的指针			
			lpDst = (char *)lpNewBits + m_lWidth * i + j;

			//取得当前指针处3*3区域的像素的值
			pixels[0]=(unsigned char)*(lpDst+m_lWidth-1);
			pixels[1]=(unsigned char)*(lpDst+m_lWidth);
			pixels[2]=(unsigned char)*(lpDst+m_lWidth+1);
			pixels[3]=(unsigned char)*(lpDst-1);
			pixels[4]=(unsigned char)*lpDst;
			pixels[5]=(unsigned char)*(lpDst+1);
			pixels[6]=(unsigned char)*(lpDst-m_lWidth-1);
			pixels[7]=(unsigned char)*(lpDst-m_lWidth);
			pixels[8]=(unsigned char)*(lpDst-m_lWidth+1);

			//取得最大象素值
			pixel=255;
			for(k=0;k<9;k++)
			{
				pixel=min(pixel,pixels[k]);
			}

			//获得边缘象素值
			*lpSrc=(unsigned char)fabs((float)(pixels[4]-pixel));
		}
	}

	/*// 复制变换后的图像
	memcpy(m_lpBits,lpNewBits,m_lWidth*m_lHeight);*/

	// 返回
	return TRUE;
}

inline BOOL CImgPro::SobelThin(int n)
{
	//用Sobel算子细化边缘
	// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;
	LPSTR   lpDst3;
	
	// 指向缓存位图图像的指针
	LPSTR	lpNewBmpBits1;
	HLOCAL	hNewBmpBits1;
	LPSTR	lpNewBmpBits2;
	HLOCAL	hNewBmpBits2;
	LPSTR	lpNewBmpBits3;
	HLOCAL	hNewBmpBits3;

	//象素差值
	int pixel;

	//循环变量
	long i;
	long j;

	// 模板高度
	int	iTempH;
	
	// 模板宽度
	int	iTempW;
	
	// 模板系数
	FLOAT fTempC;
	
	// 模板中心元素X坐标
	int	iTempMX;
	
	// 模板中心元素Y坐标
	int	iTempMY;
	
	//模板数组
	FLOAT aTemplate[9];

	// 暂时分配内存，以保存新图像
	hNewBmpBits1 = LocalAlloc(LHND, m_lWidth * m_lHeight);

	if (hNewBmpBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBmpBits1 = (char * )LocalLock(hNewBmpBits1);

	// 暂时分配内存，以保存新图像
	hNewBmpBits2 = LocalAlloc(LHND, m_lWidth *m_lHeight);

	if (hNewBmpBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBmpBits2 = (char * )LocalLock(hNewBmpBits2);

	// 暂时分配内存，以保存新图像
	hNewBmpBits3 = LocalAlloc(LHND, m_lWidth *m_lHeight);

	if (hNewBmpBits3 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBmpBits3 = (char * )LocalLock(hNewBmpBits3);

	// 拷贝源图像到缓存图像中
	lpDst1 = (char *)lpNewBmpBits1;
	memcpy(lpNewBmpBits1, m_lpBits, m_lWidth * m_lHeight);
	lpDst2 = (char *)lpNewBmpBits2;
	memcpy(lpNewBmpBits2, m_lpBits, m_lWidth * m_lHeight);
	lpDst3 = (char *)lpNewBmpBits3;
	memcpy(lpNewBmpBits3, m_lpBits, m_lWidth * m_lHeight);


	// 设置Sobel模板参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 0.25;
	iTempMX = 1;
	iTempMY = 1;
	aTemplate[0] = -1.0;
	aTemplate[1] = -2.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 2.0;
	aTemplate[8] = 1.0;
		
	// 调用Template()函数
	if (!Template(lpNewBmpBits1,iTempH,iTempW,iTempMX,iTempMY,
		aTemplate,fTempC))
	{
		return FALSE;
	}
		
	// 设置Sobel模板参数
	aTemplate[0] = -1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = -2.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 2.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = 1.0;
		
	// 调用Template()函数
	if (!Template(lpNewBmpBits2,iTempH,iTempW,
		iTempMX,iTempMY,aTemplate,fTempC))
	{
		return FALSE;
	}
		
	//求两幅缓存图像的最大值
	for(j = 0; j <m_lHeight; j++)
	{
		for(i = 0;i <m_lWidth; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewBmpBits1 + m_lWidth * j + i;
				
			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewBmpBits2 + m_lWidth * j + i;
		
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		}
	}
		
	while((n--)>0)
	{
		// 复制经过第一次边缘提取的图象到缓存图象2和3
		memcpy(lpNewBmpBits2, lpNewBmpBits1, m_lWidth * m_lHeight);
		memcpy(lpNewBmpBits3, lpNewBmpBits1, m_lWidth * m_lHeight);
		
		// 设置Sobel模板参数
		aTemplate[0] = -1.0;
		aTemplate[1] = -2.0;
		aTemplate[2] = -1.0;
		aTemplate[3] = 0.0;
		aTemplate[4] = 0.0;
		aTemplate[5] = 0.0;
		aTemplate[6] = 1.0;
		aTemplate[7] = 2.0;
		aTemplate[8] = 1.0;
		
		// 调用Template()函数
		if (!Template(lpNewBmpBits2,iTempH,iTempW,iTempMX,iTempMY,
			aTemplate,fTempC))
		{
			return FALSE;
		}
		
		// 设置Sobel模板参数
		aTemplate[0] = -1.0;
		aTemplate[1] = 0.0;
		aTemplate[2] = 1.0;
		aTemplate[3] = -2.0;
		aTemplate[4] = 0.0;
		aTemplate[5] = 2.0;
		aTemplate[6] = -1.0;
		aTemplate[7] = 0.0;
		aTemplate[8] = 1.0;
		
		// 调用Template()函数
		if (!Template(lpNewBmpBits3,iTempH,iTempW,
			iTempMX,iTempMY,aTemplate,fTempC))
		{
			return FALSE;
		}
		
		//求两幅缓存图像的最大值
		for(j = 0; j <m_lHeight; j++)
		{
			for(i = 0;i <m_lWidth; i++)
			{
				// 指向缓存图像1倒数第j行，第i个象素的指针			
				lpDst2 = (char *)lpNewBmpBits2 + m_lWidth * j + i;

				// 指向缓存图像2倒数第j行，第i个象素的指针			
				lpDst3 = (char *)lpNewBmpBits3 + m_lWidth * j + i;
			
				if(*lpDst3 > *lpDst2)
					*lpDst2 = *lpDst3;
			}
		}
		
		//求两次边缘检测结果的差
		for(j = 0; j <m_lHeight; j++)
		{
			for(i = 0;i <m_lWidth; i++)
			{
				// 指向缓存图像1倒数第j行，第i个象素的指针			
				lpDst1 = (char *)lpNewBmpBits1 + m_lWidth * j + i;
				// 指向缓存图像2倒数第j行，第i个象素的指针		
				lpDst2 = (char *)lpNewBmpBits2 + m_lWidth * j + i;
				
				//求对应象素点的差
				pixel=*lpDst1-*lpDst2;

				//求最终边缘点
				if(pixel<0)
					//如果小于0，赋0
					*lpDst1=(unsigned char)0;
				else
					//大于0，赋差值
					*lpDst1=(unsigned char)pixel;
			}
		}
	}

	// 复制经过模板运算后的图像到源图像
	memcpy(m_lpBits, lpNewBmpBits1, m_lWidth * m_lHeight);

	// 释放内存
	LocalUnlock(hNewBmpBits1);
	LocalFree(hNewBmpBits1);

	LocalUnlock(hNewBmpBits2);
	LocalFree(hNewBmpBits2);

	LocalUnlock(hNewBmpBits3);
	LocalFree(hNewBmpBits3);

	// 返回
	return TRUE;
}

inline BOOL CImgPro::LaplacianThin(int n)
{
	//用Sobel算子细化Laplacian边缘
	// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;
	LPSTR   lpDst3;
	
	// 指向缓存位图图像的指针
	LPSTR	lpNewBmpBits1;
	HLOCAL	hNewBmpBits1;
	LPSTR	lpNewBmpBits2;
	HLOCAL	hNewBmpBits2;
	LPSTR	lpNewBmpBits3;
	HLOCAL	hNewBmpBits3;

	//象素差值
	int pixel;

	//循环变量
	long i;
	long j;

	// 模板高度
	int	iTempH;
	
	// 模板宽度
	int	iTempW;
	
	// 模板系数
	FLOAT fTempC;
	
	// 模板中心元素X坐标
	int	iTempMX;
	
	// 模板中心元素Y坐标
	int	iTempMY;
	
	//模板数组
	FLOAT aTemplate[9];

	// 暂时分配内存，以保存新图像
	hNewBmpBits1 = LocalAlloc(LHND, m_lWidth * m_lHeight);

	if (hNewBmpBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBmpBits1 = (char * )LocalLock(hNewBmpBits1);

	// 暂时分配内存，以保存新图像
	hNewBmpBits2 = LocalAlloc(LHND, m_lWidth *m_lHeight);

	if (hNewBmpBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBmpBits2 = (char * )LocalLock(hNewBmpBits2);

	// 暂时分配内存，以保存新图像
	hNewBmpBits3 = LocalAlloc(LHND, m_lWidth *m_lHeight);

	if (hNewBmpBits3 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewBmpBits3 = (char * )LocalLock(hNewBmpBits3);

	// 拷贝源图像到缓存图像中
	lpDst1 = (char *)lpNewBmpBits1;
	memcpy(lpNewBmpBits1, m_lpBits, m_lWidth * m_lHeight);
	lpDst2 = (char *)lpNewBmpBits2;
	memcpy(lpNewBmpBits2, m_lpBits, m_lWidth * m_lHeight);
	lpDst3 = (char *)lpNewBmpBits3;
	memcpy(lpNewBmpBits3, m_lpBits, m_lWidth * m_lHeight);


	// 设置Laplacian模板参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 0.25;
	iTempMX = 1;
	iTempMY = 1;
	aTemplate[0] = 0.0;
	aTemplate[1] = 1.0;
	aTemplate[2] = 0.0;
	aTemplate[3] = 1.0;
	aTemplate[4] = -4.0;
	aTemplate[5] = 1.0;
	aTemplate[6] = 0.0;
	aTemplate[7] = 1.0;
	aTemplate[8] = 0.0;
		
	// 调用Template()函数
	if (!Template(lpNewBmpBits1,iTempH,iTempW,iTempMX,iTempMY,
		aTemplate,fTempC))
	{
		return FALSE;
	}
				
	while((n--)>0)
	{
		// 复制经过第一次边缘提取的图象到缓存图象2和3
		memcpy(lpNewBmpBits2, lpNewBmpBits1, m_lWidth * m_lHeight);
		memcpy(lpNewBmpBits3, lpNewBmpBits1, m_lWidth * m_lHeight);
		
		// 设置Sobel模板参数
		aTemplate[0] = -1.0;
		aTemplate[1] = -2.0;
		aTemplate[2] = -1.0;
		aTemplate[3] = 0.0;
		aTemplate[4] = 0.0;
		aTemplate[5] = 0.0;
		aTemplate[6] = 1.0;
		aTemplate[7] = 2.0;
		aTemplate[8] = 1.0;
		
		// 调用Template()函数
		if (!Template(lpNewBmpBits2,iTempH,iTempW,iTempMX,iTempMY,
			aTemplate,fTempC))
		{
			return FALSE;
		}
		
		// 设置Sobel模板参数
		aTemplate[0] = -1.0;
		aTemplate[1] = 0.0;
		aTemplate[2] = 1.0;
		aTemplate[3] = -2.0;
		aTemplate[4] = 0.0;
		aTemplate[5] = 2.0;
		aTemplate[6] = -1.0;
		aTemplate[7] = 0.0;
		aTemplate[8] = 1.0;
		
		// 调用Template()函数
		if (!Template(lpNewBmpBits3,iTempH,iTempW,
			iTempMX,iTempMY,aTemplate,fTempC))
		{
			return FALSE;
		}
		
		//求两幅缓存图像的最大值
		for(j = 0; j <m_lHeight; j++)
		{
			for(i = 0;i <m_lWidth; i++)
			{
				// 指向缓存图像1倒数第j行，第i个象素的指针			
				lpDst2 = (char *)lpNewBmpBits2 + m_lWidth * j + i;

				// 指向缓存图像2倒数第j行，第i个象素的指针			
				lpDst3 = (char *)lpNewBmpBits3 + m_lWidth * j + i;
			
				if(*lpDst3 > *lpDst2)
					*lpDst2 = *lpDst3;
			}
		}
		
		//求两次边缘检测结果的差
		for(j = 0; j <m_lHeight; j++)
		{
			for(i = 0;i <m_lWidth; i++)
			{
				// 指向缓存图像1倒数第j行，第i个象素的指针			
				lpDst1 = (char *)lpNewBmpBits1 + m_lWidth * j + i;
				// 指向缓存图像2倒数第j行，第i个象素的指针		
				lpDst2 = (char *)lpNewBmpBits2 + m_lWidth * j + i;
				
				//求对应象素点的差
				pixel=*lpDst1-*lpDst2;

				//求最终边缘点
				if(pixel<0)
					//如果小于0，赋0
					*lpDst1=(unsigned char)0;
				else
					//大于0，赋差值
					*lpDst1=(unsigned char)pixel;
			}
		}
	}

	// 复制经过模板运算后的图像到源图像
	memcpy(m_lpBits, lpNewBmpBits1, m_lWidth * m_lHeight);

	// 释放内存
	LocalUnlock(hNewBmpBits1);
	LocalFree(hNewBmpBits1);

	LocalUnlock(hNewBmpBits2);
	LocalFree(hNewBmpBits2);

	LocalUnlock(hNewBmpBits3);
	LocalFree(hNewBmpBits3);

	// 返回
	return TRUE;
}

inline unsigned char CImgPro::EntropyThreshold()
{
	// 指向源图像的指针
	LPSTR	lpSrc;

	//循环变量
	long i;
	long j;
	
	//像素值
	unsigned char pixel;

	//阈值
	unsigned char iThreshold;

	//直方图数组
	long lHistogram[256];
	//灰度级出现的概率
	double fProb[256];
	//求目标区域和背景区域的熵的和
	double fOBEntropy=0.0;
	//图象的熵
	double fEntropy=0.0;
	//低灰度区的熵
	double fLowEntropy=0.0;
	//低灰度区的概率
	double fLowProb=0.0;
	//一维最大熵
	double fMaxEntropy=0.0;

	for (i = 0; i < 256;i++)
	{
		lHistogram[i]=0;
	}

	for(i=0;i<256;i++)
		fProb[i]=0.0;

	//求灰度直方图
	for (i = 0;i <m_lHeight ;i++)
	{
		for(j = 0;j <m_lWidth ;j++)
		{
			// 指向源图像倒数第j行，第i个象素的指针	

				lpSrc = (char *)m_lpBits + m_lWidth * i + j;
				pixel = (unsigned char)*lpSrc;
				lHistogram[pixel]++;
			
		}
	}

	//求灰度级出现的概率
	for(i=0;i<256;i++)
	{
		fProb[i]=((double)lHistogram[i])/(m_lWidth*m_lHeight);
	}

	//求图象的熵
	for(i=0;i<256;i++)
	{
		if(fProb[i]!=0.0)
			fEntropy+=-fProb[i]*log10(fProb[i]);
	}

	//求阈值
	for(i=0;i<256;i++)
	{
		for(j=0;j<=i;j++)
		{	
			//求低灰度区的概率与熵
			fLowProb+=fProb[j];
			if(fProb[j]!=0.0)
				fLowEntropy+=(-fProb[j]*log10(fProb[j]));
		}

		//求目标区域和背景区域的熵的和
		if(fLowProb==1)
		{
			fOBEntropy=fEntropy;
		}
		else
		{
			fOBEntropy=log10(fLowProb*(1-fLowProb))+fLowEntropy/fLowProb+
			(fEntropy-fLowEntropy)/(1-fLowProb);
		}

		if(fOBEntropy>fMaxEntropy)
		{
			//找使熵最小的iGray
			fMaxEntropy=fOBEntropy;
			iThreshold=(unsigned char)i;
		}
		
		fLowProb=0.0;
		fLowEntropy=0.0;
	}

	//返回阈值
	return iThreshold;
}
inline void CImgPro::SetPixelValue(BYTE* pData,int w,int h,POINT pt,byte gray)
{
	int x=pt.x;
	int y=pt.y;
	if(x>=0&&x<w&&y>=0&&y<h)
	{
		pData[y*w+x]=gray;
	}

}
inline unsigned char CImgPro::NormalThreshold()
{
	// 指向源图像的指针
	LPSTR	lpSrc;

	//像素值
	unsigned char pixel;

	//循环变量
	long i;
	long j;
	long k;
	long m;

	double minf;
	double q[256],f[256];

	//阈值
	unsigned char T;

	//直方图数组
	long h[256];

	for (i = 0; i < 256;i++)
	{
		h[i]=0;
		f[i]=0.0;
		q[i]=0.0;
	}

	//求灰度直方图
	for (i = 0;i <m_lHeight ;i++)
	{
		for(j = 0;j <m_lWidth ;j++)
		{
			// 指向源图像倒数第j行，第i个象素的指针	

				lpSrc = (char *)m_lpBits + m_lWidth * i + j;
				pixel = (unsigned char)*lpSrc;
				h[pixel]++;
			
		}
	}
    i=0;
	while (i*h[i]==0)
		i++;
	k=256;
	while (h[k]==0)
		k--;
	for(i;i<k;i++)
	{
		double LO1,LO,LB1,LB,QO,QB,po,pb,uo,uo1,ub,ub1,A,B,C,D,f1,f2;
		po=h[0];uo1=0.0;
		for(j=1;j<=i;j++)
		{
			po=po+h[j];
			uo1=uo1+j*h[j];
		}
		uo=uo1/po;//uo为类类内均值
		pb=h[j];ub1=j*h[j];
		for(j=j+1;j<k;j++)
		{
			pb=pb+h[j];
			ub1=ub1+j*h[j];
		}
		ub=ub1/pb;//ub为b类类内均值
        LO1=h[0]*uo*uo;
		for(m=1;m<i;m++)
		{
			LO1=LO1+h[m]*(m-uo)*(m-uo);
		}
		LO=LO1/po;//LO为o类类内方差
		LB1=h[m]*(m-ub)*(m-ub);
		for(m=m+1;m<k;m++)
		{
			LB1=LB1+h[m]*(m-ub)*(m-ub);
		}
		LB=LB1/pb;//LB为b类类内方差
		for(m=0;m<k;m++)//求得假设中分割后的分布q[]
		{
			if(m<=i)
				q[m]=exp(-(m-uo)*(m-uo)/(2*LO*LO))/(2.50599*LO);
            else
				q[m]=exp(-(m-ub)*(m-ub)/(2*LB*LB))/(2.50599*LB);
		}
		QO=q[0];
		for(j=1;j<=i;j++)
		{
			QO=QO+q[j];
		}
		QB=q[j];
		for(j=i+1;j<k;j++)
		{
			QB=QB+q[j];
		}
		//构造判决式
		A=1+h[i]/po;
		B=1+q[i]/QO;
		C=1+h[i]/pb;
		D=1+q[i]/QB;
		f1=0.0;f2=0.0;
		for(m=0;m<=i;m++)
		{
			f1=f1+A*log(A/B)+B*log(B/A);//判决式
		}
		for(m;m<k;m++)
		{
			f2=f2+C*log(C/D)+D*log(D/C);
		}
		f[i]=f1+f2;
	}
	 i=0;
	while (i*h[i]==0)
		i++;
	k=256;
	while (h[k]==0)
		k--;

	minf=f[i];T=(unsigned char)i;
	//进行最小判决
	for(j=i+1;j<k;j++)
		{
			if (minf>f[j])
			{
				minf=f[j];
				T=(unsigned char)j;
			}
		}
	return T;
}

inline unsigned char CImgPro::EqualityThreshold()
{
	// 指向源图像的指针
	LPSTR	lpSrc;

	//像素值
	unsigned char pixel;

	int i,j,k;
	unsigned char T2;
	long h[256];
	double f[256];
    double minf;
    T2=0;
	minf=0.0;

	for (i = 0; i < 256;i++)
	{
		h[i]=0;
		f[i]=0.0;
	}

	//求灰度直方图
	for (i = 0;i <m_lHeight ;i++)
	{
		for(j = 0;j <m_lWidth ;j++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			

				lpSrc = (char *)m_lpBits + m_lWidth * i + j;
				pixel = (unsigned char)*lpSrc;
				h[pixel]++;
			
		}
	}
    i=0;
	while (h[i]==0)
	    i++;
	k=255;
	while (h[k]==0)
		k--;
		
	for (i;i<k;i++)
	{
		double pb,ub,ub1,uo,uo1,po;
		uo=h[i]*log((float)h[i]);
		uo1=log((float)h[i]);
		po=0.0;
		//po=h[0];
	    //if(h[0]==0)  {uo1=0.0;uo=0.0;}
	    //else {uo1=h[0]*log(h[0]);	uo=log(h[0]);}
		for(j=0;j<=i;j++)
		{
            po=po+h[j];
			if(h[j]!=0)
			{
		    	uo=uo+log((float)h[j]);
				uo1=uo1+h[j]*log((float)h[j]);
			}
		}
	        pb=h[j];
			if (h[j]!=0)
			{ub1=h[i]*log((float)h[i]);ub=log((float)h[i]);}
			else {ub1=0.0;ub=0.0;}
		for(j=j+1;j<=k;j++)
		{
			pb=pb+h[j];
			if (h[j]!=0)
			{ub=ub+log((float)h[j]);
			ub1=ub1+h[j]*log((float)h[j]);}
		}
		f[i]=uo1/po+ub1/pb-uo/(i+1)-ub/(255-i-1);
	}
	i=0;
	while (h[i]==0)
       i++;
	T2=(unsigned char)i;
	for(j=i+1;j<k;j++)
	{	if (f[i]>f[j])
		{
			minf=f[i];
			f[i]=f[j];
			f[j]=minf;
			T2=(unsigned char)j;
		}
	}
		return T2;
}

inline unsigned char CImgPro::ImprovedThreshold()
{
	// 指向源图像的指针
	LPSTR	lpSrc;

	//像素值
	unsigned char pixel;

	//直方图数组
    float lHistogram[256];

	long i,j,k;
	unsigned char maxt;
	double maxf,HT;
	double p[256],f[256];

	for (i = 0; i < 256;i++)
	{
		lHistogram[i]=0.0;
		f[i]=0.0;
	}

    

	//求灰度直方图
	for (i = 0;i <m_lHeight ;i++)
	{
		for(j = 0;j <m_lWidth ;j++)
		{
			// 指向源图像倒数第j行，第i个象素的指针	

				lpSrc = (char *)m_lpBits + m_lWidth * i + j;
				pixel = (unsigned char)*lpSrc;
				lHistogram[pixel]++;
			
		}
	}
     HT=0.0;  //是图像像素总的熵
	
	 //求灰度级出现的概率
	for(i=0;i<256;i++)
	{
        double t;
		p[i]=lHistogram[i]/(m_lWidth*m_lHeight);
        t=-p[i]*log(p[i]);
		if(p[i]==0.0) 
			t=0.0;
	    else HT+=t;
	}
	i=0;
	while (p[i]==0)
		i++;
	k=255;
	while(p[k]==0)
		k--;
	
	for(i;i<k;i++)
	{
	
		double pt,Ht,maxi,maxj;
		Ht=0.0;  //是图像被阈值分割后目标像素的熵
    	pt=0.0;  //是一个定向素的灰度值小于或等于阈值t的概率
		
		maxi=p[0];
		for(j=0;j<=i;j++)
		{
			double m;
			m=p[j]*log(p[j]);
			if(p[j]==0.0)
				m=0.0;
			else Ht=Ht-m;
			pt=p[j]+pt;
			
	        if (maxi<p[j])
				maxi=p[j];
		}
	
		maxj=p[j];
		for(j=j+1;j<k;j++)
		{if (maxj<p[j])
			   maxj=p[j];
		}
		
		f[i]=( Ht/HT)*(log(pt)/log(maxi))+(1-Ht/HT)*(log(1-pt)/log(maxj));
	}
	i=0;
	while (p[i]==0)
		i++;
	maxf=f[i];
	maxt=(unsigned char)i;
	for(i=i+1;i<k;i++)
	{
        if (maxf<f[i])
		{
			maxf=f[i];
			maxt=(unsigned char)i;
		}
	}

	return maxt;
}


inline BOOL CImgPro::imageMatchPattern(const BYTE* pImage,
								   const int nImageWidth,
								   const int nImageHeight,
								   const int nImageRowLength,
								   CTemplate& tmpPattern,
								   const MatchPatternOptions& options,
								   const CRect& rectSearch,
								   PatternMatch* const pPatternMatch,
								   int* const numMatches)
{
	//
	//If the search image or the template is NULL,return FALSE.
	//
	if(!pImage || !tmpPattern.GetSubSample() || !tmpPattern.m_pSample){
		if(numMatches)
			*numMatches = 0;
		ASSERT(pPatternMatch);
		pPatternMatch->position.x  = 0;
		pPatternMatch->position.y  = 0;
		pPatternMatch->rotation    = 0;
		pPatternMatch->scale       = 1;
		pPatternMatch->score       = 0;
		pPatternMatch->corner[0].x = 0;
		pPatternMatch->corner[0].y = 0;
		pPatternMatch->corner[1].x = 0;
		pPatternMatch->corner[1].y = 0;
		pPatternMatch->corner[2].x = 0;
		pPatternMatch->corner[2].y = 0;
		pPatternMatch->corner[3].x = 0;
		pPatternMatch->corner[3].y = 0;
		return FALSE;
	}

	//
	//Get general information about the template.
	//
	int           nTmpWidth     = tmpPattern.m_nWidth;
	int			  nTmpHeight    = tmpPattern.m_nHeight;
	double*		  pSample       = tmpPattern.m_pSample;
	int           nSampleCount  = tmpPattern.m_nSampleCount;
	//
	HaltonSample* pHaltonSample = (HaltonSample*)tmpPattern.GetSubSample();
	ASSERT(pSample);
	ASSERT(pHaltonSample);

	int      nHaltonSampleCount = pHaltonSample->hsSamplePointCount;

	//
	//Initialize search option.
	//
	int nInitialStepSize   = options.initialStepSize ? options.initialStepSize : tmpPattern.GetStepSize();
	int nAngularStepSize   = tmpPattern.GetAngularStepSize();
	float minMatchScore    = options.minMatchScore ? options.minMatchScore : 850;
	//对比度
	double minContrast     = max(tmpPattern.m_minContrast - 200 , 100);
	double maxContrast     = tmpPattern.m_maxContrast + 50;

	//
	//Search window rectangle legality check.
	//
	ASSERT(rectSearch.left >= 0);
	ASSERT(rectSearch.top >= 0);
	ASSERT(rectSearch.right < nImageWidth);
	ASSERT(rectSearch.bottom < nImageHeight);
	ASSERT(rectSearch.Width()+1 >= nTmpWidth);
	ASSERT(rectSearch.Height()+1 >= nTmpHeight);

	//
	//Do preparation work before perform search.
	//
	int     nStartX          = rectSearch.left;
	int     nStartY          = rectSearch.top;
	int     nEndX            = rectSearch.right - nTmpWidth + 1;
	int     nEndY            = rectSearch.bottom - nTmpHeight  + 1;
	int     nSampleRowLength = nHaltonSampleCount+1;
	double* pExtraction      = tmpPattern.GetExtractionBuffer();

    ASSERT(pPatternMatch);
	pPatternMatch->position.x  = 0;
	pPatternMatch->position.y  = 0;
	pPatternMatch->rotation    = 0;
	pPatternMatch->scale       = 1;
	pPatternMatch->score       = 0;
	pPatternMatch->corner[0].x = 0;
	pPatternMatch->corner[0].y = 0;
	pPatternMatch->corner[1].x = 0;
	pPatternMatch->corner[1].y = 0;
	pPatternMatch->corner[2].x = 0;
	pPatternMatch->corner[2].y = 0;
	pPatternMatch->corner[3].x = 0;
	pPatternMatch->corner[3].y = 0;
	int    x        = 0;
	int    y        = 0;
	int    m        = 0;
	int    n        = 0;
	double ncc      = 0;
	float  maxScore = 0;

	double sumVector   = 0.0;
	double sumsqVector = 0.0;

	//
	//Do coarse search.
	//
	for(y = nStartY ; y <= nEndY ; y += nInitialStepSize)
		for(x = nStartX ; x <= nEndX ; x += nInitialStepSize)
		{

			//
			//Shift the set of reference Halton sample point coordinates to the pixel(x,y).
			//Extract the intensity values to memory location where pExtraction point to.
			//
			sumVector   = 0.0;
			sumsqVector = 0.0;
			for(n = 0 ; n < nHaltonSampleCount ; n++)
			{
				pExtraction[n] = pImage[(pHaltonSample->hsHaltonPoints[n].y + y) * nImageRowLength + \
					pHaltonSample->hsHaltonPoints[n].x + x];
		        //求和
				sumVector += pExtraction[n];
				//求平方和
		        sumsqVector += pExtraction[n]*pExtraction[n];
			}
			//求平均值
			sumVector /= nHaltonSampleCount;
			//均方差
			sumsqVector -= (nHaltonSampleCount*sumVector*sumVector);
			//均方
			sumsqVector = sqrt(sumsqVector);
			if(sumsqVector < minContrast || sumsqVector > maxContrast)
				continue;
			//
			//Normalize the extracted feature vector.
			//
			for(n = 0 ; n < nHaltonSampleCount ; n++)
				pExtraction[n] = (pExtraction[n] - sumVector)/sumsqVector;//图像的特征向量

			//
			//Compute normalized correlation between pExtraction and each line vector in pSample
			//归一化相关系数
			for(m = 0 ; m < nSampleCount ; m += nAngularStepSize)
			{
				ncc = 0.0;
				for(n = 1 ; n <= nHaltonSampleCount ; n++)
					ncc += pExtraction[n - 1] * pSample[m*nSampleRowLength + n];
                ncc *= 1000;
				maxScore = fabsf((float)ncc);
				if(maxScore > pPatternMatch->score)
				{
					pPatternMatch->position.x = (float)x;
					pPatternMatch->position.y = (float)y;
					pPatternMatch->rotation   = (float)pSample[m*nSampleRowLength];
					pPatternMatch->score = maxScore;
				}
			}
		}

	//
	//Do fine search preparation.
	//
	nStartX  = (int)max(pPatternMatch->position.x - 3 , rectSearch.left);
	nStartY  = (int)max(pPatternMatch->position.y - 3 , rectSearch.top);
	nEndX    = (int)min(pPatternMatch->position.x + 3 , rectSearch.right - nTmpWidth + 1);
	nEndY    = (int)min(pPatternMatch->position.y + 3 , rectSearch.bottom - nTmpHeight + 1);
	//int nStartAngle = pPatternMatch->rotation -;
	//int nEndAngle = ;

	//
	//Do fine search.
	//
	for(y = nStartY ; y <= nEndY ; y ++)
		for(x = nStartX ; x <= nEndX ; x ++)
		{

			//
			//Shift the set of reference Halton sample point coordinates to the pixel(x,y).
			//Extract the intensity values to memory location where pExtraction point to.
			//
			sumVector   = 0.0;
			sumsqVector = 0.0;
			for(n = 0 ; n < nHaltonSampleCount ; n++)
			{
				pExtraction[n] = pImage[(pHaltonSample->hsHaltonPoints[n].y + y) * nImageRowLength + \
					pHaltonSample->hsHaltonPoints[n].x + x];
		        sumVector += pExtraction[n];
		        sumsqVector += pExtraction[n]*pExtraction[n];
			}
			sumVector /= nHaltonSampleCount;
			sumsqVector -= (nHaltonSampleCount*sumVector*sumVector);
			sumsqVector = sqrt(sumsqVector);
			if(sumsqVector < minContrast || sumsqVector > maxContrast)
				continue;
			//
			//Normalize the extracted feature vector.
			//
			for(n = 0 ; n < nHaltonSampleCount ; n++)
				pExtraction[n] = (pExtraction[n] - sumVector)/sumsqVector;

			//
			//Compute normalized correlation between pExtraction and each line vector in pSample
			//
			for(m = 0 ; m < nSampleCount ; m ++)
			{
				ncc = 0.0;
				for(n = 1 ; n <= nHaltonSampleCount ; n++)
					ncc += pExtraction[n - 1] * pSample[m*nSampleRowLength + n];
                ncc *= 1000;
				maxScore = fabsf((float)ncc);
				if(maxScore > pPatternMatch->score)
				{
					pPatternMatch->position.x = (float)x;
					pPatternMatch->position.y = (float)y;
					pPatternMatch->rotation   = (float)pSample[m*nSampleRowLength];
					pPatternMatch->score = maxScore;
				}
			}
		}
	//
	//Post processing
	//
	pPatternMatch->position.x += nTmpWidth/2;
	pPatternMatch->position.y += nTmpHeight/2;

	double r     = sqrt(pow(nTmpWidth/2.0 , 2) + pow(nTmpHeight/2.0 , 2));
	double theta = atan2(nTmpHeight/2.0 , nTmpWidth/2.0);
	float cornerX = (float)(r*cos(theta + (PI/180)*pPatternMatch->rotation));
	float cornerY = (float)(r*sin(theta + (PI/180)*pPatternMatch->rotation));
    pPatternMatch->corner[0].x = pPatternMatch->position.x + cornerX;
	pPatternMatch->corner[0].y = pPatternMatch->position.y + cornerY;
	cornerX = (float)(r*cos(PI - theta + (PI/180)*pPatternMatch->rotation));
	cornerY = (float)(r*sin(PI - theta + (PI/180)*pPatternMatch->rotation));
    pPatternMatch->corner[1].x = pPatternMatch->position.x + cornerX;
	pPatternMatch->corner[1].y = pPatternMatch->position.y + cornerY;
	cornerX = (float)(r*cos(PI + theta + (PI/180)*pPatternMatch->rotation));
	cornerY = (float)(r*sin(PI + theta + (PI/180)*pPatternMatch->rotation));
    pPatternMatch->corner[2].x = pPatternMatch->position.x + cornerX;
	pPatternMatch->corner[2].y = pPatternMatch->position.y + cornerY;
	cornerX = (float)(r*cos(0 - theta + (PI/180)*pPatternMatch->rotation));
	cornerY = (float)(r*sin(0 - theta + (PI/180)*pPatternMatch->rotation));
    pPatternMatch->corner[3].x = pPatternMatch->position.x + cornerX;
	pPatternMatch->corner[3].y = pPatternMatch->position.y + cornerY;

//  pPatternMatch->position.y  = nImageHeight - pPatternMatch->position.y - 1;
//	pPatternMatch->corner[0].y = nImageHeight - pPatternMatch->corner[0].y - 1;
//	pPatternMatch->corner[1].y = nImageHeight - pPatternMatch->corner[1].y - 1;
//	pPatternMatch->corner[2].y = nImageHeight - pPatternMatch->corner[2].y - 1;
//	pPatternMatch->corner[3].y = nImageHeight - pPatternMatch->corner[3].y - 1;

	if(pPatternMatch->score > minMatchScore)
	{
		if(numMatches)
			*numMatches = 1;

		return TRUE;
	}

	return FALSE;
}

/*
inline void CImgPro::mergeSegments(Mat & image,Mat & segments, int & numOfSegments)
{
	//To collect pixels from each segment of the image
	vector<Mat> samples;
	//In case of multiple merging iterations, the numOfSegments should be updated
	int newNumOfSegments = numOfSegments;

	//Initialize the segment samples
	for(int i=0;i<=numOfSegments;i++)
	{
		Mat sampleImage;
		samples.push_back(sampleImage);
	}

	//collect pixels from each segments
	for(int i = 0; i < segments.rows; i++ )
	{
		for(int j = 0; j < segments.cols; j++ )
		{
			//check what segment the image pixel belongs to
			int index = segments.at<int>(i,j);
			if(index >= 0 && index<numOfSegments)
			{
				samples[index].push_back(image(Rect(j,i,1,1)));	
			}
		}
	}

	//create histograms
	vector<MatND> hist_bases;
	Mat hsv_base;
	/// Using 35 bins for hue component 
	int h_bins = 35;
	/// Using 30 bins for saturation component
	int s_bins = 30;
	int histSize[] = { h_bins,s_bins };

	// hue varies from 0 to 256, saturation from 0 to 180
	float h_ranges[] = { 0, 256 };	
	float s_ranges[] = { 0, 180 };

	const float* ranges[] = { h_ranges, s_ranges };

	// Use the 0-th and 1-st channels
	int channels[] = { 0,1 };

	// To store the histograms
	MatND hist_base;
	for(int c=1;c<numOfSegments;c++)
	{
		if(samples[c].dims>0){
			//convert the sample to HSV
			cvtColor( samples[c], hsv_base, CV_BGR2HSV );
			//calculate the histogram
			calcHist( &hsv_base, 1, channels, Mat(), hist_base,2, histSize, ranges, true, false );
			//normalize the histogram
			normalize( hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat() );
			//append to the collection 
			hist_bases.push_back(hist_base);
		}else
		{
			hist_bases.push_back(MatND());
		}

		hist_base.release();
	}

	//To store the similarity of histograms
	double similarity = 0;
	//to keep the track of already merged segments
	vector<bool> mearged;
	//initialize the merged segments tracker
	for(int k = 0; k < hist_bases.size(); k++)
	{
		mearged.push_back(false);
	}	

	//calculate the similarity of the histograms of each pair of segments
	for(int c=0;c<hist_bases.size();c++)
	{
		for(int q=c+1;q<hist_bases.size();q++)
		{
			//if the segment is not merged alreay
			if(!mearged[q])
			{
				if(hist_bases[c].dims>0 && hist_bases[q].dims>0)
				{
					//calculate the histogram similarity
					similarity = compareHist(hist_bases[c],hist_bases[q],CV_COMP_BHATTACHARYYA);
					//if similay
					if(similarity>0.8)
					{
						mearged[q]=true;
						if(q!=c)
						{
							//reduce number of segments
							newNumOfSegments--;
							for(int i = 0; i < segments.rows; i++ )
							{
								for(int j = 0; j < segments.cols; j++ )
								{
									int index = segments.at<int>(i,j);
									//merge the segment q with c
									if(index==q){
										segments.at<int>(i,j) = c;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	numOfSegments = newNumOfSegments;
}

inline Mat CImgPro::watershedSegment(Mat & image, int & noOfSegments)
{	
	//To store the gray version of the image
	Mat gray;
	//To store the thresholded image
	Mat ret;
	//convert the image to grayscale
	cvtColor(image,gray,CV_BGR2GRAY);
	imshow("Gray Image",gray);
	//threshold the image
	threshold(gray,ret,0,255,CV_THRESH_BINARY_INV+CV_THRESH_OTSU);
	imshow("Image after OTSU Thresholding",ret);
	//Execute morphological-open
	morphologyEx(ret,ret,MORPH_OPEN,Mat::ones(9,9,CV_8SC1),Point(4,4),2);
	imshow("Thresholded Image after Morphological open",ret);
	//get the distance transformation 
	Mat distTransformed(ret.rows,ret.cols,CV_32FC1);
	distanceTransform(ret,distTransformed,CV_DIST_L2,3);
	//normalize the transformed image in order to display
	normalize(distTransformed, distTransformed, 0.0, 1, NORM_MINMAX);
	imshow("Distance Transformation",distTransformed);
	//threshold the transformed image to obtain markers for watershed
	threshold(distTransformed,distTransformed,0.1,1,CV_THRESH_BINARY);
	//Renormalize to 0-255 to further calculations
	normalize(distTransformed, distTransformed, 0.0, 255.0, NORM_MINMAX);
	distTransformed.convertTo(distTransformed,CV_8UC1);
	imshow("Thresholded Distance Transformation",distTransformed);

	//calculate the contours of markers
	int i, j, compCount = 0;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(distTransformed, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	if( contours.empty() )
		return Mat();
	Mat markers(distTransformed.size(), CV_32S);
	markers = Scalar::all(0);
	int idx = 0;
	//draw contours 
	for( ; idx >= 0; idx = hierarchy[idx][0], compCount++ )
		drawContours(markers, contours, idx, Scalar::all(compCount+1), -1, 8, hierarchy, INT_MAX);

	if( compCount == 0 )
		return Mat();

	//calculate the time taken to the watershed algorithm
	double t = (double)getTickCount();
	//apply watershed with the markers as seeds
	watershed( image, markers );
	t = (double)getTickCount() - t;
	printf( "execution time = %gms\n", t*1000./getTickFrequency() );

	//create displayable image of segments
	Mat wshed = createSegmentationDisplay(markers,compCount);

	imshow( "watershed transform", wshed );
	noOfSegments = compCount;

	//returns the segments
	return markers;
}
lo.bLearnForRotation      = TRUE;
lo.finalAngularAccuracy   = 1;
lo.initialAngularAccuracy = 3;
lo.initialSampleSize      = 480;
lo.finalSampleSize        =960;
lo.initialStepSize        = 1;
lo.maxAngular             = 180;
lo.minAngular             = 0;
inline Mat CImgPro::createSegmentationDisplay(Mat & segments,int numOfSegments,Mat & image)
{
	//create a new image
	Mat wshed(segments.size(), CV_8UC3);

	//Create color tab for coloring the segments
	vector<Vec3b> colorTab;
	for(int i = 0; i < numOfSegments; i++ )
	{
		int b = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int r = theRNG().uniform(0, 255);

		colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}

	//assign different color to different segments
	for(int i = 0; i < segments.rows; i++ )
	{
		for(int j = 0; j < segments.cols; j++ )
		{
			int index = segments.at<int>(i,j);
			if( index == -1 )	
				wshed.at<Vec3b>(i,j) = Vec3b(255,255,255);
			else if( index <= 0 || index > numOfSegments )
				wshed.at<Vec3b>(i,j) = Vec3b(0,0,0);
			else
				wshed.at<Vec3b>(i,j) = colorTab[index - 1]; 
		}
	}

	//If the original image available then merge with the colors of segments
	if(image.dims>0)
		wshed = wshed*0.5+image*0.5;

	return wshed;
}
*/

inline ImageBlobs::ImageBlobs() : m_lpBits(0),m_lHeight(0),m_lWidth(0)
{

}

inline ImageBlobs::~ImageBlobs()
{
	if (m_lpBits != 0)
		delete m_lpBits;        
	delete_blobs();
}

inline void ImageBlobs::init(unsigned int width, unsigned int height)
{        
	if (m_lpBits != 0)
		delete m_lpBits;
	m_lpBits = new BYTE[height* (((width * 8 + 31) & ~31)>>3)];  
	m_lWidth=width;
	m_lHeight=height;
}

inline int ImageBlobs::find_blobs(LPBYTE image,int w,int h, unsigned int min_elements_per_blob)
{
//	if (m_lpBits == 0)       //not initialized
//		return -1;
	init(w,h);
//	m_image->copy(image);        
	memcpy(m_lpBits,image,h*(((w * 8 + 31) & ~31)>>3));
	int linebytes=(((m_lWidth * 8 + 31) & ~31)>>3);

	//--------------------------------------

	unsigned int y, x;

	for (y = 0; y < m_lHeight; y++) 
	{
		for (x = 0; x < m_lWidth; x++)
		{
			if (*(m_lpBits+y*linebytes+ x) != 0)
			{  	
				//record blob element			
				struct Blob blob; 
				blob.elements_number = 0;
				blob.area = 0; 
				
				*(m_lpBits+y*linebytes+ x)=0;
				struct Element element;
				element.coord.x = x;
				element.coord.y = y;                                                                                
				blob.elements_number = 1;                                        
				blob.elements.push_back(element);                                        
				blob.area = 0;
				memset(&blob.bounding_box, 0, sizeof(RECT));
				//find a blob start pixel
				blob.elements.reserve(m_lWidth * m_lHeight);

				//find the rest pixel belong to this blob
				unsigned int index = 0;
				while (index < blob.elements_number) 
				{
					unsigned int N = (unsigned int)blob.elements_number;
					for (unsigned int i = index; i < N; i++)
					{
						add_up_neighbour(blob, i);
						add_right_neighbour(blob, i);
						add_down_neighbour(blob, i);
						add_left_neighbour(blob, i);                                                
					}
					index = N;
				}
				if (blob.elements_number > min_elements_per_blob) 
				{
					blob.area = (unsigned int)blob.elements_number;
					blob.elements.reserve(blob.elements_number);
					m_blobs.push_back(blob);                                                                
				}  
			}
		}
	}
	return get_blobs_number();  
}


inline void ImageBlobs::delete_blobs()
{
	for (unsigned int i = 0; i < get_blobs_number(); i++) 
	{
		for (unsigned int j = 0; j < (unsigned int)m_blobs[i].elements_number; j++)
			m_blobs[i].elements[j].neighbs.clear();
		m_blobs[i].elements.clear();
	}
	m_blobs.clear();
}

inline void ImageBlobs::remove_blob_from_image(const struct Blob& blob)
{
	int linebytes=(((m_lWidth * 8 + 31) & ~31)>>3);
	for (unsigned int i = 0; i < (unsigned int)blob.elements_number; i++) 
	{
		const struct Element &element = blob.elements[i]; 
		*(m_lpBits+element.coord.y*linebytes+ element.coord.x)=0;
		//(*m_image)(element.coord.y, element.coord.x) = 0;
	}
}


inline void ImageBlobs::mark_blobs_on_image()
{
	int linebytes=(((m_lWidth * 8 + 31) & ~31)>>3);
	for (unsigned int i = 0; i < get_blobs_number(); i++) {
		const struct Blob* blob = get_blob(i);
		for (unsigned int j = 0; j < (unsigned int)blob->elements_number; j++) {
			const struct Element& element = blob->elements[j];
			*(m_lpBits+element.coord.y*linebytes+ element.coord.x)=i + 1;
			//(*m_image)(element.coord.y, element.coord.x) = i + 1;                        
		}        
	}
}


inline void ImageBlobs::find_bounding_boxes()
{
	for (unsigned int i = 0; i < get_blobs_number(); i++) {
		Blob& blob = m_blobs[i];
		blob.bounding_box.top = m_lHeight;
		blob.bounding_box.left = m_lWidth;
		blob.bounding_box.right = 0;
		blob.bounding_box.bottom = 0;
		for (unsigned int j = 0; j < (unsigned int)blob.elements_number; j++) {
			const struct Element& element = blob.elements[j];
			if (element.coord.y < blob.bounding_box.top)
				blob.bounding_box.top = element.coord.y;
			if (element.coord.x < blob.bounding_box.left)
				blob.bounding_box.left = element.coord.x;
			if (element.coord.x > blob.bounding_box.right)
				blob.bounding_box.right = element.coord.x;
			if (element.coord.y > blob.bounding_box.bottom)
				blob.bounding_box.bottom = element.coord.y;                        
		}
		blob.bounding_box.right++;
		blob.bounding_box.bottom++;
	}
}

// Inlines
inline unsigned int ImageBlobs::get_blobs_number() const
{
	return (unsigned int)m_blobs.size();
}

inline const struct Blob* ImageBlobs::get_blob(unsigned int i) const
{
	return &m_blobs[i];
}

inline int ImageBlobs::is_element_present(const struct Blob& blob, const struct Element& new_element) const
{
	//int index = 0;
	for(int i = (int)blob.elements_number - 1; i >= 0; i--) {        
		const struct Element& element = blob.elements[i];
		if (element.coord.x == new_element.coord.x &&
			element.coord.y == new_element.coord.y) {
				//wprintf(L" %d\n", blob.elements_number - 1 - i);
				return i;
		}
		//if (++index > 2)  //inspect at least 2 last elements
		//        break;                
	}
	return -1;
}


inline bool ImageBlobs::has_neighbour(const struct Element& element, const struct Element& new_element) const
{
	unsigned int N = (unsigned int)element.neighbs.size();
	if (N > 0) {
		for (unsigned int i = 0; i < N; i++) {
			if (element.neighbs[i].coord.x == new_element.coord.x &&
				element.neighbs[i].coord.y == new_element.coord.y) 
				return true;                        
		}
		return false;
	}
	else
		return false;
}

inline unsigned int ImageBlobs::add_up_neighbour(struct Blob& blob, unsigned int i)
{
	int linebytes=(((m_lWidth * 8 + 31) & ~31)>>3);
	const struct Element& element = blob.elements[i];
	if (element.coord.y - 1 < 0)
		return 0;
	else if ((BYTE)(*(m_lpBits+(element.coord.y-1)*linebytes+element.coord.x))>0)//(*m_image)(element.coord.y - 1, element.coord.x) > 0) {
	{
		*(m_lpBits+(element.coord.y-1)*linebytes+element.coord.x)=0;
		struct Element new_element;
		new_element.coord.x = element.coord.x;
		new_element.coord.y = element.coord.y - 1;
		new_element.neighbs.push_back(element);
		blob.elements_number++;
		blob.elements.push_back(new_element);                        
		return 1;
		if (has_neighbour(element, new_element) == false) { 
			int index = is_element_present(blob, new_element);
			if (index >= 0) {
				blob.elements[index].neighbs.push_back(element);
				return 0;
			}                        
			new_element.neighbs.push_back(element);
			blob.elements_number++;
			blob.elements.push_back(new_element);                        
			return 1;
		}
		else
			return 0;
	}
	else 
		return 0;
}

inline unsigned int ImageBlobs::add_right_neighbour(struct Blob& blob, unsigned int i)
{
	int linebytes=(((m_lWidth * 8 + 31) & ~31)>>3);
	const struct Element& element = blob.elements[i];
	if (element.coord.x + 1 >= m_lWidth)
		return 0;
	else if ((*(m_lpBits+(element.coord.y)*linebytes+element.coord.x+1))>0)//(*m_image)(element.coord.y, element.coord.x + 1) > 0) {
	{	
		*(m_lpBits+(element.coord.y)*linebytes+element.coord.x+1)=0;
		struct Element new_element;
		new_element.coord.x = element.coord.x + 1;
		new_element.coord.y = element.coord.y; 
		blob.elements_number++;
		blob.elements.push_back(new_element);                        
		return 1;
		if (has_neighbour(element, new_element) == false) {
			int index = is_element_present(blob, new_element);
			if (index >= 0) {
				blob.elements[index].neighbs.push_back(element);
				return 0;
			}                        
			blob.elements_number++;
			blob.elements.push_back(new_element);                        
			return 1;
		}
		else
			return 0;
	}
	else 
		return 0;
}

inline unsigned int ImageBlobs::add_down_neighbour(struct Blob& blob, unsigned int i)
{
	int linebytes=(((m_lWidth * 8 + 31) & ~31)>>3);
	const struct Element& element = blob.elements[i];
	if (element.coord.y + 1 >= m_lHeight)
		return 0;
	else if ((*(m_lpBits+(element.coord.y+1)*linebytes+element.coord.x))>0)//(*m_image)(element.coord.y + 1, element.coord.x) > 0) {
	{	
		*(m_lpBits+(element.coord.y+1)*linebytes+element.coord.x)=0;
		struct Element new_element;
		new_element.coord.x = element.coord.x;
		new_element.coord.y = element.coord.y + 1; 
		blob.elements_number++;
		blob.elements.push_back(new_element);                        
		return 1;
		if (has_neighbour(element, new_element) == false) {
			int index = is_element_present(blob, new_element);
			if (index >= 0) {
				blob.elements[index].neighbs.push_back(element);
				return 0;
			}                        
			blob.elements_number++;
			blob.elements.push_back(new_element);                        
			return 1;
		}
		else
			return 0;
	}
	else 
		return 0;
}
inline const LPBYTE ImageBlobs::get_image(int &w,int &h) const
{
	w=m_lWidth;
	h=m_lHeight;
	return m_lpBits;
}
inline unsigned int ImageBlobs::add_left_neighbour(struct Blob& blob, unsigned int i)
{
	int linebytes=(((m_lWidth * 8 + 31) & ~31)>>3);
	const struct Element& element = blob.elements[i];
	if (element.coord.x - 1 < 0)
		return 0;
	else if ((*(m_lpBits+(element.coord.y-0)*linebytes+element.coord.x-1))>0)//(*m_image)(element.coord.y, element.coord.x - 1) > 0) {
	{	
		*(m_lpBits+(element.coord.y-0)*linebytes+element.coord.x-1)=0;
		struct Element new_element;
		new_element.coord.x = element.coord.x - 1;
		new_element.coord.y = element.coord.y;   
		blob.elements_number++;
		blob.elements.push_back(new_element);                        
		return 1;
		if (has_neighbour(element, new_element) == false) {
			int index = is_element_present(blob, new_element);
			if (index >= 0) {
				blob.elements[index].neighbs.push_back(element);
				return 0;
			}                        
			blob.elements_number++;
			blob.elements.push_back(new_element);                        
			return 1;
		}
		else
			return 0;
	}
	else 
		return 0;
}

#endif

