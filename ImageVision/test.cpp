// test.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageVision.h"
#include "test.h"
#include "afxdialogex.h"
#include "Tools.h"


// test �Ի���

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


// test ��Ϣ�������


void test::OnBnClickedButtonCofirm()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	g.mytest.val1 = m_val1;
	g.mytest.val2 = m_val2;
	g.mytest.val3 = m_val3;
	g.mc.Triger();
}



void test::OnBnClickedButtonP1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	win_tool wt;
	CString app = wt.AppPath();
	CFileDialog fileDlg(TRUE, (LPCTSTR)"ccf", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"�����ļ� (*.ccf)|*.ccf|All Files (*.*)|*.*||");	//�򿪶�ȡ�ļ��Ի���
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	win_tool wt;
	CString app = wt.AppPath();
	CFileDialog fileDlg(TRUE, (LPCTSTR)"ccf", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"�����ļ� (*.ccf)|*.ccf|All Files (*.*)|*.*||");	//�򿪶�ȡ�ļ��Ի���
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetDlgItem(IDC_EDIT_VAL1)->SetWindowText(g.ini.m_TopCamConfig);
	GetDlgItem(IDC_EDIT_VAL2)->SetWindowText(g.ini.m_BottomCamConfig);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
