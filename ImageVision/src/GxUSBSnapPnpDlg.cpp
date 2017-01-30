// src\GxUSBSnapPnpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\RobortImage.h"
#include "..\include\GxUSBSnapPnpDlg.h"
#include "afxdialogex.h"


// CGxUSBSnapPnpDlg 对话框

IMPLEMENT_DYNAMIC(CGxUSBSnapPnpDlg, CDialogEx)

CGxUSBSnapPnpDlg::CGxUSBSnapPnpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGxUSBSnapPnpDlg::IDD, pParent)
{

}

CGxUSBSnapPnpDlg::~CGxUSBSnapPnpDlg()
{
}

void CGxUSBSnapPnpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESULT, m_listResult);
}


BEGIN_MESSAGE_MAP(CGxUSBSnapPnpDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CGxUSBSnapPnpDlg 消息处理程序


BOOL CGxUSBSnapPnpDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN)) 
	{		
		return TRUE;
	}   
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))  
	{   
		return   TRUE; 
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


LRESULT CGxUSBSnapPnpDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(message == WM_DEVICECHANGE)
	{
		switch(wParam) 
		{
		case DBT_DEVICEARRIVAL:                  //检测到有设备接入
			{  
				PDEV_BROADCAST_HDR  devHdr;
				devHdr = (PDEV_BROADCAST_HDR)lParam;

				// 获取当前USB端口号名称
				PDEV_BROADCAST_PORT port = (PDEV_BROADCAST_PORT)(lParam);
				CString str = L"";
				str.Format(L"%s",port->dbcp_name);

				switch (devHdr->dbch_devicetype)
				{
				case DBT_DEVTYP_DEVICEINTERFACE:  //接入的为已注册的设备
					{
						if (IsSnapPnpSuccess(str,DBT_DEVICEARRIVAL))
						{
							// 有设备热插拔成功则更新界面
							UpdateDeviceList();
						}
					}
					break;
				default:
					break;
				}				
			}
			break;

		case DBT_DEVICEREMOVECOMPLETE:            //检测到有设备拔出
			{
				PDEV_BROADCAST_HDR  devHdr;
				devHdr = (PDEV_BROADCAST_HDR)lParam;

				// 获取当前USB端口号名称
				PDEV_BROADCAST_PORT port = (PDEV_BROADCAST_PORT)(lParam);
				CString str = L"";
				str.Format(L"%s",port->dbcp_name);

				switch ( devHdr->dbch_devicetype )
				{
				case DBT_DEVTYP_DEVICEINTERFACE :  // 拔出的为以注册的设备
					{
						if (IsSnapPnpSuccess(str,DBT_DEVICEREMOVECOMPLETE))
						{
							// 判断设备热插拔成功则更新界面
							UpdateDeviceList();
						}
					}
					break;
				default:
					break;
				}
			}
			break;

		default:
			break;
		}	
	}
	return CDialogEx::DefWindowProc(message, wParam, lParam);
}


BOOL CGxUSBSnapPnpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int i = 0;

	// 初始化可识别设备的GUID
	static GUID CameraGUIDs[] = 
	{	
		//GUID
		{0x889ff8a4, 0x40c8, 0x47bc, {0x99, 0x1e, 0x65, 0xc3, 0xb6, 0x83, 0x1f, 0x68}}
	}; 

	// 循环注册设备的GUID
	for(i = 0; i < 9; i++)
	{
		RegHVSnapPnpGUID(m_hWnd,CameraGUIDs[i]);
	}

	/// 初始化List表头信息
	InitListHeader();

//	GX_STATUS  emStatus = GX_STATUS_SUCCESS;
	/// 初始化设备库
//	emStatus = GXInitLib();  
//	if (emStatus != GX_STATUS_SUCCESS)
//	{
//		MessageBox(L"初始化设备库失败！");
//		return FALSE;
//	}

	//初始运行程序时，先枚举一次设备并更新设备信息到界面
	UpdateDeviceList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

CUSBPort::CUSBPort()
:m_emStatus(STATUS_STEADY)
{

}
CUSBPort::~CUSBPort()
{

}

//----------------------------------------------------------------------------------
/**
\brief   更新当前USB端口连接设备的状态
\note    设备接入时还需要进行一次自启动(相当于一次插拔操作)后才会稳定,因此接入设备的过程为:
            首次接入(STATUS_FIRST_ARRIVAL)
		 -->自启动时的拔出(STATUS_MID_REMOVE)
		 -->第二次接入,此时设备已稳定(STATUS_STEADY)
         
\param   message  [in]    标识接入消息或拔出消息

\return  无
*/
//----------------------------------------------------------------------------------
void CUSBPort::ChangeCurStatus(UINT message)
{
	switch(m_emStatus)
	{
	case STATUS_STEADY:
		{
			if (message == DBT_DEVICEARRIVAL)
			{
				m_emStatus  = STATUS_FIRST_ARRIVAL;
			}
		}
		break;
	case STATUS_MID_REMOVE:
		m_emStatus = STATUS_STEADY;
		break;
	case STATUS_FIRST_ARRIVAL:
        m_emStatus = STATUS_MID_REMOVE;
		break;
	default:
		break;
	}
}

//----------------------------------------------------------------------------------
/**
\brief   获取当前USB端口连接设备的状态

\return  m_emStatus 连接状态
*/
//----------------------------------------------------------------------------------
DEVICE_CHANGE_STATUS CUSBPort::GetCurStatus()
{
	return m_emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief   创建CUSBPort对象
\Param   strUSBPortName     [in ]    USB端口名称

\return  pUSBPort    指向CUSBPort对象,若端口已被创建,则指向的为已创建的对象
*/
//----------------------------------------------------------------------------------
CUSBPort *CGxUSBSnapPnpDlg::GetUSBPortObject(CString strUSBPortName)
{
	CUSBPort *pUSBPort = NULL;
	std::map<CString, CUSBPort*>::iterator it;

	it = m_mapUSBPortList.find(strUSBPortName); 
	if (it == m_mapUSBPortList.end())
	{
		// strUSBPortName标识端口不存在,则New新的对象
		pUSBPort = new CUSBPort;
		m_mapUSBPortList.insert(std::make_pair(strUSBPortName,pUSBPort));
	}
	else
	{
		pUSBPort = it->second;
	}

	return pUSBPort;
}

//----------------------------------------------------------------------------------
/**
\brief  初始化列表表头

\return 无
*/
//----------------------------------------------------------------------------------
void CGxUSBSnapPnpDlg::InitListHeader()
{
	//设置表格样式
	m_listResult.SetExtendedStyle(LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//添加表头
	m_listResult.InsertColumn(0 , L"序号", LVCFMT_LEFT, 110);
	m_listResult.InsertColumn(1 , L"名称", LVCFMT_LEFT, 310);
}

//----------------------------------------------------------------------------------
/**
\brief   判断当前USB端口连接设备是否热插拔成功
\Param   strUSBPortName     [in]    USB端口名称
\Param   message            [in]    标识接入消息或拔出消息

\return  TRUE:成功   FALSE:还未完成
*/
//----------------------------------------------------------------------------------
BOOL CGxUSBSnapPnpDlg::IsSnapPnpSuccess(CString strUSBPortName, UINT message)
{
	CUSBPort *pUSBPort = NULL;

	// 获取StrUSBPortName标识USB端口对象指针
	pUSBPort = GetUSBPortObject(strUSBPortName);

	// 根据message更新USBPort状态
	pUSBPort->ChangeCurStatus(message);

	// 当前连接状态为STATUS_STEADY时表示已热插拔成功
	if (pUSBPort->GetCurStatus() == STATUS_STEADY)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}

void CGxUSBSnapPnpDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 释放资源库
//	emStatus = GXCloseLib();
//	if (emStatus != GX_STATUS_SUCCESS)
//	{
//		MessageBox(L"GXCloseLib:释放资源库失败");
//	}

	// 释放程序资源
	std::map<CString, CUSBPort*>::iterator it = m_mapUSBPortList.begin();
	for (; it != m_mapUSBPortList.end(); it++)
	{
		CUSBPort *pTemp = it->second;
		if (pTemp != NULL)
		{
			delete pTemp;
			pTemp = NULL;
		}
	}
	m_mapUSBPortList.clear();
	CDialogEx::OnClose();
}
//----------------------------------------------------------------------------------
/**
\brief  注册相机型号，在指定窗口返回相关信息
\param  handle   [in]  窗口句柄
\param  stGuid   [in]  设备的GUID

\return false：注册失败；ture：注册成功 
*/
//----------------------------------------------------------------------------------
BOOL CGxUSBSnapPnpDlg::RegHVSnapPnpGUID(HANDLE handle, GUID stGuid)
{
	BOOL bReturn = FALSE;
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
	ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	NotificationFilter.dbcc_classguid = stGuid; 
	HDEVNOTIFY hDevInterfaceNotify = ::RegisterDeviceNotification(handle, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
	if (hDevInterfaceNotify)
	{
		bReturn = TRUE;
	}

	return bReturn;
}

//----------------------------------------------------------------------------------
/**
\brief  枚举设备,并将枚举到的设备信息更新到界面上

\return 无
*/
//----------------------------------------------------------------------------------
void CGxUSBSnapPnpDlg::UpdateDeviceList()
{
	GX_STATUS            emStatus     = GX_STATUS_ERROR;  
	size_t               nSize        = 0;
	uint32_t             nDeviceNum   = 0;                   ///< 获取到的设备数
	GX_DEVICE_BASE_INFO *pDeviceInfo  = NULL;                ///< 存储设备信息
	CString              strTemp      = L"";

	//清空List表
	m_listResult.DeleteAllItems();

	//枚举设备
	emStatus = GXUpdateDeviceList(&nDeviceNum, 2000);	
	if (emStatus == GX_STATUS_SUCCESS)		
	{	
		if (nDeviceNum != 0)
		{
			// 为保存设备信息申请资源
			pDeviceInfo = new GX_DEVICE_BASE_INFO[nDeviceNum];
			if (pDeviceInfo == NULL)
			{
				MessageBox(L"分配缓冲区失败！");
				return;
			}
			nSize = nDeviceNum * sizeof(GX_DEVICE_BASE_INFO);	

			//获取所有设备的基础信息		
			emStatus = GXGetAllDeviceBaseInfo(pDeviceInfo, &nSize);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				if (pDeviceInfo != NULL)
				{
					delete[]pDeviceInfo;
					pDeviceInfo = NULL;
				}
				MessageBox(L"获取设备信息失败!");
				return;
			}

			//将获取到的设备信息更新到界面
			GX_DEVICE_BASE_INFO *pTempBaseInfo = pDeviceInfo;
			for (int i = 0; i < (int)nDeviceNum; i++)
			{
				strTemp.Format(L"%d",i);
				m_listResult.InsertItem(i,strTemp);
				strTemp.Format(L"%s",A_to_W(pTempBaseInfo->szDisplayName).c_str());
				m_listResult.SetItemText(i,1,strTemp);
				pTempBaseInfo++;
			}

		}
	}
	else
	{
		MessageBox(L"获取设备信息失败!");
		return;
	}

	// 释放函数资源
	if (pDeviceInfo != NULL)
	{
		delete[]pDeviceInfo;
		pDeviceInfo = NULL;
	}
}
