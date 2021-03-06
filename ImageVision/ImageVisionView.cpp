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

// ImageVisionView.cpp : CImageVisionView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ImageVision.h"
#endif

#include "ImageVisionDoc.h"
#include "ImageVisionView.h"
#include "MainFrm.h"
#include "test.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageVisionView

IMPLEMENT_DYNCREATE(CImageVisionView, CView)

BEGIN_MESSAGE_MAP(CImageVisionView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_MESSAGE(WMMSG_PARAMESSAGE, &CImageVisionView::ReceiveParaMessage)
	ON_COMMAND(ID_BUTTON_CHANGEEF, &CImageVisionView::OnButtonChangeef)
	ON_COMMAND(ID_BUTTON_LOADIMG, &CImageVisionView::OnButtonLoadimg)
	ON_COMMAND(ID_BUTTON_SNAP, &CImageVisionView::OnButtonSnap)
	ON_COMMAND(ID_BUTTON_SAVEIMG, &CImageVisionView::OnButtonSaveimg)
	ON_COMMAND(ID_BUTTON_TOP, &CImageVisionView::OnButtonTop)
	ON_COMMAND(ID_BUTTON_BOTTOM, &CImageVisionView::OnButtonBottom)
	ON_COMMAND(ID_BUTTON_TRAIN, &CImageVisionView::OnButtonTrain)
//	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
//	ON_WM_PAINT()
ON_WM_PAINT()
ON_COMMAND(ID_BUTTON_SHOW_RESULT, &CImageVisionView::OnButtonShowResult)
ON_WM_TIMER()
ON_COMMAND(ID_BUTTON2, &CImageVisionView::OnButtonTestDlg)
ON_COMMAND(ID_BUTTON_CLEAR, &CImageVisionView::OnButtonClear)
ON_COMMAND(ID_BUTTON_TESTPIC, &CImageVisionView::OnButtonTestpic)
ON_WM_MBUTTONDOWN()
ON_COMMAND(ID_BUTTON_TESTGROUP, &CImageVisionView::OnButtonTestgroup)
//ON_COMMAND(ID_BUTTON3, &CImageVisionView::showStitching)
//ON_COMMAND(ID_BUTTON_READPICS, &CImageVisionView::OnButtonReadpics)
ON_COMMAND(ID_BUTTON_RPICS, &CImageVisionView::OnButtonRpics)
ON_COMMAND(ID_BUTTON_STITCH, &CImageVisionView::OnButtonStitch)
END_MESSAGE_MAP()

// CImageVisionView 构造/析构

CImageVisionView::CImageVisionView()
{
	// TODO:  在此处添加构造代码
	m_RectTracker.m_nStyle = CRectTracker::dottedLine | CRectTracker::resizeInside;  // 样式    
	m_RectTracker.m_nHandleSize = 10;  //手柄大小  
	m_RectTracker.m_rect.SetRectEmpty(); // 最初的矩形   
	Tick = 0;
}

CImageVisionView::~CImageVisionView()
{
}

BOOL CImageVisionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
#if 0
	g.AutoDetect = STOP;

	if (daheng) delete daheng;
	daheng = NULL;
	daheng = new CDahengCam();

	if (1 != daheng->CameraInit())
	{
		AfxMessageBox(L"未发现相机");
	}

	RunDetectionThread();
	
	if (0 == g.mc.init()) { AfxMessageBox(L"初始化失败或者没检测到IO卡"); }
	RunIOThread();
#endif
	//SetTimer(1, 20, NULL);
	return CView::PreCreateWindow(cs);
}

// CImageVisionView 绘制

void CImageVisionView::OnPaint()
{
	
	CPaintDC pDC(this); // device context for painting
	//OnDraw(&pDC);

	if (g.delay_display)
	{
		OnDraw(&pDC);
		g.delay_display = false;
	}
	if (g.isRect) m_RectTracker.Draw(&pDC);	
}





void CImageVisionView::OnDraw(CDC* pDC)
{
	CImageVisionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
#ifdef _OPENCV	
	// TODO:  在此处为本机数据添加绘制代码
	CString str;
	switch (g.AutoDetect)
	{
	case STOP:str.Format(_T("检测总数：%d 正面：%d 反面：%d---"), g.detectNo, g.fontNo, g.backNo); break;
	case SHOWEFFECT:str.Format(_T("实时检测---检测总数：%d 正面：%d 反面：%d---"), g.detectNo, g.fontNo, g.backNo); break;
	case RUN:str = L"拍照中 "; break;
	default:str.Empty();
	}
	str = str + g.resultmsg;

	CRect rt;
	GetClientRect(&rt);
	int nWndWidth = rt.right - rt.left;
	int nWndHeight = rt.bottom - rt.top;
	switch (g.CameraNum)
	{
	case 1: break;
	case 2: nWndHeight /= 2; break;
	case 3:
	case 4: nWndWidth /= 2; nWndHeight /= 2; break;
	default:return;
	}
	g.CameraMat_Mutex.Lock();

	if (!g.Opencv.mat0.empty())
	{
		cv::Point st, ed;
		st.x = g.ImgROI.x;
		st.y = g.ImgROI.y;
		ed.x = st.x + g.ImgROI.width;
		ed.y = st.y + g.ImgROI.height;
		if (g.b_cvcolor)
		{
			g.cvcolor += 2;
			if (g.cvcolor > 253)
				g.b_cvcolor = false;
		}
		else
		{
			g.cvcolor -= 2;
			if (g.cvcolor < 2)
				g.b_cvcolor = true;
		}

		rectangle(g.Opencv.mat0, st, ed, Scalar(g.cvcolor, 60, 255 - g.cvcolor), 2, 1, 0);
	}


	uchar *pdata = NULL;
	if (g.Opencv.CopyMatDataToMem(g.Opencv.mat0, &pdata))
	{

		CDC dcMem;
		CBitmap bmp; //内存中承载临时图象的位图
		bmp.CreateCompatibleBitmap(pDC, rt.Width(), rt.Height());
		dcMem.CreateCompatibleDC(pDC); //依附窗口DC创建兼容内存DC
		//创建兼容位图(必须用pDC创建，否则画出的图形变成黑色)
		CBitmap *pOldBit = dcMem.SelectObject(&bmp);
		//按原来背景填充客户区，不然会是黑色
		HDC  hDC = dcMem.GetSafeHdc();
		::SetStretchBltMode(hDC, COLORONCOLOR);
		
		LPBITMAPINFO pBitmapInfo = g.Opencv.CreateMapInfo(g.Opencv.mat0);
		const int height = g.Opencv.mat0.rows;
		const int width = g.Opencv.mat0.cols;
		::StretchDIBits(hDC,
			0,//nWndWidth,
			0,//nWndHeight,
			nWndWidth,
			nWndHeight,
			0,
			0,
			width,
			height,
			(BYTE*)pdata,
			pBitmapInfo,
			DIB_RGB_COLORS,
			SRCCOPY
			);
		GetClientRect(&rt);
		dcMem.TextOut(50, 80, str);
		hDC = pDC->GetSafeHdc();
		pDC->BitBlt(0, 0, rt.Width(), rt.Height(), &dcMem, 0, 0, SRCCOPY);

		//将内存DC上的图象拷贝到前台
		//绘图完成后的清理
		dcMem.DeleteDC();     //删除DC
		bmp.DeleteObject(); //删除位图
		free(pdata);

	}	
	g.resultmsg.Empty();
	g.CameraMat_Mutex.Unlock();
#endif

}

void CImageVisionView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	if (g.isRect)
	{
		g.isRect = false;
		get_mfc_roi();
		g.delay_display = true;
		return;
	}
	//Draw();
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageVisionView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageVisionView 诊断

#ifdef _DEBUG
void CImageVisionView::AssertValid() const
{
	CView::AssertValid();
}


void CImageVisionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageVisionDoc* CImageVisionView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageVisionDoc)));
	return (CImageVisionDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageVisionView 消息处理程序


void CImageVisionView::OnButtonChangeef()
{
	// TODO:  在此添加命令处理程序代码
	if (RUN == g.AutoDetect)
	{
		CaptureAndDection();
	}
	else if (SHOWEFFECT == g.AutoDetect)
	{
		StopCapture();
	}
	else
	{
		RunCature();
	}
}


void CImageVisionView::OnButtonLoadimg()
{
	// TODO:  在此添加命令处理程序代码
#ifdef _OPENCV
	try{
		CFileDialog fileDlg(TRUE, (LPCTSTR)"BMP", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"All Files (*.*)|*.*|位图文件 (*.bmp)|*.bmp||");	//打开读取文件对话框

		if (fileDlg.DoModal() != IDOK) return;

		if (STOP != g.AutoDetect)
		{
			g.AutoDetect = STOP;
			Sleep(100);
		}

		g.Opencv.LoadMatImg((LPCSTR)CStringA(fileDlg.GetPathName()));					//读取图像，并载入到Mat类里面

		if (g.Opencv.mat0.empty()) { AfxMessageBox(L"不能读取图片"); return; }			//读取失败则返回

		g.Opencv.MonoInvert(g.Opencv.mat0);

		g.Opencv.m_RawImg = g.Opencv.mat0.clone(); //原始图片

		//Invalidate(TRUE);															//重绘窗口
	//	Draw();

	}
	catch (...)
	{
		AfxMessageBox(L"文件路径读取错误");
	}
#endif
}


void CImageVisionView::OnButtonSnap()
{
	// TODO:  在此添加命令处理程序代码
	//g.AutoDetect = RunCaptureThread();
#ifdef _OPENCV
	Sleep(50);
	g.AutoDetect = STOP;
	Sleep(50);
	g.Opencv.mat0 = g.Opencv.Cameramat.clone();
	g.Opencv.m_RawImg = g.Opencv.mat0.clone();
	Draw();
#endif
}


void CImageVisionView::OnButtonSaveimg()
{
	// TODO:  在此添加命令处理程序代码
#ifdef _OPENCV
	int state = g.AutoDetect;
	g.AutoDetect = STOP;
	Sleep(50);
	//会崩溃？
	g.Opencv.SaveImage(g.Opencv.Cameramat);
	g.AutoDetect = state;
#endif
}

void CImageVisionView::OnButtonShowResult()
{
	// TODO:  在此添加命令处理程序代码
//	Detection_Trigger();
#ifdef _OPENCV
	if (STOP != g.AutoDetect)
		g.AutoDetect = STOP;
	g.Opencv.m_RawImg = g.Opencv.Cameramat.clone();
	
	g.Opencv.mat0 = g.Opencv.m_RawImg.clone();

	//if (!g.ProcessImg) { AfxMessageBox(L"找不到函数地址"); return; }
	if (!g.DealImage)	{ AfxMessageBox(L"找不到函数地址"); return; }

	float m_result = g.ProcessImg(g.Opencv.mat0, g.ImgROI);
	if (g.Opencv.mat0.empty()) cout << "empty" << endl;
//	float m_result = g.Opencv.ProcessImg(g.Opencv.mat0, g.ImgROI);
	if (-1.0 == m_result) g.resultmsg = L" 检测结果为：反面 ";
	else if (1.0 == m_result) g.resultmsg = L" 检测结果为：正面 ";
	else g.resultmsg = L" 发生未知错误";
	Draw();


	if (STOP != g.AutoDetect)
		g.AutoDetect = STOP;


	if (!g.DealImage)	{ AfxMessageBox(L"找不到函数地址"); return; }
	float m_result = g.DealImage(g.Opencv.m_RawImg, g.Opencv.mat0, g.ImgROI);


	//cout << g.Opencv.mat0 << endl;




	if (-1.0 == m_result) g.resultmsg = L" 检测结果为：反面 ";
	else if (1.0 == m_result) g.resultmsg = L" 检测结果为：正面 ";
	else g.resultmsg = L" 发生未知错误";
	Draw();
#endif

	
#ifdef _TEST

#endif // _TEST


}


void CImageVisionView::OnButtonTop()
{
	// TODO:  在此添加命令处理程序代码
	CSelectFolderDlg FDlg;
	CString str = FDlg.Show();
	g.TopSideImgPath.clear();
	ProcPicDir(str, g.TopSideImgPath);
}


void CImageVisionView::OnButtonBottom()
{
	// TODO:  在此添加命令处理程序代码
	CSelectFolderDlg FDlg;
	CString str = FDlg.Show();
	g.BottomSideImgPath.clear();
	ProcPicDir(str, g.BottomSideImgPath);
}


void CImageVisionView::OnButtonTrain()
{
	// TODO:  在此添加命令处理程序代码
#ifdef _OPENCV
	g.Opencv.hog_svm_train();
#endif
}


LRESULT CImageVisionView::ReceiveParaMessage(WPARAM wParam, LPARAM lParam)
{
	return 0;
#ifdef _OPENCV
	if (STOP != g.AutoDetect)
	{
		g.Opencv.m_RawImg = g.Opencv.Cameramat.clone();
	
	}
	else
		g.Opencv.mat0 = g.Opencv.Cameramat.clone();
	g.AutoDetect = STOP;

	g.Opencv.Convert2BGR(g.Opencv.mat0, g.Opencv.mat0);
	Point st, ed;
	st.x = g.ImgROI.x;
	st.y = g.ImgROI.y;
	ed.x = st.x + g.ImgROI.width;
	ed.y = st.y + g.ImgROI.height;

	daheng->SetShutter(g.ini.m_shutter);

	if (g.Opencv.mat0.empty())
	{
		AfxMessageBox(L"没有检测到图像存在，设置失败");
	}

	rectangle(g.Opencv.mat0, st, ed, Scalar(250, 100, 100), 1, 1, 0);
	//Invalidate(TRUE);
	return 0;
#endif
}

#if 0
UINT CImageVisionView::CaptrueThread(LPVOID lParam)
{
	CImageVisionView* view = (CImageVisionView*)lParam;
	int state = g.AutoDetect;
	while (0)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(g.evt_capture.evt, 1))
		{

			g.Opencv.mat0 = g.Opencv.Cameramat.clone();
			g.evt_capture.ResetEvent();
			view->Invalidate(FALSE);

		}
	}
	return 0;
}



bool CImageVisionView::RunCaptureThread()
{
	pCaputreThread = AfxBeginThread(CaptrueThread, this);
	//AfxBeginThread(CaptrueThread,AfxGetMainWnd()->m_hWnd, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	return true;
}
#endif

bool CImageVisionView::RunDetectionThread()
{
	//pDetectionThread = AfxBeginThread(DetectionThread, AfxGetMainWnd()->m_hWnd, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	pDetectionThread = AfxBeginThread(DetectionThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	return true;
}

bool CImageVisionView::RunIOThread()
{
	pIOThread = AfxBeginThread(IOThread, this);
		return true;


}

UINT CImageVisionView::IOThread(LPVOID lParam)
{
	CImageVisionView* view = (CImageVisionView*)lParam;
	while (1)
	{	
		if (g.mc.isInitOK)
		{
			view->Detection_Trigger();
		}			
		Sleep(3);
	}
}



















//映射感兴趣区域
void CImageVisionView::get_mfc_roi()
{
#ifdef _OPENCV
	int x = (int)m_RectTracker.m_rect.left;
	int y = (int)m_RectTracker.m_rect.top;
	int r = (int)m_RectTracker.m_rect.right;
	int b = (int)m_RectTracker.m_rect.bottom;

	int nwr = 0;
	int nwb = 0;
	CRect rt;
	GetClientRect(&rt);

	g.MFCROI.x = x > rt.left ? x : rt.left;
	g.MFCROI.y = y > rt.top ? y : rt.top;
	nwr = rt.right > r ? r : rt.right;
	nwb = rt.bottom > b ? b : rt.bottom;
	g.MFCROI.width = nwr - g.MFCROI.x;
	g.MFCROI.height = nwb - g.MFCROI.y;
	g.MFCFrame.x = rt.left;
	g.MFCFrame.y = rt.right;
	g.MFCFrame.width = rt.right - rt.left;
	g.MFCFrame.height = rt.bottom - rt.top;

	if (g.Opencv.mat0.empty())
	{
		AfxMessageBox(L"没有检测到图像存在，设置失败！");
		return;
	}

	g.widthRatio = (1.0)*g.Opencv.mat0.cols / g.MFCFrame.width;
	g.heightRatio = (1.0)*g.Opencv.mat0.rows / g.MFCFrame.height;


	g.ImgROI.x = (int)(g.MFCROI.x*g.widthRatio);
	g.ImgROI.y = (int)(g.MFCROI.y*g.heightRatio);
	g.ImgROI.width = (int)(g.MFCROI.width *g.widthRatio);
	g.ImgROI.height = (int)(g.MFCROI.height*g.heightRatio);

	g.Opencv.Convert2BGR(g.Opencv.mat0, g.Opencv.mat0);
	Point st, ed;

	g.Opencv.MonoInvert(g.Opencv.mat0);

	st.x = g.ImgROI.x;
	st.y = g.ImgROI.y;
	ed.x = st.x + g.ImgROI.width;
	ed.y = st.y + g.ImgROI.height;


	rectangle(g.Opencv.mat0, st, ed, Scalar(60, 255, 100), 1, 1, 0);
	//Invalidate(TRUE);

	g.Opencv.MonoInvert(g.Opencv.mat0);

	g.ImgROI.y = g.Opencv.mat0.rows - g.ImgROI.y - g.ImgROI.height;

	g.ini.m_nSear_X = g.ImgROI.x;
	g.ini.m_nSear_Y = g.ImgROI.y;
	g.ini.m_nSear_W = g.ImgROI.width;
	g.ini.m_nSear_H = g.ImgROI.height;

	g.ini.SaveParaFile(PARA_ALL);

	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CView *pview = (CView *)pMain->ControlView;
	pview->SendMessage(WMMSG_UPDATEUI, 1, 1); //发消息,刷新显示
#endif

}


void CImageVisionView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (!g.isRect) return;



	CRect cwrect;
	//GetDlgItem(IDC_STATIC_IMGCAPTURE)->GetWindowRect(&cwrect); //获取窗体中控件的区域
	//ScreenToClient(&cwrect); //转换为相对区域的CRect  if(point.x>cwrect.left&& point.y>cwrect.top    //确保按下的坐标在控件区域内   && point.x
	{
		if (m_RectTracker.HitTest(point) < 0)     //如果未击中矩形选择框,重新画选择框
		{
			m_RectTracker.TrackRubberBand(this, point, TRUE);
			m_RectTracker.m_rect.NormalizeRect();   //正规化矩形（关于正规化矩形下面有介绍）
		}
		else           //如果击中矩形选择框
		{
			m_RectTracker.Track(this, point, TRUE);
			m_RectTracker.m_rect.NormalizeRect();   //正规化矩形
			//SendMessage(WM_LBUTTONUP,NULL,NULL);
		}
		Draw();
		Invalidate(FALSE);   //刷新窗口区域，使得CrectTracker对象重绘到窗口上
	}

	CView::OnLButtonDown(nFlags, point);
}

bool CImageVisionView::ProcPicDir(CString strPicDir, vector<CString>& strPath)
{
	strPath.clear();
	CFileFind fileFinder;

	if (strPicDir.Right(1) == TEXT("\\"))
	{
		int nPos = strPicDir.ReverseFind(TEXT('\\'));
		strPicDir = strPicDir.Left(nPos);
	}

	CString strPicFile = TEXT("");
	strPicFile.Format(TEXT("%s\\%s"), strPicDir, TEXT("*.*"));

	BOOL bWorking = fileFinder.FindFile(strPicFile);
	while (bWorking)
	{
		bWorking = fileFinder.FindNextFile();
		if (fileFinder.IsDots())
		{
			continue;
		}

		CString strFilePath = fileFinder.GetFilePath();
		//		if (fileFinder.IsDirectory())
		//		{
		//继续遍历目录
		//			ProcPicDir(strFilePath);
		//		}
		//		else
		//		{
		int nPos = strFilePath.ReverseFind(TEXT('.'));
		CString strExt = strFilePath.Right(strFilePath.GetLength() - nPos - 1);
		if (strExt.CompareNoCase(TEXT("jpg")) == 0 ||
			strExt.CompareNoCase(TEXT("jpeg")) == 0 ||
			strExt.CompareNoCase(TEXT("bmp")) == 0)
		{
			//处理图片
			//...
			strPath.push_back(strFilePath);
		}
	}
	fileFinder.Close();
	return true;
}






void CImageVisionView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
//	if (1 == nIDEvent){ Detection_Trigger(); }
//	if (2 == nIDEvent) {
	
//		if (!g.evt_detect.EventState()) g.evt_detect.SetEvent();
	
//	}
	/*
	switch (nIDEvent)
	{
	case 1:
		Detection_Trigger(); 
		break;
	case CAPANDDECTION:
		if(!g.evt_detect.EventState()) g.evt_detect.SetEvent(); 
		break;
	default:
		break;
	}
	*/
	CView::OnTimer(nIDEvent);
}


float CImageVisionView::ImgResult()
{
#ifdef _OPENCV
	float m_result = 0.0;
	g.Opencv.mat0 = g.Opencv.Cameramat.clone();
	g.Opencv.m_RawImg = g.Opencv.mat0.clone();
	m_result = g.Opencv.ProcessImg(g.Opencv.mat0, g.ImgROI);
	if (-1.0 == m_result) g.resultmsg = L" 检测结果为：反面 ";
	else if (1.0 == m_result) g.resultmsg = L" 检测结果为：正面 ";
	else g.resultmsg = L" 发生未知错误";
	return m_result;
#endif
}




void CImageVisionView::RunCature()
{
	g.AutoDetect = RUN;
}


void CImageVisionView::CaptureAndDection()
{
	g.AutoDetect = SHOWEFFECT;
}


void CImageVisionView::StopCapture()
{
	g.AutoDetect = STOP;
	Draw();
}


//#pragma omp threadprivate
UINT CImageVisionView::DetectionThread(LPVOID lParam)
{
	CImageVisionView* view = (CImageVisionView*)lParam;
	return 0;
#ifdef _OPENCV
	while (1)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(g.evt_detect.evt, 1))
		{
			switch (g.AutoDetect)
			{
			case RUN:
				g.Opencv.mat0 = g.Opencv.Cameramat.clone();
				view ->Draw();
				break;
			case STOP:Sleep(100); break;
			case SHOWEFFECT:
				view->ImgResult();
				
				view->Draw();			
				break;
			default:
				//AfxMessageBox(L"产生未知冲突，软件选择只有运行和效果两个！");
				break;
			}
			g.evt_detect.ResetEvent();
		}
	}
#endif
}


void CImageVisionView::Draw(const Hobject &hobject)
{
	CRect	rtWin, rtWin1;
	CWnd *pWnd = CImageVisionView::GetWindow(1);//我没做成active的原因是不管active不active都需要在规定的窗口显示图片，项目是要在两个单文档窗口显示图片
	pWnd->GetWindowRect(&rtWin);
	CDC *pDC = pWnd->GetDC();
	GetClientRect(&rtWin);
	g.show_image.Dis_Obj_extend(hobject,rtWin, pWnd);
	ReleaseDC(pDC);
}


void CImageVisionView::Draw()
{
//	CPaintDC dc(this); // device context for painting

	CDC *pDC = CImageVisionView::GetDC();
	OnDraw(pDC);
	ReleaseDC(pDC);
#if 0
	CWnd* pMainWnd = AfxGetMainWnd();
	CDC * pDC = pMainWnd->GetDC();

	CRect rt;
	GetClientRect(&rt);
	int nWndWidth = rt.right - rt.left;
	int nWndHeight = rt.bottom - rt.top;
	switch (g.CameraNum)
	{
	case 1: break;
	case 2: nWndHeight /= 2; break;
	case 3:
	case 4: nWndWidth /= 2; nWndHeight /= 2; break;
	default:return;
	}


	HDC  hDC = pDC->GetSafeHdc();

	::SetStretchBltMode(hDC, COLORONCOLOR);
	uchar *pdata = NULL, *pdata1 = NULL, *pdata2 = NULL, *pdata3 = NULL;

	if (!g.Opencv.mat0.empty())
	{
		cv::Point st, ed;
		st.x = g.ImgROI.x;
		st.y = g.ImgROI.y;
		ed.x = st.x + g.ImgROI.width;
		ed.y = st.y + g.ImgROI.height;
		if (g.b_cvcolor)
		{
			g.cvcolor++;
			if (g.cvcolor > 254)
				g.b_cvcolor = false;
		}
		else
		{
			g.cvcolor--;
			if (g.cvcolor < 1)
				g.b_cvcolor = true;
		}

		rectangle(g.Opencv.mat0, st, ed, Scalar(60, 255 - g.cvcolor, g.cvcolor), 2, 1, 0);
	}

	if (g.Opencv.CopyMatDataToMem(g.Opencv.mat0, &pdata))
	{

		LPBITMAPINFO pBitmapInfo = g.Opencv.CreateMapInfo(g.Opencv.mat0);
		const int height = g.Opencv.mat0.rows;
		const int width = g.Opencv.mat0.cols;
		::StretchDIBits(hDC,
			0,//nWndWidth,
			0,//nWndHeight,
			nWndWidth,
			nWndHeight,
			0,
			0,
			width,
			height,
			(BYTE*)pdata,
			pBitmapInfo,
			DIB_RGB_COLORS,
			SRCCOPY
			);
		//free(pdata);
	}

	if (g.Opencv.CopyMatDataToMem(g.Opencv.mat1, &pdata1))
	{
		size_t height = src.rows;
		size_t step = src.step;
		size_t offset = 0 == src.step % 4 ? 0 : 4 - src.step % 4;
		size_t size_i = (offset + src.step) * height;
		*data = (uchar*)malloc(2 * size_i);
		if (NULL == *data) return false;
		uchar *idata = src.data;
		uchar *ddata = *data;
		for (int h = 0; h < height; h++)
		{
			memcpy(ddata, idata, step);
			ddata = ddata + step + offset;
			idata = idata + step;
		}

		LPBITMAPINFO pBitmapInfo = g.Opencv.CreateMapInfo(g.Opencv.mat1);
		const int height = g.Opencv.mat1.rows;
		const int width = g.Opencv.mat1.cols;
		::StretchDIBits(hDC,
			nWndWidth,
			0,//nWndHeight,
			nWndWidth,
			nWndHeight,
			0,
			0,
			width,
			height,
			pdata1,
			pBitmapInfo,
			DIB_RGB_COLORS,
			SRCCOPY
			);
		free(pdata1);
	}

	if (g.Opencv.CopyMatDataToMem(g.Opencv.mat2, &pdata2))
	{
		size_t height = src.rows;
		size_t step = src.step;
		size_t offset = 0 == src.step % 4 ? 0 : 4 - src.step % 4;
		size_t size_i = (offset + src.step) * height;
		*data = (uchar*)malloc(2 * size_i);
		if (NULL == *data) return false;
		uchar *idata = src.data;
		uchar *ddata = *data;
		for (int h = 0; h < height; h++)
		{
			memcpy(ddata, idata, step);
			ddata = ddata + step + offset;
			idata = idata + step;
		}

		LPBITMAPINFO pBitmapInfo = g.Opencv.CreateMapInfo(g.Opencv.mat2);
		const int height = g.Opencv.mat2.rows;
		const int width = g.Opencv.mat2.cols;
		::StretchDIBits(hDC,
			0,//nWndWidth,
			nWndHeight,
			nWndWidth,
			nWndHeight,
			0,
			0,
			width,
			height,
			pdata2,
			pBitmapInfo,
			DIB_RGB_COLORS,
			SRCCOPY
			);
		free(pdata2);
	}

	if (g.Opencv.CopyMatDataToMem(g.Opencv.mat3, &pdata3))
	{
		size_t height = src.rows;
		size_t step = src.step;
		size_t offset = 0 == src.step % 4 ? 0 : 4 - src.step % 4;
		size_t size_i = (offset + src.step) * height;
		*data = (uchar*)malloc(2 * size_i);
		if (NULL == *data) return false;
		uchar *idata = src.data;
		uchar *ddata = *data;
		for (int h = 0; h < height; h++)
		{
			memcpy(ddata, idata, step);
			ddata = ddata + step + offset;
			idata = idata + step;
		}

		LPBITMAPINFO pBitmapInfo = g.Opencv.CreateMapInfo(g.Opencv.mat3);
		const int height = g.Opencv.mat3.rows;
		const int width = g.Opencv.mat3.cols;
		::StretchDIBits(hDC,
			nWndWidth,
			nWndHeight,
			nWndWidth,
			nWndHeight,
			0,
			0,
			width,
			height,
			pdata3,
			pBitmapInfo,
			DIB_RGB_COLORS,
			SRCCOPY
			);
		free(pdata3);
	}

#endif



	//if (g.isRect) m_RectTracker.Draw(&dc);
}


void CImageVisionView::OnButtonTestDlg()
{
	// TODO:  在此添加命令处理程序代码
	test testDlg;
	testDlg.DoModal();
}


void CImageVisionView::OnButtonClear()
{
	// TODO:  在此添加命令处理程序代码
//	g.detectNo = 0;
	g.frontNo = 0;
	g.backNo = 0;
	Draw();
	//Invalidate(FALSE);
}


void CImageVisionView::Detection_Trigger()
{
	//触发拍照检测
#ifdef _OPENCV
	if (g.mc.Triger())
	{

	
		//g.mc.Write_Output(OUT_OVER_DETECT, 0);
		g.AutoDetect = STOP;

		float l_result = ImgResult();

		if (MatEqual(g.Opencv.m_RawImg, g.Opencv.OldMat))
		{
			Draw();
			return;
		}
		if (1.0 == l_result) {
			//g.mc.Write_Output(OUT_FRONT, 0);
			g.mc.Front();
			g.fontNo++;
		}
		else if (-1.0 == l_result)
		{
			//g.mc.Write_Output(OUT_BACK, 0);
			g.mc.Back();
			g.backNo++;
		}
		else
		{
			g.mc.Reset();
			//g.mc.Write_Output(OUT_ERROR, 1);
			g.absentNo++;
			
		}
		Draw();
		Sleep(g.ini.m_delay);
		g.detectNo += 1;
		g.mc.Reset();
		if (g.ini.m_saveImgPerTime) g.Opencv.SaveImage(g.Opencv.m_RawImg); //检测时候保存图片
	}
#endif
}

void CImageVisionView::OnButtonTestpic()
{
	// TODO:  在此添加命令处理程序代码



}


void CImageVisionView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
#ifdef _OPENCV
	if (g.testPicTime >= g.testPic.size()) return;
	g.AutoDetect = FALSE;
	g.Opencv.LoadMatImg((LPCSTR)CStringA(g.testPic[g.testPicTime]));
	g.Opencv.MonoInvert(g.Opencv.mat0);
	float m_result = g.Opencv.ProcessImg(g.Opencv.mat0, g.ImgROI);
	if (-1.0 == m_result) g.resultmsg = L" 检测结果为：反面 ";
	else if (1.0 == m_result) g.resultmsg = L" 检测结果为：正面 ";
	else g.resultmsg = L"图片不能读取或未知错误";

	g.testPicTime++;
	Draw();
#endif
	CView::OnMButtonDown(nFlags, point);
}


void CImageVisionView::OnButtonTestgroup()
{
	// TODO:  在此添加命令处理程序代码
	CSelectFolderDlg FDlg;
	CString str = FDlg.Show();
	g.testPic.clear();
	ProcPicDir(str, g.testPic);
	g.testPicTime = 0;
}


bool CImageVisionView::MatEqual(cv::Mat& img1, cv::Mat& img2)
{
	bool flag = true;
	const int width = (int)img1.cols;
	const int height = (int)img1.rows;
	const int widthStep = (int)img1.step;
	uchar* data1 = img1.data;
	const int width2 = (int)img2.cols;
	const int height2 = (int)img2.rows;
	const int widthStep2 = (int)img2.step;
	uchar* data2 = img2.data;
	if (width != width2 || height != height2 || widthStep != widthStep2)
	{
		img2 = img1.clone();
		return false;
	}
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < widthStep; w++)
		{
			if(data1[w] != data2[w])
			{
				img2 = img1.clone();
				return false;
			}
				
		}
		data1 += widthStep;
		data2 += widthStep;
	}
	img2 = img1.clone();
	return true;
}

#if 0
void CImageVisionView::showStitching()
{
	// TODO:  在此添加命令处理程序代码

}
#endif

//void CImageVisionView::OnButtonReadpics()
//{
	// TODO:  在此添加命令处理程序代码

//}


void CImageVisionView::OnButtonRpics()
{
	// TODO:  在此添加命令处理程序代码
	CSelectFolderDlg FDlg;
	CString str = FDlg.Show();
	g.BottomSideImgPath.clear();
	ProcPicDir(str, g.StitchingPath);
}


void CImageVisionView::OnButtonStitch()
{
	// TODO:  在此添加命令处理程序代码
#ifdef _OPENCV
	if (g.StitchingPath.size() > 500 || g.StitchingPath.size() < 2) { AfxMessageBox(L"image read too much or too low!"); return; }
	vector<cv::Mat> src;
	for (int i = 0; i < g.StitchingPath.size(); i++)
	{
		
		cv::Mat mat = imread((LPCSTR)CStringA(g.StitchingPath[i]));
		if (mat.empty()) { AfxMessageBox(L"can not load image"); return; }
		src.push_back(mat);
		
	}
	cv::Mat pono;
	int res = g.Stitching(src, pono);
#ifdef _TEST
	if (pono.empty()) cout << "pono is empty!" << endl;
#endif
	if (-1 == res)
	{
		{ AfxMessageBox(L"failed"); return; }
	}

	if (0 == res)
	{
		{ AfxMessageBox(L"not enough images"); return; }
	}
	else
	{
		g.Opencv.mat0 = pono.clone();
		Draw();
	}
#endif
}
