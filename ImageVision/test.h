#pragma once


// test 对话框

class test : public CDialogEx
{
	DECLARE_DYNAMIC(test)

public:
	test(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~test();

// 对话框数据
	enum { IDD = IDD_DIALOG_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCofirm();
	int m_val1;
	int m_val2;
	double m_val3;
	afx_msg void OnBnClickedButtonP1();
	afx_msg void OnBnClickedButtonP2();
	virtual BOOL OnInitDialog();
};
