#pragma once
#include "stdafx.h"
#include "opencv.hpp"
using namespace cv;
using namespace cv::ml;

#define _HALCON
#ifdef _HALCON
#include "HalconCpp.h"
#ifdef _TEST
#include <iostream>
#include <vector>
using namespace std;
#endif


#ifdef _WIN64
#pragma comment(lib,"lib/halcon/x64/halconcpp.lib")
#else
#pragma comment(lib,"lib/halcon/x86/halconcpp.lib")
#endif

using namespace Halcon;



#pragma warning(disable:4101)


struct _Params
{
	double threshold_get_prod;//获取物品二值化参数
	double TrainScore;
	double MatchScore;
	double ROI_row1; //参数是从row开始，所以。。。
	double ROI_col1;
	double ROI_row2;
	double ROI_col2;

};

class _halcon
{


public:
	_halcon(){};
	~_halcon(){};


	cv::Mat HImageToIplImage(Hobject &Hobj)
	{
		cv::Mat pImage;
		Hlong htChannels;
		char cType[MAX_STRING];
		Hlong     width, height;
		width = height = 0;
		//转换图像格式  
		convert_image_type(Hobj, &Hobj, "byte");
		count_channels(Hobj, &htChannels);
		if (htChannels == 1)
		{
			unsigned char *ptr;
			get_image_pointer1(Hobj, (Hlong *)&ptr, cType, &width, &height);
			pImage = cv::Mat(height, width, CV_8UC1);
			memcpy(pImage.data, ptr, width * height);

		}
		else if (htChannels == 3)
		{
			unsigned char *ptrRed, *ptrGreen, *ptrBlue;
			ptrRed = ptrGreen = ptrBlue = NULL;
			get_image_pointer3(Hobj, (Hlong *)&ptrRed, (Hlong *)&ptrGreen, (Hlong *)&ptrBlue, cType, &width, &height);
			pImage = cv::Mat(height, width, CV_8UC3);
			for (int row = 0; row < height; row++)
			{
				uchar* ptr = pImage.ptr<uchar>(row);
				for (int col = 0; col < width; col++)
				{
					ptr[3 * col] = ptrBlue[row * width + col];
					ptr[3 * col + 1] = ptrGreen[row * width + col];
					ptr[3 * col + 2] = ptrRed[row * width + col];
				}
			}

		}
		return pImage;
	}

	Hobject IplImageToHImage(cv::Mat& pImage)
	{
		Hobject Hobj = NULL;
		if (3 == pImage.channels())
		{
			cv::Mat pImageRed, pImageGreen, pImageBlue;
			std::vector<cv::Mat> sbgr(3);
			cv::split(pImage, sbgr);

			int length = pImage.rows * pImage.cols;
			uchar *dataBlue = new uchar[length];
			uchar *dataGreen = new uchar[length];
			uchar *dataRed = new uchar[length];

			int height = pImage.rows;
			int width = pImage.cols;
			for (int row = 0; row <height; row++)
			{
				uchar* ptr = pImage.ptr<uchar>(row);
				for (int col = 0; col < width; col++)
				{
					dataBlue[row * width + col] = ptr[3 * col];
					dataGreen[row * width + col] = ptr[3 * col + 1];
					dataRed[row * width + col] = ptr[3 * col + 2];
				}
			}
			//gen_image3_extern(&Hobj, "byte", width, height, (long)dataRed, (long)dataGreen, (long)dataBlue, (long)free);
			gen_image3(&Hobj, "byte", width, height, (Hlong)(dataRed), (Hlong)(dataGreen), (Hlong)(dataBlue));
			delete[] dataRed;
			delete[] dataGreen;
			delete[] dataBlue;
		}
		else if (1 == pImage.channels())
		{
		
			int height = pImage.rows;
			int width = pImage.cols;
			uchar *dataGray = new uchar[width * height];
			memcpy(dataGray, pImage.data, width * height);
			gen_image1(&Hobj, "byte", width, height, (Hlong)(dataGray));
			//gen_image1_extern(&Hobj, "byte", (Hlong)width, (Hlong)height, (Hlong)(dataGray), (long)free);
			delete[] dataGray;
		}

		return Hobj;
	}


};

class _CMutexLock
{
public:
	_CMutexLock();
	~_CMutexLock();
	void init();
	void release();
	void lock();
	void unlock();// 设计的时候，不要unwaite放置到unlock里面去，否则会导致职责不分明，如果有内部控制的还会导致无法唤醒。  
	void waite();// 当获取不到数据，那么waite挂起线程，等待其它线程通知释放  
	void unwaite();// 生产了数据那么需要调用unwaite.  
private:
#ifdef _MSC_VER 
	HANDLE m_mutex;
	HANDLE m_event;//事件如果有信号那么可以正常执行，如果无信号那么只能等待  
#else  
	HANDLE m_mutex;
	HANDLE m_event;//事件如果有信号那么可以正常执行，如果无信号那么只能等待 
	pthread_mutex_t m_mutex;
	pthread_cond_t m_condition;
	pthread_mutexattr_t m_mutexAttr;
#endif  
};




class showImage
{
public:
	showImage(){
		RtWin.left = 0;
		RtWin.right = 0;
		RtWin.top = 0;
		RtWin.bottom = 0;
		RtWin1 = RtWin;
		m_dis_hd = NULL;
		m_dis_hd1 = NULL;



	};
	~showImage(){};

	void Init(int ID, CWnd *Wnd){
		switch (ID)
		{
		case 1:WindowHandle = (Hlong)Wnd->m_hWnd; break;
		case 2:WindowHandle1 = (Hlong)Wnd->m_hWnd; break;
		default:
			break;
		}
	
	};
	CCriticalSection m_CriticalImageCard;
	HTuple & get_WindowHandle(){ return WindowHandle; }
	HTuple & get_WindowHandle1(){ return WindowHandle1; }

	void Dis_Obj(const Hobject& hobject)
	{
		Lock.lock();
		try{
			Hlong Width, Height;
			Halcon::get_image_pointer1(hobject, NULL, NULL, &Width, &Height);
			Halcon::set_part(m_dis_hd,NULL,NULL,Height,Width);
			Halcon::disp_obj(hobject, m_dis_hd);
		}
		catch (HException& except)
		{
			TRACE(except.message);
		}
		Lock.unlock();
	}

	void Dis_Obj1(const Hobject& hobject)
	{
		Lock1.lock();
		try{
			Hlong Width, Height;
			Halcon::get_image_pointer1(hobject, NULL, NULL, &Width, &Height);
			Halcon::set_part(m_dis_hd1, NULL, NULL, Height, Width);
			Halcon::disp_obj(hobject, m_dis_hd1);
		}
		catch (HException& except)
		{
			TRACE(except.message);
		}
		Lock1.unlock();
	}

	void Dis_Obj_extend(const Hobject& hobject, CRect rt, CWnd *Wnd){
		try{
			set_window_attr("border_width", 0);
		//	set_window_attr("border_height", 0);
			HTuple WH = (Hlong)Wnd->m_hWnd;
			//HTuple m_WH;
			set_check("~father");
			if (NULL != m_dis_hd)
				close_window(m_dis_hd);
			if (HDevWindowStack::IsOpen())
				close_window(HDevWindowStack::Pop());		
			open_window(0, 0, rt.Width(), rt.Height(), WH, "visible", "", &m_dis_hd);
			set_color(m_dis_hd, "red");
			set_check("father");
		//	set_draw(m_dis_hd, "margin");
			disp_obj(hobject, m_dis_hd);
			
			
		}
		catch (HException& except)
		{
			TRACE(except.message);
		}
	
	}

	void Dis_Obj_extend1(const Hobject& hobject, CRect rt, CWnd *Wnd){
		try{
			set_window_attr("border_width", 0);
			HTuple WH = (Hlong)Wnd->m_hWnd;
			//HTuple m_WH;
			if (NULL != m_dis_hd1)
				close_window(m_dis_hd1);
			if (HDevWindowStack::IsOpen())
				close_window(HDevWindowStack::Pop());

			//	set_check("~father");
			//	set_window_attr("background_color", "black");
			//	open_window(0, 0, rt.Width(), rt.Height(), WH, "visible", "", &m_dis_hd);
			open_window(0, 0, rt.Width(), rt.Height(), WH, "", "", &m_dis_hd1);
			set_color(m_dis_hd1, "red");
			//	set_font(WindowHandle, "-Courier New-18-*-*-*-*-");
			//	set_check("father");
			//	set_draw(m_dis_hd, "margin");
			disp_obj(hobject, m_dis_hd1);


		}
		catch (HException& except)
		{
			TRACE(except.message);
		}

	}

	void Dis_Obj(const Hobject& hobject,CRect rt)
	{
		try{
			HDevWindowStack::Push(m_dis_hd);
			disp_obj(hobject, m_dis_hd);
		}
		catch (HException& except)
		{
			TRACE(except.message);
		}

	}

	void Dis_Obj1(const Hobject& hobject,CRect rt)
	{
		try{
			HDevWindowStack::Push(m_dis_hd1);
			disp_obj(hobject, m_dis_hd1);
		}
		catch (HException& except)
		{
			TRACE(except.message);
		}

	}

	void open_window_extend(CRect rt)
	{
		Lock.lock();
		try{
			set_window_attr("border_width", 0);
			set_check("~father");
			if (NULL != m_dis_hd)
				close_window(m_dis_hd);
			//if (HDevWindowStack::IsOpen())
				//close_window(HDevWindowStack::Pop());
			open_window(0, 0, rt.Width(), rt.Height(), WindowHandle, "visible", "", &m_dis_hd);
			set_check("father");
			set_color(m_dis_hd, "green");		
		}
		catch (HException& except)
		{
			TRACE(except.message);
		}
		Lock.unlock();
	} 

	void open_window_extend1(CRect rt)
	{
		Lock1.lock();
		try{
			set_window_attr("border_width", 0);
			set_check("~father");
			if (NULL != m_dis_hd1)
				close_window(m_dis_hd1);
			//if (HDevWindowStack::IsOpen())
				//close_window(HDevWindowStack::Pop());
			open_window(0, 0, rt.Width(), rt.Height(), WindowHandle1, "visible", "", &m_dis_hd1);
			set_check("father");
			set_color(m_dis_hd1, "green");
			//set_draw(m_dis_hd, "margin");
		}
		catch (HException& except)
		{
			TRACE(except.message);
		}
		Lock1.unlock();
	}

	void set_wnd(HWND hWnd){
		WindowHandle = (Hlong)hWnd;
	};

	void set_wnd1(HWND hWnd){
		WindowHandle1 = (Hlong)hWnd;
	};


	void close()
	{
		if (NULL != m_dis_hd)
			close_window(m_dis_hd);
		if (NULL != m_dis_hd1)
			close_window(m_dis_hd1);
		if (HDevWindowStack::IsOpen())
			close_window(HDevWindowStack::Pop());
		if (HDevWindowStack::IsOpen())
			close_window(HDevWindowStack::Pop());
	}

	HTuple & get_disp_hd(){ return (*this).m_dis_hd; }
	HTuple & get_disp_hd1(){ return (*this).m_dis_hd1; }
private:
	HTuple WindowHandle, WindowHandle1;
	HTuple m_dis_hd, m_dis_hd1;
	_CMutexLock Lock, Lock1;
	CRect RtWin, RtWin1;
};




#endif