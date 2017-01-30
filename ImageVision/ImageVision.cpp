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

// ImageVision.cpp : 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
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


// CImageVisionApp 构造

CImageVisionApp::CImageVisionApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则: 
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO:  将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("ImageVision.AppID.NoVersion"));

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中

   FrontResult = 0;
   BackResult = 0;
   isFrontDetect = false;
   isBackDetect = false;

}

// 唯一的一个 CImageVisionApp 对象

CImageVisionApp theApp;


// CImageVisionApp 初始化

BOOL CImageVisionApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO:  应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。  文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CImageVisionDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CImageVisionView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 调度在命令行中指定的命令。  如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CMainFrame *pMF = (CMainFrame *)pMain;
	//启动时候最大化窗口
	pMF->ResizeWin(pMF);
	//将视窗的句柄传给halcon的绘图，使得后面可以得到绘图的句柄
	g.RunningTick = GetTickCount();

#ifdef _DEBUG
#pragma warning(disable: 4996)
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#else
	//黑窗口
#ifdef _TEST
#pragma warning(disable: 4996)
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif

#endif

#ifdef _OPENCV
	g.Opencv.load_train_data();
#endif
	
	m_pMainWnd->SetWindowText(L"浙江爱易特智能技术有限公司-1761Card");
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();


	//hardware init
	g.mc.init();
	if (!RunDelayInitThread()){ AfxMessageBox(L"启动相机初始化失败！"); return FALSE; }

	if (!RunMainThread()){ AfxMessageBox(L"启动主线程失败！"); return FALSE; }
	
	if (!RunSensorThread()){ AfxMessageBox(L"启动感应器线程失败！"); return FALSE; }

	return TRUE;
}

int CImageVisionApp::ExitInstance()
{
	//TODO:  处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CImageVisionApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CImageVisionApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CImageVisionApp 自定义加载/保存方法

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

// CImageVisionApp 消息处理程序

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
		str.Format(_T("只找到%d张卡！"), CameraCounter);
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
			size_t width = g.pTopDalsa->m_Buffers->GetWidth();//ccf文件设定好的
			size_t height = g.pTopDalsa->m_Buffers->GetHeight();//ccf文件设定好的
			size_t NumberBufferResources = g.pTopDalsa->m_Buffers->GetCount(); //只能设定一个
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
				AfxMessageBox(L"顶部拍摄超时！");
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
			size_t width = g.pBottomDalsa->m_Buffers->GetWidth();//ccf文件设定好的
			size_t height = g.pBottomDalsa->m_Buffers->GetHeight();//ccf文件设定好的
			size_t NumberBufferResources = g.pBottomDalsa->m_Buffers->GetCount(); //只能设定一个
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
				AfxMessageBox(L"底部相机拍摄超时！");
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
	// TODO:  在此添加命令处理程序代码
	CMainFrame *pMF = (CMainFrame*)pMain;
	pMF->ResizeWin(pMF);
}


void CImageVisionApp::OnButtonLoadImg()
{
	// TODO:  在此添加命令处理程序代码
	CFileDialog fileDlg(TRUE, (LPCTSTR)"BMP", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"All Files (*.*)|*.*|位图文件 (*.bmp)|*.bmp||");	//打开读取文件对话框
	if (fileDlg.DoModal() != IDOK) return;
	read_image(&g.hRawImage,(char*)(LPCSTR)CStringA(fileDlg.GetPathName()));
	g.show_image.Dis_Obj(g.hRawImage);
}


void CImageVisionApp::OnButtonLoadImg2()
{
	// TODO:  在此添加命令处理程序代码
	CFileDialog fileDlg(TRUE, (LPCTSTR)"BMP", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"All Files (*.*)|*.*|位图文件 (*.bmp)|*.bmp||");	//打开读取文件对话框
	if (fileDlg.DoModal() != IDOK) return;
	read_image(&g.hRawImage1, (char*)(LPCSTR)CStringA(fileDlg.GetPathName()));
	g.show_image.Dis_Obj1(g.hRawImage1);
}


void CImageVisionApp::OnButtonSetting()
{
	// TODO:  在此添加命令处理程序代码
	test t;
	t.DoModal();
}


void CImageVisionApp::OnButtonGrab()
{
	// TODO:  在此添加命令处理程序代码
	if (g.pTopDalsa)
	{
		if (g.pTopDalsa->m_Xfer->Wait(g.ini.m_TopSnapWait))
		{
			AfxMessageBox(L"采集中");
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
	// TODO:  在此添加命令处理程序代码
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
		//常闭
		if (!g.mc.Read_Input(IN_EMGENCY)) { int state = SYSEMGENCY | SYSNEEDORIGIN; g.Controller.SysState |= state; g.evtEmgencyStop.SetEvent(); g.mc.Stop(FIRST_MOTOR); g.mc.Stop(SECOND_MOTOR); g.evt_procedure.ResetEvent(); }
		else{ g.Controller.SysState &= ~SYSEMGENCY; }
//注意调试的时候要检查的输入点状态
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
	Sleep(100);//等着电机匀速
	g.evt_capture.SetEvent();								//
	::WaitForSingleObject(g.evt_capture.evt, INFINITE);		//等待相机拍完成摄图片
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
	//pApp->FrontResult = g.img_deal.Back_Deal(g.hRawImage); //如果放反
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
	::WaitForSingleObject(g.evt_capture1.evt, INFINITE);		//等待相机拍完成摄图片
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
	Controller.DelayForGetChainBar = GetTickCount();						//给第一块放导板的气缸计时，防止时间太短没放好就进行运动
	Controller.isGettingBar = false;
	Controller.DelayForPutOffChainBar = GetTickCount();						//给第一块放导板的气缸计时，防止时间太短没放好就进行运动
	Controller.isPuttingBar = false;
	Controller.DelayForPutBackChainBar = GetTickCount();						//给第一块放导板的气缸计时，防止时间太短没缩回来就进行运动
	Controller.isBackBar = false;
	Controller.isMotionChange = 0;											//用于循环检测是否有改变动作
	Controller.DelayForOutResult = GetTickCount();
	Controller.FrontDetecting = false;						//正面正在检测
	Controller.BackDetecting = false;
#endif
}


std::vector<int> CImageVisionApp::GetSysStatus()
{
	//代码研究用，用于浅层学习
	std::vector<int> status;
	status.push_back(g.mc.Read_Input(IN_START));
	status.push_back(g.mc.Read_Input(IN_FIRST_DETECT_PRODUCT));
	status.push_back(g.mc.Read_Input(IN_SECOND_DETECT_PRODUCT));
	status.push_back(g.mc.Read_Input(IN_EMGENCY));
	status.push_back(g.mc.Read_Input(IN_ALM1));                                                                                                             
	status.push_back(g.mc.Read_Input(IN_ALM2));
	status.push_back(g.mc.Read_Input(IN_FIRST_DETECT_PRODUCT));
#if 0
#define IN_START					10			//启动按钮板卡标识18
#define IN_FIRST_DETECT_PRODUCT		11			//第一工位检测物料 19
#define IN_SECOND_DETECT_PRODUCT	12			//第二工位检测物料板卡标识32
#define IN_EMGENCY					13			//急停按钮 板卡表示33
#define IN_ALM1						14			//电机报警1 板卡标识34
#define IN_ALM2						15			//电机报警2 板卡标识35
#endif
}

/*

int CImageVisionApp::GetAxis1Status()//获取机械当前状态
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
		!g.mc.Controller.FHavingBar && //表示正在抓bar中
		g.mc.Read_Input(IN_PRESSURE) && 
		g.mc.Read_Input(IN_CYL_DOWN))												//状态2，当第一个检测检测到位置，并捡起来导板
	{
		return 2;
	}

	//检测到已经准备好上升气缸的状态，并且通气了
	if (!g.mc.isRunning(FIRST_MOTOR) &&																			//检测到位置正确，注意参数，状态3
		g.mc.Get_Position(FIRST_MOTOR) == 0 &&									//函数不能被赋值
		g.mc.Read_Input(IN_CYL_DOWN) &&
		!g.mc.Read_Input(IN_PRESSURE) &&
		!g.mc.Controller.FHavingBar
		)
		return 3;

	//回到上升点条件正常情况
	if (!g.mc.isRunning(FIRST_MOTOR) &&											//检测到位置不正确，注意参数，状态3，运行状态中不存在这种状态
		g.mc.Get_Position(FIRST_MOTOR) == 0 &&									//函数不能被赋值
		g.mc.Read_Input(IN_CYL_UP) &&
		!g.mc.Read_Input(IN_PRESSURE) &&
		!isFrontDetect &&
		g.mc.Controller.FHavingBar
		)
		return 4;

	//运行过程异常
	if (!g.mc.isRunning(FIRST_MOTOR) &&											//检测到位置不正确，注意参数，状态3，运行状态中不存在这种状态
		g.mc.Get_Position(FIRST_MOTOR) != g.ini.m_FstMtr.Waves &&									//函数不能被赋值
		g.mc.Read_Input(IN_CYL_UP) &&
		g.mc.Read_Input(IN_PRESSURE) &&
		//!isFrontDetect &&
		g.mc.Controller.FHavingBar
		)
		return 5;

	return 0;
}
int CImageVisionApp::GetAxis2Status()//获取机械当前状态
{

}

*/

int CImageVisionApp::Procedure()
{
	static long FrontStep = 0;//表示正面运行的步骤索引
	static long BackStep = 0;//表示背面运行的步骤索引
	const long fTStep = 6;//正面运行一共的步骤
	const long bTStep = 6;
	//影响运行的因素
	//int SysState = SYSNEEDORIGIN | SYSEMGENCY | FST_MOTOR_NOT_ALREADY | SND_MOTOR_NOT_ALREADY | SYSTRAINING | SYSTOPTRAIN | SYSBOTTOMTRAIN;
	int SysState = SYSNEEDORIGIN | SYSEMGENCY | TOPCAMERROR | BOTTOMERROR | FST_MOTOR_NOT_ALREADY | SND_MOTOR_NOT_ALREADY | SYSTRAINING | SYSTOPTRAIN | SYSBOTTOMTRAIN;
	SysState = SysState & g.Controller.SysState;
	if (SysState) return SysState;

	g.Controller.SysState |= SYSRUNNING;										//标记系统正在运行之中


	if (g.mc.Read_Input(IN_FIRST_DETECT_PRODUCT) &&
		g.mc.Get_Position(FIRST_MOTOR) == 0 &&
		//g.mc.Triger() &&
		g.mc.Read_Input(IN_START) &&
		g.mc.Read_Input(IN_PRESSURE) &&
		!isFrontDetect &&
		!g.mc.Controller.FHavingBar
		)
	{
		g.mc.Write_Output(OUT_CYL, ON); //下降气缸
		FrontStep = 1;
	}

	if (!isFrontDetect &&
		!g.mc.isRunning(FIRST_MOTOR) &&
		g.mc.Get_Position(FIRST_MOTOR) == 0 &&
		!g.mc.Controller.FHavingBar && //表示正在抓bar中
		g.mc.Read_Input(IN_PRESSURE) &&
		g.mc.Read_Input(IN_CYL_DOWN)
		)//状态2，当第一个检测检测到位置，并捡起来导板
	{
		if (1 == FrontStep) 		g.mc.Controller.DelayForGetChainBar = GetTickCount(); //不需要延时，因为气缸上限感应器感应到肯定
		FrontStep = 2;			//激活函数?
		if (2 == FrontStep
			&&((GetTickCount() - g.mc.Controller.DelayForGetChainBar)) > 10000)
		{
			g.Controller.SysState |= SYSNEEDORIGIN;
			g.mc.Write_Output(OUT_PRESSURE, OFF);
			g.mc.Write_Output(OUT_CYL, OFF); //下降气缸
			AfxMessageBox(L"超时检测,是否气不足或者物品摆放不正确?");
			goto endthefun;
		}
		g.mc.Write_Output(OUT_PRESSURE, ON);
	}

	if (!g.mc.isRunning(FIRST_MOTOR) &&																			//检测到位置正确，注意参数，状态3
		g.mc.Get_Position(FIRST_MOTOR) == 0 &&									//函数不能被赋值
		g.mc.Read_Input(IN_CYL_DOWN) &&
		!g.mc.Read_Input(IN_PRESSURE) &&
		!g.mc.Controller.FHavingBar
		)
	{
		if (2 == FrontStep) 		g.mc.Controller.DelayForGetChainBar = GetTickCount(); //不需要延时，因为气缸上限感应器感应到肯定
		FrontStep = 3;
		g.mc.Controller.FHavingBar = true;
		g.mc.Write_Output(OUT_CYL, OFF);
	}

	//回到上升点条件正常情况
	if (!g.mc.isRunning(FIRST_MOTOR) &&											//检测到位置不正确，注意参数，状态3，运行状态中不存在这种状态
		g.mc.Get_Position(FIRST_MOTOR) == 0 &&									//函数不能被赋值
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
			g.FrontDetectNo++;						//样品ID索引
			g.mc.Controller.isMotionChange++;
		}
	}

	if (!g.mc.isRunning(FIRST_MOTOR) &&											//检测到位置不正确，注意参数，状态3，运行状态中不存在这种状态
		g.mc.Get_Position(FIRST_MOTOR) != g.ini.m_FstMtr.Waves &&									//函数不能被赋值
		g.mc.Read_Input(IN_CYL_UP) &&
		g.mc.Read_Input(IN_PRESSURE) &&
		g.mc.Controller.FHavingBar
		)
	{
		FrontStep = 5;
		g.Controller.SysState |= SYSNEEDORIGIN;									//检测到负压不正确
		AfxMessageBox(L"掉落物品,请复位");
		goto endthefun;
	}

	
	//准备好交接状态
	if (!g.mc.isRunning(FIRST_MOTOR) &&											//第一个电机移动到第二个电机上方位置的时候检测到位置正确的时候就开始放下物料
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
			AfxMessageBox(L"等待电机二到原点超时!");
			goto endthefun;
		}
	}
	//准备好交接状态
	if (!g.mc.isRunning(FIRST_MOTOR) 											//第一个电机移动到第二个电机上方位置的时候检测到位置正确的时候就开始放下物料
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
	if (!g.mc.isRunning(FIRST_MOTOR) &&											//检测到位置正确的时候就开始放下物料，注意判断参数是电机停止了，并且气缸已经放好物料
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

	//放好料了，应该缩回来
	if (!g.mc.isRunning(FIRST_MOTOR) &&											//检测到位置正确的时候就开始放下物料，注意判断参数是电机停止了，并且气缸已经放好物料
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
		g.backNo = g.frontNo;//传递编号
	}
	//放好料了已经缩回来了
	if (!g.mc.isRunning(FIRST_MOTOR) &&											//检测到位置正确的时候就开始放下物料，注意判断参数是电机停止了，并且气缸已经放好物料
		g.mc.Get_Position(FIRST_MOTOR) == g.ini.m_FstMtr.Waves &&
		g.mc.Read_Input(IN_PRESSURE) &&
		g.mc.Read_Input(IN_CYL_UP) &&
		0 == g.mc.Get_Position(SECOND_MOTOR) &&
		!g.mc.isRunning(SECOND_MOTOR) //&&
		&& g.mc.Controller.BHavingBar //前一个是否有导板也无所谓了
		)
	{
		BackStep = 3;
		g.mc.MoveToOrigin(FIRST_MOTOR,g.ini.m_FstMtr.MaxSpeed,g.ini.m_FstMtr.Tacc);
	}

	//放好料了已经缩回来了
	if (!g.mc.isRunning(SECOND_MOTOR) &&											//检测到位置正确的时候就开始放下物料，注意判断参数是电机停止了，并且气缸已经放好物料
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


	if (!g.mc.isRunning(SECOND_MOTOR) &&											//检测到位置正确的时候就开始放下物料，注意判断参数是电机停止了，并且气缸已经放好物料
		//g.mc.Read_Input(IN_PRESSURE) &&
		//g.mc.Read_Input(IN_CYL_UP) &&
		g.mc.Get_Position(SECOND_MOTOR) == g.ini.m_SndMtr.Waves &&
		g.mc.Controller.BHavingBar
		&& !isBackDetect
		//&& 4 == BackStep
		)
	{
		if (4 == BackStep) g.mc.Controller.DelayForGetChainBar = GetTickCount();  //超时间检测
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
	g.Controller.SysState &= ~SYSRUNNING;							//取消运行标记
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
	// TODO:  在此添加命令处理程序代码
	AfxMessageBox(L"OK");
}
