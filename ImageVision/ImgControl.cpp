// ImgControl.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageVision.h"
#include "ImgControl.h"
#include "afxdialogex.h"


// CImgControl 对话框

IMPLEMENT_DYNAMIC(CImgControl, CDialogEx)

CImgControl::CImgControl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImgControl::IDD, pParent)
{

}

CImgControl::~CImgControl()
{
}

void CImgControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImgControl, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_UI, &CImgControl::OnBnClickedButtonUi)
END_MESSAGE_MAP()


// CImgControl 消息处理程序




void CImgControl::OnBnClickedButtonUi()
{
	// TODO:  在此添加控件通知处理程序代码
}
