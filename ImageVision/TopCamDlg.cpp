// TopCamDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageVision.h"
#include "TopCamDlg.h"
#include "afxdialogex.h"
#include "Tools.h"
#include <iostream>
using namespace std;
// CTopCamDlg 对话框

IMPLEMENT_DYNAMIC(CTopCamDlg, CDialogEx)

CTopCamDlg::CTopCamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTopCamDlg::IDD, pParent)
{
	isTrain = false;
	isCap = false;
}

CTopCamDlg::~CTopCamDlg()
{
}

void CTopCamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTopCamDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTopCamDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_SEL_TROI, &CTopCamDlg::OnBnClickedButtonSelTroi)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_TROI, &CTopCamDlg::OnBnClickedButtonShowTroi)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_TSAVEMODEL, &CTopCamDlg::OnBnClickedButtonShowTsavemodel)
	ON_CBN_SELCHANGE(IDC_COMBO_MODELSEL, &CTopCamDlg::OnSelchangeComboModelsel)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_MODEL, &CTopCamDlg::OnBnClickedButtonShowModel)
	ON_BN_CLICKED(IDC_BUTTON_TRAINMODEL, &CTopCamDlg::OnBnClickedButtonTrainmodel)
	ON_BN_CLICKED(IDC_BUTTON_SELALL, &CTopCamDlg::OnBnClickedButtonSelall)
	ON_BN_CLICKED(IDC_BUTTON_CAP, &CTopCamDlg::OnBnClickedButtonCap)
END_MESSAGE_MAP()


// CTopCamDlg 消息处理程序


void CTopCamDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	int res = -1; //防止下面不执行的时候res会出现一个随机值
	HException::InstallHHandler(&CPPExpDefaultExceptionHandler);
	try{
		set_check("give_error");

		_Params p;
		p.threshold_get_prod = g.ini.top_threshold;
		p.MatchScore = g.ini.top_match_score;
		p.TrainScore = g.ini.top_train_score;
		p.ROI_row1 = g.ini.TROI_y;
		p.ROI_col1 = g.ini.TROI_x;
		p.ROI_row2 = g.ini.TROI_h;
		p.ROI_col2 = g.ini.TROI_w;

		res = g.img_deal.Front_Deal(g.hRawImage, g.show_image.get_disp_hd(), p);


	}
	catch (HException &HDevExpDefaultException)
	{
		set_check("~give_error");
	}

	return;



}


void CTopCamDlg::OnBnClickedButtonSelTroi()
{
	// TODO:  在此添加控件通知处理程序代码
	 	if (NO_RECT != g.Controller.ROI_rect) { AfxMessageBox(show_roi_msg(g.Controller.ROI_rect)); return; }
	 		HTuple  Exception;
	 		HException::InstallHHandler(&CPPExpDefaultExceptionHandler);
	 		set_check("~give_error");
	 		try
	 		{
	 			g.Controller.ROI_rect = TROI_RECT;
	 			double row, col, row2, col2;
	 			Hobject ROI;
	 			Halcon::set_color(g.show_image.get_disp_hd(),"green");
	 			set_draw(g.show_image.get_disp_hd(), "margin");
	 			draw_rectangle1(g.show_image.get_disp_hd(), &row, &col, &row2, &col2);
	 			gen_rectangle1(&ROI, row, col,row2, col2);
	 			disp_obj(ROI, g.show_image.get_disp_hd());
	 			g.ini.TROI_x = col;
	 			g.ini.TROI_y = row;
	 			g.ini.TROI_w = col2;
	 			g.ini.TROI_h = row2;
	 			g.ini.SaveParaFile(PARA_IMAGE);
	 			g.Controller.ROI_rect = NO_RECT;
	 		}
	 		catch (HException &HDevExpDefaultException)
	 		{
	 			HDevExpDefaultException.ToHTuple(&Exception);
	 			write_string(36000, Exception);
	 			set_check("~give_error");
	 			g.Controller.ROI_rect = NO_RECT;
	 			AfxMessageBox(L"设定搜索范围失败！");
	 		}
	// 
	// 	return;
}


void CTopCamDlg::OnBnClickedButtonShowTroi()
{
	// TODO:  在此添加控件通知处理程序代码
	
#if 1
	if (NO_RECT != g.Controller.ROI_rect) { AfxMessageBox(show_roi_msg(g.Controller.ROI_rect)); return; }
	HTuple  Exception;
	HException::InstallHHandler(&CPPExpDefaultExceptionHandler);
	set_check("~give_error");
	try
	{ 
		double row = g.ini.TROI_y, col = g.ini.TROI_x, col2 = g.ini.TROI_w, row2 = g.ini.TROI_h;
		Hobject ROI;
		Halcon::set_color(g.show_image.get_disp_hd(), "red");
		set_draw(g.show_image.get_disp_hd(), "margin");
		gen_rectangle1(&ROI, row, col, row2, col2);
		disp_obj(ROI, g.show_image.get_disp_hd());
	}
	catch (HException &HDevExpDefaultException)
	{
		HDevExpDefaultException.ToHTuple(&Exception);
		write_string(36000, Exception);
		set_check("~give_error");;
#ifdef _TEST
		cout << "error to show" << endl;
#endif
	}
#endif

}

void CTopCamDlg::OnBnClickedButtonShowTsavemodel()
{
	// TODO:  在此添加控件通知处理程序代码
	if (WAIT_OBJECT_0 == ::WaitForSingleObject(g.evt_capture.evt, 1)) { AfxMessageBox(L"相机正在拍摄中！"); return; }
	if (NO_RECT != g.Controller.ROI_rect) { AfxMessageBox(show_roi_msg(g.Controller.ROI_rect)); return; }

	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COMBO_MODELSEL);
	int nSel = pBox->GetCurSel();
	// 根据选中项索引获取该项字符串
	CString str;
	pBox->GetLBText(nSel, str);
	
	win_tool wt;
	CString AppPath = wt.AppPath();
	AppPath += L"\\" + g.prjini.m_Prj_Name + L"\\TopModel\\";
	CreateDirectory(AppPath, NULL);

	HException::InstallHHandler(&CPPExpDefaultExceptionHandler);
	try{
		set_check("~give_error");
		CString ImageName = AppPath + str + ".bmp";
		write_image(g.hRawImage, "bmp", 0, (char*)(LPCSTR)(CStringA)ImageName);
		g.show_image.Dis_Obj(g.hRawImage);
	}
	catch (HException &HDevExpDefaultException)
	{
		set_check("~give_error");
	}


}


void CTopCamDlg::OnSelchangeComboModelsel()
{
	// TODO:  在此添加控件通知处理程序代码
	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COMBO_MODELSEL);
	int nSel = pBox->GetCurSel();
	// 根据选中项索引获取该项字符串
	CString str;
	pBox->GetLBText(nSel, str);
	nSel = _ttoi(str);

}


BOOL CTopCamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COMBO_MODELSEL);// ->AddString(L"1");
	for (int i = 1; i <= 12; i++)
	{
		CString str;
		str.Format(_T("%d"), i);
		pBox->AddString(str);
	}
	pBox->SetCurSel(0);

	int l_selsetcheck = g.ini.m_TopModel;
	const int start_index = IDC_CHECK1;
	const int end_index = IDC_CHECK12;
	int l_mask = 1;
	for (int i = 1; i <= (1 + end_index - start_index); i++)
	{
		if (l_selsetcheck&l_mask)
		{
			((CButton*)GetDlgItem(start_index + i - 1))->SetCheck(1);
		}
		l_mask = l_mask << 1;
	}
	
	RunTrain();
//	Sleep(3000);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CTopCamDlg::OnBnClickedButtonShowModel()
{
	win_tool wt;
	CString fileName = wt.Get_Date_Time();
	CString AppPath = wt.AppPath();
	AppPath = AppPath + L"\\" + g.prjini.m_Prj_Name + L"\\" + L"ScaledImage";
	CreateDirectory(AppPath, NULL);
	CString str = AppPath + L"\\" + fileName + L".bmp";

#ifdef _HALCON
	HException::InstallHHandler(&CPPExpDefaultExceptionHandler);
	try{
		set_check("give_error");
		Halcon::Hobject ScaledImage;
		Halcon::HTuple Width, Height, iniWidth((double)g.ini.m_topwScaleTo);
		get_image_size(g.hRawImage, &Width, &Height);
		if (g.ini.m_TopRatio <= 0)
		{
			double w = Width[0].D();
			double h = Height[0].D();
			double Ratio = h / w;
			Height = (int)(Ratio*(double)g.ini.m_topwScaleTo);
		}
		Width = (int)g.ini.m_topwScaleTo;
		zoom_image_size(g.hRawImage, &ScaledImage, Width, Height, "weighted");
		Halcon::write_image(ScaledImage, "bmp", NULL, (char*)(LPCSTR)(CStringA)str);
		g.show_image.Dis_Obj(ScaledImage);
	}
	catch (HException &CPPExpDefaultExceptionHandler)
	{
		set_check("~give_error");
		TRACE(CPPExpDefaultExceptionHandler);
		AfxMessageBox(L"没有图片，保存失败");	
	}
#endif
	// TODO:  在此添加控件通知处理程序代码
}


void CTopCamDlg::OnBnClickedButtonTrainmodel()
{
	// TODO:  在此添加控件通知处理程序代码
	//GetDlgItem(IDC_BUTTON_TRAINMODEL)->
	if (SYSRUNNING == (SYSRUNNING & g.Controller.SysState) || SYSTRAINING == (SYSTRAINING & g.Controller.SysState))
	{
		AfxMessageBox(L"系统正在运行中或者正在训练中！");
		return;
	}

	int l_selsetcheck = 0;
	const int start_index = IDC_CHECK1;
	const int end_index = IDC_CHECK12;
	int l_mask = 1;
	for (int i = 1; i <= (1 + end_index - start_index); i++)
	{
		if (((CButton*)GetDlgItem(start_index + i - 1))->GetCheck())
		{
			l_selsetcheck |= l_mask;
		}
		l_mask = l_mask << 1;
	}
	g.ini.m_TopModel = l_selsetcheck;
	g.ini.SaveParaFile(PARA_IMAGE);
	RunTrain();
}

void CTopCamDlg::OnBnClickedButtonSelall()
{

	// TODO:  在此添加控件通知处理程序代码
	int  i = IDC_CHECK1;
	int iend = IDC_CHECK12;
	for (; i <= iend; i++)
	{
		((CButton*)GetDlgItem(i))->SetCheck(1);
	}
}



bool CTopCamDlg::RunTrain()
{
	if (isTrain) return false;
	if (SYSTRAINING == (SYSTRAINING & g.Controller.SysState)) return false;
	pRunTrain = AfxBeginThread(RunTrainThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	if (!pRunTrain) return false;
	return true;
}

UINT CTopCamDlg::RunTrainThread(LPVOID lp)
{
	CTopCamDlg* pDlg = (CTopCamDlg*)lp;
	pDlg->isTrain = true;
	g.Controller.SysState |= SYSTRAINING;
	if (pDlg->train())
	{
		g.Controller.SysState &= ~SYSTOPTRAIN;//让系统无法进行训练，如果在运行的时候
	}
	else
	{
		g.Controller.SysState |= SYSTOPTRAIN;
		AfxMessageBox(L"1.确保画了搜索框\r\n2.确保有图片并且大小一致\r\n3.确保参数正确");
	}
	pDlg->isTrain = false;
	g.Controller.SysState &= ~SYSTOPTRAIN;//让系统无法进行训练，如果在运行的时候
	g.Controller.SysState &= ~SYSTRAINING;
	return 0;
}



vector<CString> CTopCamDlg::get_train_img_path()
{
	vector<CString> str;
	win_tool wt;
	CString AppPath = wt.AppPath();
	AppPath += L"\\" + g.prjini.m_Prj_Name + L"\\TopModel\\";

	int l_selsetcheck  = g.ini.m_TopModel;
	int l_mask = 1;
	for (int i = 1; i <= _MODEL_MAX; i++)
	{
		if (l_selsetcheck&l_mask)
		{ 
		CString l_serial, path;
		l_serial.Format(_T("%d.bmp"), i);
		path = AppPath + l_serial;
		str.push_back(path);
		}
		l_mask = l_mask << 1;
	}
	return str;
}



bool CTopCamDlg::train_img(vector<CString> str)
{
	vector<std::string> imgpath;
	for (size_t i = 0; i < str.size(); i++)
	{
		imgpath.push_back((std::string)(CStringA)str[i]);
	}


#ifdef _HALCON
	_Params p;
	p.threshold_get_prod = g.ini.top_threshold;
	p.MatchScore = g.ini.top_match_score;
	p.TrainScore = g.ini.top_train_score;
		vector<Hobject> ROIS;
		Halcon::Hobject ROI;
		p.ROI_row1 = g.ini.TROI_y;
		p.ROI_col1 = g.ini.TROI_x;
		p.ROI_row2 = g.ini.TROI_h;
		p.ROI_col2 = g.ini.TROI_w;
		set_check("give_error");
		HException::InstallHHandler(&CPPExpDefaultExceptionHandler);
		try{
			return g.img_deal.Front_Train(imgpath, g.show_image.get_disp_hd(), p);
		}
	
		catch (HException &HDevExpDefaultException)
		{
			set_check("~give_error");
			return false;
		}

#endif
	
	return true;
}

bool CTopCamDlg::train()
{
	return train_img(get_train_img_path());
}




void CTopCamDlg::OnBnClickedButtonCap()
{
	// TODO:  在此添加控件通知处理程序代码
	if (isCap)
	{
		GetDlgItem(IDC_BUTTON_CAP)->SetWindowText(L"合成中");
		return;
	}
	pCap = AfxBeginThread(RunCapThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	return;


#ifdef _TEST
	//g.mc.MoveToOrigin(FIRST_MOTOR, g.ini.m_FstMtr.MaxSpeed, g.ini.m_FstMtr.Tacc);

#else
	if (FST_MOTOR_NOT_ALREADY == (FST_MOTOR_NOT_ALREADY&g.Controller.SysState))
	{
		AfxMessageBox(L"第二轴电机没有复位！");
	}
#endif
	if (WAIT_OBJECT_0 == ::WaitForSingleObject(g.evt_capture.evt, 0) || g.mc.isRunning(FIRST_MOTOR))
	{
		GetDlgItem(IDC_BUTTON_CAP)->SetWindowText(L"合成中");
		Sleep(500);
		return;
	}


}


UINT CTopCamDlg::RunCapThread(LPVOID lp)
{
	CTopCamDlg *pDlg = (CTopCamDlg*)lp;
	pDlg->isCap = true;

	if (WAIT_OBJECT_0 == ::WaitForSingleObject(g.evt_capture.evt, 0) || g.mc.isRunning(FIRST_MOTOR))
	{
		pDlg->GetDlgItem(IDC_BUTTON_CAP)->SetWindowText(L"合成中");
		Sleep(500);
		pDlg->GetDlgItem(IDC_BUTTON_CAP)->SetWindowText(L"采集图片");
		goto endfun;
	}

	g.mc.MoveToOrigin(FIRST_MOTOR, g.ini.m_FstMtr.MaxSpeed, g.ini.m_FstMtr.Tacc);
	if (!g.mc.Wait(FIRST_MOTOR, 10000))
	{
		goto endfun;
	}
	g.mc.Stop(FIRST_MOTOR);
	Sleep(200);
	g.mc.MoveToDes(FIRST_MOTOR, g.ini.m_FstMtr.InitSpeed, g.ini.m_FstMtr.MaxSpeed, g.ini.m_FstMtr.Tacc, g.ini.m_FstMtr.Waves);
	g.evt_capture.SetEvent();
	::WaitForSingleObject(g.evt_capture.evt, INFINITE);

	endfun:
	pDlg->isCap = false;
	pDlg->GetDlgItem(IDC_BUTTON_CAP)->SetWindowText(L"采集图片");
	return 0;
}