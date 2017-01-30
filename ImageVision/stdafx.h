// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif






#ifndef ______INCLUDE_FILE______
#define ______INCLUDE_FILE______


#define MAX_CAMERA 1

#define u8t	unsigned char

//全局变量参数
enum{
	INVALID = 0, NoError = 0, STOP = 0, PAUSE = 1, DETECTION = 1,CAPANDDECTION = 2,RUN = 2, SHOWEFFECT = 3, SHOWIMAGE = 4
};
//全局变量参数

enum{
	NoCard, V1380LEADTECH
};



#ifdef _OPENCV
#include "opencv.h"
#endif
#include "encrypt.h"
#include "ImageDeal.h"
//#include <mutex>


#ifdef _DEBUG

#include "opencv2\opencv.hpp"

//#include "imgDeal.h"
//#pragma comment(lib,"../x64/DEBUG/img0.0.1.lib")
//#include "opencv2\opencv.hpp"

#else

#endif


#ifdef _WIN64

#include "imgDeal.h"

#pragma comment(lib,"../lib/img0.0.1.lib")

#include "opencv2\opencv.hpp"

#ifdef _DEBUG
#pragma comment(lib,"opencv/x64/opencv_world310.lib")
#else
#pragma comment(lib,"opencv/x64/opencv_world310.lib")
#endif

#else
#include "imgDeal.h"
#include "opencv2\opencv.hpp"
//#pragma comment(lib,"camera/lib/x86/GxIAPI.lib")
//#pragma comment(lib,"camera/lib/x86/DxImageProc.lib")
#pragma comment(lib,"opencv/x86/opencv_core2411.lib")
#pragma comment(lib,"opencv/x86/opencv_highgui2411.lib")
#pragma comment(lib,"opencv/x86/opencv_imgproc2411.lib")
#pragma comment(lib,"opencv/x86/opencv_calib3d2411.lib")
#pragma comment(lib,"lib/img0.0.1.lib")

#endif

#include "motioncard/MotionCard.h"
#include "include\Preferences.h"

#ifdef LEADTECH
#ifdef _WIN64
//#include "DMC1380/x64/Dmc1380.h"
#pragma comment(lib,"motioncard\\DMC1380\\x64\\Dmc1380.lib")
#else
//#include "DMC1380/x86/Dmc1380.h"
#pragma comment(lib,"motioncard\\DMC1380\\x86\\Dmc1380.lib")
#endif
#endif






#define WMMSG_PARAMESSAGE WM_USER+1001
#define WMMSG_UPDATEUI WM_USER+1002
#define WMMSG_RESIZEWIN WM_USER+1003

//#include "camera\DahengCam.h"

#include "CamDALSA.h"
#include <afx.h>

class exEvent
{
public:
	exEvent()
		:evt(FALSE, TRUE)
		, isSetEvent(false)
		, Wmessage(0)
		, Lmessage(0)
	{
	};
	~exEvent(){};

	bool SetEvent()		{ evt.SetEvent();	isSetEvent = true;	 return true; }
	bool ResetEvent()	{ evt.ResetEvent(); isSetEvent = false;	 return false; }
	bool EventState()	{ return isSetEvent; }

	unsigned int Wmessage;
	unsigned int Lmessage;
	CEvent evt;
private:
	bool isSetEvent;
};


#define IN_PRODUCT_COME		1
#define OUT_FRONT			0x02
#define OUT_BACK			0x04
#define OUT_NO_PRODUCT		3
#define OUT_ERROR			4
#define OUT_SOFT_START		5
#define OUT_OVER_DETECT		6


#define  _MODEL_MAX			12

enum RECTANGLE{
	NO_RECT = 0, TROI_RECT, TCHAR_RECT, BROI_RECT, BCHAR_RECT
};

enum TIMERUSE
{
	ORIGIN = 1,CAMERCATCH = 2,BOTTOMCAMERCATCH = 3
};



#define NONESTATE				(0)
#define SYSEMGENCY				(1<<0)
#define SYSSTOP					(1<<1)
#define SYSRUNNING				(1<<2)
#define SYSTRAINING				(1<<3)
#define TOPCAMERROR				(1<<4)
#define BOTTOMERROR				(1<<5)
#define SYSTOPTRAIN				(1<<6)
#define SYSBOTTOMTRAIN			(1<<7)
#define FST_MOTOR_NOT_ALREADY	(1<<8)
#define SND_MOTOR_NOT_ALREADY	(1<<9)
#define SYSPRESSURE				(1<<10)
#define TOP_CAPING				(1<<11)
#define BOTTOM_CAPING			(1<<12)
#define SYSFRONTTIMEOUT			(1<<13)
#define SYSBACKTIMEOUT			(1<<14)
#define SYSNEEDORIGIN			(1<<15)

#define SENSOR_PRESS			1
#define SENSOR_CYL				2

#define SYSSENSORPRESSURE		(SENSOR_PRESS<<16)
#define SYSSENSORCYL			(SENSOR_CYL<<16)

#define PRODUCT_OK	2
#define PRODUCT_NG	1
#define PRODUCT_ERROR 0

#ifdef _HALCON
void CPPExpDefaultExceptionHandler(const Halcon::HException& except);
#endif
CString show_roi_msg(UINT id);
class __global
{
public:
	__global()
		:CameraNum(1)
		, SelTest(0)
		, IO_Device(0)
		, AutoDetect(RUN)
		, isCapture(false)
		, hog_width(512) //default 64  //16的倍数
		, hog_height(64) //default 128
		, imgReuslt(0)
		, isRect(false)
		, widthRatio(0.0)
		, heightRatio(0.0)
		, CameraMat_Mutex(FALSE, NULL, NULL)
		, isShowOver(false)
		, delay_display(false)
		, testPicTime(0)
		, evtEmgencyStop(FALSE,TRUE)
		
	{
		pBottomDalsa = NULL;
		pTopDalsa = NULL;
		Controller.ROI_rect = NO_RECT;
		Controller.MotorState |= FST_MOTOR_NOT_ALREADY;
		Controller.MotorState |= SND_MOTOR_NOT_ALREADY;
		Controller.SysState = 0;
		FrontDetectNo = 0;
		BackDetectNo = 0;
		frontNo = 0;
		backNo = 0;
		absentNo = 0;
		RunningTick = GetTickCount();
		HINSTANCE hDll = LoadLibrary(L"img0.0.1.dll");
		ProcessImg = NULL;
		DealImage = NULL;
		Stitching = NULL;
		evt_procedure.ResetEvent();
		if (hDll)
		{
			ProcessImg = (fun)GetProcAddress(hDll, "ProcessImg");
			DealImage = (fun1)GetProcAddress(hDll, "DealImage");
			Stitching = (stitching_fun)GetProcAddress(hDll, "stitching");
#ifdef _HALCON
			halcon_deal = (_halcon_deal)GetProcAddress(hDll, "HalconDeal");

			Front_deal = (_Front_Deal)GetProcAddress(hDll, "Front_deal");
			Back_Deal = (_Back_Deal)GetProcAddress(hDll, "Back_Deal");

			Front_Train = (_Front_Train)GetProcAddress(hDll, "Front_Train");
			Back_Train = (_Back_Train)GetProcAddress(hDll, "Back_Train");


#endif
		}
		else
		{
			AfxMessageBox(L"没有发现img0.0.1.dll");
			exit(0);
		}
	};
	virtual ~__global()
	{
	};

public:
	//当前链接相机数量
	int CameraNum;
	//test
	int SelTest;
	//IO卡号码
	int IO_Device;
	//控制界面状态
	int AutoDetect;
	bool isCapture;
	bool isShowOver;
	//橡皮筋是否可用
	bool isRect;
	
	struct Controller //操控变量
	{
		int ROI_rect;
		int MotorState;
		int SysState;
	}Controller;

	//错误代码
	CString ErrorString(int code)
	{
		CString str;
		if (SYSEMGENCY == (SYSEMGENCY&code)) str += L"系统报警-";
		if (SYSSTOP == (SYSSTOP&code)) str += L"系统停止-";
		if (SYSRUNNING == (SYSRUNNING&code)) str += L"系统运行中-";
		if (SYSTRAINING == (SYSTRAINING&code)) str += L"模板提取中-";
		if (TOPCAMERROR == (TOPCAMERROR	&code)) str += L"顶部相机未找到-";
		if (BOTTOMERROR == (BOTTOMERROR&code)) str += L"底部相机未找到-";
		if (SYSTOPTRAIN == (SYSTOPTRAIN&code)) str += L"顶部相机训练中-";
		if (SYSBOTTOMTRAIN == (SYSBOTTOMTRAIN&code)) str += L"底部相机训练中-";
		if (FST_MOTOR_NOT_ALREADY == (FST_MOTOR_NOT_ALREADY&code)) str += L"电机一报警-";
		if (SND_MOTOR_NOT_ALREADY == (SND_MOTOR_NOT_ALREADY&code)) str += L"电机二报警-";
		if (SYSPRESSURE == (SYSPRESSURE&code)) str += L"负压报警-";
		if (TOP_CAPING == (TOP_CAPING&code)) str += L"相机一采图中-";
		if (BOTTOM_CAPING == (BOTTOM_CAPING&code)) str += L"相机二采图中-";
		if (SYSFRONTTIMEOUT == (SYSFRONTTIMEOUT	&code)) str += L"顶部检测超时-";
		if (SYSBACKTIMEOUT == (SYSBACKTIMEOUT&code)) str += L"底部检测超时-";
		if (SYSNEEDORIGIN == (SYSNEEDORIGIN&code)) str += L"需要复位-";
		if (SYSSENSORPRESSURE == (SYSSENSORPRESSURE&code)) str += L"负压感应器感应超时-";
		if (SYSSENSORCYL == (SYSSENSORCYL&code)) str += L"气缸感应器感应超时-";
		return str;
	}
	//决定是哪个橡皮筋使用，画哪个对话框
	
	//用于感兴趣区域颜色渐变
	uchar cvcolor;
	bool b_cvcolor;
	//用于计算感兴趣区域
	cv::Rect ImgROI, MFCROI, ImgFrame, MFCFrame;
	//用于计算感兴趣区域比例
	double widthRatio, heightRatio;
	//daheng 相机


	//dalsa相机
	CamDALSA *pBottomDalsa;
	CamDALSA *pTopDalsa;
	


	int FrontDetectNo; //正面检测序列
	int BackDetectNo;//背面检测序列
	int frontNo;
	int backNo;
	int absentNo;
	unsigned long RunningTick;
	int imgReuslt;
	int hog_width;
	int hog_height;
	
	//给ONpaint调用，因为mfc里面某些函数调用以后会对窗口刷新，导致看不见图片，在OnPaint调用后给刷新回来
	bool delay_display;
	//存储了结果
	typedef   float(WINAPI   *fun)(cv::Mat&, cv::Rect);
	typedef	float(WINAPI *fun1)(const cv::Mat&, cv::Mat&, cv::Rect);
	typedef int(WINAPI *stitching_fun)(vector<cv::Mat>&, cv::Mat&);
#ifdef _HALCON
	//halcon showImage
	typedef Hobject(WINAPI *_halcon_deal)(const cv::Mat&, cv::Rect, float&);
	typedef Hobject(WINAPI *_Front_Deal)(const Hobject&, const vector<Halcon::Hobject>&, int& res, const _Params&);//image roi,result
	typedef Hobject(WINAPI *_Back_Deal)(const Hobject&, const vector<Halcon::Hobject>&, int& res, const _Params&);
	typedef bool(WINAPI *_Front_Train)(const vector<std::string>&, const vector<Halcon::Hobject>&, const _Params&);
	typedef bool(WINAPI *_Back_Train)(const vector<std::string>&, const vector<Halcon::Hobject>&, const _Params&);
	_halcon_deal halcon_deal;
	_Front_Deal Front_deal;
	_Back_Deal	Back_Deal;

	_Front_Train Front_Train;
	_Back_Train Back_Train;


	showImage show_image;
	Hobject hImage;
	Hobject hRawImage;
	Hobject hRawImage1;
#endif
	fun ProcessImg;
	fun1 DealImage;
	stitching_fun Stitching;

#ifdef __AFXWIN_H__
	//WIN32 API
	CEvent evtEmgencyStop;
	exEvent evt_capture;
	exEvent evt_capture1;
	exEvent evt_capture2;
	exEvent evt_capture3;
	//动作流程
	exEvent evt_procedure;
	//触发检测
	CMutex CameraMat_Mutex;//互斥量，为了相机读取的数据不冲突产生崩溃
	//mutex mutex_tcamera;//互斥量，为了相机读取的数据不冲突产生崩溃
	//mutex mutex_bcamera;////互斥量，为了相机读取的数据不冲突产生崩溃
	exEvent evt_detect;
	CString resultmsg;
	CString train_data_path;
	vector<CString> TopSideImgPath;
	vector<CString> BottomSideImgPath;
	vector<CString> StitchingPath;
	vector<CString> testPic;
	int testPicTime;
#else
	//standard C++
	vector<std::string> TopSideImgPath;
	vector<std::string> BottomSideImgPath;
	std::string train_data_path;
	std::string resultmsg;
#endif
#ifdef _OPENCV
	opencv Opencv;
#endif
	CPreferences ini;
	CPreferences prjini;
	Motion mc;
	CImageDeal img_deal;

	struct tagTEST
	{
		int val1;
		int val2;
		double val3;
		int val4;
		int val5;
			

	};
	tagTEST mytest;
};
extern __global g;





#endif




#ifndef SELECT_FOLDER_DLG_H  
#define SELECT_FOLDER_DLG_H  


#ifndef BIF_NEWDIALOGSTYLE  
#define  BIF_NEWDIALOGSTYLE  0x0040  
#endif  

class CSelectFolderDlg
{
public:
	//创建一个选择文件夹的对话框，返回所选路径  
	static CString Show()
	{
		TCHAR           szFolderPath[MAX_PATH] = { 0 };
		CString         strFolderPath = TEXT("");

		BROWSEINFO      sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot = 0;
		sInfo.lpszTitle = _T("请选择一个文件夹：");
		sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
		sInfo.lpfn = NULL;

		// 显示文件夹选择对话框  
		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
		if (lpidlBrowse != NULL)
		{
			// 取得文件夹名  
			if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
			{
				strFolderPath = szFolderPath;
			}
		}
		if (lpidlBrowse != NULL)
		{
			::CoTaskMemFree(lpidlBrowse);
		}

		return strFolderPath;

	}

};

#endif

//#define EXCEPTION_EXECUTE_HANDLER 0






#define _TEST