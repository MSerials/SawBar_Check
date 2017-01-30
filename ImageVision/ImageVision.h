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

// ImageVision.h : ImageVision 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CImageVisionApp:
// 有关此类的实现，请参阅 ImageVision.cpp
//

class CImageVisionApp : public CWinAppEx
{
public:
	CImageVisionApp();
	void *pMain;

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnAppAbout();
	afx_msg void OnButtonBackSize();
	afx_msg void OnButtonLoadImg();
	afx_msg void OnButtonLoadImg2();
	afx_msg void OnButtonSetting();
	afx_msg void OnButtonGrab();
	afx_msg void OnButtonIocardset();

	int Procedure();
	//int GetAxis1Status();//获取机械当前状态
	//int GetAxis2Status();//获取机械当前状态
	std::vector<int> GetSysStatus();
	void InitData();

	//延时初始化线程，保证界面优先打开，并等待硬件初始化
	bool RunDelayInitThread();
	CWinThread* pDelayInit;
	static UINT DelayInit(LPVOID Lparam);

	//正面相机拍照线程
	bool RunTopImageThread();
	CWinThread* pTopImage;
	static UINT TopImageThread(LPVOID Lparam);

	//背面相机拍照线程
	bool RunBottomImageThread();
	CWinThread* pBottomImage;
	static UINT BottomImageThread(LPVOID Lparam);

	//主逻辑运动线程
	bool RunMainThread();
	CWinThread* pMainThread;
	static UINT MainThread(LPVOID Lparam);

	//感应器线程
	bool RunSensorThread();
	CWinThread* pSensorThread;
	static UINT SensorThread(LPVOID Lparam);

	//正面相机处理线程
	bool RunFrontDeTection();
	CWinThread* pFrontThread;
	bool isFrontDetect;
	static UINT FrontDeTection(LPVOID lp);

	//背面相机处理线程
	bool RunBackDeTection();
	CWinThread* pBackThread;
	bool isBackDetect;
	static UINT BackDeTection(LPVOID lp);

	void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvetn, DWORD dwTime);

	int FrontResult;
	int BackResult;
#if 0
	struct _Controller
	{
		DWORD FrontDeTectionTicks;// = GetTickCount();
		DWORD DelayForGetChainBar;// r = GetTickCount();						//给第一块放导板的气缸计时，防止时间太短没放好就进行运动
		bool isGettingBar;// = false;
		DWORD DelayForPutOffChainBar;// = GetTickCount();						//给第一块放导板的气缸计时，防止时间太短没放好就进行运动
		bool isPuttingBar;// = false;
		DWORD DelayForPutBackChainBar;// = GetTickCount();						//给第一块放导板的气缸计时，防止时间太短没缩回来就进行运动
		bool isBackBar;// = false;
		DWORD isMotionChange;// = 0;											//用于循环检测是否有改变动作
		DWORD DelayForOutResult;

		bool FrontDetecting;// = false;
		bool BackDetecting;
	} Controller;
#endif

	afx_msg void OnButtonCamera();
};

extern CImageVisionApp theApp;
