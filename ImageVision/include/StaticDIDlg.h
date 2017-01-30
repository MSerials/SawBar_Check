#pragma once

//#include "resource.h"
// CStaticDIDlg 对话框
//#define IDD_STATICDIDLG 234
class CStaticDIDlg : public CDlgBase
{
	DECLARE_DYNAMIC(CStaticDIDlg)

public:
	CStaticDIDlg(CWnd* pParent =NULL);   // 标准构造函数
	virtual ~CStaticDIDlg()
	{
		if(m_portData!=NULL)
		{
			delete[]m_portData;
			m_portData=NULL;
		}
	}

// 对话框数据
	enum { IDD = IDD_STATICDI_DIALOG };

public:

int m_imageList[5]; 
	InstantDiCtrl *      m_instantDiCtrl;
	InstantDoCtrl *      m_instantDoCtrl;
	DevConfParam         m_confParam; // the device's configure information from config form.
	byte* m_portData;
	int m_Iports[8][12];

	int m_IpicBoxStatus[64]; 
	int m_IportCount;
	int m_IportNum0;
	
	//DO
	int m_ports[2][12];
	int m_picBoxStatus[16]; 
	int m_portCount;
	int m_portNum0;
	void showPicture(int nID, int imageNum); 
	void refreshButton();
	byte drawButton(int i, int j, ErrorCode ret, int  portNumber,byte * portValue);	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClick(UINT nID);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

};
extern CStaticDIDlg* pStaticDIDlg;