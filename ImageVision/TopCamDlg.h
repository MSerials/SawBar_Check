#pragma once


// CTopCamDlg �Ի���

class CTopCamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTopCamDlg)

public:
	CTopCamDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTopCamDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_TOPCAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonSelTroi();
	afx_msg void OnBnClickedButtonShowTroi();
	afx_msg void OnBnClickedButtonShowTsavemodel();
	afx_msg void OnBnClickedButtonSelall();
	afx_msg void OnSelchangeComboModelsel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonShowModel();
	afx_msg void OnBnClickedButtonTrainmodel();
private:
	bool RunTrain();
	CWinThread *pRunTrain;
	bool isTrain;
	static UINT RunTrainThread(LPVOID lp);
	bool train();
	vector<CString> get_train_img_path();
	bool train_img(vector<CString> str);


	CWinThread *pCap;
	bool isCap;
	static UINT RunCapThread(LPVOID);
	
public:
	afx_msg void OnBnClickedButtonCap();
};
