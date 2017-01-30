// src/StaticDIDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../RobortImage.h"
//#include "../include/StaticDIDlg.h"
#include "afxdialogex.h"
#include "../RobortImageDlg.h"

// CStaticDIDlg 对话框

IMPLEMENT_DYNAMIC(CStaticDIDlg, CDialogEx)
CStaticDIDlg* pStaticDIDlg=NULL;

CStaticDIDlg::CStaticDIDlg(CWnd* pParent /*=NULL*/): CDlgBase(L"IDD_STATICDIDLG",CStaticDIDlg::IDD, pParent)
{
	pStaticDIDlg=this;
	m_portData =  NULL;
	m_portCount = 0;
	m_portNum0 = 0;
	memset(&m_confParam, 0, sizeof(m_confParam));
}



void CStaticDIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CStaticDIDlg, CDialogEx)
	ON_WM_PAINT()
	ON_CONTROL_RANGE(STN_CLICKED,IDC_PCIT000,IDC_PCIT017, OnClick)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CStaticDIDlg message handlers

// CStaticDIDlg 消息处理程序

BOOL CStaticDIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int ports[2][12] = {{IDC_PORTNUM0,IDC_PORTVAL0,IDC_GROUPBOX00,IDC_GROUPBOX01,IDC_PICT00,IDC_PCIT01,IDC_PCIT02,IDC_PCIT03,IDC_PCIT04,IDC_PCIT05,IDC_PCIT06,IDC_PCIT07},       
	{IDC_PORTNUM1,IDC_PORTVAL1,IDC_GROUPBOX10,IDC_GROUPBOX11,IDC_PCIT10,IDC_PCIT11,IDC_PCIT12,IDC_PCIT13,IDC_PCIT14,IDC_PCIT15,IDC_PCIT16,IDC_PCIT17},
	};

	for ( int i = 0; i < 2; i++ )
	{
		for ( int j = 0; j < 12; j++ )
		{
			m_Iports[i][j] = ports[i][j]; 
		}
	}

	int Oports[2][12] = {{IDC_PORTNUM00,IDC_PORTVAL00,IDC_GROUPBOX000,IDC_GROUPBOX001,IDC_PCIT000,IDC_PCIT001,IDC_PCIT002,IDC_PCIT003,IDC_PCIT004,IDC_PCIT005,IDC_PCIT006,IDC_PCIT007},       
	{IDC_PORTNUM01,IDC_PORTVAL10,IDC_GROUPBOX001,IDC_GROUPBOX011,IDC_PCIT010,IDC_PCIT011,IDC_PCIT012,IDC_PCIT013,IDC_PCIT014,IDC_PCIT015,IDC_PCIT016,IDC_PCIT017},
	};

	for ( int i = 0; i < 2; i++ )
	{
		for ( int j = 0; j < 12; j++ )
		{
			m_ports[i][j] = Oports[i][j]; 
		}
	}

	if(pGlobalDialog!=NULL)
	{
		m_instantDiCtrl = pGlobalDialog->m_instantDiCtrl;
		m_instantDoCtrl = pGlobalDialog->m_instantDoCtrl;
		m_confParam=pGlobalDialog->m_confParam;
	}
	else
	{
		AfxMessageBox(L"开启程序失败，请重试！");
		PostQuitMessage(0);
		return FALSE;
	}
	//	ErrorCode	errorCode;
	DeviceInformation devInfo(m_confParam.deviceNumber);
	ErrorCode errorCode = m_instantDiCtrl->setSelectedDevice(devInfo);
	if(errorCode!= Success)
	{
		//CheckError(errorCode);
		return false;
	}
	m_instantDiCtrl->getSelectedDevice(devInfo);
	errorCode = m_instantDoCtrl->setSelectedDevice(devInfo);
	if(errorCode!= Success)
		return false;//CheckError(errorCode);
	m_instantDoCtrl->getSelectedDevice(devInfo);

	m_IportCount=m_instantDiCtrl->getFeatures()->getPortCount();
	m_portData = new byte[m_IportCount];
	memset(m_portData,0,m_IportCount);
	//if(pGlobalDialog!=NULL)m_instantDiCtrl=pGlobalDialog->m_instantDiCtrl;
	m_imageList[0] = IDB_LEDLOW;
	m_imageList[1] = IDB_LEDHIGH;
	m_imageList[2] = IDB_BUTTONUP;
	m_imageList[3] = IDB_BUTTONDOWN;
	m_imageList[4] = IDB_BUTTONDISABLE;
	SetTimer(0,50,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void CStaticDIDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDlgBase::OnPaint()
	
	CRect   rect;   
	GetClientRect(&rect);   
	CDC   dcMem;   
	dcMem.CreateCompatibleDC(&dc);   
	CBitmap   bmpBackground;   
	bmpBackground.LoadBitmap(IDB_BACK);     
	BITMAP   bitmap;   
	bmpBackground.GetBitmap(&bitmap);   
	CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);

	//显示图片
	dc.StretchBlt(-38, -30, bitmap.bmWidth, bitmap.bmHeight, &dcMem, 
		0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);  
	// draw rect
	CBrush brush;   
	brush.CreateSolidBrush(RGB(236,233,216));       
	CBrush* pOldBrush = dc.SelectObject(&brush); 
	dc.Rectangle(14,58,393,220);   
	dc.SelectObject(pOldBrush); 
	brush.DeleteObject(); 
}

void CStaticDIDlg::OnClick(UINT nID)
{
	int clickCh = nID - IDC_PCIT000;
	int clickPort = clickCh/8 ;

	m_picBoxStatus[clickCh] = (0 == m_picBoxStatus[clickCh])?1:0;
	showPicture(nID,(0 == m_picBoxStatus[clickCh])?2:3);
	byte portData = 0;
	for ( int i = 0; i < 8; i++)
	{   
		if ( 1 == m_picBoxStatus[8*clickPort+i])
		{
			portData = (byte)(portData | (1 << i));
		}
	}

	CString portVal;
	portVal.Format(_T("%X"),portData);  
	GetDlgItem(m_ports[clickPort][1])->SetWindowText(portVal);

	ErrorCode errorCode;
	if(m_instantDoCtrl!=NULL)
		errorCode= m_instantDoCtrl->Write(clickPort+m_portNum0, portData);
	else
		errorCode=ErrorDeviceNotOpened;
	if(errorCode != Success)
	{
		CString str;
		str.Format(_T("I/O卡操作出错, 错误代码为: 0x%X !\n"),errorCode);
		AfxMessageBox(str);
		
	}
}

void CStaticDIDlg::showPicture( int nID, int imageNum )
{
	CBitmap   bmp;   
	bmp.LoadBitmap(m_imageList[imageNum]); 
	((CStatic *)GetDlgItem(nID))->SetBitmap((HBITMAP)bmp); 
	bmp.Detach();
	CRect rc;
	GetDlgItem(nID)->GetClientRect(&rc);
	InvalidateRect(&rc,false);
}

void CStaticDIDlg::refreshButton()
{

	int portCount = m_instantDoCtrl->getFeatures()->getPortCount();
	byte * portValue = new byte[portCount];
	ErrorCode errorCode; 
	if(m_instantDoCtrl!=NULL)
		errorCode= m_instantDoCtrl->Read(0, portCount, portValue);
	else
		errorCode=ErrorDeviceNotOpened;
	if(errorCode != Success)
	{
		CString str;
		str.Format(_T("I/O卡刷新出错, 错误代码为: 0x%X !\n"),errorCode);
		AfxMessageBox(str);

	}

	for ( int k = 0; k < portCount; k++ )
	{			

		m_portNum0 = k;
		for ( int i = 0; i < 2 ; i++ )
		{	
			int portNumber = i;		   
			CString portNum;
			portNum.Format(_T("%d"), portNumber);  
			GetDlgItem(m_ports[i][0])->SetWindowText(portNum);

			CString portVal;
			portVal.Format(_T("%X"),portValue[portNumber]);  
			GetDlgItem(m_ports[i][1])->SetWindowText(portVal);

			byte portData = 0;
			for (int j=0; j < 12; j++)
			{
				portData = drawButton( i, j, errorCode, portNumber,portValue);
			}
			if ( Success == errorCode )
			{		
				CString portVal;
				portVal.Format(_T("%X"),portData);  
				GetDlgItem(m_ports[i][1])->SetWindowText(portVal);
			}
		}
		
		
	}
	delete [] portValue;
}

byte CStaticDIDlg::drawButton(int i, int j, ErrorCode ret, int  portNumber,byte * portValue)
{	
	int dicBitValue;
	int dataBitValue;
	byte portData = 0;
	if ( j > 3)
	{ 
		ICollection<uint8>* doDataMask = m_instantDoCtrl->getFeatures()->getDataMask();
		uint8 dataMask = doDataMask->getItem(portNumber);

		dicBitValue = (dataMask >>(j-4)) & 0x1;
		dataBitValue = (portValue[portNumber] >>(j-4)) & 0x1;

		if ( Success == ret )
		{
			if ( 0 == dicBitValue )
			{
				showPicture(m_ports[i][j],2);
				GetDlgItem(m_ports[i][j])->EnableWindow(FALSE);
				m_picBoxStatus[i*8+j-4] = 2;
			}
			else
			{	
				if ( 0 == dataBitValue )
				{
					GetDlgItem(m_ports[i][j])->EnableWindow(TRUE);
					showPicture(m_ports[i][j],0);
					m_picBoxStatus[i*8+j-4] = 0;
				}
				else
				{
					if ( 1 == dataBitValue )
					{
						GetDlgItem(m_ports[i][j])->EnableWindow(TRUE);
						showPicture(m_ports[i][j],1);
						m_picBoxStatus[i*8+j-4] = 1;
					}
				}
				if ( 1 == dicBitValue && 1 == dataBitValue )
				{
					portData = (byte)(portData | (1 << (j-4)));
				}
			}
		}
		else
		{
			if ( 0 == dataBitValue )
			{
				GetDlgItem(m_ports[i][j])->EnableWindow(true);
				showPicture(m_ports[i][j],0);
				m_picBoxStatus[i*8+j-4] = 0;
			}
			else
			{
				if ( 1 == dataBitValue )
				{
					GetDlgItem(m_ports[i][j])->EnableWindow(true);
					showPicture(m_ports[i][j],1);
					m_picBoxStatus[i*8+j-4] = 1;
				}
			}
		}
	}
	return portData;
}

void CStaticDIDlg::OnTimer(UINT_PTR nIDEvent)
{
	refreshButton();
	return;
	byte* portData = new byte[m_IportCount];
	for ( int k = 0; k < m_IportCount; k++ )
	{
		portData[k] = m_portData[k];
	}
	ErrorCode errorCode = m_instantDiCtrl->Read(0, m_IportCount, m_portData);
	if(errorCode!=Success)
	{
		TRACE("刷新输入数据出错！");
		if(portData!=NULL)
		{
			delete[]portData;
			portData=NULL;
		}
		return;
	}//CheckError(errorCode);

	CString portVal; 

	int newBitValue = 0;
	int oldBitValue = 0;
	int portCount = 0;
	portCount=m_IportCount;
	for ( int i = 0; i < 2; i++ )
	{
		for ( int j = 0; j < 12; j++ )
		{		 
			if ( j > 3)
			{  
				oldBitValue = (portData[i] >>(j-4)) & 0x1;
				newBitValue = (m_portData[i] >>(j-4)) & 0x1;
				if ( newBitValue != oldBitValue )
				{
					CBitmap  bmp;   
					bmp.LoadBitmap(m_imageList[newBitValue]); 
					((CStatic*)GetDlgItem(m_Iports[i][j]))->SetBitmap((HBITMAP)bmp);
					bmp.Detach();
					CRect rc;
					GetDlgItem(m_Iports[i][j])->GetClientRect(&rc);
					InvalidateRect(&rc,false);
					//refresh the port value
					portVal.Format(_T("%X"),m_portData[i]);  
					GetDlgItem(m_Iports[i][1])->SetWindowText(portVal);
				}
			}
		}
	}
	if ( portData != NULL)
	{
		delete []portData;
		portData = NULL;
	}
	CDialog::OnTimer(nIDEvent);
}