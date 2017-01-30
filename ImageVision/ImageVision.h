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

// ImageVision.h : ImageVision Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CImageVisionApp:
// �йش����ʵ�֣������ ImageVision.cpp
//

class CImageVisionApp : public CWinAppEx
{
public:
	CImageVisionApp();
	void *pMain;

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
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
	//int GetAxis1Status();//��ȡ��е��ǰ״̬
	//int GetAxis2Status();//��ȡ��е��ǰ״̬
	std::vector<int> GetSysStatus();
	void InitData();

	//��ʱ��ʼ���̣߳���֤�������ȴ򿪣����ȴ�Ӳ����ʼ��
	bool RunDelayInitThread();
	CWinThread* pDelayInit;
	static UINT DelayInit(LPVOID Lparam);

	//������������߳�
	bool RunTopImageThread();
	CWinThread* pTopImage;
	static UINT TopImageThread(LPVOID Lparam);

	//������������߳�
	bool RunBottomImageThread();
	CWinThread* pBottomImage;
	static UINT BottomImageThread(LPVOID Lparam);

	//���߼��˶��߳�
	bool RunMainThread();
	CWinThread* pMainThread;
	static UINT MainThread(LPVOID Lparam);

	//��Ӧ���߳�
	bool RunSensorThread();
	CWinThread* pSensorThread;
	static UINT SensorThread(LPVOID Lparam);

	//������������߳�
	bool RunFrontDeTection();
	CWinThread* pFrontThread;
	bool isFrontDetect;
	static UINT FrontDeTection(LPVOID lp);

	//������������߳�
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
		DWORD DelayForGetChainBar;// r = GetTickCount();						//����һ��ŵ�������׼�ʱ����ֹʱ��̫��û�źþͽ����˶�
		bool isGettingBar;// = false;
		DWORD DelayForPutOffChainBar;// = GetTickCount();						//����һ��ŵ�������׼�ʱ����ֹʱ��̫��û�źþͽ����˶�
		bool isPuttingBar;// = false;
		DWORD DelayForPutBackChainBar;// = GetTickCount();						//����һ��ŵ�������׼�ʱ����ֹʱ��̫��û�������ͽ����˶�
		bool isBackBar;// = false;
		DWORD isMotionChange;// = 0;											//����ѭ������Ƿ��иı䶯��
		DWORD DelayForOutResult;

		bool FrontDetecting;// = false;
		bool BackDetecting;
	} Controller;
#endif

	afx_msg void OnButtonCamera();
};

extern CImageVisionApp theApp;
