
#include "StdAfx.h"
#include "..\include\MyImageTarget.h"




////////////////////////////////////////////////////////////////////////

Accumulator::Accumulator(const int imgWidth, const int imgHeight)
{	
	rhos      = (int)((imgWidth + imgHeight) * SQRT2);
	rhoOffset = rhos/2;
	rhosLow   = rhos/7;
	rhosHigh  = rhos - rhos/7;
	thetas    = 180;

	// prepare accumulator array - initialize it with zeroes
	bins = new int[rhos*thetas];
	ZeroMemory(bins, sizeof(int)*rhos*thetas);
}


const float Accumulator::sins[180] = 
{(float)0.0,(float)0.017452,(float)0.034899,(float)0.052336,(float)0.069756,(float)0.087156,
(float)0.104528,(float)0.121869,(float)0.139173,(float)0.156434,(float)0.173648,(float)0.190809,
(float)0.207912,(float)0.224951,(float)0.241922,(float)0.258819,(float)0.275637,(float)0.292372,
(float)0.309017,(float)0.325568,(float)0.342020,(float)0.358368,(float)0.374607,(float)0.390731,
(float)0.406737,(float)0.422618,(float)0.438371,(float)0.453990,(float)0.469472,(float)0.484810,
(float)0.500000,(float)0.515038,(float)0.529919,(float)0.544639,(float)0.559193,(float)0.573576,
(float)0.587785,(float)0.601815,(float)0.615662,(float)0.629320,(float)0.642788,(float)0.656059,
(float)0.669131,(float)0.681998,(float)0.694658,(float)0.707107,(float)0.719340,(float)0.731354,
(float)0.743145,(float)0.754710,(float)0.766044,(float)0.777146,(float)0.788011,(float)0.798636,
(float)0.809017,(float)0.819152,(float)0.829038,(float)0.838671,(float)0.848048,(float)0.857167,
(float)0.866025,(float)0.874620,(float)0.882948,(float)0.891007,(float)0.898794,(float)0.906308,
(float)0.913545,(float)0.920505,(float)0.927184,(float)0.933580,(float)0.939693,(float)0.945519,
(float)0.951057,(float)0.956305,(float)0.961262,(float)0.965926,(float)0.970296,(float)0.974370,
(float)0.978148,(float)0.981627,(float)0.984808,(float)0.987688,(float)0.990268,(float)0.992546,
(float)0.994522,(float)0.996195,(float)0.997564,(float)0.998630,(float)0.999391,(float)0.999848,
(float)1.0,(float)0.999848,(float)0.999391,(float)0.998630,(float)0.997564,(float)0.996195,
(float)0.994522,(float)0.992546,(float)0.990268,(float)0.987688,(float)0.984808,(float)0.981627,
(float)0.978148,(float)0.974370,(float)0.970296,(float)0.965926,(float)0.961262,(float)0.956305,
(float)0.951057,(float)0.945519,(float)0.939693,(float)0.933580,(float)0.927184,(float)0.920505,
(float)0.913545,(float)0.906308,(float)0.898794,(float)0.891007,(float)0.882948,(float)0.874620,
(float)0.866025,(float)0.857167,(float)0.848048,(float)0.838671,(float)0.829038,(float)0.819152,
(float)0.809017,(float)0.798635,(float)0.788011,(float)0.777146,(float)0.766044,(float)0.754710,
(float)0.743145,(float)0.731354,(float)0.719340,(float)0.707107,(float)0.694658,(float)0.681998,
(float)0.669131,(float)0.656059,(float)0.642788,(float)0.629321,(float)0.615661,(float)0.601815,
(float)0.587785,(float)0.573576,(float)0.559193,(float)0.544639,(float)0.529919,(float)0.515038,
(float)0.500000,(float)0.484810,(float)0.469472,(float)0.453991,(float)0.438371,(float)0.422618,
(float)0.406737,(float)0.390731,(float)0.374607,(float)0.358368,(float)0.342020,(float)0.325568,
(float)0.309017,(float)0.292372,(float)0.275637,(float)0.258819,(float)0.241922,(float)0.224951,
(float)0.207912,(float)0.190809,(float)0.173648,(float)0.156434,(float)0.139173,(float)0.121869,
(float)0.104528,(float)0.087156,(float)0.069756,(float)0.052336,(float)0.034899,(float)0.017452};

const float Accumulator::coss[180] = 
{(float)1.0,(float)0.999848,(float)0.999391,(float)0.998630,(float)0.997564,(float)0.996195,(float)0.994522,
(float)0.992546,(float)0.990268,(float)0.987688,(float)0.984808,(float)0.981627,(float)0.978148,(float)0.974370,
(float)0.970296,(float)0.965926,(float)0.961262,(float)0.956305,(float)0.951057,(float)0.945519,(float)0.939693,
(float)0.933580,(float)0.927184,(float)0.920505,(float)0.913545,(float)0.906308,(float)0.898794,(float)0.891007,
(float)0.882948,(float)0.874620,(float)0.866025,(float)0.857167,(float)0.848048,(float)0.838671,(float)0.829038,
(float)0.819152,(float)0.809017,(float)0.798636,(float)0.788011,(float)0.777146,(float)0.766044,(float)0.754710,
(float)0.743145,(float)0.731354,(float)0.719340,(float)0.707107,(float)0.694658,(float)0.681998,(float)0.669131,
(float)0.656059,(float)0.642788,(float)0.629320,(float)0.615662,(float)0.601815,(float)0.587785,(float)0.573576,
(float)0.559193,(float)0.544639,(float)0.529919,(float)0.515038,(float)0.500000,(float)0.484810,(float)0.469472,
(float)0.453991,(float)0.438371,(float)0.422618,(float)0.406737,(float)0.390731,(float)0.374607,(float)0.358368,
(float)0.342020,(float)0.325568,(float)0.309017,(float)0.292372,(float)0.275637,(float)0.258819,(float)0.241922,
(float)0.224951,(float)0.207912,(float)0.190809,(float)0.173648,(float)0.156434,(float)0.139173,(float)0.121869,
(float)0.104528,(float)0.087156,(float)0.069757,(float)0.052336,(float)0.034899,(float)0.017452,(float)0.0,
(float)-0.017452,(float)-0.034899,(float)-0.052336,(float)-0.069756,(float)-0.087156,(float)-0.104529,(float)-0.121869,
(float)-0.139173,(float)-0.156434,(float)-0.173648,(float)-0.190809,(float)-0.207912,(float)-0.224951,(float)-0.241922,
(float)-0.258819,(float)-0.275637,(float)-0.292372,(float)-0.309017,(float)-0.325568,(float)-0.342020,(float)-0.358368,
(float)-0.374607,(float)-0.390731,(float)-0.406737,(float)-0.422618,(float)-0.438371,(float)-0.453990,(float)-0.469472,
(float)-0.484810,(float)-0.500000,(float)-0.515038,(float)-0.529919,(float)-0.544639,(float)-0.559193,(float)-0.573576,
(float)-0.587785,(float)-0.601815,(float)-0.615661,(float)-0.629320,(float)-0.642788,(float)-0.656059,(float)-0.669131,
(float)-0.681998,(float)-0.694658,(float)-0.707107,(float)-0.719340,(float)-0.731354,(float)-0.743145,(float)-0.754710,
(float)-0.766044,(float)-0.777146,(float)-0.788011,(float)-0.798635,(float)-0.809017,(float)-0.819152,(float)-0.829037,
(float)-0.838671,(float)-0.848048,(float)-0.857167,(float)-0.866025,(float)-0.874620,(float)-0.882948,(float)-0.891006,
(float)-0.898794,(float)-0.906308,(float)-0.913545,(float)-0.920505,(float)-0.927184,(float)-0.933580,(float)-0.939693,
(float)-0.945519,(float)-0.951056,(float)-0.956305,(float)-0.961262,(float)-0.965926,(float)-0.970296,(float)-0.974370,
(float)-0.978148,(float)-0.981627,(float)-0.984808,(float)-0.987688,(float)-0.990268,(float)-0.992546,(float)-0.994522,
(float)-0.996195,(float)-0.997564,(float)-0.998630,(float)-0.999391,(float)-0.999848};



CMyImageTarget::CMyImageTarget(LPBYTE lpBits,LONG lWidth,LONG lHeight):CImgPro(lpBits,lWidth,lHeight)
{
	m_nESCPixel=0;
	edgePt=new LINEEDGEPOINT[m_lWidth];
	edgePtHW2B=new LINEEDGEPOINT[m_lWidth];
	edgePtVW2B=new LINEEDGEPOINT[m_lHeight];
	edgePtVB2W=new LINEEDGEPOINT[m_lHeight];	
}


CMyImageTarget::~CMyImageTarget(void)
{
	if(edgePt!=NULL)delete edgePt;
	if(edgePtHW2B!=NULL) delete edgePtHW2B;
	if(edgePtVW2B!=NULL) delete edgePtVW2B;
	if(edgePtVB2W!=NULL) delete edgePtVB2W;

}

BOOL CMyImageTarget::FilterFitLine(POINT *pInput, long lDataSize, 
						 double *dStartY, double *dEndY)
{
	long i;
	double *pFitY;
	double sum_x, sumsq_x, sum_xy, sum_y, sumsq_y;
	double divisor, slope, intercept;
	
	long lFilterDataSize;
	double dTotalDistance,dDistance,dDiff;
	POINTL *pFilterInput;

	lFilterDataSize=0;
	dTotalDistance=0;
	if (lDataSize <= 0)
		return 0;

	pFitY = (double *) GlobalAlloc(GMEM_FIXED, sizeof(double) * lDataSize);
	if (!pFitY)
	{
		return 0;
	}

	pFilterInput = (POINTL *) GlobalAlloc(GMEM_FIXED, sizeof(double) * lDataSize);
	if (!pFilterInput)
	{
		return 0;
	}
	sum_x = 0;
	sumsq_x = 0;
	sum_xy = 0;
	sum_y = 0;
	sumsq_y = 0;

	for (i = 0; i < lDataSize; i++)
	{
		if (pInput[i].x > 0 && pInput[i].y > 0)
		{
			sum_x += pInput[i].x;
			sumsq_x += pow((double) pInput[i].x, 2);
			sum_xy += pInput[i].x * pInput[i].y;
			sum_y += pInput[i].y;
		}
	}

	divisor = lDataSize * sumsq_x - sum_x * sum_x;

	if (divisor)
	{
		slope = (lDataSize * sum_xy - sum_x * sum_y) / divisor;
		intercept = (sum_y - slope * sum_x) / lDataSize;	
		for (i = 0; i < lDataSize; i++)
		{
			pFitY[i] = slope * pInput[i].x + intercept;		
		}

		//*dStartY = pFitY[0];
		//*dEndY = pFitY[lDataSize - 1];

		//filter error point
		//filter the point, its distance >2 times dDiff
		for (i = 0; i < lDataSize; i++)
		{
			dTotalDistance=dTotalDistance+abs(pInput[i].y-pFitY[i]);
		}
		dDiff=dTotalDistance/lDataSize;

		lFilterDataSize=0;
		for (i = 0; i < lDataSize; i++)
		{
			dDistance=abs(pInput[i].y-pFitY[i]);
			if(dDistance<=(dDiff/2))
			{			
				pFilterInput[lFilterDataSize].y=pInput[i].y;
				pFilterInput[lFilterDataSize].x=pInput[i].x;
				lFilterDataSize++;
			}
		}

		if(lFilterDataSize>(lDataSize/8))
		{
			sum_x = 0;
			sumsq_x = 0;
			sum_xy = 0;
			sum_y = 0;
			sumsq_y = 0;
			for (i = 0; i < lFilterDataSize; i++)
			{
				if (pFilterInput[i].x > 0 && pFilterInput[i].y > 0)
				{
					sum_x += pFilterInput[i].x;
					sumsq_x += pow((double) pFilterInput[i].x, 2);
					sum_xy += pFilterInput[i].x * pFilterInput[i].y;
					sum_y += pFilterInput[i].y;
				}
			}

			divisor = lFilterDataSize * sumsq_x - sum_x * sum_x;
			if (divisor)
			{
				slope = (lFilterDataSize * sum_xy - sum_x * sum_y) / divisor;
				intercept = (sum_y - slope * sum_x) / lFilterDataSize;		
				
				*dStartY = slope * pInput[0].x + intercept;
				*dEndY = slope * pInput[lDataSize-1].x + intercept;
			}
		}
		else
		{
			*dStartY = slope * pInput[0].x + intercept;
			*dEndY = slope * pInput[lDataSize-1].x + intercept;
		}
	}
	else
	{
		GlobalFree(pFitY);
		GlobalFree(pFilterInput);
		return 0;
	}

	GlobalFree(pFitY);
	GlobalFree(pFilterInput);
	return 1;
}
int CMyImageTarget::GetLinePointFromBin(Bin linedataH1,Bin linedataH2,Bin linedataV1,Bin linedataV2)
{
	int x, y;
	Line top;
	top.rho=linedataH1.rho;
	top.theta=linedataH1.theta;
	Line bottom;
	bottom.rho=linedataH2.rho;
	bottom.theta=linedataH2.theta;

	Line left;
	left.rho=linedataV1.rho;
	left.theta=linedataV1.theta;
	Line right;
	right.rho=linedataV2.rho;
	right.theta=linedataV2.theta;
	int width=m_lWidth;
	int height=m_lHeight;
	int m_rhos = (int)((width + height) * SQRT2)/2;
	// left line
	POINT pt;
	for (y = 0; y < m_lHeight; y++)
	{	
		x = (int)(y*cos(left.theta*DEG2RAD)/sin(left.theta*DEG2RAD) + (left.rho - m_rhos)/sin(left.theta*DEG2RAD));
		if (x>0 && x<width && y>0 && y<height)
		{
			pt.x=x;
			pt.y=y;
			DrawPoint(pt,127,1);
		}
	}

	// bottom line
	for (x = 0; x < m_lWidth; x++)
	{	
		y = (int)(x*cos(bottom.theta*DEG2RAD)/sin(bottom.theta*DEG2RAD) + (bottom.rho - m_rhos)/sin(bottom.theta*DEG2RAD));
		if (x>0 && x<width && y>0 && y<height)
		{
			pt.x=x;
			pt.y=y;
			DrawPoint(pt,127,1);
		}
	}

	// right line
	for (y = 0; y <m_lHeight; y++)
	{
		x = (int)(y*cos(right.theta*DEG2RAD)/sin(right.theta*DEG2RAD) + (right.rho - m_rhos)/sin(right.theta*DEG2RAD));
		if (x>0 && x<width && y>0 && y<height)
		{
			pt.x=x;
			pt.y=y;
			DrawPoint(pt,127,1);
		}
	}
	// top line
	for (x = 0; x < m_lWidth; x++)
	{	
		y = (int)(x*cos(top.theta*DEG2RAD)/sin(top.theta*DEG2RAD) + (top.rho - m_rhos)/sin(top.theta*DEG2RAD));
		if (x>0 && x<width && y>0 && y<height)
		{
			pt.x=x;
			pt.y=y;
			DrawPoint(pt,127,1);
		}
	}




	return 0;
}



BOOL CMyImageTarget::LAx_B(double &a0, double &a1,POINT *linepoint,int size)
{
	double sumXiYi(0.0), sumXi(0.0), sumYi(0.0), sumXi_2(0.0), X_Average(0.0), Y_Average(0.0);
	INT_PTR pointcount =0;// X_Con_Lin_Reg.GetSize();
	for (int i=0; i<size; i++)
	{
			sumXiYi +=linepoint[i].x*linepoint[i].y;
			sumXi += linepoint[i].x;
			sumYi += linepoint[i].y;
			sumXi_2 +=linepoint[i].x*linepoint[i].x;

	}
	pointcount=size;
	if(pointcount<=0)
		return FALSE;
	/*求平均值*/
	X_Average = sumXi/pointcount;
	Y_Average = sumYi/pointcount;

	/*y=a0+a1x*/
	a1 = (sumXiYi-pointcount*X_Average*Y_Average)/(sumXi_2-pointcount*X_Average*X_Average);
	a0 = Y_Average - a1*X_Average;
	return TRUE;
}

//图像中心？
BOOL CMyImageTarget::linearReg_X_a0_a1Y(double &a0, double &a1,LINEEDGEPOINT *linepoint,int size)
{
	double sumXiYi(0.0), sumXi(0.0), sumYi(0.0), sumXi_2(0.0), X_Average(0.0), Y_Average(0.0);
	INT_PTR pointcount =0;// X_Con_Lin_Reg.GetSize();
	for (int i=0; i<size; i++)
	{
		if(linepoint[i].length>0)
		{
			sumXiYi +=linepoint[i].ypos*linepoint[i].xpos;
			sumXi += linepoint[i].ypos;
			sumYi += linepoint[i].xpos;
			sumXi_2 +=linepoint[i].ypos*linepoint[i].ypos;
			pointcount++;
		}
	}
	if(pointcount<=0)
		return FALSE;
	/*求平均值*/
	X_Average = sumXi/pointcount;
	Y_Average = sumYi/pointcount;

	/*y=a0+a1x*/
	a1 = (sumXiYi-pointcount*X_Average*Y_Average)/(sumXi_2-pointcount*X_Average*X_Average);
	a0 = Y_Average - a1*X_Average;
	return TRUE;
}

//图像中心？
BOOL CMyImageTarget::linearReg_Y_a0_a1X(double &a0, double &a1,LINEEDGEPOINT *linepoint,int size)
{
/*	double t1=0,t2=0,t3=0,t4=0;
	for(int x=0;x<size;++x)
	{
		t1+=linepoint[x].xpos*linepoint[x].xpos;
		t2+=linepoint[x].xpos;
		t3+=(linepoint[x].xpos*linepoint[x].ypos);
		t4+=linepoint[x].ypos;
	}
	a1=(t3*size-t2*t4)/(t1*size-t2*t2);
	a0=(t1*t4-t2*t3)/(t1*size-t2*t2);
	return TRUE;*/
	//////////////////////////////
	double sumXiYi(0.0), sumXi(0.0), sumYi(0.0), sumXi_2(0.0), X_Average(0.0), Y_Average(0.0);
	INT_PTR pointcount =0;// X_Con_Lin_Reg.GetSize();
	for (int i=0; i<size; i++)
	{
		if(linepoint[i].xpos>0)
		{
			sumXiYi +=linepoint[i].xpos*linepoint[i].ypos;
			sumXi += linepoint[i].xpos;
			sumYi += linepoint[i].ypos;
			sumXi_2 +=linepoint[i].xpos*linepoint[i].xpos;
			pointcount++;
		}
	}
	if(pointcount<=0)
		return FALSE;
	/*求平均值*/
	X_Average = sumXi/pointcount;
	Y_Average = sumYi/pointcount;

	/*y=a0+a1x*/
	a1 = (sumXiYi-pointcount*X_Average*Y_Average)/(sumXi_2-pointcount*X_Average*X_Average);
	a0 = Y_Average - a1*X_Average;
	return TRUE;
}

int CMyImageTarget::FindObjectRect(SRect_I srect,int i_Step,int DifferRange)
{
	//find H1_B2W
/*	LINEEDGEPOINT *edgeflag=new LINEEDGEPOINT[m_lHeight];
	LINEEDGEPOINT *edgeVflag=new LINEEDGEPOINT[m_lWidth];
	LINEEDGEPOINT *edgePt=new LINEEDGEPOINT[m_lWidth];
	LINEEDGEPOINT *edgePtHW2B=new LINEEDGEPOINT[m_lWidth];
 
	LINEEDGEPOINT *edgePtVW2B=new LINEEDGEPOINT[m_lHeight];
	LINEEDGEPOINT *edgePtVB2W=new LINEEDGEPOINT[m_lHeight];*/
	BYTE* Data=new BYTE[m_lWidth*m_lHeight];
	memcpy(Data,m_lpBits,m_lWidth*m_lHeight);
	//int u;
	reset();
	//Threshold(150,u);
	int kk;
	POINT* pInputLinePoint;
	double aa,bb;
	int Pointcounter=0;

	int lenthreshold=5;
	int xx,yy;
	int xx1,yy1,xx2,yy2;
	xx1=srect.point1.x;
	yy1=srect.point1.y;
	xx2=srect.point2.x;
	yy2=srect.point2.y;
	

	//i_Step=1;
	int iSkip=i_Step;
	
	for(xx=xx1;xx<xx2;xx+=i_Step)
	{
		int lengthsum=0;
		int edgePtCount=0;
		iSkip=i_Step;
		for(yy=yy1;yy<yy2;yy+=iSkip)
		{
			iSkip=i_Step;
			if((*(Data+yy*m_lWidth+xx))>=127&&(*(Data+(yy-1)*m_lWidth+xx))<127)
			{
				int yyy=yy+1;
				int len=1;
				while((*(Data+yyy*m_lWidth+xx))>=127&&yyy<m_lHeight)
				{
					len++;
					yyy++;
				}
				//iSkip=yyy-yy;
				if(len>=lenthreshold)
				{
					edgePt[xx].xpos=xx;
					edgePt[xx].ypos=yy;
					edgePt[xx].length=len;
					POINT pttt;
					pttt.x=xx;
					pttt.y=yy;
					//DrawPoint(pttt,127,1);
					//SetPixelValue(Data,m_lWidth,m_lHeight,pttt,255);
					break;
				}
			}
		}
	}

	int pointlinecounter;
	pointlinecounter=0;
	int miny=m_lHeight;
	int tempx,tempy;
	int astart=0,bend=0;
	int ix,iy;
	pInputLinePoint=new POINT[m_lWidth];

	for(int i=0;i<m_lWidth;i++)
	{
		if(edgePt[i].length>0)
		{
			pInputLinePoint[pointlinecounter].x=edgePt[i].xpos;
			pInputLinePoint[pointlinecounter].y=edgePt[i].ypos+m_nESCPixel;
			pointlinecounter++;
		}
	}
	//sort top

	for( iy=0;iy<pointlinecounter-1;iy++)
	{
		for(ix=1;ix<pointlinecounter-1-iy;ix++)
		{
			if(pInputLinePoint[ix].y<pInputLinePoint[ix-1].y)
			{
				tempy=pInputLinePoint[ix-1].y;
				tempx=pInputLinePoint[ix-1].x;
				pInputLinePoint[ix-1].y=pInputLinePoint[ix].y;
				pInputLinePoint[ix-1].x=pInputLinePoint[ix].x;
				pInputLinePoint[ix].y=tempy;
				pInputLinePoint[ix].x=tempx;
			}
		}
	}
	Pointcounter=0;
	astart=(pointlinecounter-1)/3;

	LAx_B(aa,bb,&pInputLinePoint[astart],astart);
	linetop.a=aa;
	linetop.b=bb;
	//linearReg_Y_a0_a1X(aa,bb,edgePt,m_lWidth);
	for(int ss=0;ss<m_lWidth;ss++)
	{
		POINT _pttt;
		_pttt.x=ss;
		_pttt.y=aa+bb*ss;
		if(_pttt.y<0)
			_pttt.y=0;
		if(_pttt.y>=m_lHeight)
			_pttt.y=m_lHeight-1;
		//DrawPoint(_pttt,127,1);
	}
	//H W2B
	for(xx=xx1;xx<xx2;xx+=i_Step)
	{
		int lengthsum=0;
		int edgePtCount=0;
		iSkip=i_Step;
		for(yy=yy2;yy>yy1;yy-=iSkip)
		{
			iSkip=i_Step;
			if((*(Data+yy*m_lWidth+xx))>=127&&(*(Data+(yy+1)*m_lWidth+xx))<127)
			{
				int yyy=yy-1;
				int len=1;
				while((*(Data+yyy*m_lWidth+xx))>=127&&yyy>yy1)
				{
					len++;
					yyy--;
				}
				//iSkip=yy-yyy;
				if(len>=lenthreshold)
				{
					edgePtHW2B[xx].xpos=xx;
					edgePtHW2B[xx].ypos=yy;
					edgePtHW2B[xx].length=len;
				
					POINT pttt;
					pttt.x=xx;
					pttt.y=yy;
					//DrawPoint(pttt,127,1);
					//SetPixelValue(Data,m_lWidth,m_lHeight,pttt,255);
					break;
				}
			}
		}
	}
	
	if(pInputLinePoint!=NULL) delete pInputLinePoint;
	pInputLinePoint=new POINT[m_lWidth];
	pointlinecounter=0;

	for(int i=0;i<m_lWidth;i++)
	{
		if(edgePt[i].length>0)
		{
			pInputLinePoint[pointlinecounter].x=edgePtHW2B[i].xpos;
			pInputLinePoint[pointlinecounter].y=edgePtHW2B[i].ypos-m_nESCPixel;
			pointlinecounter++;
		}
	}
	//sort bottom

	for( iy=0;iy<pointlinecounter-1;iy++)
	{
		for(ix=1;ix<pointlinecounter-1-iy;ix++)
		{
			if(pInputLinePoint[ix].y>pInputLinePoint[ix-1].y)
			{
				tempy=pInputLinePoint[ix-1].y;
				tempx=pInputLinePoint[ix-1].x;
				pInputLinePoint[ix-1].y=pInputLinePoint[ix].y;
				pInputLinePoint[ix-1].x=pInputLinePoint[ix].x;
				pInputLinePoint[ix].y=tempy;
				pInputLinePoint[ix].x=tempx;
			}
		}
	}
	Pointcounter=0;
	astart=(pointlinecounter-1)/3;

	LAx_B(aa,bb,&pInputLinePoint[astart],astart);

	linebottm.a=aa;
	linebottm.b=bb;
	//linearReg_Y_a0_a1X(aa,bb,edgePtHW2B,m_lWidth);
	for(int ss=0;ss<m_lWidth;ss++)
	{
		POINT _pttt;
		_pttt.x=ss;
		_pttt.y=aa+bb*ss;
		if(_pttt.y<0)
			_pttt.y=0;
		if(_pttt.y>=m_lHeight)
			_pttt.y=m_lHeight-1;
		//DrawPoint(_pttt,127,1);
	}
	//V B2W
	for(yy=yy1;yy<yy2;yy+=i_Step)
	{
		int lengthsum=0;
		int edgePtCount=0;
		iSkip=i_Step;
		for(xx=xx1;xx<xx2;xx+=iSkip)
		{
			iSkip=i_Step;
			if((*(Data+yy*m_lWidth+xx))<127&&(*(Data+yy*m_lWidth+xx+1))>127)
			{
				int xxx=xx+1;
				int len=0;
				while((*(Data+yy*m_lWidth+xxx))>=127&&xxx<xx2)
				{
					len++;
					xxx++;
				}
				//iSkip=xxx-xx-1;
				if(len>=lenthreshold)
				{
					edgePtVW2B[yy].xpos=xx;
					edgePtVW2B[yy].ypos=yy;
					edgePtVW2B[yy].length=len;
					
					POINT pttt;
					pttt.x=xx;
					pttt.y=yy;
					//DrawPoint(pttt,127,1);
					//SetPixelValue(Data,m_lWidth,m_lHeight,pttt,255);
					break;
				}
			}
		}
	}

	if(pInputLinePoint!=NULL) delete pInputLinePoint;
	pInputLinePoint=new POINT[m_lHeight];
	pointlinecounter=0;

	for(int i=0;i<m_lHeight;i++)
	{
		if(edgePtVW2B[i].length>0)
		{
			pInputLinePoint[pointlinecounter].x=edgePtVW2B[i].xpos;
			pInputLinePoint[pointlinecounter].y=edgePtVW2B[i].ypos;
			pointlinecounter++;
		}
	}
	//sort left

	for( iy=0;iy<pointlinecounter-1;iy++)
	{
		for(ix=1;ix<pointlinecounter-1-iy;ix++)
		{
			if(pInputLinePoint[ix].x>pInputLinePoint[ix-1].x)
			{
				tempy=pInputLinePoint[ix-1].y;
				tempx=pInputLinePoint[ix-1].x;
				pInputLinePoint[ix-1].y=pInputLinePoint[ix].y;
				pInputLinePoint[ix-1].x=pInputLinePoint[ix].x;
				pInputLinePoint[ix].y=tempy;
				pInputLinePoint[ix].x=tempx;
			}
		}
	}
	Pointcounter=0;
	astart=(pointlinecounter-1)/3;

	//LAx_B(aa,bb,&pInputLinePoint[astart],astart);
	aa=(pInputLinePoint[astart].x+pInputLinePoint[astart+astart].x)/2;
	bb=0;
	if(aa>m_lWidth/3) aa=m_lWidth/4;
	lineleft.a=aa+m_nESCPixel;
	lineleft.b=bb;
	
	//linearReg_X_a0_a1Y(aa,bb,edgePtVW2B,m_lHeight);
	for(int ss=0;ss<m_lHeight;ss++)
	{
		POINT _pttt;
		_pttt.y=ss;
		_pttt.x=aa+bb*ss;
		if(_pttt.x<0)
			_pttt.x=0;
		if(_pttt.x>=m_lWidth)
			_pttt.x=m_lWidth-1;
		
		//DrawPoint(_pttt,127,1);
	}
	
	//V B3W
	//V W2B
	for(yy=yy1;yy<yy2;yy+=i_Step)
	{
		int lengthsum=0;
		int edgePtCount=0;
		iSkip=i_Step;
		for(xx=xx2;xx>xx1;xx-=iSkip)
		{
			iSkip=i_Step;
			if((*(Data+yy*m_lWidth+xx))<=127&&(*(Data+yy*m_lWidth+xx-1))>127)
			{
				int xxx=xx-1;
				int len=0;
				while((*(Data+yy*m_lWidth+xxx))>=127&&xxx>xx1)
				{
					len++;
					xxx--;
				}
				
				if(len>=lenthreshold)
				{
					edgePtVB2W[yy].xpos=xx;
					edgePtVB2W[yy].ypos=yy;
					edgePtVB2W[yy].length=len;
					
					POINT pttt;
					pttt.x=xx;
					pttt.y=yy;
					//DrawPoint(pttt,127,2);
					//SetPixelValue(Data,m_lWidth,m_lHeight,pttt,255);
					break;
				}
				else
					iSkip=xx-xxx;
			}
		}
	}
	if(pInputLinePoint!=NULL) delete pInputLinePoint;
	pInputLinePoint=new POINT[m_lHeight];
	pointlinecounter=0;

	for(int i=0;i<m_lHeight;i++)
	{
		if(edgePtVB2W[i].length>0)
		{
			pInputLinePoint[pointlinecounter].x=edgePtVB2W[i].xpos;
			pInputLinePoint[pointlinecounter].y=edgePtVB2W[i].ypos;
			pointlinecounter++;
		}
	}
	//sort left

	for( iy=0;iy<pointlinecounter-1;iy++)
	{
		for(ix=1;ix<pointlinecounter-1-iy;ix++)
		{
			if(pInputLinePoint[ix].x>pInputLinePoint[ix-1].x)
			{
				tempy=pInputLinePoint[ix-1].y;
				tempx=pInputLinePoint[ix-1].x;
				pInputLinePoint[ix-1].y=pInputLinePoint[ix].y;
				pInputLinePoint[ix-1].x=pInputLinePoint[ix].x;
				pInputLinePoint[ix].y=tempy;
				pInputLinePoint[ix].x=tempx;
			}
		}
	}
	Pointcounter=0;
	astart=(pointlinecounter-1)/3;

	//LAx_B(aa,bb,&pInputLinePoint[astart],astart);
	aa=(pInputLinePoint[astart].x+pInputLinePoint[astart+astart].x)/2;
	bb=0;
	if(aa<m_lWidth*2/3) aa=m_lWidth*3/4;
	lineright.a=aa-m_nESCPixel;
	lineright.b=bb;
	//linearReg_X_a0_a1Y(aa,bb,edgePtVB2W,m_lHeight);
	for(int ss=0;ss<m_lHeight;ss++)
	{
		POINT _pttt;
		_pttt.y=ss;
		_pttt.x=aa+bb*ss;
		if(_pttt.x<0)
			_pttt.x=0;
		if(_pttt.x>=m_lWidth)
			_pttt.x=m_lWidth-1;

		//DrawPoint(_pttt,127,1);
	}
	///////////////////////
/*	if(edgeflag!=NULL) delete edgeflag;

	if(edgePt!=NULL)delete edgePt;
	if(edgePtHW2B!=NULL) delete edgePtHW2B;
	if(edgePtVW2B!=NULL) delete edgePtVW2B;
	if(edgePtVB2W!=NULL) delete edgePtVB2W;*/
//	GetLinePointFromBin(lineH1,lineH2,lineV1,lineV2);
//	memcpy(m_lpBits,Data,m_lWidth*m_lHeight);


	
	memcpy(m_lpBits,Data,m_lWidth*m_lHeight);
	if(Data!=NULL) delete Data;
	if(pInputLinePoint!=NULL) delete pInputLinePoint;
	return 0;
}

void CMyImageTarget::SetFlag(MYLINE _linetop,MYLINE _linebottm,MYLINE _lineleft,MYLINE _lineright)
{

for(int x=0;x<m_lWidth;x++)
{

	//
	int ty=_linetop.a+_linetop.b*x;
	int by=_linebottm.a+_linebottm.b*x;
	if(ty<0)
		ty=0;
	if(ty>=m_lHeight)
		ty=m_lHeight;
	if(by<0)
		by=0;
	if(by>=m_lHeight)
		by=m_lHeight;

	for(int y=0;y<m_lHeight;y++)
	{
		if(y<ty||y>by||x<_lineleft.a||x>_lineright.a)
		{
			*(m_lpBitsObjectflag+y*m_lWidth+x)=255;
			*(m_lpBits+y*m_lWidth+x)=255;
		}
		else
		{
			*(m_lpBitsObjectflag+y*m_lWidth+x)=0;
		}
	}
}
//memcpy(m_lpBits,m_lpBitsObjectflag,m_lWidth*m_lHeight);
}

int CMyImageTarget::DrawLine(MYLINE line)
{
	double aa=line.a;
	double bb=line.b;
	if(bb!=0)
	{
		for(int ss=0;ss<m_lWidth;ss++)
		{
			POINT _pttt;
			_pttt.x=ss;
			_pttt.y=aa+bb*ss;
			if(_pttt.y<0)
				_pttt.y=0;
			if(_pttt.y>=m_lHeight)
				_pttt.y=m_lHeight-1;
			DrawPoint(_pttt,127,1);
		}
	}
	else
	{
		for(int ss=0;ss<m_lHeight;ss++)
		{
			POINT _pttt;
			_pttt.x=aa+bb*ss;
			_pttt.y=ss;
			if(_pttt.y<0)
				_pttt.y=0;
			if(_pttt.y>=m_lHeight)
				_pttt.y=m_lHeight-1;
			DrawPoint(_pttt,127,1);
		}
	}
	return TRUE;
}
int CMyImageTarget::GetImageResult(int &Pixel, unsigned char iWin,unsigned char _iThreshold)
{
	//获取阈值
	int iThreshold=180;

	/*//改进一维最大熵求最佳阈值
	unsigned char ImprovedThreshold();//2
	//均匀分布阈值
	unsigned char EqualityThreshold();//1
	
	unsigned char NormalThreshold();//0

	//一维最大熵求最佳阈值
	unsigned char EntropyThreshold();//3
	//Ostu阀值,求连通区域的二值化
	unsigned char OstuThreshold();//4
	//模糊阀值
	unsigned char FuzzyThreshold(unsigned char iWin);//6
	//迭代阈值
	unsigned char IterativeThreshold();//5
	*/
	if(_iThreshold<=0)
	{
		switch(m_idOfThreshold)
		{
		case 0:
			iThreshold=NormalThreshold();
			break;
		case 1:
			iThreshold=EqualityThreshold();
			break;
		case 2:
			iThreshold=ImprovedThreshold();
			break;
		case 3:
			iThreshold=EntropyThreshold();
			break;
		case 4:
			iThreshold=OstuThreshold();
			break;
		case 5:
			iThreshold=IterativeThreshold();
			break;
		case 6:
			iThreshold=FuzzyThreshold(iWin);
			break;

		}
	}
	else
		iThreshold=_iThreshold;
	
	int sum=0;
	//调用阈值化函数
	if(m_idThreshNoise==0)
	{
		if(!Threshold(iThreshold,sum))
		{
			//MessageBox(L"分配内存失败",L"系统提示",MB_ICONINFORMATION|MB_OK);
			return 0;
		}
	}
	else
	{		
		if(iThreshold<m_iThreshNoiseLow||(!Threshold(m_iThreshNoiseLow,m_iThreshNoiseHi,iThreshold,sum)))
		{
			//MessageBox(L"分配内存失败",L"系统提示",MB_ICONINFORMATION|MB_OK);
			return 0;
		}
	}
	Pixel=sum;
	if(sum>=m_iBackPixelMargin&&sum<=m_iObjectPixelHiMargin)
	{
		if(sum>=m_iFrontPixelMargin)
		{
			//为正面
			return 1;
		}
		else
		{
			//为反面
			return 2;
		}
	}
	else
	{
		//没有产品
		return 0;
	}
	return 0;

}