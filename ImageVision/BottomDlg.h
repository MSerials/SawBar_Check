#pragma once


// CBottomDlg 对话框

class CBottomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBottomDlg)

public:
	CBottomDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBottomDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_BOTTOMCAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelBroi();
	afx_msg void OnBnClickedButtonShowBroi();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonBshowTsavemodel();
	afx_msg void OnBnClickedButtonBselall();
	afx_msg void OnBnClickedButtonBtrainmodel();
private:
	bool RunTrain();
	CWinThread *pRunTrain;
	bool isRunTrain;
	static UINT RunTrainThread(LPVOID lp);

	CWinThread *pCap;
	bool isCap;
	static UINT RunCapThread(LPVOID lp);


	bool train();
	vector<CString> get_train_img_path();
	bool train_img(vector<CString> str);

	

public:
	afx_msg void OnBnClickedButtonBcap();
	afx_msg void OnBnClickedButtonTest();
};


extern CBottomDlg* BottomDlg;