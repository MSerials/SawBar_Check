// src\GxUSBSnapPnpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\RobortImage.h"
#include "..\include\GxUSBSnapPnpDlg.h"
#include "afxdialogex.h"


// CGxUSBSnapPnpDlg �Ի���

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


// CGxUSBSnapPnpDlg ��Ϣ�������


BOOL CGxUSBSnapPnpDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
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
	// TODO: �ڴ����ר�ô����/����û���
	if(message == WM_DEVICECHANGE)
	{
		switch(wParam) 
		{
		case DBT_DEVICEARRIVAL:                  //��⵽���豸����
			{  
				PDEV_BROADCAST_HDR  devHdr;
				devHdr = (PDEV_BROADCAST_HDR)lParam;

				// ��ȡ��ǰUSB�˿ں�����
				PDEV_BROADCAST_PORT port = (PDEV_BROADCAST_PORT)(lParam);
				CString str = L"";
				str.Format(L"%s",port->dbcp_name);

				switch (devHdr->dbch_devicetype)
				{
				case DBT_DEVTYP_DEVICEINTERFACE:  //�����Ϊ��ע����豸
					{
						if (IsSnapPnpSuccess(str,DBT_DEVICEARRIVAL))
						{
							// ���豸�Ȳ�γɹ�����½���
							UpdateDeviceList();
						}
					}
					break;
				default:
					break;
				}				
			}
			break;

		case DBT_DEVICEREMOVECOMPLETE:            //��⵽���豸�γ�
			{
				PDEV_BROADCAST_HDR  devHdr;
				devHdr = (PDEV_BROADCAST_HDR)lParam;

				// ��ȡ��ǰUSB�˿ں�����
				PDEV_BROADCAST_PORT port = (PDEV_BROADCAST_PORT)(lParam);
				CString str = L"";
				str.Format(L"%s",port->dbcp_name);

				switch ( devHdr->dbch_devicetype )
				{
				case DBT_DEVTYP_DEVICEINTERFACE :  // �γ���Ϊ��ע����豸
					{
						if (IsSnapPnpSuccess(str,DBT_DEVICEREMOVECOMPLETE))
						{
							// �ж��豸�Ȳ�γɹ�����½���
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	int i = 0;

	// ��ʼ����ʶ���豸��GUID
	static GUID CameraGUIDs[] = 
	{	
		//GUID
		{0x889ff8a4, 0x40c8, 0x47bc, {0x99, 0x1e, 0x65, 0xc3, 0xb6, 0x83, 0x1f, 0x68}}
	}; 

	// ѭ��ע���豸��GUID
	for(i = 0; i < 9; i++)
	{
		RegHVSnapPnpGUID(m_hWnd,CameraGUIDs[i]);
	}

	/// ��ʼ��List��ͷ��Ϣ
	InitListHeader();

//	GX_STATUS  emStatus = GX_STATUS_SUCCESS;
	/// ��ʼ���豸��
//	emStatus = GXInitLib();  
//	if (emStatus != GX_STATUS_SUCCESS)
//	{
//		MessageBox(L"��ʼ���豸��ʧ�ܣ�");
//		return FALSE;
//	}

	//��ʼ���г���ʱ����ö��һ���豸�������豸��Ϣ������
	UpdateDeviceList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
\brief   ���µ�ǰUSB�˿������豸��״̬
\note    �豸����ʱ����Ҫ����һ��������(�൱��һ�β�β���)��Ż��ȶ�,��˽����豸�Ĺ���Ϊ:
            �״ν���(STATUS_FIRST_ARRIVAL)
		 -->������ʱ�İγ�(STATUS_MID_REMOVE)
		 -->�ڶ��ν���,��ʱ�豸���ȶ�(STATUS_STEADY)
         
\param   message  [in]    ��ʶ������Ϣ��γ���Ϣ

\return  ��
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
\brief   ��ȡ��ǰUSB�˿������豸��״̬

\return  m_emStatus ����״̬
*/
//----------------------------------------------------------------------------------
DEVICE_CHANGE_STATUS CUSBPort::GetCurStatus()
{
	return m_emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief   ����CUSBPort����
\Param   strUSBPortName     [in ]    USB�˿�����

\return  pUSBPort    ָ��CUSBPort����,���˿��ѱ�����,��ָ���Ϊ�Ѵ����Ķ���
*/
//----------------------------------------------------------------------------------
CUSBPort *CGxUSBSnapPnpDlg::GetUSBPortObject(CString strUSBPortName)
{
	CUSBPort *pUSBPort = NULL;
	std::map<CString, CUSBPort*>::iterator it;

	it = m_mapUSBPortList.find(strUSBPortName); 
	if (it == m_mapUSBPortList.end())
	{
		// strUSBPortName��ʶ�˿ڲ�����,��New�µĶ���
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
\brief  ��ʼ���б��ͷ

\return ��
*/
//----------------------------------------------------------------------------------
void CGxUSBSnapPnpDlg::InitListHeader()
{
	//���ñ����ʽ
	m_listResult.SetExtendedStyle(LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//��ӱ�ͷ
	m_listResult.InsertColumn(0 , L"���", LVCFMT_LEFT, 110);
	m_listResult.InsertColumn(1 , L"����", LVCFMT_LEFT, 310);
}

//----------------------------------------------------------------------------------
/**
\brief   �жϵ�ǰUSB�˿������豸�Ƿ��Ȳ�γɹ�
\Param   strUSBPortName     [in]    USB�˿�����
\Param   message            [in]    ��ʶ������Ϣ��γ���Ϣ

\return  TRUE:�ɹ�   FALSE:��δ���
*/
//----------------------------------------------------------------------------------
BOOL CGxUSBSnapPnpDlg::IsSnapPnpSuccess(CString strUSBPortName, UINT message)
{
	CUSBPort *pUSBPort = NULL;

	// ��ȡStrUSBPortName��ʶUSB�˿ڶ���ָ��
	pUSBPort = GetUSBPortObject(strUSBPortName);

	// ����message����USBPort״̬
	pUSBPort->ChangeCurStatus(message);

	// ��ǰ����״̬ΪSTATUS_STEADYʱ��ʾ���Ȳ�γɹ�
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// �ͷ���Դ��
//	emStatus = GXCloseLib();
//	if (emStatus != GX_STATUS_SUCCESS)
//	{
//		MessageBox(L"GXCloseLib:�ͷ���Դ��ʧ��");
//	}

	// �ͷų�����Դ
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
\brief  ע������ͺţ���ָ�����ڷ��������Ϣ
\param  handle   [in]  ���ھ��
\param  stGuid   [in]  �豸��GUID

\return false��ע��ʧ�ܣ�ture��ע��ɹ� 
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
\brief  ö���豸,����ö�ٵ����豸��Ϣ���µ�������

\return ��
*/
//----------------------------------------------------------------------------------
void CGxUSBSnapPnpDlg::UpdateDeviceList()
{
	GX_STATUS            emStatus     = GX_STATUS_ERROR;  
	size_t               nSize        = 0;
	uint32_t             nDeviceNum   = 0;                   ///< ��ȡ�����豸��
	GX_DEVICE_BASE_INFO *pDeviceInfo  = NULL;                ///< �洢�豸��Ϣ
	CString              strTemp      = L"";

	//���List��
	m_listResult.DeleteAllItems();

	//ö���豸
	emStatus = GXUpdateDeviceList(&nDeviceNum, 2000);	
	if (emStatus == GX_STATUS_SUCCESS)		
	{	
		if (nDeviceNum != 0)
		{
			// Ϊ�����豸��Ϣ������Դ
			pDeviceInfo = new GX_DEVICE_BASE_INFO[nDeviceNum];
			if (pDeviceInfo == NULL)
			{
				MessageBox(L"���仺����ʧ�ܣ�");
				return;
			}
			nSize = nDeviceNum * sizeof(GX_DEVICE_BASE_INFO);	

			//��ȡ�����豸�Ļ�����Ϣ		
			emStatus = GXGetAllDeviceBaseInfo(pDeviceInfo, &nSize);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				if (pDeviceInfo != NULL)
				{
					delete[]pDeviceInfo;
					pDeviceInfo = NULL;
				}
				MessageBox(L"��ȡ�豸��Ϣʧ��!");
				return;
			}

			//����ȡ�����豸��Ϣ���µ�����
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
		MessageBox(L"��ȡ�豸��Ϣʧ��!");
		return;
	}

	// �ͷź�����Դ
	if (pDeviceInfo != NULL)
	{
		delete[]pDeviceInfo;
		pDeviceInfo = NULL;
	}
}
