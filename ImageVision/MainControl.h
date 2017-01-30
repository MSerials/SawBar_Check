#pragma once
#include "afxwin.h"
#include "SerialPort.h"

// CMainControl 对话框

class CMainControl : public CDialogEx
{
	DECLARE_DYNAMIC(CMainControl)

public:
	CMainControl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainControl();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAINCONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComBoxSel;
	virtual BOOL OnInitDialog();
//	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonIomotion();
	afx_msg void OnBnClickedButtonImgset();
	afx_msg void OnBnClickedButtonToOrigin();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	void ToOrigin();
	bool RunOrigin();
	CWinThread * pRunOrigin;
	bool isOrigin;
	static UINT pRunOriginThread(LPVOID lp);
	CSerialPort port;
	bool isPortOK;
	CSerialPort port1;
	bool isPortOK1;
public:
	afx_msg void OnSelchangeCombselprj();
	afx_msg void OnBnClickedButtonAddprj();
};
