#pragma once


// CImgControl 对话框

class CImgControl : public CDialogEx
{
	DECLARE_DYNAMIC(CImgControl)

public:
	CImgControl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImgControl();

// 对话框数据
	enum { IDD = IDD_DIALOG_IMGCTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonUi();
};
