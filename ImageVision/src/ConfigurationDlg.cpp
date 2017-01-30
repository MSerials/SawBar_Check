// ConfigureForm.cpp : 
//
#include "stdafx.h"
#include "../RobortImage.h"
#include "../include/ConfigurationDlg.h"
#include <tchar.h>
#ifndef UNICODE
TCHAR const * WCHAR_TO_TCHAR(WCHAR const * in, TCHAR * out)
{
   WideCharToMultiByte (CP_OEMCP,NULL,in,-1,out,MAX_DEVICE_DESC_LEN,NULL,FALSE);
   return out;
}
#else
TCHAR const * WCHAR_TO_TCHAR(WCHAR const * in, TCHAR * out)
{
   return in;
}
#endif

#ifndef UNICODE
WCHAR const * TCHAR_TO_WCHAR(TCHAR const * in,  WCHAR* out)
{
   MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)in, -1, out, MAX_DEVICE_DESC_LEN);
   return out;
}
#else
WCHAR const * TCHAR_TO_WCHAR(TCHAR const * in,  WCHAR* out)
{
   return in;
}
#endif
// CConfigureForm dialog

IMPLEMENT_DYNAMIC(CConfigurationDlg, CDlgBase)

CConfigurationDlg::CConfigurationDlg(CWnd* pParent /*=NULL*/)
: CDlgBase(L"IDD_CONFIGURATION",CConfigurationDlg::IDD,pParent)
{}

CConfigurationDlg::~CConfigurationDlg()
{

}

void CConfigurationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Device, m_comboBox_Device);
}


BEGIN_MESSAGE_MAP(CConfigurationDlg, CDialogEx)
//{{AFX_MSG_MAP(CConfigureForm)
ON_BN_CLICKED(IDOK, OnOk)
ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
ON_WM_CLOSE()
//}}AFX_MSG_MAP

END_MESSAGE_MAP()


DevConfParam CConfigurationDlg::GetParameter()
{
   DevConfParam param;
   param.deviceNumber = (int)m_comboBox_Device.GetItemData(m_comboBox_Device.GetCurSel());
   return param;
}

BOOL CConfigurationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// create available device list.
	SetControlWinText(0);
	SetControlText(3202, 2) ;
   InstantDiCtrl * instantDiCtrl = AdxInstantDiCtrlCreate();
   ICollection<DeviceTreeNode>* sptedDevices = instantDiCtrl->getSupportedDevices();

   if (sptedDevices->getCount() == 0)
   {
      GetDlgItem(IDOK)->EnableWindow(FALSE);
      GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
      AfxMessageBox(_T("No device to support the currently demonstrated function!"));
      return FALSE;
   }else{
      for (int i = 0; i < sptedDevices->getCount(); ++i)
      {
         DeviceTreeNode const & node = sptedDevices->getItem(i);
         TRACE("%d, %s\n", node.DeviceNumber, node.Description);
         TCHAR des[MAX_DEVICE_DESC_LEN];
         m_comboBox_Device.AddString(WCHAR_TO_TCHAR(node.Description,des));
         m_comboBox_Device.SetItemData(i,node.DeviceNumber);
      }
      sptedDevices->Dispose();
   }
   instantDiCtrl->Dispose();
   m_comboBox_Device.SetCurSel(0);

	return TRUE;
}

void CConfigurationDlg::OnOk()
{
   this->GetParent()->ShowWindow(SW_SHOW);
   this->GetParent()->EnableWindow(TRUE);
   this->ShowWindow(SW_HIDE);
   // Send message to configure the device selected.
   ::SendMessage(this->GetParent()->m_hWnd,WM_CUSTOME_CFG_CHGED,0,0);
	CDialogEx::OnOK();
}



void CConfigurationDlg::OnBnClickedCancel()
{
   // TODO: Add your control notification handler code here
   CString windowText;
   this->GetParent()->GetWindowText(windowText);
   if (!windowText.Compare(_T("StaticDI")))
   {
      ::SendMessage(this->GetParent()->m_hWnd,WM_CUSTOME_CFG_CHGED,1,0);
      this->CloseWindow();
   } 
   OnCancel();
}

void CConfigurationDlg::OnClose()
{
   // TODO: Add your message handler code here and/or call default
   if ( m_comboBox_Device.GetCount() <= 0)
   {
      PostQuitMessage(0);//need enable for run
   }
   CDialogEx::OnClose();
}