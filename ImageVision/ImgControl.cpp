// ImgControl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageVision.h"
#include "ImgControl.h"
#include "afxdialogex.h"


// CImgControl �Ի���

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


// CImgControl ��Ϣ�������




void CImgControl::OnBnClickedButtonUi()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
