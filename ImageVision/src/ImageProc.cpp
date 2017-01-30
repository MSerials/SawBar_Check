// ImageProc.cpp: implementation of the CImageProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../include/ImageProc.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImageProc::CImageProc()
{

}

CImageProc::~CImageProc()
{

}

//******************求圆的重心**********************
//**************************************************

SCircle_F CImageProc::CircleWeight(unsigned char *ImagePointer,
				int Lowhigh,SPoint_I CircleCenter,int OuterR)
{
	short i=0,j=0;
	int r;
	double	pyCounter=0,pxCounter=0;
	double pCounter=0;
	double Cxy=0;
	SCircle_F aimcircle;
	double Px,Py,R2;
	int wx1,wy1;

	wx1=CircleCenter.x;
	wy1=CircleCenter.y;
	r=OuterR;
	for (i=wy1-r;i<wy1+r;i++)
	{
		Cxy=sqrt((float)(r*r-(i-wy1)*(i-wy1)));
		for (j=(short)(wx1-Cxy);j<(short)(wx1+Cxy);j++)
		{
			if (*(ImagePointer+ImageWidth*i+j)<Lowhigh)
			{
				pyCounter=pyCounter+i;
				pxCounter=pxCounter+j;
				pCounter=pCounter+1;				
			}
		}
	}
	if (pCounter<1)
	{
		Px=0;
		Py=0;
		R2=0;
		aimcircle.key=false;

	}
	else
	{
		Px=pxCounter/pCounter;
		Py=pyCounter/pCounter;
		R2=sqrt(pCounter/3.1415926);
		aimcircle.key=true;

	}
	aimcircle.a=(float)Px;
	aimcircle.b=(float)Py;
	aimcircle.r=(float)R2;
	return aimcircle;
}

//****************************模板匹配**********************//
//**********************************************************//

SPoint_I CImageProc::matchGravityValueImage(unsigned char *xxx,
				    unsigned char yyy[ImageHeight][ImageWidth],SRect_I srect,
					SRect_I trect,int lowhigh,double *dMatchPixelRate)
{
	
	int Aim=1000000000, Add=0, Aimx=0, Aimy=0;
	int Aimxt=0,Aimyt=0;
	int i,j,p,q;
    int Totalpixel,MatchPixNum;
	short wx1,wy1,wx2,wy2,twx1,twy1,twx2,twy2;//,Px,Py;
	SPoint_I ActTempPnt;
	wx1=srect.point1.x;
	wy1=srect.point1.y;
	wx2=srect.point2.x;
	wy2=srect.point2.y;
	twx1=trect.point1.x;
	twy1=trect.point1.y;
	twx2=trect.point2.x;
	twy2=trect.point2.y;
	
	for (i=wy1;i<(wy2-(twy2-twy1)-1);i=i+5)
	{
		for (j=wx1;j<(wx2-(twx2-twx1)-1);j=j+5)
		{
			for (p=0;p<twy2-twy1-1;p=p+5)
			{
				for (q=0;q<twx2-twx1-1;q=q+5)  
				{
					if ((*(xxx+(i+p)*ImageWidth+(j+q))>lowhigh && yyy[p][q]==0) || 
						(*(xxx+(i+p)*ImageWidth+(j+q))<lowhigh && yyy[p][q]==255))
					{
						Add=Add+1;
					}
					
				}
			}
			if (Add<Aim)
			{
				Aim=Add;
				Aimx=i;
				Aimy=j;
			}
			Add=0;
		}
		
	}
	Totalpixel=(twx2-twx1)*(twy2-twy1);
	Aim=(twx2-twx1)*(twy2-twy1);
	
	if (Aimx<wy1+6)
	{
		Aimx=wy1+6;
	}
	if (Aimx>wy2-twy2+twy1-1)
	{
		Aimx=wy2-twy2+twy1-1;
	}
	if (Aimy<wx1+6)
	{
		Aimy=wx1+6;
	}
	if (Aimy>wx2-twx2+twx1-1)
	{
		Aimy=wx2-twx2+twx1-1;
	}
	Aimxt=Aimx+5;
	Aimyt=Aimy+5;
	
	for (i=Aimx-5; i<Aimxt;i++)
	{
		for (j=Aimy-5;j<Aimyt;j++)
		{
			for (p=0;p<twy2-twy1-1;p++)
			{
				for (q=0;q<twx2-twx1-1;q++)
				{
					if ((*(xxx+(i+p)*ImageWidth+(j+q))>lowhigh && yyy[p][q]==0) || 
						(*(xxx+(i+p)*ImageWidth+(j+q))<lowhigh && yyy[p][q]==255))
					{
						Add=Add+1;
					}
					
				}
			}
			if (Add<=Aim)
			{
				Aim=Add;
				Aimx=i;
				Aimy=j;
				MatchPixNum=Add;
			}			
			Add=0;
		}
	}
	double dRate;
	dRate=0.0;
	dRate=(double)MatchPixNum/(double)Totalpixel;
	*dMatchPixelRate=dRate;
	ActTempPnt.x=Aimy;
	ActTempPnt.y=Aimx;	
	return(ActTempPnt);	
}	

//***************** Search Window 范围检查 ****************************
//*********************************************************************
inline BOOL CImageProc::CheckRange(SRect_I wnd)
{
	if(wnd.point1.x>=639 || wnd.point1.x<=1 || wnd.point2.x>=639 || wnd.point1.x<=1 ||
		wnd.point1.y>=479 || wnd.point1.y<=1 || wnd.point2.y>=479 || wnd.point2.y<=1 )
		return FALSE;
	else
		return TRUE;
}

inline BOOL CImageProc::CheckTempRange(SRect_I wnd)
{
	if(wnd.point1.x>=635 || wnd.point1.x<=5 || wnd.point2.x>=635 || wnd.point1.x<=5 ||
		wnd.point1.y>=475 || wnd.point1.y<=5 || wnd.point2.y>=475 || wnd.point2.y<=5 )
		return FALSE;
	else
		return TRUE;

}

//********************* 边缘检测与直线拟和－－水平直线 *********************
//***************************************************************************
SLine_F CImageProc::FindHline(unsigned char *XXX,
			                 SRect_I srect, 
							 int DifferRange)
{
	typedef struct 
	{
		float y;
	    int x;
	    float lenth;
	}vergeH;
    vergeH vergep[ImageWidth];
	vergeH n;
	SLine_F aimline;
	int i=0,j=0,l=0,k=0;
	int m,r;
	int p=0,q=0;
	int count=0;
	static short diffecoffi1[ImageWidth][ImageHeight];
    static short total[ImageWidth];
	short Y1;
	static float pk[ImageWidth];
	static float vergey[ImageWidth];
	float c=0.0,d=0.0;
	double diststd=0.0;
	double Sumxy=0.0,Sumy=0.0;
	double Sumxx=0,Sumx=0;
	float lenth[ImageWidth];
	float UpValue,DownValue;
    float posiy1,posiy2;
	short wx1,wx2,wy1,wy2;
	float a,b;
	if(CheckRange(srect))
	{
		wx1=srect.point1.x;
		wy1=srect.point1.y;
		wx2=srect.point2.x;
		wy2=srect.point2.y;
		
	for(i=wx1;i<wx2;i=i+1)
	{ 
		for(j=wy1;j<wy2;j++)
		{  
			p=*(XXX+ImageWidth*(j+1)+i);
			q=*(XXX+ImageWidth*j+i);
			if(abs(p-q)<DifferRange)
			{
				diffecoffi1[i][j]=0;
			}
			else
			{
				diffecoffi1[i][j]=abs(p-q);
			}
		}
	}
	
    for(i=wx1;i<wx2;i=i+1)
	{
		total[i]=0;
		for(j=wy1;j<wy2;j++)
		{
			total[i]=total[i]+diffecoffi1[i][j];
		}
	}
	
	for(i=wx1;i<wx2;i=i+1)
	{
        vergey[k]=0.0;
		if (total[i]==0)
		{
            vergey[k]=(float)(wy1+wy2)/2;
			count++;
			if(count>((wx2-wx1)*0.2))
			{
				aimline.Symbol=0;
				return(aimline);
			}
		}
		else
		{
			aimline.Symbol=1;
			for(j=wy1;j<wy2;j++)
			{	
				pk[j]=float(diffecoffi1[i][j])/float(total[i]);
				vergey[k]=vergey[k]+(j+1)*(pk[j]);
				
			}
		}
		k=k+1;
	}
	
	if(count<(wx2-wx1)*0.5)
	{
	// finish find some position,Fitting...;
    k=0;
	Sumxx=0.0;
	Sumx=0.0;
	Sumxy=0.0;
	Sumy=0.0;
	for(i=wx1;i<wx2;i=i+1)
	{
		Sumxx=Sumxx+i*i;
		Sumxy=Sumxy+i*vergey[k];
		Sumx=Sumx+i;
		Sumy=Sumy+vergey[k];
		k=k+1;
		m=k;
	}
    
    a=(float)(((Sumy*Sumxx)-(Sumx*Sumxy))/((m*Sumxx)-(Sumx*Sumx)));
	b=(float)(((m*Sumxy)-(Sumx*Sumy))/((m*Sumxx)-(Sumx*Sumx)));
	posiy1=a+b*wx1;
	posiy2=a+b*wx2;
	
	k=0;
	for(i=wx1;i<wx2;i=i+1)
	{
		UpValue=b*i-vergey[k]+a;
		DownValue=(float)sqrt(b*b+1);
		lenth[k]=UpValue/DownValue;
		if (lenth[k]<0)
		{
			lenth[k]=-lenth[k];
		}
		k=k+1;
	}
    
	k=0;
	Y1=wx1;
    for(i=0;i<m;i++)
	{
		vergep[i].x=Y1;
		vergep[i].y=vergey[k];
		vergep[i].lenth=lenth[k];
		k=k+1;
		Y1=Y1+1;
	}
	    
	for(i=1;i<m;i++)
	{
		for(j=0;j<m-i;j++)
		{
			if(vergep[j].lenth>vergep[j+1].lenth)
			{
				n=vergep[j];
				vergep[j]=vergep[j+1];
				vergep[j+1]=n;
			}
		}
	}
    r=int(m*0.8);
	Sumxx=0.0;
	Sumxy=0.0;
	Sumx=0.0;
	Sumy=0.0;
    for(i=0;i<r;i++)
	{
		diststd=diststd+vergep[i].lenth;
		Sumxx=Sumxx+(vergep[i].x)*(vergep[i].x);
		Sumxy=Sumxy+(vergep[i].y)*(vergep[i].x);
		Sumx=Sumx+(vergep[i].x);
		Sumy=Sumy+(vergep[i].y);
		
	}
    diststd=diststd/r;
    a=(float)((Sumy*Sumxx-Sumx*Sumxy)/(r*Sumxx-Sumx*Sumx));
	b=(float)((r*Sumxy-Sumx*Sumy)/(r*Sumxx-Sumx*Sumx));
    posiy1=a+b*wx1;
	posiy2=a+b*wx2;

	aimline.a=a;
	aimline.b=b;
	aimline.Std=(float)diststd;
	aimline.point1.x=0;
	aimline.point1.y=(int)a;
	aimline.point2.x=ImageWidth;
	aimline.point2.y=(int)(a+b*ImageWidth);
	return (aimline);
	}
	else
	{

		aimline.Symbol=0;
		return(aimline);
	}
	}
	else
	{
		aimline.Symbol=0;
		return(aimline);
	}

}

SLine_F CImageProc::FindHlineB2W(unsigned char *XXX,
			                 SRect_I srect, 
							 int DifferRange)
{
	typedef struct 
	{
		float y;
	    int x;
	    float lenth;
	}vergeH;
    vergeH vergep[ImageWidth];
	vergeH n;
	SLine_F aimline;
	int i=0,j=0,l=0,k=0;
	int m,r;
	int p=0,q=0;
	int count=0;
	static short diffecoffi1[ImageWidth][ImageHeight];
    static short total[ImageWidth];
	short Y1;
	static float pk[ImageWidth];
	static float vergey[ImageWidth];
	float c=0.0,d=0.0;
	double diststd=0.0;
	double Sumxy=0.0,Sumy=0.0;
	double Sumxx=0,Sumx=0;
	float lenth[ImageWidth];
	float UpValue,DownValue;
    float posiy1,posiy2;
	short wx1,wx2,wy1,wy2;
	float a,b;
	if(CheckRange(srect))
	{
		wx1=srect.point1.x;
		wy1=srect.point1.y;
		wx2=srect.point2.x;
		wy2=srect.point2.y;
		
	for(i=wx1;i<wx2;i=i+1)
	{ 
		for(j=wy1;j<wy2;j++)
		{  
			p=*(XXX+ImageWidth*(j+1)+i);
			q=*(XXX+ImageWidth*j+i);
			if(p-q<DifferRange)
			{
				diffecoffi1[i][j]=0;
			}
			else
			{
				diffecoffi1[i][j]=p-q;
			}
		}
	}
	
    for(i=wx1;i<wx2;i=i+1)
	{
		total[i]=0;
		for(j=wy1;j<wy2;j++)
		{
			total[i]=total[i]+diffecoffi1[i][j];
		}
	}
	
	for(i=wx1;i<wx2;i=i+1)
	{
        vergey[k]=0.0;
		if (total[i]==0)
		{
/*			vergey[k]=0;
            vergey[k]=(float)(wy1+wy2)/2;
			;
			if(count>((wx2-wx1)*0.2))
			{
				aimline.Symbol=0;
				return(aimline);
			}
*/			count++;
			vergey[k]=0;
		}
		else
		{
			aimline.Symbol=1;
			for(j=wy1;j<wy2;j++)
			{	
				pk[j]=float(diffecoffi1[i][j])/float(total[i]);
				vergey[k]=vergey[k]+(j+1)*(pk[j]);
				
			}
		}
		k=k+1;
	}
	
	if(count<(wx2-wx1)*0.5)
	{
	// finish find some position,Fitting...;
		int vp=0;
		k=0;
		Sumxx=0.0;
		Sumx=0.0;
		Sumxy=0.0;
		Sumy=0.0;
		for(i=wx1;i<wx2;i=i+1)
		{
			if(vergey[k]!=0)
			{
				Sumxx=Sumxx+i*i;
				Sumxy=Sumxy+i*vergey[k];
				Sumx=Sumx+i;
				Sumy=Sumy+vergey[k];
				vp++;
			}
			k=k+1;
			m=k;
		}
    
		a=(float)(((Sumy*Sumxx)-(Sumx*Sumxy))/((vp*Sumxx)-(Sumx*Sumx)));
		b=(float)(((vp*Sumxy)-(Sumx*Sumy))/((vp*Sumxx)-(Sumx*Sumx)));
		posiy1=a+b*wx1;
		posiy2=a+b*wx2;
		
		k=0;
		for(i=wx1;i<wx2;i=i+1)
		{
			if(vergey[k]!=0)
			{
				UpValue=b*i-vergey[k]+a;
				DownValue=(float)sqrt(b*b+1);
				lenth[k]=UpValue/DownValue;
				if (lenth[k]<0)
				{
					lenth[k]=-lenth[k];
				}
			}
			else
				lenth[k]=999;
			k=k+1;
		}
    
		k=0;
		Y1=wx1;
		for(i=0;i<m;i++)
		{
			vergep[i].x=Y1;
			vergep[i].y=vergey[k];
			vergep[i].lenth=lenth[k];
			k=k+1;
			Y1=Y1+1;
		}
			
		for(i=1;i<m;i++)
		{
			for(j=0;j<m-i;j++)
			{
				if(vergep[j].lenth>vergep[j+1].lenth)
				{
					n=vergep[j];
					vergep[j]=vergep[j+1];
					vergep[j+1]=n;
				}
			}
		}
		if(count>(wx2-wx1)*0.2)
			r=int(wx2-wx1-count-1);
		else
			r=int(m*0.8-1);
		Sumxx=0.0;
		Sumxy=0.0;
		Sumx=0.0;
		Sumy=0.0;
		for(i=0;i<r;i++)
		{
			diststd=diststd+vergep[i].lenth;
			Sumxx=Sumxx+(vergep[i].x)*(vergep[i].x);
			Sumxy=Sumxy+(vergep[i].y)*(vergep[i].x);
			Sumx=Sumx+(vergep[i].x);
			Sumy=Sumy+(vergep[i].y);
			
		}
		diststd=diststd/r;
		a=(float)((Sumy*Sumxx-Sumx*Sumxy)/(r*Sumxx-Sumx*Sumx));
		b=(float)((r*Sumxy-Sumx*Sumy)/(r*Sumxx-Sumx*Sumx));
		posiy1=a+b*wx1;
		posiy2=a+b*wx2;

		aimline.a=a;
		aimline.b=b;
		aimline.Std=0;
		aimline.Std=(float)diststd;
		aimline.Std=0;
		aimline.point1.x=0;
		aimline.point1.y=(int)a;
		aimline.point2.x=ImageWidth;
		aimline.point2.y=(int)(a+b*ImageWidth);
		return (aimline);
	}
	else
	{
		aimline.Symbol=0;
		return(aimline);
	}
	}
	else
	{
		aimline.Symbol=0;
		return(aimline);
	}
}


SLine_F CImageProc::FindHlineW2B(unsigned char *XXX,
			                 SRect_I srect, 
							 int DifferRange)
{
	typedef struct 
	{
		float y;
	    int x;
	    float lenth;
	}vergeH;
    vergeH vergep[ImageWidth];
	vergeH n;
	SLine_F aimline;
	int i=0,j=0,l=0,k=0;
	int m,r;
	int p=0,q=0;
	int count=0;
	static short diffecoffi1[ImageWidth][ImageHeight];
    static short total[ImageWidth];
	short Y1;
	static float pk[ImageWidth];
	static float vergey[ImageWidth];
	float c=0.0,d=0.0;
	double diststd=0.0;
	double Sumxy=0.0,Sumy=0.0;
	double Sumxx=0,Sumx=0;
	float lenth[ImageWidth];
	float UpValue,DownValue;
    float posiy1,posiy2;
	short wx1,wx2,wy1,wy2;
	float a,b;
	if(CheckRange(srect))
	{
		wx1=srect.point1.x;
		wy1=srect.point1.y;
		wx2=srect.point2.x;
		wy2=srect.point2.y;
		
	for(i=wx1;i<wx2;i=i+1)
	{ 
		for(j=wy1;j<wy2;j++)
		{  
			p=*(XXX+ImageWidth*(j+1)+i);
			q=*(XXX+ImageWidth*j+i);
			if(q-p<DifferRange)
			{
				diffecoffi1[i][j]=0;
			}
			else
			{
				diffecoffi1[i][j]=q-p;
			}
		}
	}
	
    for(i=wx1;i<wx2;i=i+1)
	{
		total[i]=0;
		for(j=wy1;j<wy2;j++)
		{
			total[i]=total[i]+diffecoffi1[i][j];
		}
	}
	
	for(i=wx1;i<wx2;i=i+1)
	{
        vergey[k]=0.0;
		if (total[i]==0)
		{
/*            vergey[k]=(float)(wy1+wy2)/2;
			count++;
			if(count>((wx2-wx1)*0.2))
			{
				aimline.Symbol=0;
				return(aimline);
			}
*/			vergey[k]=0;
			count++;
		}
		else
		{
			aimline.Symbol=1;
			for(j=wy1;j<wy2;j++)
			{	
				pk[j]=float(diffecoffi1[i][j])/float(total[i]);
				vergey[k]=vergey[k]+(j+1)*(pk[j]);
				
			}
		}
		k=k+1;
	}
	



	if(count<(wx2-wx1)*0.5)
	{
	// finish find some position,Fitting...;
    int vp=0;
	k=0;
	Sumxx=0.0;
	Sumx=0.0;
	Sumxy=0.0;
	Sumy=0.0;
	for(i=wx1;i<wx2;i=i+1)
	{
		if(vergey[k]!=0)
		{
			Sumxx=Sumxx+i*i;
			Sumxy=Sumxy+i*vergey[k];
			Sumx=Sumx+i;
			Sumy=Sumy+vergey[k];
			vp++;
		}
		k=k+1;
		m=k;
	}
    
    a=(float)(((Sumy*Sumxx)-(Sumx*Sumxy))/((vp*Sumxx)-(Sumx*Sumx)));
	b=(float)(((vp*Sumxy)-(Sumx*Sumy))/((vp*Sumxx)-(Sumx*Sumx)));
	posiy1=a+b*wx1;
	posiy2=a+b*wx2;
	
	k=0;
	for(i=wx1;i<wx2;i=i+1)
	{
		if(vergey[k]!=0)
		{
			UpValue=b*i-vergey[k]+a;
			DownValue=(float)sqrt(b*b+1);
			lenth[k]=UpValue/DownValue;
			if (lenth[k]<0)
			{
				lenth[k]=-lenth[k];
			}
		}
		else
			lenth[k]=999;
		k=k+1;
	}
    
	k=0;
	Y1=wx1;
    for(i=0;i<m;i++)
	{
		vergep[i].x=Y1;
		vergep[i].y=vergey[k];
		vergep[i].lenth=lenth[k];
		k=k+1;
		Y1=Y1+1;
	}
	    
	for(i=1;i<m;i++)
	{
		for(j=0;j<m-i;j++)
		{
			if(vergep[j].lenth>vergep[j+1].lenth)
			{
				n=vergep[j];
				vergep[j]=vergep[j+1];
				vergep[j+1]=n;
			}
		}
	}

	if(count>(wx2-wx1)*0.2)
		r=int(wx2-wx1-count-1);
	else
		r=int(m*0.8-1);
	Sumxx=0.0;
	Sumxy=0.0;
	Sumx=0.0;
	Sumy=0.0;
    for(i=0;i<r;i++)
	{
		diststd=diststd+vergep[i].lenth;
		Sumxx=Sumxx+(vergep[i].x)*(vergep[i].x);
		Sumxy=Sumxy+(vergep[i].y)*(vergep[i].x);
		Sumx=Sumx+(vergep[i].x);
		Sumy=Sumy+(vergep[i].y);
		
	}
    diststd=diststd/r;
    a=(float)((Sumy*Sumxx-Sumx*Sumxy)/(r*Sumxx-Sumx*Sumx));
	b=(float)((r*Sumxy-Sumx*Sumy)/(r*Sumxx-Sumx*Sumx));
    posiy1=a+b*wx1;
	posiy2=a+b*wx2;

	aimline.a=a;
	aimline.b=b;
	aimline.Std=(float)diststd;
	aimline.Std=0;
	aimline.point1.x=0;
	aimline.point1.y=(int)a;
	aimline.point2.x=ImageWidth;
	aimline.point2.y=(int)(a+b*ImageWidth);
	return (aimline);
	}
	else
	{

		aimline.Symbol=0;
		return(aimline);
	}
}
else
{
	aimline.Symbol=0;
	return(aimline);
}

}


//********************* 边缘检测与直线拟和－－垂直直线 *********************
//***************************************************************************
SLine_F CImageProc::FindVline(unsigned char *ImageArray,
			                 SRect_I srect,
							 int DifferRange)			  
{  

	typedef struct 
	{
	  double x;
	  int y;
	  double lenth;
	}vergeV;

	vergeV vergep[ImageHeight];
	vergeV n;
	int i=0,j=0,l=0,k=0;
	int m,r;
	int p=0,q=0,count=0;
	static short diffecoffi1[ImageHeight][ImageWidth];
    static short total[ImageHeight];
	short Y1;
	static float pk[ImageWidth];
	static float vergey[ImageHeight];
	float c=0.0,d=0.0;
	double Sumxy=0.0,Sumy=0.0;
	double Sumxx=0,Sumx=0;
	float lenth[ImageHeight];
	float UpValue,DownValue;
	double diststd=0.0;
	float posix1,posix2;
	short wx1,wx2,wy1,wy2;
	float a,b;
    SLine_F aimline;
	if(CheckRange(srect))
	{
	wx1=srect.point1.x;
	wy1=srect.point1.y;
	wx2=srect.point2.x;
	wy2=srect.point2.y;
	/*
	if((wx1>wx2)||(wy1>wy2)||(wx1<1)||(wy1<1)||(wx2>639)||(wy2>479))
	{symbol=0;
	aimline.a=0;
	aimline.b=0;
	return 0;}
	*/
	//start
	
	for(i=wy1;i<wy2;i=i+1)
	{ 
		for(j=wx1;j<wx2;j++)
		{  
			p=*(ImageArray+ImageWidth*i+j+1);
			q=*(ImageArray+ImageWidth*i+j);
			if (abs(p-q)<DifferRange)
			{
				diffecoffi1[i][j]=0;
			}
			else
			{
				diffecoffi1[i][j]=abs(p-q);
			}
		}
	}
	// Differential coefficient   
    for(i=wy1;i<wy2;i=i+1)
	{
		total[i]=0;
		for(j=wx1;j<wx2;j++)
		{
			total[i]=total[i]+diffecoffi1[i][j];
		}
	}
	// Total  differential
	for(i=wy1;i<wy2;i=i+1)
	{
        vergey[k]=0.0;
		if (total[i]==0)
		{
//			total[i]=(wx2-wx1)/2;
			vergey[k]=(float)(wy1+wy2)/2;
			count++;
			if (count>((wy2-wy1)*0.2))
			{
				aimline.Symbol=0;
				return(aimline);
			}
		}
		else
		{
			aimline.Symbol=1;
			for(j=wx1;j<wx2;j++)
			{ 
				pk[j]=float(diffecoffi1[i][j])/float(total[i]);
				vergey[k]=vergey[k]+(j+1)*(pk[j]);
				
			}
		}
		k=k+1;
	}
	// Expectation value
	
	k=0;
	for(i=wy1;i<wy2;i=i+1)
	{
		Sumxx=Sumxx+i*i;
		Sumxy=Sumxy+i*vergey[k];
		Sumx=Sumx+i;
		Sumy=Sumy+vergey[k];
		k=k+1;
		m=k;
	}
	
    a=(float)((Sumy*Sumxx-Sumx*Sumxy)/(m*Sumxx-Sumx*Sumx));
	b=(float)((m*Sumxy-Sumx*Sumy)/(m*Sumxx-Sumx*Sumx));
    c=1/b;
	d=-a/b;
	a=d;
	b=c;
	posix1=(wy1-a)/b;
	posix2=(wy2-a)/b;
	// Line parameter	
	k=0;
	for(i=wy1;i<wy2;i=i+1)
	{
		UpValue=b*vergey[k]-i+a;
		DownValue=(float)sqrt(b*b+1);
		lenth[k]=UpValue/DownValue;
		if (lenth[k]<0)
		{
			lenth[k]=-lenth[k];
		}
		k=k+1;
	}
    
	k=0;
	Y1=wy1;
    for(i=0;i<m;i++)
	{
		vergep[i].x=vergey[k];
		vergep[i].y=Y1;
		vergep[i].lenth=lenth[k];
		k=k+1;
		Y1=Y1+1;
	}
	
    
	for(i=1;i<m;i++)
	{
		for(j=0;j<m-i;j++)
		{
			if(vergep[j].lenth>vergep[j+1].lenth)
			{
				n=vergep[j];
				vergep[j]=vergep[j+1];
				vergep[j+1]=n;
			}
		}
	}
    r=int(m*0.8);
	Sumxx=0.0;
	Sumxy=0.0;
	Sumx=0.0;
	Sumy=0.0;
    for(i=0;i<r;i++)
	{
		diststd=diststd+vergep[i].lenth;
		Sumxx=Sumxx+(vergep[i].y)*(vergep[i].y);
		Sumxy=Sumxy+(vergep[i].x)*(vergep[i].y);
		Sumx=Sumx+(vergep[i].y);
		Sumy=Sumy+(vergep[i].x);
		
	}
	diststd=diststd/r;
    a=(float)((Sumy*Sumxx-Sumx*Sumxy)/(r*Sumxx-Sumx*Sumx));
	b=(float)((r*Sumxy-Sumx*Sumy)/(r*Sumxx-Sumx*Sumx));
    a=-a/b;
	b=1/b;
	posix1=(wy1-a)/b;
	posix2=(wy2-a)/b;

	aimline.a=(float)a;
	aimline.b=(float)b;
	aimline.point1.x=(int)(-a/b);
	aimline.point1.y=0;
	aimline.point2.x=(int)((ImageHeight-a)/b);
	aimline.point2.y=ImageHeight;
    aimline.Std=(float)diststd;

	return (aimline);
	}
	else
	{
		aimline.Symbol=0;
		return(aimline);
	}

}

SLine_F CImageProc::FindVlineB2W(unsigned char *ImageArray,
			                 SRect_I srect,
							 int DifferRange)			  
{  

	typedef struct 
	{
	  double x;
	  int y;
	  double lenth;
	}vergeV;

	vergeV vergep[ImageHeight];
	vergeV n;
	int i=0,j=0,l=0,k=0;
	int m,r;
	int p=0,q=0,count=0;
	static short diffecoffi1[ImageHeight][ImageWidth];
    static short total[ImageHeight];
	short Y1;
	static float pk[ImageWidth];
	static float vergey[ImageHeight];
	float c=0.0,d=0.0;
	double Sumxy=0.0,Sumy=0.0;
	double Sumxx=0,Sumx=0;
	float lenth[ImageHeight];
	float UpValue,DownValue;
	double diststd=0.0;
	float posix1,posix2;
	short wx1,wx2,wy1,wy2;
	float a,b;
    SLine_F aimline;
	if(CheckRange(srect))
	{
	wx1=srect.point1.x;
	wy1=srect.point1.y;
	wx2=srect.point2.x;
	wy2=srect.point2.y;
	/*
	if((wx1>wx2)||(wy1>wy2)||(wx1<1)||(wy1<1)||(wx2>639)||(wy2>479))
	{symbol=0;
	aimline.a=0;
	aimline.b=0;
	return 0;}
	*/
	//start
	
	for(i=wy1;i<wy2;i=i+1)
	{ 
		for(j=wx1;j<wx2;j++)
		{  
			p=*(ImageArray+ImageWidth*i+j+1);
			q=*(ImageArray+ImageWidth*i+j);
			if (p-q<DifferRange)
			{
				diffecoffi1[i][j]=0;
			}
			else
			{
				diffecoffi1[i][j]=p-q;
			}
		}
	}
	// Differential coefficient   
    for(i=wy1;i<wy2;i=i+1)
	{
		total[i]=0;
		for(j=wx1;j<wx2;j++)
		{
			total[i]=total[i]+diffecoffi1[i][j];
		}
	}
	// Total  differential
	for(i=wy1;i<wy2;i=i+1)
	{
        vergey[k]=0.0;
		if (total[i]==0)
		{
//			total[i]=(wx2-wx1)/2;
/*			vergey[k]=(float)(wy1+wy2)/2;
			count++;
			if (count>((wy2-wy1)*0.2))
			{
				aimline.Symbol=0;
				return(aimline);
			}
*/
			count++;
			vergey[k]=0;
		}
		else
		{
			aimline.Symbol=1;
			for(j=wx1;j<wx2;j++)
			{ 
				pk[j]=float(diffecoffi1[i][j])/float(total[i]);
				vergey[k]=vergey[k]+(j+1)*(pk[j]);
				
			}
		}
		k=k+1;
	}


	if(count<(wx2-wx1)*0.5)
	{
	// Expectation value
	int vp=0;
	k=0;
	for(i=wy1;i<wy2;i=i+1)
	{
		if(vergey[k]!=0)
		{
			Sumxx=Sumxx+i*i;
			Sumxy=Sumxy+i*vergey[k];
			Sumx=Sumx+i;
			Sumy=Sumy+vergey[k];
			vp++;
		}
		k=k+1;
		m=k;
	}
	
    a=(float)((Sumy*Sumxx-Sumx*Sumxy)/(vp*Sumxx-Sumx*Sumx));
	b=(float)((vp*Sumxy-Sumx*Sumy)/(vp*Sumxx-Sumx*Sumx));
    c=1/b;
	d=-a/b;
	a=d;
	b=c;
	posix1=(wy1-a)/b;
	posix2=(wy2-a)/b;
	// Line parameter	
	k=0;
	for(i=wy1;i<wy2;i=i+1)
	{
		if(vergey[k]!=0)
		{
			UpValue=b*vergey[k]-i+a;
			DownValue=(float)sqrt(b*b+1);
			lenth[k]=UpValue/DownValue;
			if (lenth[k]<0)
			{
				lenth[k]=-lenth[k];
			}
		}
		else
			lenth[k]=999;
		k=k+1;
	}
    
	k=0;
	Y1=wy1;
    for(i=0;i<m;i++)
	{
		vergep[i].x=vergey[k];
		vergep[i].y=Y1;
		vergep[i].lenth=lenth[k];
		k=k+1;
		Y1=Y1+1;
	}
	
    
	for(i=1;i<m;i++)
	{
		for(j=0;j<m-i;j++)
		{
			if(vergep[j].lenth>vergep[j+1].lenth)
			{
				n=vergep[j];
				vergep[j]=vergep[j+1];
				vergep[j+1]=n;
			}
		}
	}

	if(count>(wx2-wx1)*0.2)
		r=int(wx2-wx1-count-1);
	else
		r=int(m*0.8-1);
	Sumxx=0.0;
	Sumxy=0.0;
	Sumx=0.0;
	Sumy=0.0;
    for(i=0;i<r;i++)
	{
		diststd=diststd+vergep[i].lenth;
		Sumxx=Sumxx+(vergep[i].y)*(vergep[i].y);
		Sumxy=Sumxy+(vergep[i].x)*(vergep[i].y);
		Sumx=Sumx+(vergep[i].y);
		Sumy=Sumy+(vergep[i].x);
		
	}
	diststd=diststd/r;
    a=(float)((Sumy*Sumxx-Sumx*Sumxy)/(r*Sumxx-Sumx*Sumx));
	b=(float)((r*Sumxy-Sumx*Sumy)/(r*Sumxx-Sumx*Sumx));
    a=-a/b;
	b=1/b;
	posix1=(wy1-a)/b;
	posix2=(wy2-a)/b;

	aimline.a=(float)a;
	aimline.b=(float)b;
	aimline.point1.x=(int)(-a/b);
	aimline.point1.y=0;
	aimline.point2.x=(int)((ImageHeight-a)/b);
	aimline.point2.y=ImageHeight;
    aimline.Std=(float)diststd;
    aimline.Std=0;

	return (aimline);
	}
	else
	{
		aimline.Symbol=0;
		return(aimline);
	}
}
{
	aimline.Symbol=0;
	return(aimline);
}
}

SLine_F CImageProc::FindVlineW2B(unsigned char *ImageArray,
			                 SRect_I srect,
							 int DifferRange)			  
{  

	typedef struct 
	{
	  double x;
	  int y;
	  double lenth;
	}vergeV;

	vergeV vergep[ImageHeight];
	vergeV n;
	int i=0,j=0,l=0,k=0;
	int m,r;
	int p=0,q=0,count=0;
	static short diffecoffi1[ImageHeight][ImageWidth];
    static short total[ImageHeight];
	short Y1;
	static float pk[ImageWidth];
	static float vergey[ImageHeight];
	float c=0.0,d=0.0;
	double Sumxy=0.0,Sumy=0.0;
	double Sumxx=0,Sumx=0;
	float lenth[ImageHeight];
	float UpValue,DownValue;
	double diststd=0.0;
	float posix1,posix2;
	short wx1,wx2,wy1,wy2;
	float a,b;
    SLine_F aimline;
	if(CheckRange(srect))
	{
	wx1=srect.point1.x;
	wy1=srect.point1.y;
	wx2=srect.point2.x;
	wy2=srect.point2.y;
	/*
	if((wx1>wx2)||(wy1>wy2)||(wx1<1)||(wy1<1)||(wx2>639)||(wy2>479))
	{symbol=0;
	aimline.a=0;
	aimline.b=0;
	return 0;}
	*/
	//start
	
	for(i=wy1;i<wy2;i=i+1)
	{ 
		for(j=wx1;j<wx2;j++)
		{  
			p=*(ImageArray+ImageWidth*i+j+1);
			q=*(ImageArray+ImageWidth*i+j);
			if (q-p<DifferRange)
			{
				diffecoffi1[i][j]=0;
			}
			else
			{
				diffecoffi1[i][j]=q-p;
			}
		}
	}
	// Differential coefficient   
    for(i=wy1;i<wy2;i=i+1)
	{
		total[i]=0;
		for(j=wx1;j<wx2;j++)
		{
			total[i]=total[i]+diffecoffi1[i][j];
		}
	}
	// Total  differential
	for(i=wy1;i<wy2;i=i+1)
	{
        vergey[k]=0.0;
		if (total[i]==0)
		{
//			total[i]=(wx2-wx1)/2;
/*			vergey[k]=(float)(wy1+wy2)/2;
			count++;
			if (count>((wy2-wy1)*0.5))
			{
				aimline.Symbol=0;
				return(aimline);
			}
*/
			count++;
			vergey[k]=0;
		
		}
		else
		{
			aimline.Symbol=1;
			for(j=wx1;j<wx2;j++)
			{ 
				pk[j]=float(diffecoffi1[i][j])/float(total[i]);
				vergey[k]=vergey[k]+(j+1)*(pk[j]);
				
			}
		}
		k=k+1;
	}

	if(count<(wx2-wx1)*0.5)
	{
	// Expectation value
	int vp=0;
	k=0;
	for(i=wy1;i<wy2;i=i+1)
	{
		if(vergey[k]!=0)
		{
			Sumxx=Sumxx+i*i;
			Sumxy=Sumxy+i*vergey[k];
			Sumx=Sumx+i;
			Sumy=Sumy+vergey[k];
			vp++;
		}		
		k=k+1;
		m=k;
	}
	
    a=(float)((Sumy*Sumxx-Sumx*Sumxy)/(vp*Sumxx-Sumx*Sumx));
	b=(float)((vp*Sumxy-Sumx*Sumy)/(vp*Sumxx-Sumx*Sumx));
    c=1/b;
	d=-a/b;
	a=d;
	b=c;
	posix1=(wy1-a)/b;
	posix2=(wy2-a)/b;
	// Line parameter	
	k=0;
	for(i=wy1;i<wy2;i=i+1)
	{		
		if(vergey[k]!=0)
		{
			UpValue=b*vergey[k]-i+a;
			DownValue=(float)sqrt(b*b+1);
			lenth[k]=UpValue/DownValue;
			if (lenth[k]<0)
			{
				lenth[k]=-lenth[k];
			}
		}
		else
			lenth[k]=999;
		k=k+1;
	}
    
	k=0;
	Y1=wy1;
    for(i=0;i<m;i++)
	{
		vergep[i].x=vergey[k];
		vergep[i].y=Y1;
		vergep[i].lenth=lenth[k];
		k=k+1;
		Y1=Y1+1;
	}
	
    
	for(i=1;i<m;i++)
	{
		for(j=0;j<m-i;j++)
		{
			if(vergep[j].lenth>vergep[j+1].lenth)
			{
				n=vergep[j];
				vergep[j]=vergep[j+1];
				vergep[j+1]=n;
			}
		}
	}

	if(count>(wx2-wx1)*0.2)
		r=int(wx2-wx1-count-1);
	else
		r=int(m*0.8-1);
	
	Sumxx=0.0;
	Sumxy=0.0;
	Sumx=0.0;
	Sumy=0.0;
    for(i=0;i<r;i++)
	{
		diststd=diststd+vergep[i].lenth;
		Sumxx=Sumxx+(vergep[i].y)*(vergep[i].y);
		Sumxy=Sumxy+(vergep[i].x)*(vergep[i].y);
		Sumx=Sumx+(vergep[i].y);
		Sumy=Sumy+(vergep[i].x);
		
	}
	diststd=diststd/r;
    a=(float)((Sumy*Sumxx-Sumx*Sumxy)/(r*Sumxx-Sumx*Sumx));
	b=(float)((r*Sumxy-Sumx*Sumy)/(r*Sumxx-Sumx*Sumx));
    a=-a/b;
	b=1/b;
	posix1=(wy1-a)/b;
	posix2=(wy2-a)/b;

	aimline.a=(float)a;
	aimline.b=(float)b;
	aimline.point1.x=(int)(-a/b);
	aimline.point1.y=0;
	aimline.point2.x=(int)((ImageHeight-a)/b);
	aimline.point2.y=ImageHeight;
    aimline.Std=(float)diststd;
    aimline.Std=0;

	return (aimline);
	}
	else
	{
		aimline.Symbol=0;
		return(aimline);
	}
}
{
	aimline.Symbol=0;
	return(aimline);
}
}

//***********************Sobel边缘提取 *****************************
//******************************************************************
void CImageProc::Sobel(unsigned char*ImagePointer,SRect_I HoughWnd)
{
    int w,h;
    int i,j;
	int buf,buf1,buf2,buf3,buf4;
	unsigned char sobelimg[ImageWidth][ImageHeight];
	unsigned char *p;
	p=ImagePointer;
	w=ImageWidth;
	h=ImageHeight;
	int HoughSx1=HoughWnd.point1.x;
	int HoughSx2=HoughWnd.point2.x;
	int HoughSy1=HoughWnd.point1.y;
	int HoughSy2=HoughWnd.point2.y;

	for(i=HoughSx1;i<HoughSx2;i++)
	{
		for(j=HoughSy1;j<HoughSy2;j++)
		{
			if((i-1)>=0&&(i+1)<w&&(j-1)>=0&&(j+1)<h)
			{
				buf1=(*(p+(j+1)*ImageWidth+i-1))+2*(*(p+(j+1)*ImageWidth+i))+*(p+(j+1)*ImageWidth+i+1);
				buf2=*(p+(j-1)*ImageWidth+i-1)+2*(*(p+(j-1)*ImageWidth+i))+*(p+(j-1)*ImageWidth+i+1);
				buf3=*(p+(j-1)*ImageWidth+i-1)+2*(*(p+j*ImageWidth+i-1))+*(p+ImageWidth*(j+1)+i-1);
				buf4=*(p+(j-1)*ImageWidth+i+1)+2*(*(p+j*ImageWidth+i+1))+*(p+(j+1)*ImageWidth+i+1);
				buf1=buf1-buf2;
				buf3=buf3-buf4;
                buf=abs(buf1)+abs(buf3);
				if(buf>255)
				{
					sobelimg[i][j]=(unsigned char)255;
				}
				else
				{
					sobelimg[i][j]=(unsigned char)buf;
				}
			}
			else
			{
				sobelimg[i][j]=*(p+j*ImageWidth+i);
			}

		}
	}

	for(i=HoughSx1;i<HoughSx2;i++)
		for(j=HoughSy1;j<HoughSy2;j++)
			*(p+j*ImageWidth+i)=sobelimg[i][j];
		
	w=ImageWidth;
	h=ImageHeight;
	for(i=HoughSx1;i<HoughSx2;i++)
		for(j=HoughSy1;j<HoughSy2;j++)
			*(p+j*w+i)=255-*(p+j*w+i);

}
//*************************** 细化 ************************************
//*******************************************************************
void CImageProc::Thinning(unsigned char*ImagePointer,SRect_I HoughWnd)
{
	int i,j;
	int num;
	unsigned char *pData=ImagePointer;
	int Height=HoughWnd.point2.y-HoughWnd.point1.y;
	int Width=HoughWnd.point2.x-HoughWnd.point1.x;
	unsigned char* ptemp=(unsigned char *)malloc(Height*Width*sizeof(unsigned char));
	//细化结束标志；
	BOOL Finished;
	//各个变量用来存储(i,j)位置的八邻域像素点的灰度；
	int nw,n,ne,w,e,sw,s,se;
	//细化表；
	static int erasetable[256]={
				0,0,1,1,0,0,1,1,
				1,1,0,1,1,1,0,1,
				1,1,0,0,1,1,1,1,
				0,0,0,0,0,0,0,1,

				0,0,1,1,0,0,1,1,
				1,1,0,1,1,1,0,1,
				1,1,0,0,1,1,1,1,
				0,0,0,0,0,0,0,1,

				1,1,0,0,1,1,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,

				1,1,0,0,1,1,0,0,
				1,1,0,1,1,1,0,1,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,

				0,0,1,1,0,0,1,1,
				1,1,0,1,1,1,0,1,
				1,1,0,0,1,1,1,1,
				0,0,0,0,0,0,0,1,

				0,0,1,1,0,0,1,1,
				1,1,0,1,1,1,0,1,
				1,1,0,0,1,1,1,1,
				0,0,0,0,0,0,0,0,

				1,1,0,0,1,1,0,0,
				0,0,0,0,0,0,0,0,
				1,1,0,0,1,1,1,1,
				0,0,0,0,0,0,0,0,

				1,1,0,0,1,1,0,0,
				1,1,0,1,1,1,0,0,
				1,1,0,0,1,1,1,0,
				1,1,0,0,1,0,0,0
								};
		if(pData==NULL)
		{
			AfxMessageBox(L"图像数据为空，请读取图像数据");
			return;
		} 
		for(i=HoughWnd.point1.x;i<HoughWnd.point2.x;i++)
			for(j=HoughWnd.point1.y;j<HoughWnd.point2.y;j++)
				*(ptemp+(i-HoughWnd.point1.x)+(j-HoughWnd.point1.y)*Width)=*(pData+ImageWidth*j+i);
			
		Finished=FALSE;
		//开始细化；
		while(!Finished)
		{
			Finished=TRUE;
			//水平扫描；
			for (i=10;i<Height-10;i++)
			{ 
				for(j=10;j<Width-10;j++)
				{
//			for(i=HoughSy1;i<HoughSy2;i++)
//			{
//				for(j=HoughSx1;j<HoughSx2;j++)
//				{
					if(*(ptemp+Width*(Height-i-1)+j)==0)
					{
						w=*(ptemp+Width*(Height-i-1)+j-1);
						e=*(ptemp+Width*(Height-i-1)+j+1);
						//判断(i,j)是否是边界点，如是，求该点的八邻域灰度值(0/255)，根据各点的权重，计算对应查找表的索引；
						if( (w==255)|| (e==255))
						{	
							nw=*(ptemp+Width*(Height-i)+j-1);
							n=*(ptemp+Width*(Height-i)+j);
							ne=*(ptemp+Width*(Height-i)+j+1);
							sw=*(ptemp+Width*(Height-i-2)+j-1);
							s=*(ptemp+Width*(Height-i-2)+j);
							se=*(ptemp+Width*(Height-i-2)+j+1); 
							num=nw/255+n/255*2+ne/255*4+w/255*8+e/255*16+sw/255*32+s/255*64+se/255*128;
							if(erasetable[num]==1)
							{ 
							//查表，如果符合条件，将边界点修改为图像的背景；
								*(ptemp+Width*(Height-i-1)+j)=(BYTE)255;
								Finished=FALSE;//再次进行扫描；
								j++;
							}
						}
					} 
				}
			}
			//垂直扫描；
			for (j=10;j<Width-10;j++)
			{ 
				for(i=10;i<Height-10;i++)
				{
//			for(j=HoughSy1;j<HoughSy2;j++)
//			{
//				for(i=HoughSx1;i<HoughSx2;i++)
//				{
					if(*(ptemp+Width*(Height-i-1)+j)==0)
					{
						n=*(ptemp+Width*(Height-i)+j);
						s=*(ptemp+Width*(Height-i-2)+j);
						if( (n==255)|| (s==255))
						{
							nw=*(ptemp+Width*(Height-i)+j-1);
							ne=*(ptemp+Width*(Height-i)+j+1);
							w=*(ptemp+Width*(Height-i-1)+j-1);
							e=*(ptemp+Width*(Height-i-1)+j+1);
							sw=*(ptemp+Width*(Height-i-2)+j-1);
							se=*(ptemp+Width*(Height-i-2)+j+1); 
							num=nw/255+n/255*2+ne/255*4+w/255*8+e/255*16+sw/255*32+s/255*64+se/255*128;
							if(erasetable[num]==1)
							{
								//查表，如果符合条件，将边界点修改为图像的背景；
								*(ptemp+Width*(Height-i-1)+j)=(BYTE)255;
								Finished=FALSE;//再次进行扫描；
								i++; 
							}
						}
					} 
				}
			} 	
			
		}

		for(i=HoughWnd.point1.x;i<HoughWnd.point2.x;i++)
			for(j=HoughWnd.point1.y;j<HoughWnd.point2.y;j++)
				*(pData+ImageWidth*j+i)=*(ptemp+(i-HoughWnd.point1.x)+(j-HoughWnd.point1.y)*Width);


}
//************************* 哈夫变换提取直线*******************
//*************************************************************
SLine_F CImageProc::HoughLine(unsigned char *ImagePointer,
							 SRect_I HoughWnd,double *mtsin,
							 double *mtcos)
{

    register int i,j,c;
    int temp;
//    double radian;
	int height,width;
	height=HoughWnd.point2.y-HoughWnd.point1.y;//m_ImageBox.GetImageHeight();
	width=HoughWnd.point2.x-HoughWnd.point1.x;//m_ImageBox.GetImageWidth();
	int HoughSx1=HoughWnd.point1.x;
	int HoughSy1=HoughWnd.point1.y;
	int HoughSx2=HoughWnd.point2.x;
	int HoughSy2=HoughWnd.point2.y;
//    double PI=3.1415926535;
//    double increment=PI/180;
    int r_Max=(int)(sqrt((float)(height*height+width*width))+0.5);
    int * ptemp=(int *)malloc(r_Max*180*sizeof(int));
    SLine_F *lpmyline=(SLine_F *)malloc(r_Max*180*sizeof(SLine_F));
	SLine_F *templine,MaxLine;//,SecondLine;    
	BYTE vp;
	unsigned char* P;
	P=ImagePointer;
	
    for(i=0;i<(long)r_Max*180;i++)
	{
		templine=(SLine_F*)(lpmyline+i);
		(*templine).point2.y=32767;
		(*templine).Symbol=false;
	}
	memset(ptemp,0,r_Max*180*sizeof(int));
//        radian=increment*c;
    for(i=HoughSx1;i<HoughSx2;i++)
    {
        for(j=HoughSy1;j<HoughSy2;j++)
		{
			vp=*(P+ImageWidth*j+i);
			if(vp==0)
			{
				for(c=0;c<180;c++)
				{
                    temp=(int)fabs(((i-HoughSx1)*mtcos[c]+(j-HoughSy1)*mtsin[c]+0.5));
                    ptemp[c*r_Max+temp]++;
                    templine=(SLine_F*)(lpmyline+c*r_Max+temp);
					
					if(j>(*templine).point1.y)
					{
						(*templine).point1.x=i;
						(*templine).point1.y=j;
						(*templine).Symbol=1;
					}
					
					if(j<(*templine).point2.y)
					{
						(*templine).point2.x=i;
						(*templine).point2.y=j;
						(*templine).Symbol=1;
					}
                }
            }
        }
    }

	long FirstLineRho;
	long FirstLineTheta;
	long nMaxHough=0;//Hough图像最大值

    int k,m;
	for( k = 0 ; k < r_Max ; k++)
	{
		for( m = 0 ; m < 180 ; m++)
		{
			templine=(SLine_F*)(lpmyline+m*r_Max+k);
			if(k==0&&m==0)
			{
				nMaxHough = ptemp[m*r_Max+k];
				MaxLine.Symbol=false;
			}

			if(nMaxHough<ptemp[m*r_Max+k])
			{
				nMaxHough = ptemp[m*r_Max+k];
				FirstLineRho=k;
				FirstLineTheta=m;
				MaxLine.point1.x=(*templine).point1.x;
				MaxLine.point1.y=(*templine).point1.y;
				MaxLine.point2.x=(*templine).point2.x;
				MaxLine.point2.y=(*templine).point2.y;
				MaxLine.Symbol=true;
			}
			
		}
	}
    free(lpmyline);
	free(ptemp);
	return(MaxLine);
}
//**********************拟和圆主函数***************************
//**************************************************************
SCircle_F CImageProc::CircleFitting(unsigned char *ImageArray,
									SPoint_I point,float StartAngle,
									float EndAngle,int InnerCR,
									int OuterCR,int GrayRange,
									int TotalDiffRange,float fTargetRate,double& dActualRate)
{      
	int i;
    int FittingPoints;
    int ValidData=0;
    float Angle;
	float Rate=0;	
	FittingPoints=int(EndAngle-StartAngle);
	SPoint_F DispersePoints;
	SCircle_F FittingCircle;
	SPoint_F MatchCenter;
	SPoint_F *FittingPoint=new SPoint_F[FittingPoints];
//    double a,b;

    
    Angle=StartAngle;
	MatchCenter.x=(float)point.x;
	MatchCenter.y=(float)point.y;
	
	for(i=0;i<FittingPoints;i++)
	{
     DispersePoints=FindDiagonalWtoB(ImageArray,Angle,InnerCR,
		 OuterCR,MatchCenter,GrayRange,TotalDiffRange);
	 if(DispersePoints.x!=0&&DispersePoints.y!=0)
	 {
		ValidData++;
	 }
     FittingPoint[i]=DispersePoints;
     Angle=Angle+1;
	}
    Rate=float(ValidData)/float(FittingPoints);

	if(Rate<fTargetRate)
	{
        FittingCircle.a=0;
		FittingCircle.b=0;
		FittingCircle.r=0;
		FittingCircle.key=FALSE;
	}
	else
	{
        FittingCircle=CircleFitToData(FittingPoint,ValidData,FittingPoints);
    }
	delete []FittingPoint;
    return(FittingCircle);
}
//****************************边缘点检测函数**************************
//********************************************************************

SPoint_F CImageProc::FindDiagonalWtoB(unsigned char *ImageArray,
					                       float Angle,int r,int R,
						                   SPoint_F CenterPos,int Range,
						                   int DiffRange)
{
    SPoint_F EdgePoint;	
	int i,j,k=0;
	int x1,y1,x2,y2;
    int Difference[ImageWidth];
    int TotalDiffer=0;
	int symbol;
	double Probility[ImageWidth];
    double Expectation=0;
	double A,B;
    
    if(Angle>360)
	{
		Angle=Angle-360;
	}

	if((Angle>=0 && Angle<=45)||(Angle>315 && Angle<=360))
    {
		x1=int(CenterPos.x+r*cos((Angle/360)*2*PI));
        x2=int(CenterPos.x+R*cos((Angle/360)*2*PI));
		symbol=0;
	}
	else if(Angle>45 && Angle<=135)
	{
		y1=int(CenterPos.y+r*sin((Angle/360)*2*PI));
		y2=int(CenterPos.y+R*sin((Angle/360)*2*PI));
		symbol=1;
	}
    else if(Angle>135 && Angle<=225)
	{
		x1=int(CenterPos.x+R*cos((Angle/360)*2*PI));
		x2=int(CenterPos.x+r*cos((Angle/360)*2*PI));
		symbol=0;
	}
	else if(Angle>225 && Angle<=315)
	{
        y1=int(CenterPos.y+R*sin((Angle/360)*2*PI));
		y2=int(CenterPos.y+r*sin((Angle/360)*2*PI));
		symbol=1;
	}


 if(symbol==0)
 {
	for(i=x1;i<x2;i++)             // calculate difference
	{ 
	   B=tan((Angle/360)*2*PI);
	   A=CenterPos.y-B*CenterPos.x;  //  Y=A+BX       
	   y1=int(A+B*i);
       y2=int(A+B*(i+1));
       Difference[i]=*(ImageArray+ImageWidth*y2+i+1)-*(ImageArray+ImageWidth*y1+i);
	   if(Difference[i]>(-1*Range)&&Difference[i]<Range)  
	   {
		   Difference[i]=0;
	   }
		if(Difference[i]<0)
		{
			TotalDiffer=TotalDiffer-Difference[i];
		}
		else
		{
	        TotalDiffer=TotalDiffer+Difference[i];
		}

	}

  if(TotalDiffer<=DiffRange)
	{
        EdgePoint.x=0;
		EdgePoint.y=0;
	}
  else
	{
	   for(i=x1;i<x2;i++)            // calculate probility
	   {
	      Probility[i]=float(Difference[i])/float(TotalDiffer);
		  if(Probility[i]<0)
		  {
			  Probility[i]=(-1)*Probility[i];
		  }

	   }
       for(j=1;j<(x2-x1+1);j++)
	   {
          Expectation=Expectation+Probility[x1+j-1]*(j);    //Edge point X pos
	   }
	
     EdgePoint.x=(float)(x1+Expectation);
	 if(Angle==0||Angle==360||Angle==180)
	 {
		 EdgePoint.y=CenterPos.y;
	 }
	 else
	 {
	     EdgePoint.y=(float)(A+B*(EdgePoint.x));
	 }
  }
 }
else
  {
     for(i=y1;i<y2;i++)
	 {
        if(Angle==90||Angle==270)
		{
			x1=int(CenterPos.x);
			x2=x1;
		}
		else
		{
		    B=tan((Angle/360)*2*PI);
	        A=CenterPos.y-B*CenterPos.x;  //  Y=A+BX 
			x1=int((i-A)/B);
		    x2=int(((i+1)-A)/B);
		}
		Difference[i]=*(ImageArray+ImageWidth*(i+1)+x2)-*(ImageArray+640*i+x1);
	   
		if(Difference[i]<Range&&Difference[i]>(-1*Range))
	   {
		   Difference[i]=0;
	   }
		if(Difference[i]<0)
		{
			TotalDiffer=TotalDiffer-Difference[i];
		}
		else
		{
	        TotalDiffer=TotalDiffer+Difference[i];
		}
	 }
	 if(TotalDiffer<=DiffRange)
	 {
        EdgePoint.x=0;
	    EdgePoint.y=0;
//		(*EdgePoint).key=false;
	 }
	 else
	 {
		 for(i=y1;i<y2;i++)
		 {
			 Probility[i]=float(Difference[i])/float(TotalDiffer);
		     if(Probility[i]<0)
			 {
			  Probility[i]=(-1)*Probility[i];
			 }
		 }
		 for(i=1;i<y2-y1+1;i++)
		 {
			 Expectation=Expectation+Probility[y1+i-1]*(i);
		 }
     EdgePoint.y=(float)(y1+Expectation);
	 if(Angle==90||Angle==270)
	 {
		 EdgePoint.x=CenterPos.x;
	 }
	 else
	 {
	     EdgePoint.x=(float)((EdgePoint.y-A)/B);
	 }

	 }

}
  return(EdgePoint);
}
//**************************边缘点拟和成圆函数******************
//**************************************************************

SCircle_F CImageProc::CircleFitToData(SPoint_F Points[],
			int validnum,int FitNumber)
{
//	struct ValidPoint n;
	double A,B,C,D,E;
	double Sx,Sy,Sxy,Sx2,Sy2,Sx3,Sy3,Sxy2,Syx2;
//    int i,l,r;
	double j;
    SCircle_F K;
//	double PointToCircleLenth=0;
//    int ValidPoint2=0;
//	SCircle_F k;
	Sx=0;
	Sy=0;
	Sx2=0;
	Sy2=0;
	Sxy=0;
	Sx3=0;
	Sy3=0;
	Sxy2=0;
	Syx2=0;
    A=0;
	B=0;
	C=0;
	D=0;
	E=0;
    int i=0;
	//First Fitting
	for(i=0;i<FitNumber;i++)
	{
		if(Points[i].x!=0&&Points[i].y!=0)
		{
			Sx=Sx+Points[i].x;
			Sy=Sy+Points[i].y;
			Sxy=Sxy+(Points[i].x*Points[i].y);
			Sxy2=Sxy2+(Points[i].x*Points[i].y*Points[i].y);
			Syx2=Syx2+(Points[i].y*Points[i].x*Points[i].x);
			Sx2=Sx2+(Points[i].x*Points[i].x);
			Sy2=Sy2+(Points[i].y*Points[i].y);
			Sx3=Sx3+(Points[i].x*Points[i].x*Points[i].x);
			Sy3=Sy3+(Points[i].y*Points[i].y*Points[i].y);
		}
	}
    A=(validnum*Sx2)-(Sx*Sx);
	B=(validnum*Sxy)-(Sx*Sy);
    C=(validnum*Sy2)-(Sy*Sy);
	D=0.5*((validnum*Sxy2)-(Sx*Sy2)+(validnum*Sx3)-(Sx*Sx2));
 	E=0.5*((validnum*Syx2)-(Sy*Sx2)+(validnum*Sy3)-(Sy*Sy2));
       
    K.a=(float)((D*C-B*E)/(A*C-B*B));
	K.b=(float)((A*E-B*D)/(A*C-B*B));
	K.r=0;
	for(i=0;i<FitNumber;i++)
	{
		if(Points[i].x!=0&&Points[i].y!=0)
		{
	       	j=(Points[i].x-K.a)*(Points[i].x-K.a)+(Points[i].y-K.b)*(Points[i].y-K.b);
            K.r=(float)(K.r+sqrt(j));
		}
	}
	K.r=(float)(K.r/validnum);
//    (*K).key=true;
                                  // Second fitting   
/*	validnum=0;
	Sx=0;
	Sy=0;
	Sx2=0;
	Sy2=0;
	Sxy=0;
	Sx3=0;
	Sy3=0;
	Sxy2=0;
	Syx2=0;
	for(i=0;i<FitNumber;i++)
	{
  ValidData[i].Lenth=sqrt((Points[i].X-(*K).Ca)*(Points[i].X-(*K).Ca)+
	  (Points[i].Y-(*K).Cb)*(Points[i].Y-(*K).Cb));
	  ValidData[i].X=Points[i].X;
	  ValidData[i].Y=Points[i].Y;
	  ValidData[i].key=Points[i].key;
	}
	for(i=1;i<FitNumber;i++)
	{
		for(l=0;l<FitNumber-i;l++)
		{
			if(ValidData[l].Lenth>ValidData[l+1].Lenth)
			{
				n=ValidData[l];
				ValidData[l]=ValidData[l+1];
				ValidData[l+1]=n;
			}
		}
	}
    r=int(FitNumber*0.8);
	
	for(i=0;i<r;i++)
	{
       	if(ValidData[i].key==true)
		{
			validnum=validnum+1;
			Sx=Sx+ValidData[i].X;
			Sy=Sy+ValidData[i].Y;
			Sxy=Sxy+(ValidData[i].X*ValidData[i].Y);
			Sxy2=Sxy2+(ValidData[i].X*ValidData[i].Y*ValidData[i].Y);
			Syx2=Syx2+(ValidData[i].Y*ValidData[i].X*ValidData[i].X);
			Sx2=Sx2+(ValidData[i].X*ValidData[i].X);
			Sy2=Sy2+(ValidData[i].Y*ValidData[i].Y);
			Sx3=Sx3+(ValidData[i].X*ValidData[i].X*ValidData[i].X);
			Sy3=Sy3+(ValidData[i].Y*ValidData[i].Y*ValidData[i].Y);
		}
	}
    A=(validnum*Sx2)-(Sx*Sx);
	B=(validnum*Sxy)-(Sx*Sy);
    C=(validnum*Sy2)-(Sy*Sy);
	D=0.5*((validnum*Sxy2)-(Sx*Sy2)+(validnum*Sx3)-(Sx*Sx2));
 	E=0.5*((validnum*Syx2)-(Sy*Sx2)+(validnum*Sy3)-(Sy*Sy2));
       
    (*K).Ca=(D*C-B*E)/(A*C-B*B);
	(*K).Cb=(A*E-B*D)/(A*C-B*B);
	for(i=0;i<r;i++)
	{
		if(ValidData[i].key)
		{
	       	j=(ValidData[i].X-(*K).Ca)*(ValidData[i].X-(*K).Ca)+(ValidData[i].Y-(*K).Cb)*(ValidData[i].Y-(*K).Cb);
            (*K).radius=(*K).radius+sqrt(j);
			ValidData[i].Lenth =sqrt(j);

		}
	}
	(*K).radius=(*K).radius/validnum;
    
	for(i=0;i<r;i++)
	{
		if(ValidData[i].key)
		{
            ValidData[i].Lenth =ValidData[i].Lenth-(*K).radius;
            PointToCircleLenth=PointToCircleLenth+ValidData[i].Lenth; 
			ValidPoint2=ValidPoint2+1; 
		}
	}

	PointToCircleLenth=PointToCircleLenth/ValidPoint2;
	if(PointToCircleLenth>Spec)
	{
       (*K).key=false;
	}
	else 
	{
       (*K).key=true;
	}
*/
	return(K);

}

void CImageProc::SetLowhigh(unsigned char *ImagePointer,int lowhigh)
{
	int i,j;

	for(i=0;i<640;i++)
		for(j=0;j<480;j++)
		{
			if(*(ImagePointer+640*j+i)<lowhigh)
			{
				*(ImagePointer+640*j+i)=0;
			}
			else
			{
				*(ImagePointer+640*j+i)=255;
			}
		}

}



