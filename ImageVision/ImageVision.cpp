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

// ImageVision.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ImageVision.h"
#include "MainFrm.h"
#include "control.h"
#include "TopView.h"
#include "BottomView.h"
#include "ImageVisionDoc.h"
#include "ImageVisionView.h"
#include "Tools.h"
#include "test.h"
#include "MotionCtrl.h"
#include "SerialPort.h"
//#include "..//Classes/Gui/AcqConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageVisionApp

BEGIN_MESSAGE_MAP(CImageVisionApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CImageVisionApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_BUTTON_BACK_SIZE, &CImageVisionApp::OnButtonBackSize)
	ON_COMMAND(ID_BUTTON_LOAD_IMG, &CImageVisionApp::OnButtonLoadImg)
	ON_COMMAND(ID_BUTTON_LOAD_IMG2, &CImageVisionApp::OnButtonLoadImg2)
	ON_COMMAND(ID_BUTTON_SETTING, &CImageVisionApp::OnButtonSetting)
	ON_COMMAND(ID_BUTTON_GRAB, &CImageVisionApp::OnButtonGrab)
ON_COMMAND(ID_BUTTON_IOCARDSET, &CImageVisionApp::OnButtonIocardset)
ON_COMMAND(ID_BUTTON_CAMERA, &CImageVisionApp::OnButtonCamera)
END_MESSAGE_MAP()


// CImageVisionApp ����

CImageVisionApp::CImageVisionApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���: 
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO:  ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("ImageVision.AppID.NoVersion"));

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��

   FrontResult = 0;
   BackResult = 0;
   isFrontDetect = false;
   isBackDetect = false;

}

// Ψһ��һ�� CImageVisionApp ����

CImageVisionApp theApp;


// CImageVisionApp ��ʼ��

BOOL CImageVisionApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣  �ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CImageVisionDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CImageVisionView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ��������������ָ�������  ���
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CMainFrame *pMF = (CMainFrame *)pMain;
	//����ʱ����󻯴���
	pMF->ResizeWin(pMF);
	//���Ӵ��ľ������halcon�Ļ�ͼ��ʹ�ú�����Եõ���ͼ�ľ��
	g.RunningTick = GetTickCount();

#ifdef _DEBUG
#pragma warning(disable: 4996)
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#else
	//�ڴ���
#ifdef _TEST
#pragma warning(disable: 4996)
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif

#endif

#ifdef _OPENCV
	g.Opencv.load_train_data();
#endif
	
	m_pMainWnd->SetWindowText(L"�㽭���������ܼ������޹�˾-1761Card");
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();


	//hardware init
	g.mc.init();
	if (!RunDelayInitThread()){ AfxMessageBox(L"���������ʼ��ʧ�ܣ�"); return FALSE; }

	if (!RunMainThread()){ AfxMessageBox(L"�������߳�ʧ�ܣ�"); return FALSE; }
	
	if (!RunSensorThread()){ AfxMessageBox(L"������Ӧ���߳�ʧ�ܣ�"); return FALSE; }

	return TRUE;
}

int CImageVisionApp::ExitInstance()
{
	//TODO:  �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CImageVisionApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CImageVisionApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CImageVisionApp �Զ������/���淽��

void CImageVisionApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CImageVisionApp::LoadCustomState()
{
}

void CImageVisionApp::SaveCustomState()
{
}

// CImageVisionApp ��Ϣ�������

bool CImageVisionApp::RunDelayInitThread()
{
	pDelayInit = AfxBeginThread(DelayInit, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	if (pDelayInit)
		return true;
	return false;
}


UINT CImageVisionApp::DelayInit(LPVOID Lparam)
{
	//Sleep(800);
	CImageVisionApp *pC = (CImageVisionApp*)Lparam;
	CSerialPort SerialPort;
	int CameraCounter = 0;
	g.Controller.SysState |= SYSNEEDORIGIN;
	g.Controller.SysState |= TOPCAMERROR;
	g.Controller.SysState |= BOTTOMERROR;
	
	g.pBottomDalsa = new CamDALSA();
	g.pTopDalsa = new CamDALSA();

	vector<std::string> serverList = g.pBottomDalsa->GetDeviceSituation1();
	CString serverName, serverName1;
	for (size_t i = 0; i < serverList.size(); i++)
	{
		if (0 == i) serverName = (CString)(CStringA)serverList[0].c_str();
		else if (1 == i) serverName1 = (CString)(CStringA)serverList[i].c_str();
	}

	win_tool wt;
	CString AppPath = wt.AppPath();
	CString TopCamConfig = AppPath + L"\\" + g.ini.m_TopCamConfig;
	CString BotCamConfig = AppPath + L"\\" + g.ini.m_BottomCamConfig;

	int TopError = g.pTopDalsa->Init(serverName, TopCamConfig, 0);
	int BottomError = g.pBottomDalsa->Init(serverName1, BotCamConfig,0);
	if (TopError) { delete g.pTopDalsa; g.pTopDalsa = NULL; 
#ifdef _TEST
	cout << "Init Top Camera Error Code is:"<<TopError << endl;
#endif
	}
	else 
	{
		CameraCounter++;
		g.Controller.SysState &= ~TOPCAMERROR;
		pC->RunTopImageThread();
	}
	if (BottomError) { delete g.pBottomDalsa; g.pBottomDalsa = NULL;
#ifdef _TEST
	cout << "Init Bottom Camera Error Code is:" << BottomError << endl;
#endif
	}
	else
	{
		CameraCounter++;
		g.Controller.SysState &= ~BOTTOMERROR;
		pC->RunBottomImageThread();
	}
	if (2 > CameraCounter)
	{
		CString str;
		str.Format(_T("ֻ�ҵ�%d�ſ���"), CameraCounter);
		AfxMessageBox(str);
	}

	return 0;
}


bool CImageVisionApp::RunTopImageThread()
{
	pTopImage = AfxBeginThread(TopImageThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	if (!pTopImage)
		return false;
	return true;
	;
}


UINT CImageVisionApp::TopImageThread(LPVOID Lparam)
{
	CImageVisionApp *pC = (CImageVisionApp*)Lparam;
	while (1)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(g.evt_capture.evt, 1))
		{
			g.pTopDalsa->m_Xfer->Snap();
			
			bool GrabSuccess = true;
			DWORD Tick = GetTickCount();
			size_t width = g.pTopDalsa->m_Buffers->GetWidth();//ccf�ļ��趨�õ�
			size_t height = g.pTopDalsa->m_Buffers->GetHeight();//ccf�ļ��趨�õ�
			size_t NumberBufferResources = g.pTopDalsa->m_Buffers->GetCount(); //ֻ���趨һ��
			SapFormat iFormat = g.pTopDalsa->m_Buffers->GetFormat();
			if (SapFormatMono8 != iFormat)
			{
				AfxMessageBox(L"only support SapFormatMono8!");
				return 1;
			}
			size_t len = width * height;
			if (!g.pTopDalsa->m_Xfer->Wait(g.ini.m_TopSnapWait))
			{
				g.pTopDalsa->m_Xfer->Abort();
				GrabSuccess = false;
				AfxMessageBox(L"�������㳬ʱ��");
			}
			uchar *dataGray = new uchar[len];
			g.pTopDalsa->m_Buffers->Read(0, len, dataGray);

			if (!g.ini.isTopScale)
				gen_image1(&g.hRawImage, "byte", width, height, (Hlong)(dataGray));
			else
			{
				Halcon::Hobject Hobj;
				gen_image1(&Hobj, "byte", width, height, (Hlong)(dataGray));


				Halcon::HTuple Width, Height, Ratio, iniWidth((double)g.ini.m_topwScaleTo);
				get_image_size(Hobj, &Width, &Height);
				if (g.ini.m_TopRatio <= 0)
				{
					double w = Width[0].D();
					double h = Height[0].D();
					double Ratio = h / w;
					Height = (int)(Ratio*(double)g.ini.m_topwScaleTo);
				}
				Width = (int)g.ini.m_topwScaleTo;
				zoom_image_size(Hobj, &g.hRawImage, Width, Height, "weighted");
			}

			g.show_image.Dis_Obj(g.hRawImage);
			g.evt_capture.ResetEvent();
			delete[] dataGray;
		}
		Sleep(1);
	}
	return 0;
}

bool CImageVisionApp::RunBottomImageThread()
{
	pBottomImage = AfxBeginThread(BottomImageThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	if (!pBottomImage)
		return false;
	return true;
}

UINT CImageVisionApp::BottomImageThread(LPVOID Lparam)
{
	CImageVisionApp *pC = (CImageVisionApp*)Lparam;
	while (1)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(g.evt_capture1.evt, 1))
		{
			g.pBottomDalsa->m_Xfer->Snap();
			bool GrabSuccess = true;
			size_t width = g.pBottomDalsa->m_Buffers->GetWidth();//ccf�ļ��趨�õ�
			size_t height = g.pBottomDalsa->m_Buffers->GetHeight();//ccf�ļ��趨�õ�
			size_t NumberBufferResources = g.pBottomDalsa->m_Buffers->GetCount(); //ֻ���趨һ��
			SapFormat iFormat = g.pBottomDalsa->m_Buffers->GetFormat();
			if (SapFormatMono8 != iFormat)
			{
				AfxMessageBox(L"only support SapFormatMono8!");
				return 1;
			}
			size_t len = width * height;
			if (!g.pBottomDalsa->m_Xfer->Wait(g.ini.m_BottomWait))
			{
				g.pBottomDalsa->m_Xfer->Abort();
				GrabSuccess = false;
				AfxMessageBox(L"�ײ�������㳬ʱ��");
			}
			uchar *dataGray = new uchar[len];
			g.pBottomDalsa->m_Buffers->Read(0, len, dataGray);
		


			if (!g.ini.isBottomScale)
				gen_image1(&g.hRawImage1, "byte", width, height, (Hlong)(dataGray));
			else
			{
				Halcon::Hobject Hobj;
				gen_image1(&Hobj, "byte", width, height, (Hlong)(dataGray));
				Halcon::HTuple Width, Height, Ratio, iniWidth((double)g.ini.m_botwScaleTo);
				get_image_size(Hobj, &Width, &Height);
				if (g.ini.m_TopRatio <= 0)
				{
					double w = Width[0].D();
					double h = Height[0].D();
					double Ratio = h / w;
					Height = (int)(Ratio*(double)g.ini.m_botwScaleTo);
				}
				Width = (int)g.ini.m_topwScaleTo;
				zoom_image_size(Hobj, &g.hRawImage1, Width, Height, "weighted");
			}
			g.show_image.Dis_Obj1(g.hRawImage1);
			g.evt_capture1.ResetEvent();
			delete[] dataGray;
		}
		Sleep(1);
	}
	return 0;
}





void CImageVisionApp::OnButtonBackSize()
{
	// TODO:  �ڴ���������������
	CMainFrame *pMF = (CMainFrame*)pMain;
	pMF->ResizeWin(pMF);
}


void CImageVisionApp::OnButtonLoadImg()
{
	// TODO:  �ڴ���������������
	CFileDialog fileDlg(TRUE, (LPCTSTR)"BMP", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"All Files (*.*)|*.*|λͼ�ļ� (*.bmp)|*.bmp||");	//�򿪶�ȡ�ļ��Ի���
	if (fileDlg.DoModal() != IDOK) return;
	read_image(&g.hRawImage,(char*)(LPCSTR)CStringA(fileDlg.GetPathName()));
	g.show_image.Dis_Obj(g.hRawImage);
}


void CImageVisionApp::OnButtonLoadImg2()
{
	// TODO:  �ڴ���������������
	CFileDialog fileDlg(TRUE, (LPCTSTR)"BMP", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"All Files (*.*)|*.*|λͼ�ļ� (*.bmp)|*.bmp||");	//�򿪶�ȡ�ļ��Ի���
	if (fileDlg.DoModal() != IDOK) return;
	read_image(&g.hRawImage1, (char*)(LPCSTR)CStringA(fileDlg.GetPathName()));
	g.show_image.Dis_Obj1(g.hRawImage1);
}


void CImageVisionApp::OnButtonSetting()
{
	// TODO:  �ڴ���������������
	test t;
	t.DoModal();
}


void CImageVisionApp::OnButtonGrab()
{
	// TODO:  �ڴ���������������
	if (g.pTopDalsa)
	{
		if (g.pTopDalsa->m_Xfer->Wait(g.ini.m_TopSnapWait))
		{
			AfxMessageBox(L"�ɼ���");
			return;
		}
		else
		{
			g.evt_capture.SetEvent();
		}

	}

}



//bool isRunTrain;
//bool isCap;


void CImageVisionApp::OnButtonIocardset()
{
	// TODO:  �ڴ���������������
	CMotionCtrl dlg;
	dlg.DoModal();
}






bool CImageVisionApp::RunMainThread()
{
	pMainThread = AfxBeginThread(MainThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	if (!pMainThread)
		return false;
	return true;
}







bool CImageVisionApp::RunSensorThread()
{
	pSensorThread = AfxBeginThread(SensorThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	if (!pSensorThread)
		return false;
	return true;

}

#ifdef _0HALCON
Halcon::clear_window(g.show_image.get_disp_hd());
Halcon::set_color(g.show_image.get_disp_hd(), "red");
//Halcon::write_string(g.show_image.get_disp_hd(), (char*)(LPCSTR)(CStringA)g.ErrorString(state));
#endif

UINT CImageVisionApp::SensorThread(LPVOID Lparam)
{
	CImageVisionApp *pApp = (CImageVisionApp*)Lparam;
	for (;;)
	{
		//����
		if (!g.mc.Read_Input(IN_EMGENCY)) { int state = SYSEMGENCY | SYSNEEDORIGIN; g.Controller.SysState |= state; g.evtEmgencyStop.SetEvent(); g.mc.Stop(FIRST_MOTOR); g.mc.Stop(SECOND_MOTOR); g.evt_procedure.ResetEvent(); }
		else{ g.Controller.SysState &= ~SYSEMGENCY; }
//ע����Ե�ʱ��Ҫ���������״̬
		if(!g.mc.Read_Input(IN_ALM1)){g.Controller.SysState |= FST_MOTOR_NOT_ALREADY;}
		else {g.Controller.SysState &= ~FST_MOTOR_NOT_ALREADY;}

		if(!g.mc.Read_Input(IN_ALM2)){g.Controller.SysState |= SND_MOTOR_NOT_ALREADY;}
		else {g.Controller.SysState &= ~SND_MOTOR_NOT_ALREADY;}

	//	if (g.mc.Read_Input(IN_PRESSURE)){ g.Controller.SysState |= SYSPRESSURE; }
	//	else { g.Controller.SysState &= ~SYSPRESSURE; }

#if 0
		if (g.mc.Triger())
		{
			int state = SYSRUNNING | SYSSTOP;
			state = ~state & g.Controller.SysState;
			if (!state) 
				g.evt_procedure.SetEvent();
			else
				AfxMessageBox(g.ErrorString(state));
		}
#endif
		Sleep(8);
	}
	return 0;
}


bool CImageVisionApp::RunFrontDeTection()
{
	if (isFrontDetect) return false;
	pFrontThread = AfxBeginThread(FrontDeTection, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	return true;
}

UINT CImageVisionApp::FrontDeTection(LPVOID lp)
{
	CImageVisionApp * pApp = (CImageVisionApp*)lp;
	pApp->isFrontDetect = true;
	g.mc.MoveToDes(FIRST_MOTOR, g.ini.m_FstMtr.InitSpeed, g.ini.m_FstMtr.MaxSpeed, g.ini.m_FstMtr.Tacc, g.ini.m_FstMtr.Waves);
	Sleep(100);//���ŵ������
	g.evt_capture.SetEvent();								//
	::WaitForSingleObject(g.evt_capture.evt, INFINITE);		//�ȴ�����������ͼƬ
	_Params p;
	p.threshold_get_prod = g.ini.top_threshold;
	p.MatchScore = g.ini.top_match_score;
	p.TrainScore = g.ini.top_train_score;
	p.ROI_row1 = g.ini.TROI_y;
	p.ROI_col1 = g.ini.TROI_x;
	p.ROI_row2 = g.ini.TROI_h;
	p.ROI_col2 = g.ini.TROI_w;
	//g.img_deal.Front_Deal(g.hRawImage, g.show_image.get_disp_hd(), p);
	ProductInfo[g.frontNo%MAX_PRODUCT].fdata = g.img_deal.Front_Deal(g.hRawImage, g.show_image.get_disp_hd(), p);
	//pApp->FrontResult = g.img_deal.Back_Deal(g.hRawImage); //����ŷ�
	pApp->isFrontDetect = false;
	return 0;
}


bool CImageVisionApp::RunBackDeTection()
{
	if (isBackDetect) return false;
	pBackThread = AfxBeginThread(BackDeTection, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	return true;
}

UINT CImageVisionApp::BackDeTection(LPVOID lp)
{
	CImageVisionApp * pApp = (CImageVisionApp*)lp;
	pApp->isBackDetect = true;
	g.mc.MoveToDes(SECOND_MOTOR, g.ini.m_SndMtr.InitSpeed, g.ini.m_SndMtr.MaxSpeed, g.ini.m_SndMtr.Tacc, g.ini.m_SndMtr.Waves);
	Sleep(100);
	g.evt_capture1.SetEvent();								//
	::WaitForSingleObject(g.evt_capture1.evt, INFINITE);		//�ȴ�����������ͼƬ
	_Params p;
	p.threshold_get_prod = g.ini.bottom_threshold;
	p.MatchScore = g.ini.bottom_match_score;
	p.TrainScore = g.ini.bottom_train_score;
	p.ROI_row1 = g.ini.BROI_y;
	p.ROI_col1 = g.ini.BROI_x;
	p.ROI_row2 = g.ini.BROI_h;
	p.ROI_col2 = g.ini.BROI_w;
	ProductInfo[g.backNo%MAX_PRODUCT].bdata = g.img_deal.Back_Deal(g.hRawImage, g.show_image.get_disp_hd1(), p);
	//pApp->BackResult = g.img_deal.Front_Deal(g.hRawImage1);
	pApp->isBackDetect = false;
	return 0;
}


void CALLBACK CImageVisionApp::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvetn, DWORD dwTime)
{

	return;

}

void CImageVisionApp::InitData()
{
#if 0
	Controller.FrontDeTectionTicks = GetTickCount();
	Controller.DelayForGetChainBar = GetTickCount();						//����һ��ŵ�������׼�ʱ����ֹʱ��̫��û�źþͽ����˶�
	Controller.isGettingBar = false;
	Controller.DelayForPutOffChainBar = GetTickCount();						//����һ��ŵ�������׼�ʱ����ֹʱ��̫��û�źþͽ����˶�
	Controller.isPuttingBar = false;
	Controller.DelayForPutBackChainBar = GetTickCount();						//����һ��ŵ�������׼�ʱ����ֹʱ��̫��û�������ͽ����˶�
	Controller.isBackBar = false;
	Controller.isMotionChange = 0;											//����ѭ������Ƿ��иı䶯��
	Controller.DelayForOutResult = GetTickCount();
	Controller.FrontDetecting = false;						//�������ڼ��
	Controller.BackDetecting = false;
#endif
}


std::vector<int> CImageVisionApp::GetSysStatus()
{
	//�����о��ã�����ǳ��ѧϰ
	std::vector<int> status;
	status.push_back(g.mc.Read_Input(IN_START));
	status.push_back(g.mc.Read_Input(IN_FIRST_DETECT_PRODUCT));
	status.push_back(g.mc.Read_Input(IN_SECOND_DETECT_PRODUCT));
	status.push_back(g.mc.Read_Input(IN_EMGENCY));
	status.push_back(g.mc.Read_Input(IN_ALM1));                                                                                                             
	status.push_back(g.mc.Read_Input(IN_ALM2));
	status.push_back(g.mc.Read_Input(IN_FIRST_DETECT_PRODUCT));
#if 0
#define IN_START					10			//������ť�忨��ʶ18
#define IN_FIRST_DETECT_PRODUCT		11			//��һ��λ������� 19
#define IN_SECOND_DETECT_PRODUCT	12			//�ڶ���λ������ϰ忨��ʶ32
#define IN_EMGENCY					13			//��ͣ��ť �忨��ʾ33
#define IN_ALM1						14			//�������1 �忨��ʶ34
#define IN_ALM2						15			//�������2 �忨��ʶ35
#endif
}

/*

int CImageVisionApp::GetAxis1Status()//��ȡ��е��ǰ״̬
{
	if (g.mc.Read_Input(IN_FIRST_DETECT_PRODUCT) &&
		g.mc.Get_Position(FIRST_MOTOR) == 0 &&
		g.mc.Triger() &&
		g.mc.Read_Input(IN_PRESSURE) &&
		!isFrontDetect &&
		!g.mc.Controller.FHavingBar
		
		)
		return 1;

	if (!isFrontDetect &&
		!g.mc.isRunning(FIRST_MOTOR) && 
		g.mc.Get_Position(FIRST_MOTOR) == 0 && 
		!g.mc.Controller.FHavingBar && //��ʾ����ץbar��
		g.mc.Read_Input(IN_PRESSURE) && 
		g.mc.Read_Input(IN_CYL_DOWN))												//״̬2������һ������⵽λ�ã�������������
	{
		return 2;
	}

	//��⵽�Ѿ�׼�����������׵�״̬������ͨ����
	if (!g.mc.isRunning(FIRST_MOTOR) &&																			//��⵽λ����ȷ��ע�������״̬3
		g.mc.Get_Position(FIRST_MOTOR) == 0 &&									//�������ܱ���ֵ
		g.mc.Read_Input(IN_CYL_DOWN) &&
		!g.mc.Read_Input(IN_PRESSURE) &&
		!g.mc.Controller.FHavingBar
		)
		return 3;

	//�ص������������������
	if (!g.mc.isRunning(FIRST_MOTOR) &&											//��⵽λ�ò���ȷ��ע�������״̬3������״̬�в���������״̬
		g.mc.Get_Position(FIRST_MOTOR) == 0 &&									//�������ܱ���ֵ
		g.mc.Read_Input(IN_CYL_UP) &&
		!g.mc.Read_Input(IN_PRESSURE) &&
		!isFrontDetect &&
		g.mc.Controller.FHavingBar
		)
		return 4;

	//���й����쳣
	if (!g.mc.isRunning(FIRST_MOTOR) &&											//��⵽λ�ò���ȷ��ע�������״̬3������״̬�в���������״̬
		g.mc.Get_Position(FIRST_MOTOR) != g.ini.m_FstMtr.Waves &&									//�������ܱ���ֵ
		g.mc.Read_Input(IN_CYL_UP) &&
		g.mc.Read_Input(IN_PRESSURE) &&
		//!isFrontDetect &&
		g.mc.Controller.FHavingBar
		)
		return 5;

	return 0;
}
int CImageVisionApp::GetAxis2Status()//��ȡ��е��ǰ״̬
{

}

*/

int CImageVisionApp::Procedure()
{
	static long FrontStep = 0;//��ʾ�������еĲ�������
	static long BackStep = 0;//��ʾ�������еĲ�������
	const long fTStep = 6;//��������һ���Ĳ���
	const long bTStep = 6;
	//Ӱ�����е�����
	//int SysState = SYSNEEDORIGIN | SYSEMGENCY | FST_MOTOR_NOT_ALREADY | SND_MOTOR_NOT_ALREADY | SYSTRAINING | SYSTOPTRAIN | SYSBOTTOMTRAIN;
	int SysState = SYSNEEDORIGIN | SYSEMGENCY | TOPCAMERROR | BOTTOMERROR | FST_MOTOR_NOT_ALREADY | SND_MOTOR_NOT_ALREADY | SYSTRAINING | SYSTOPTRAIN | SYSBOTTOMTRAIN;
	SysState = SysState & g.Controller.SysState;
	if (SysState) return SysState;

	g.Controller.SysState |= SYSRUNNING;										//���ϵͳ��������֮��


	if (g.mc.Read_Input(IN_FIRST_DETECT_PRODUCT) &&
		g.mc.Get_Position(FIRST_MOTOR) == 0 &&
		//g.mc.Triger() &&
		g.mc.Read_Input(IN_START) &&
		g.mc.Read_Input(IN_PRESSURE) &&
		!isFrontDetect &&
		!g.mc.Controller.FHavingBar
		)
	{
		g.mc.Write_Output(OUT_CYL, ON); //�½�����
		FrontStep = 1;
	}

	if (!isFrontDetect &&
		!g.mc.isRunning(FIRST_MOTOR) &&
		g.mc.Get_Position(FIRST_MOTOR) == 0 &&
		!g.mc.Controller.FHavingBar && //��ʾ����ץbar��
		g.mc.Read_Input(IN_PRESSURE) &&
		g.mc.Read_Input(IN_CYL_DOWN)
		)//״̬2������һ������⵽λ�ã�������������
	{
		if (1 == FrontStep) 		g.mc.Controller.DelayForGetChainBar = GetTickCount(); //����Ҫ��ʱ����Ϊ�������޸�Ӧ����Ӧ���϶�
		FrontStep = 2;			//�����?
		if (2 == FrontStep
			&&((GetTickCount() - g.mc.Controller.DelayForGetChainBar)) > 10000)
		{
			g.Controller.SysState |= SYSNEEDORIGIN;
			g.mc.Write_Output(OUT_PRESSURE, OFF);
			g.mc.Write_Output(OUT_CYL, OFF); //�½�����
			AfxMessageBox(L"��ʱ���,�Ƿ������������Ʒ�ڷŲ���ȷ?");
			goto endthefun;
		}
		g.mc.Write_Output(OUT_PRESSURE, ON);
	}

	if (!g.mc.isRunning(FIRST_MOTOR) &&																			//��⵽λ����ȷ��ע�������״̬3
		g.mc.Get_Position(FIRST_MOTOR) == 0 &&									//�������ܱ���ֵ
		g.mc.Read_Input(IN_CYL_DOWN) &&
		!g.mc.Read_Input(IN_PRESSURE) &&
		!g.mc.Controller.FHavingBar
		)
	{
		if (2 == FrontStep) 		g.mc.Controller.DelayForGetChainBar = GetTickCount(); //����Ҫ��ʱ����Ϊ�������޸�Ӧ����Ӧ���϶�
		FrontStep = 3;
		g.mc.Controller.FHavingBar = true;
		g.mc.Write_Output(OUT_CYL, OFF);
	}

	//�ص������������������
	if (!g.mc.isRunning(FIRST_MOTOR) &&											//��⵽λ�ò���ȷ��ע�������״̬3������״̬�в���������״̬
		g.mc.Get_Position(FIRST_MOTOR) == 0 &&									//�������ܱ���ֵ
		g.mc.Read_Input(IN_CYL_UP) &&
		  !g.mc.Read_Input(IN_CYL_DOWN)
		&& !g.mc.Read_Input(IN_PRESSURE) &&
		!isFrontDetect &&
		g.mc.Controller.FHavingBar
		)
	{
		FrontStep = 4;
		if (RunFrontDeTection())
		{
			g.FrontDetectNo++;						//��ƷID����
			g.mc.Controller.isMotionChange++;
		}
	}

	if (!g.mc.isRunning(FIRST_MOTOR) &&											//��⵽λ�ò���ȷ��ע�������״̬3������״̬�в���������״̬
		g.mc.Get_Position(FIRST_MOTOR) != g.ini.m_FstMtr.Waves &&									//�������ܱ���ֵ
		g.mc.Read_Input(IN_CYL_UP) &&
		g.mc.Read_Input(IN_PRESSURE) &&
		g.mc.Controller.FHavingBar
		)
	{
		FrontStep = 5;
		g.Controller.SysState |= SYSNEEDORIGIN;									//��⵽��ѹ����ȷ
		AfxMessageBox(L"������Ʒ,�븴λ");
		goto endthefun;
	}

	
	//׼���ý���״̬
	if (!g.mc.isRunning(FIRST_MOTOR) &&											//��һ������ƶ����ڶ�������Ϸ�λ�õ�ʱ���⵽λ����ȷ��ʱ��Ϳ�ʼ��������
		g.mc.Get_Position(FIRST_MOTOR) == g.ini.m_FstMtr.Waves  &&
		!g.mc.Read_Input(IN_PRESSURE) &&
		g.mc.Read_Input(IN_CYL_UP) &&
		g.mc.Controller.FHavingBar
		)
	{
		if (5 == FrontStep) g.mc.Controller.DelayForGetChainBar = GetTickCount();
		FrontStep = 6;
		if (6 == FrontStep
			&& ((GetTickCount() - g.mc.Controller.DelayForGetChainBar)) > 10000)
		{
			g.Controller.SysState |= SYSNEEDORIGIN;
			AfxMessageBox(L"�ȴ��������ԭ�㳬ʱ!");
			goto endthefun;
		}
	}
	//׼���ý���״̬
	if (!g.mc.isRunning(FIRST_MOTOR) 											//��һ������ƶ����ڶ�������Ϸ�λ�õ�ʱ���⵽λ����ȷ��ʱ��Ϳ�ʼ��������
		&& g.mc.Get_Position(FIRST_MOTOR) == g.ini.m_FstMtr.Waves
		&& !g.mc.Read_Input(IN_PRESSURE)
		&& g.mc.Read_Input(IN_CYL_UP)
		&& 0 == g.mc.Get_Position(SECOND_MOTOR)
		&& !g.mc.isRunning(SECOND_MOTOR)
		&& !g.mc.Read_Input(IN_SECOND_DETECT_PRODUCT)
		&& g.mc.Controller.FHavingBar
		&& !g.mc.Controller.BHavingBar
		)
	{
		if (6 == FrontStep) g.mc.Controller.DelayForPutOffChainBar = GetTickCount();
		FrontStep = 7;
		BackStep = 0;
		g.mc.Write_Output(OUT_CYL, ON);
	}

	//
	if (!g.mc.isRunning(FIRST_MOTOR) &&											//��⵽λ����ȷ��ʱ��Ϳ�ʼ�������ϣ�ע���жϲ����ǵ��ֹͣ�ˣ����������Ѿ��ź�����
		g.mc.Get_Position(FIRST_MOTOR) == g.ini.m_FstMtr.Waves &&
		!g.mc.Read_Input(IN_PRESSURE) &&
		g.mc.Read_Input(IN_CYL_DOWN) &&
		0 == g.mc.Get_Position(SECOND_MOTOR) &&
		!g.mc.isRunning(SECOND_MOTOR)
		&& g.mc.Controller.FHavingBar
		&& !g.mc.Controller.BHavingBar
		)
	{
		if (8 == FrontStep) g.mc.Controller.DelayForPutOffChainBar = GetTickCount();
		FrontStep = 9;
		BackStep = 1;
		g.mc.Write_Output(OUT_PRESSURE, OFF);
	}

	//�ź����ˣ�Ӧ��������
	if (!g.mc.isRunning(FIRST_MOTOR) &&											//��⵽λ����ȷ��ʱ��Ϳ�ʼ�������ϣ�ע���жϲ����ǵ��ֹͣ�ˣ����������Ѿ��ź�����
		g.mc.Get_Position(FIRST_MOTOR) == g.ini.m_FstMtr.Waves &&
		g.mc.Read_Input(IN_PRESSURE) &&
		g.mc.Read_Input(IN_CYL_DOWN) &&
		0 == g.mc.Get_Position(SECOND_MOTOR) &&
		!g.mc.isRunning(SECOND_MOTOR) //&&
		&& g.mc.Controller.FHavingBar
		)
	{
		BackStep = 2;
		g.mc.Write_Output(OUT_CYL, OFF);
		g.mc.Controller.FHavingBar = false;
		g.mc.Controller.BHavingBar = true;
		g.backNo = g.frontNo;//���ݱ��
	}
	//�ź������Ѿ���������
	if (!g.mc.isRunning(FIRST_MOTOR) &&											//��⵽λ����ȷ��ʱ��Ϳ�ʼ�������ϣ�ע���жϲ����ǵ��ֹͣ�ˣ����������Ѿ��ź�����
		g.mc.Get_Position(FIRST_MOTOR) == g.ini.m_FstMtr.Waves &&
		g.mc.Read_Input(IN_PRESSURE) &&
		g.mc.Read_Input(IN_CYL_UP) &&
		0 == g.mc.Get_Position(SECOND_MOTOR) &&
		!g.mc.isRunning(SECOND_MOTOR) //&&
		&& g.mc.Controller.BHavingBar //ǰһ���Ƿ��е���Ҳ����ν��
		)
	{
		BackStep = 3;
		g.mc.MoveToOrigin(FIRST_MOTOR,g.ini.m_FstMtr.MaxSpeed,g.ini.m_FstMtr.Tacc);
	}

	//�ź������Ѿ���������
	if (!g.mc.isRunning(SECOND_MOTOR) &&											//��⵽λ����ȷ��ʱ��Ϳ�ʼ�������ϣ�ע���жϲ����ǵ��ֹͣ�ˣ����������Ѿ��ź�����
		//g.mc.Read_Input(IN_PRESSURE) &&
		g.mc.Read_Input(IN_CYL_UP) &&
		0 == g.mc.Get_Position(SECOND_MOTOR) &&
		g.mc.Controller.BHavingBar
		&& !isBackDetect
		)
	{
		BackStep = 4;
		if (RunBackDeTection())
		{
			g.mc.Controller.isMotionChange++;
			g.mc.Controller.DelayForGetChainBar = GetTickCount();
			BackStep = 4;
		}
	}


	if (!g.mc.isRunning(SECOND_MOTOR) &&											//��⵽λ����ȷ��ʱ��Ϳ�ʼ�������ϣ�ע���жϲ����ǵ��ֹͣ�ˣ����������Ѿ��ź�����
		//g.mc.Read_Input(IN_PRESSURE) &&
		//g.mc.Read_Input(IN_CYL_UP) &&
		g.mc.Get_Position(SECOND_MOTOR) == g.ini.m_SndMtr.Waves &&
		g.mc.Controller.BHavingBar
		&& !isBackDetect
		//&& 4 == BackStep
		)
	{
		if (4 == BackStep) g.mc.Controller.DelayForGetChainBar = GetTickCount();  //��ʱ����
		BackStep = 5;
		if ((1 == ProductInfo[g.backNo%MAX_PRODUCT].bdata) && (1 == ProductInfo[g.backNo%MAX_PRODUCT].fdata)) g.mc.SysOutOK();
		else g.mc.SysOutNG();
		if (5 == BackStep && ((GetTickCount() - g.mc.Controller.DelayForGetChainBar) > 2000))   
		{
			g.mc.MoveToOrigin(SECOND_MOTOR, g.ini.m_SndMtr.MaxSpeed, g.ini.m_SndMtr.Tacc);
			g.mc.Controller.BHavingBar = false;
			BackStep = 0;
			g.mc.SysOutVoid();
		}
	}

endthefun:
	g.Controller.SysState &= ~SYSRUNNING;							//ȡ�����б��
	return g.Controller.SysState;
}



UINT CImageVisionApp::MainThread(LPVOID Lparam)
{

	CImageVisionApp *pApp = (CImageVisionApp*)Lparam;
	while (1)
	{
		if (WAIT_OBJECT_0 == ::WaitForSingleObject(g.evt_procedure.evt, 0))
		{
			int res = pApp->Procedure();
			if (res)
			{
				g.evt_procedure.ResetEvent();
				AfxMessageBox(g.ErrorString(res));
			}
		}
		Sleep(2);
	}
	return 0;
}


void CImageVisionApp::OnButtonCamera()
{
	// TODO:  �ڴ���������������
	AfxMessageBox(L"OK");
}
