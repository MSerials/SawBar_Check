#pragma once


// CMotionCtrl 对话框

class CMotionCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CMotionCtrl)

public:
	CMotionCtrl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMotionCtrl();

// 对话框数据
	enum { IDD = IDD_DIALOG_MCARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonComfirm();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonSavepos();
	afx_msg void OnBnClickedButtonSetzero();
	afx_msg void OnBnClickedButtonSavepos2();
	afx_msg void OnBnClickedButtonSetzero2();
	afx_msg void OnBnClickedButtonComfirm2();
	afx_msg void OnBnClickedButtonHomerun();
	afx_msg void OnBnClickedButtonHomerun2();
	afx_msg void OnBnClickedButtonCyl();
	afx_msg void OnBnClickedButtonPres();
	afx_msg void OnBnClickedButtonCancalerror();

	CWinThread *pHomeThread;
	bool isHomeThread;
	static UINT HomeThread(LPVOID lp);
	CWinThread *pHomeThread1;
	bool isHomeThread1;
	static UINT HomeThread1(LPVOID lp);
};
