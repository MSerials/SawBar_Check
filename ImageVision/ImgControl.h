#pragma once


// CImgControl �Ի���

class CImgControl : public CDialogEx
{
	DECLARE_DYNAMIC(CImgControl)

public:
	CImgControl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImgControl();

// �Ի�������
	enum { IDD = IDD_DIALOG_IMGCTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonUi();
};
