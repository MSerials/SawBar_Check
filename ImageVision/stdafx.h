// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��









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

//ȫ�ֱ�������
enum{
	INVALID = 0, NoError = 0, STOP = 0, PAUSE = 1, DETECTION = 1,CAPANDDECTION = 2,RUN = 2, SHOWEFFECT = 3, SHOWIMAGE = 4
};
//ȫ�ֱ�������

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
		, hog_width(512) //default 64  //16�ı���
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
			AfxMessageBox(L"û�з���img0.0.1.dll");
			exit(0);
		}
	};
	virtual ~__global()
	{
	};

public:
	//��ǰ�����������
	int CameraNum;
	//test
	int SelTest;
	//IO������
	int IO_Device;
	//���ƽ���״̬
	int AutoDetect;
	bool isCapture;
	bool isShowOver;
	//��Ƥ���Ƿ����
	bool isRect;
	
	struct Controller //�ٿر���
	{
		int ROI_rect;
		int MotorState;
		int SysState;
	}Controller;

	//�������
	CString ErrorString(int code)
	{
		CString str;
		if (SYSEMGENCY == (SYSEMGENCY&code)) str += L"ϵͳ����-";
		if (SYSSTOP == (SYSSTOP&code)) str += L"ϵͳֹͣ-";
		if (SYSRUNNING == (SYSRUNNING&code)) str += L"ϵͳ������-";
		if (SYSTRAINING == (SYSTRAINING&code)) str += L"ģ����ȡ��-";
		if (TOPCAMERROR == (TOPCAMERROR	&code)) str += L"�������δ�ҵ�-";
		if (BOTTOMERROR == (BOTTOMERROR&code)) str += L"�ײ����δ�ҵ�-";
		if (SYSTOPTRAIN == (SYSTOPTRAIN&code)) str += L"�������ѵ����-";
		if (SYSBOTTOMTRAIN == (SYSBOTTOMTRAIN&code)) str += L"�ײ����ѵ����-";
		if (FST_MOTOR_NOT_ALREADY == (FST_MOTOR_NOT_ALREADY&code)) str += L"���һ����-";
		if (SND_MOTOR_NOT_ALREADY == (SND_MOTOR_NOT_ALREADY&code)) str += L"���������-";
		if (SYSPRESSURE == (SYSPRESSURE&code)) str += L"��ѹ����-";
		if (TOP_CAPING == (TOP_CAPING&code)) str += L"���һ��ͼ��-";
		if (BOTTOM_CAPING == (BOTTOM_CAPING&code)) str += L"�������ͼ��-";
		if (SYSFRONTTIMEOUT == (SYSFRONTTIMEOUT	&code)) str += L"������ⳬʱ-";
		if (SYSBACKTIMEOUT == (SYSBACKTIMEOUT&code)) str += L"�ײ���ⳬʱ-";
		if (SYSNEEDORIGIN == (SYSNEEDORIGIN&code)) str += L"��Ҫ��λ-";
		if (SYSSENSORPRESSURE == (SYSSENSORPRESSURE&code)) str += L"��ѹ��Ӧ����Ӧ��ʱ-";
		if (SYSSENSORCYL == (SYSSENSORCYL&code)) str += L"���׸�Ӧ����Ӧ��ʱ-";
		return str;
	}
	//�������ĸ���Ƥ��ʹ�ã����ĸ��Ի���
	
	//���ڸ���Ȥ������ɫ����
	uchar cvcolor;
	bool b_cvcolor;
	//���ڼ������Ȥ����
	cv::Rect ImgROI, MFCROI, ImgFrame, MFCFrame;
	//���ڼ������Ȥ�������
	double widthRatio, heightRatio;
	//daheng ���


	//dalsa���
	CamDALSA *pBottomDalsa;
	CamDALSA *pTopDalsa;
	


	int FrontDetectNo; //����������
	int BackDetectNo;//����������
	int frontNo;
	int backNo;
	int absentNo;
	unsigned long RunningTick;
	int imgReuslt;
	int hog_width;
	int hog_height;
	
	//��ONpaint���ã���Ϊmfc����ĳЩ���������Ժ��Դ���ˢ�£����¿�����ͼƬ����OnPaint���ú��ˢ�»���
	bool delay_display;
	//�洢�˽��
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
	//��������
	exEvent evt_procedure;
	//�������
	CMutex CameraMat_Mutex;//��������Ϊ�������ȡ�����ݲ���ͻ��������
	//mutex mutex_tcamera;//��������Ϊ�������ȡ�����ݲ���ͻ��������
	//mutex mutex_bcamera;////��������Ϊ�������ȡ�����ݲ���ͻ��������
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
	//����һ��ѡ���ļ��еĶԻ��򣬷�����ѡ·��  
	static CString Show()
	{
		TCHAR           szFolderPath[MAX_PATH] = { 0 };
		CString         strFolderPath = TEXT("");

		BROWSEINFO      sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot = 0;
		sInfo.lpszTitle = _T("��ѡ��һ���ļ��У�");
		sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
		sInfo.lpfn = NULL;

		// ��ʾ�ļ���ѡ��Ի���  
		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
		if (lpidlBrowse != NULL)
		{
			// ȡ���ļ�����  
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