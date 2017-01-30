#pragma once


// CTabControl
#include "BottomDlg.h"
#include "TopCamDlg.h"
#include "MainControl.h"

class CTabControl : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabControl)

public:
	CTabControl();
	virtual ~CTabControl();
	void InitData();


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);



private:
	CBottomDlg		m_BottomDlg;
	CTopCamDlg		m_TopDlg;
	CMainControl	m_MainCtrl;

};


