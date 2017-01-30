// test.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageVision.h"
#include "test.h"
#include "afxdialogex.h"
#include "Tools.h"


// test 对话框

IMPLEMENT_DYNAMIC(test, CDialogEx)

test::test(CWnd* pParent /*=NULL*/)
	: CDialogEx(test::IDD, pParent)
	, m_val1(0)
	, m_val2(0)
	, m_val3(0)
{

//	g.mytest.val1 = g.ini.m_TopCamConfig;
//	g.mytest.val2 =	
}

test::~test()
{
}

void test::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_VAL1, m_val1);
	DDX_Text(pDX, IDC_EDIT_VAL2, m_val2);
	DDX_Text(pDX, IDC_EDIT_VAL3, m_val3);
}


BEGIN_MESSAGE_MAP(test, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_COFIRM, &test::OnBnClickedButtonCofirm)
	ON_BN_CLICKED(IDC_BUTTON_P1, &test::OnBnClickedButtonP1)
	ON_BN_CLICKED(IDC_BUTTON_P2, &test::OnBnClickedButtonP2)
END_MESSAGE_MAP()


// test 消息处理程序


void test::OnBnClickedButtonCofirm()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	g.mytest.val1 = m_val1;
	g.mytest.val2 = m_val2;
	g.mytest.val3 = m_val3;
	g.mc.Triger();
}



void test::OnBnClickedButtonP1()
{
	// TODO:  在此添加控件通知处理程序代码
	win_tool wt;
	CString app = wt.AppPath();
	CFileDialog fileDlg(TRUE, (LPCTSTR)"ccf", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"配置文件 (*.ccf)|*.ccf|All Files (*.*)|*.*||");	//打开读取文件对话框
	fileDlg.m_pOFN->lpstrInitialDir = app;
	if (fileDlg.DoModal() != IDOK) return;
	CString str = fileDlg.GetPathName();
	str = str.Right(str.GetLength() - str.ReverseFind('\\') - 1);
	g.ini.m_TopCamConfig = str;
	g.ini.SaveParaFile(PARA_PRJ);
	GetDlgItem(IDC_EDIT_VAL1)->SetWindowText(str);

}


void test::OnBnClickedButtonP2()
{
	// TODO:  在此添加控件通知处理程序代码
	win_tool wt;
	CString app = wt.AppPath();
	CFileDialog fileDlg(TRUE, (LPCTSTR)"ccf", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"配置文件 (*.ccf)|*.ccf|All Files (*.*)|*.*||");	//打开读取文件对话框
	fileDlg.m_pOFN->lpstrInitialDir = app;
	if (fileDlg.DoModal() != IDOK) return;
	CString str = fileDlg.GetPathName();
	str = str.Right(str.GetLength() - str.ReverseFind('\\') - 1);
	//str = str.Right(str.Find(':'));
	g.ini.m_BottomCamConfig = str;
	g.ini.SaveParaFile(PARA_PRJ);
	GetDlgItem(IDC_EDIT_VAL2)->SetWindowText(str);
}


BOOL test::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_EDIT_VAL1)->SetWindowText(g.ini.m_TopCamConfig);
	GetDlgItem(IDC_EDIT_VAL2)->SetWindowText(g.ini.m_BottomCamConfig);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
