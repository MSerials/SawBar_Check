#include "StdAfx.h"
#include "..\include\DahengImavision.h"


CDahengImavision::CDahengImavision(void)
{
	m_hDevices = NULL;
	m_pBaseinfo = NULL;
	m_pstCam = NULL;
	m_npfpsCont = NULL;
	m_stFrameDatas=NULL;
	
	m_nCamNum=0;
}


CDahengImavision::~CDahengImavision(void)
{
}

//----------------------------------------------------------------------------------
/**
\brief  �����ر��������

\return �޷���ֵ
*/
//----------------------------------------------------------------------------------
GX_STATUS CDahengImavision::CloseCamer()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	uint32_t  i = 0;

	//if(m_nCamNum==0)
	//	return GX_STATUS_NOT_FOUND_DEVICE;
	//������û�رգ���ر����
	for(i = 0; i < (uint32_t)m_nCamNum; i++)
	{
		if(m_pstCam[i].bIsSnap)
		{
			//ֹͣ�ɼ�
			emStatus = GXSendCommand(m_hDevices[i], GX_COMMAND_ACQUISITION_STOP);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				return emStatus;
			}

			//ע���ص�
			emStatus = GXUnregisterCaptureCallback(m_hDevices[i]);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				return emStatus;
			}
			m_pstCam[i].bIsSnap = FALSE;
		}
		if(m_pstCam[i].bIsOpen)
		{
			//�ر����
			emStatus = GXCloseDevice(m_hDevices[i]);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				return emStatus;
			}

			m_hDevices[i] = NULL;
			m_pstCam[i].bIsOpen = FALSE;
		}
	}

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ���Ϊ�����豸�����Ϣ�������Դ

\return ��
*/
//----------------------------------------------------------------------------------
void CDahengImavision::ClearBuffer()
{

	// �����豸���
	if(m_hDevices != NULL)
	{
		delete[]m_hDevices;
		m_hDevices = NULL;
	}

	// �����豸��Ϣ
	if(m_pBaseinfo != NULL)
	{
		delete[]m_pBaseinfo;
		m_pBaseinfo = NULL;
	}

	// �Զ����豸��Ϣ�ṹ��
	if(m_pstCam != NULL)
	{
		delete[]m_pstCam;
		m_pstCam = NULL;
	}

	// ֡����Ϣ
	if(m_npfpsCont!=NULL)
	{
		delete[]m_npfpsCont;
		m_npfpsCont = NULL;
	}
	if(m_stFrameDatas!=NULL)
	{
		delete[] m_stFrameDatas;
		m_stFrameDatas=NULL;
	}
	m_nCamNum=0;
}

//----------------------------------------------------------------------------------
/**
\brief  ����������Ϣ
\param  status  ������

\return ��
*/
//----------------------------------------------------------------------------------
void CDahengImavision::ShowErrorString(GX_STATUS emErrorStatus)
{
	char      chErrorInof[512] = {0};
	size_t    nSize            = 512;
	GX_STATUS emStatus         = GX_STATUS_ERROR;

	emStatus = GXGetLastError (&emErrorStatus, chErrorInof, &nSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		AfxMessageBox(L"GXGetLastError�ӿڵ���ʧ�ܣ�");
	}
	else
	{
		AfxMessageBox((A_to_W(chErrorInof)).c_str());
	}	
}

//----------------------------------------------------------------------------------
/**
\brief  ö���豸,��ȡ�豸����

\return ��
*/
//----------------------------------------------------------------------------------
void CDahengImavision::UpdateDeviceList()
{
	GX_STATUS  emStatus   = GX_STATUS_SUCCESS;
	uint32_t   nDeviceNum = 0;
	bool       bRet       = true;
	size_t     nSize      = 0;

	//ö���豸
	emStatus = GXUpdateDeviceList(&nDeviceNum, 1000);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		return;
	}

	//�豸��������0,������Դ��ȡ�豸��Ϣ
	if (nDeviceNum > 0)
	{
		// Ϊ��ȡ�豸��Ϣ׼����Դ
		m_pBaseinfo = new GX_DEVICE_BASE_INFO[nDeviceNum];
		if (m_pBaseinfo == NULL)
		{
			AfxMessageBox(L"Ϊ��ȡ�豸��Ϣ������Դʧ��");

			// �������ڵ��豸������Ϊ0
			nDeviceNum = 0;
			m_nCamNum = nDeviceNum;
			return ;
		}

		//��ȡ����ö���������Ϣ
		nSize = nDeviceNum * sizeof(GX_DEVICE_BASE_INFO);  //����������С
		emStatus = GXGetAllDeviceBaseInfo(m_pBaseinfo, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			ShowErrorString(emStatus);
			delete []m_pBaseinfo;
			m_pBaseinfo = NULL;

			// ���豸������Ϊ0
			nDeviceNum = 0;
			m_nCamNum = nDeviceNum;
			return ;
		}
	}

	m_nCamNum = nDeviceNum;
}

//----------------------------------------------------------------------------------
/**
\brief  Ϊ�����ڷ�����Դ

\return ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CDahengImavision::AllocBuffere()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	uint32_t  nMinDevMum = 0;

	// ֹͣ��ʾ�豸��Ϣ
	m_nOperateID = -1;

	// ���豸�������ȹر�
	emStatus = CloseCamer();
	VERIFY_STATUS_RET(emStatus);

	
	ClearBuffer();

	// ö���豸
	UpdateDeviceList();

	// �豸����Ϊ0��ֱ�ӷ���
	if (m_nCamNum <= 0)
	{
		return 0;
	}

	uint32_t i = 0;
	
	//---------------------����ռ�-----------------------------
	//����������
	
	m_hDevices = new GX_DEV_HANDLE[m_nCamNum];
	if (m_hDevices == NULL)
	{
		ClearBuffer();
		return GX_STATUS_INVALID_PARAMETER;
	}

	//����������ݽṹ��
	m_pstCam=new CAMER_INFO[m_nCamNum];
	if (m_pstCam == NULL)
	{
		ClearBuffer();
		return GX_STATUS_ERROR;
	}

	m_npfpsCont=new int[m_nCamNum];
	if (m_npfpsCont == NULL)
	{
		ClearBuffer();
		return GX_STATUS_ERROR;
	}

	m_stFrameDatas=new GX_FRAME_DATA[m_nCamNum];
	if(m_stFrameDatas==NULL)
	{
		ClearBuffer();
		return GX_STATUS_ERROR;
	}

	if(m_nCamNum<0)
		return GX_STATUS_NOT_FOUND_DEVICE;
	//---------------------��ʼ����Դ��Ϣ-----------------------------
	for (i=0; i<m_nCamNum; i++)
	{
		m_hDevices[i]  = NULL;
		m_npfpsCont[i] = 0;

		m_pstCam[i].bIsColorFilter = false;
		m_pstCam[i].bIsOpen        = FALSE;
		m_pstCam[i].bIsSnap        = FALSE;
		m_pstCam[i].pBmpInfo       = NULL;
		m_pstCam[i].pRawBuffer     = NULL;
		m_pstCam[i].pImageBuffer   = NULL;
		m_pstCam[i].fFps           = 0.0;
		m_pstCam[i].nBayerLayout   = 0;
		m_pstCam[i].nImageHeight   = 0;
		m_pstCam[i].nImageWidth    = 0;
		m_pstCam[i].nPayLoadSise   = 0;
		memset(m_pstCam[i].chBmpBuf,0,sizeof(m_pstCam[i].chBmpBuf));
		//�򿪲���ȡ����
		emStatus=InitDevice(i);
		VERIFY_STATUS_RET(emStatus);
	}

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡ�豸�Ŀ�ߵ���Ϣ

\return ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CDahengImavision::GetDeviceInitParam(int nCamID)
{
	GX_STATUS emStatus       = GX_STATUS_ERROR;
	bool      bIsImplemented = false;
	if(nCamID>m_nCamNum)
		return 0;
	// ��ѯ��ǰ����Ƿ�֧��GX_ENUM_PIXEL_COLOR_FILTER
	emStatus = GXIsImplemented(m_hDevices[nCamID], GX_ENUM_PIXEL_COLOR_FILTER, &bIsImplemented);
	VERIFY_STATUS_RET(emStatus);
	m_pstCam[nCamID].bIsColorFilter = bIsImplemented;

	// ֧�ֱ�ʾ�����ɫͼ��
	if(bIsImplemented)
	{
		emStatus = GXGetEnum(m_hDevices[nCamID], GX_ENUM_PIXEL_COLOR_FILTER, &m_pstCam[nCamID].nBayerLayout);
		VERIFY_STATUS_RET(emStatus);
	}

	// ��ȡ���
	emStatus = GXGetInt(m_hDevices[nCamID], GX_INT_WIDTH, &m_pstCam[nCamID].nImageWidth);
	VERIFY_STATUS_RET(emStatus);

	// ��ȡ�߶�
	emStatus = GXGetInt(m_hDevices[nCamID], GX_INT_HEIGHT, &m_pstCam[nCamID].nImageHeight);
	VERIFY_STATUS_RET(emStatus);

	// ��ȡͼ���С
	emStatus = GXGetInt(m_hDevices[nCamID],GX_INT_PAYLOAD_SIZE,&m_pstCam[nCamID].nPayLoadSise);
	//ΪGetImage��������ռ�
	m_stFrameDatas[nCamID].pImgBuf = new BYTE[(size_t)m_pstCam[nCamID].nPayLoadSise];
	if (m_stFrameDatas[nCamID].pImgBuf == NULL)
	{
		emStatus=GX_STATUS_ERROR;
	}

		//---------------------------��ʼ��bitmapͷ---------------------------
	m_pstCam[nCamID].pBmpInfo								= (BITMAPINFO *)(m_pstCam[nCamID].chBmpBuf);
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biSize				= sizeof(BITMAPINFOHEADER);
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biWidth			= (LONG)m_pstCam[nCamID].nImageWidth;
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biHeight			= (LONG)m_pstCam[nCamID].nImageHeight;	

	m_pstCam[nCamID].pBmpInfo->bmiHeader.biPlanes			= 1;
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biBitCount			= 8;  // ��ɫͼ��Ϊ24,�ڰ�ͼ��Ϊ8
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biClrImportant	= 0;

	// �ڰ������Ҫ���г�ʼ����ɫ�����
	for(int i=0;i<256;i++)
	{
		m_pstCam[nCamID].pBmpInfo->bmiColors[i].rgbBlue	 = i;
		m_pstCam[nCamID].pBmpInfo->bmiColors[i].rgbGreen	 = i;
		m_pstCam[nCamID].pBmpInfo->bmiColors[i].rgbRed		 = i;
		m_pstCam[nCamID].pBmpInfo->bmiColors[i].rgbReserved = i;
	}

	//---------------------------------------------------------------------
	//----------------------------Ϊͼ�����ݷ���Buffer---------------------
	// �ڰ�ͼ��Buffer����
	m_pstCam[nCamID].pImageBuffer = new BYTE[(size_t)(m_pstCam[nCamID].nImageWidth * m_pstCam[nCamID].nImageHeight)];
	if (m_pstCam[nCamID].pImageBuffer  == NULL)
	{
		return false;
	}

	return emStatus;
}

GX_STATUS CDahengImavision::CloseCameraByID(int nCameraID) 
{
	// TODO: Add your command handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CMenu*    pMenuSub = NULL;

	if (m_pstCam[nCameraID].bIsSnap)
	{
		//ֹͣ�ɼ�
		emStatus = GXSendCommand(m_hDevices[nCameraID], GX_COMMAND_ACQUISITION_STOP);
		VERIFY_STATUS_RET(emStatus);

		//ע���ص�
		emStatus = GXUnregisterCaptureCallback(m_hDevices[nCameraID]);
		VERIFY_STATUS_RET(emStatus);

		//�ͷ���Դ
		if(m_pstCam[nCameraID].pImageBuffer != NULL)
		{
			delete[]m_pstCam[nCameraID].pImageBuffer;
			m_pstCam[nCameraID].pImageBuffer = NULL;
		}

		if (m_pstCam[nCameraID].pRawBuffer != NULL)
		{
			delete []m_pstCam[nCameraID].pRawBuffer;
			m_pstCam[nCameraID].pRawBuffer = NULL;
		}

		m_pstCam[nCameraID].bIsSnap = FALSE;
	}

	//�ر��豸
	emStatus = GXCloseDevice(m_hDevices[nCameraID]);
	VERIFY_STATUS_RET(emStatus);

	m_hDevices[nCameraID]       = NULL;
	m_pstCam[nCameraID].bIsOpen = FALSE;

	return emStatus;

}

//----------------------------------------------------------------------------------
/**
\brief  ����ȡ����ͼ���������ݺڰײ�ɫ�Ĳ�ͬ���д�����ʾ������
\param  pFrameData  [in] ָ���û������ͼ�����ݵĵ�ַָ��

\return ��
*/
//----------------------------------------------------------------------------------
GX_STATUS  CDahengImavision::GetImageFromeCamera(int nCameraID)//GX_FRAME_DATA* pFrameData)
{
	GX_STATUS emStatus  = GX_STATUS_ERROR;
	double dElapsedtime = 0;
	if(nCameraID>m_nCamNum||nCameraID<0)
		return GX_STATUS_NOT_FOUND_DEVICE;
	if(m_hDevices[nCameraID]==NULL)
		return GX_STATUS_INVALID_HANDLE;
	//������������
	emStatus = GXSendCommand(m_hDevices[nCameraID],GX_COMMAND_TRIGGER_SOFTWARE);
	VERIFY_STATUS_RET(emStatus);

	Sleep(m_nSnapTimeDelay);
	//��ȡͼ��
	int m_nEditTimeOut = 500;
	emStatus = GXGetImage(m_hDevices[nCameraID], &m_stFrameDatas[nCameraID], m_nEditTimeOut);


	//�ж�GXGetImage����ֵ
	VERIFY_STATUS_RET(emStatus);

	// ��ȡ����ͼ���Ƿ�Ϊ������ͼ��
	if (m_stFrameDatas[nCameraID].nStatus != 0)    
	{
		return GX_STATUS_ERROR;
	}
	
/*
	//���ͼ������
	memset(m_stFrameDatas[nCameraID].pImgBuf,0,(size_t)m_pstCam[nCameraID].nPayLoadSise);

	//���Ϳ�ʼ�ɼ�����
	BOOL bGetImageOK;
	bGetImageOK=FALSE;
	int nRetryCounter=0;
	while(bGetImageOK==FALSE)
	{
		//������������
		emStatus = GXSendCommand(m_hDevices[nCameraID],GX_COMMAND_TRIGGER_SOFTWARE);
		VERIFY_STATUS_RET(emStatus);
		Sleep(m_nSnapTimeDelay);
		//��ȡͼ��
		int m_nEditTimeOut = 500;
		emStatus = GXGetImage(m_hDevices[nCameraID], &m_stFrameDatas[nCameraID], m_nEditTimeOut);
		//�ж�GXGetImage����ֵ
		VERIFY_STATUS_RET(emStatus);
		
		if ( (&m_stFrameDatas[nCameraID].nStatus) == 0)
		{
			bGetImageOK=TRUE;
			//AfxMessageBox(L"1-3",MB_OK);
			break;
		}
		else     
		{
			if(nRetryCounter>5)
			{
				return GX_STATUS_TIMEOUT;
			}
			nRetryCounter++;

		}
	}
*/
	//��֧�ֲ�ɫ,ת��ΪRGBͼ������
	if (m_pstCam[nCameraID].bIsColorFilter)
	{
		//��Raw8ͼ��ת��ΪRGBͼ���Թ���ʾ
		DxRaw8toRGB24((BYTE*)m_stFrameDatas[nCameraID].pImgBuf, m_pstCam[nCameraID].pImageBuffer, (VxUint32)m_pstCam[nCameraID].nImageWidth, (VxUint32)m_pstCam[nCameraID].nImageHeight, RAW2RGB_NEIGHBOUR,
			DX_PIXEL_COLOR_FILTER(m_pstCam[nCameraID].nBayerLayout), TRUE);

		//��ת�����RGBͼ����ʾ������
		//DrawImg(m_pImgBuffer, m_nImageWidth, m_nImageHeight);
	}
	else   
	{
		// �ڰ������Ҫ��ת���ݺ���ʾ
		for(int i =0;i <m_pstCam[nCameraID].nImageHeight;i++)
		{
			memcpy(m_pstCam[nCameraID].pImageBuffer+i*m_pstCam[nCameraID].nImageWidth, (BYTE *)(m_stFrameDatas[nCameraID].pImgBuf)+(m_pstCam[nCameraID].nImageHeight-i-1)*m_pstCam[nCameraID].nImageWidth,(size_t)m_pstCam[nCameraID].nImageWidth);
		}

		//��ָ��������һ֡һ֡�Ļ�ͼ
		//DrawImg(m_pImgBuffer, m_nImageWidth, m_nImageHeight);
	}
	return GX_STATUS_SUCCESS;
}

//----------------------------------------------------------------------------------
/**
\brief  ���豸

\return ���豸�ɹ��򷵻�GX_STATUS_SUCCESS�����򷵻ش�����
*/
//----------------------------------------------------------------------------------
GX_STATUS CDahengImavision::OpenDeviceByID(int nCameraID)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	bool      bRet     = true;
	uint32_t  nDevNum  = 0;

	// ö���豸
	emStatus = GXUpdateDeviceList(&nDevNum, 1000);
	VERIFY_STATUS_RET(emStatus);

	// �жϵ�ǰ�����豸����
	if (nDevNum <= 0)
	{
		//MessageBox("δ�����豸!");
		return GX_STATUS_NOT_FOUND_DEVICE;
	}

	//================================

	GX_OPEN_PARAM  stOpenParam;

	// ��ʼ���豸�򿪲���,Ĭ�ϴ����Ϊ1���豸
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_INDEX;
	CStringA strCameraID;
	strCameraID.Format("%d",nCameraID);
	stOpenParam.pszContent = strCameraID.GetBuffer();

	// ��ö���б��е�һ̨�豸
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevices[nCameraID-1]);

	VERIFY_STATUS_RET(emStatus);
	// �����豸�򿪱�ʶ
	m_pstCam[nCameraID-1].bIsOpen = true;


	return emStatus;
}
//----------------------------------------------------------------------------------
/**
\brief  �ر��豸

\return �ر��豸�ɹ��򷵻�GX_STATUS_SUCCESS�����򷵻ش�����
*/
//----------------------------------------------------------------------------------
GX_STATUS CDahengImavision::CloseDevice(int nCameraID)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// �ر��豸
	emStatus = GXCloseDevice(m_hDevices[nCameraID]);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		return emStatus;
	}
	m_pstCam[nCameraID].bIsOpen = false;

	return emStatus;
}

// ---------------------------------------------------------------------------------
/**
\brief   �����ʼ��

\return  ��
*/
// ----------------------------------------------------------------------------------
GX_STATUS CDahengImavision::InitDevice(int nCameraID)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	//�����
	emStatus = OpenDeviceByID(nCameraID+1);
	VERIFY_STATUS_RET(emStatus);

	//�����豸��Ĭ�ϲ������ɼ�ģʽ:�����ɼ�,��������:��,����Դ:����,���ݸ�ʽ:8-bit

	//���òɼ�ģʽ�����ɼ�
	emStatus = GXSetEnum(m_hDevices[nCameraID],GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);
	VERIFY_STATUS_RET(emStatus);

	//���ô���ģʽΪ��
	emStatus = GXSetEnum(m_hDevices[nCameraID],GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_ON);
	VERIFY_STATUS_RET(emStatus);
	

	// ʹ��GxGetImage��ȡͼ��,ֻ��Ҫ���Ϳ�������
	// ���Ϳ�������
	emStatus = GXSendCommand(m_hDevices[nCameraID],GX_COMMAND_ACQUISITION_START);
	VERIFY_STATUS_RET(emStatus);

	// ���¿��ɱ�ʶ
	m_pstCam[nCameraID].bIsSnap = true;
	// ��֪��ǰ���֧���ĸ�8λͼ�����ݸ�ʽ����ֱ������
	// �����豸֧�����ݸ�ʽGX_PIXEL_FORMAT_BAYER_GR8,��ɰ������´���ʵ��
	// emStatus = GXSetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, GX_PIXEL_FORMAT_BAYER_GR8);
	// VERIFY_STATUS_RET(emStatus);

	// �������ǰ��������ݸ�ʽʱ�����Ե���SetPixelFormat8bit������ͼ�����ݸ�ʽ����Ϊ8Bit
	emStatus = SetPixelFormat8bit(nCameraID); 
	VERIFY_STATUS_RET(emStatus);

	//ѡ�񴥷�ԴΪ����
	emStatus = GXSetEnum(m_hDevices[nCameraID],GX_ENUM_TRIGGER_SOURCE, GX_TRIGGER_SOURCE_SOFTWARE);
	VERIFY_STATUS_RET(emStatus);

	//��ȡ�豸�Ŀ�ߵ���Ϣ
	emStatus = GetDeviceInitParam(nCameraID);
	VERIFY_STATUS_RET(emStatus);
	return emStatus;
}


// ---------------------------------------------------------------------------------
/**
\brief   ������������ݸ�ʽΪ8bit

\return  emStatus GX_STATUS_SUCCESS:���óɹ�������:����ʧ��
*/
// ----------------------------------------------------------------------------------
GX_STATUS CDahengImavision::SetPixelFormat8bit(int nCameraID)
{
	GX_STATUS emStatus    = GX_STATUS_SUCCESS;
	int64_t   nPixelSize  = 0;
	uint32_t  nEnmuEntry  = 0;
	size_t    nBufferSize = 0;
	BOOL      bIs8bit     = TRUE;

	GX_ENUM_DESCRIPTION  *pEnumDescription = NULL;
	GX_ENUM_DESCRIPTION  *pEnumTemp        = NULL;

	// ��ȡ���ص��С
	emStatus = GXGetEnum(m_hDevices[nCameraID], GX_ENUM_PIXEL_SIZE, &nPixelSize);
	VERIFY_STATUS_RET(emStatus);

	// �ж�Ϊ8bitʱֱ�ӷ���,��������Ϊ8bit
	if (nPixelSize == GX_PIXEL_SIZE_BPP8)
	{
		return GX_STATUS_SUCCESS;
	}
	else
	{
		// ��ȡ�豸֧�ֵ����ظ�ʽö����
		emStatus = GXGetEnumEntryNums(m_hDevices[nCameraID], GX_ENUM_PIXEL_FORMAT, &nEnmuEntry);
		VERIFY_STATUS_RET(emStatus);

		// Ϊ��ȡ�豸֧�ֵ����ظ�ʽö��ֵ׼����Դ
		nBufferSize      = nEnmuEntry * sizeof(GX_ENUM_DESCRIPTION);
		pEnumDescription = new GX_ENUM_DESCRIPTION[nEnmuEntry];

		// ��ȡ֧�ֵ�ö��ֵ
		emStatus         = GXGetEnumDescription(m_hDevices[nCameraID], GX_ENUM_PIXEL_FORMAT, pEnumDescription, &nBufferSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			if (pEnumDescription != NULL)
			{
				delete []pEnumDescription;
				pEnumDescription = NULL;
			}

			return emStatus;
		}

		// �����豸֧�ֵ����ظ�ʽ,�������ظ�ʽΪ8bit,
		// ���豸֧�ֵ����ظ�ʽΪMono10��Mono8��������ΪMono8
		for (uint32_t i = 0; i<nEnmuEntry; i++)
		{
			if ((pEnumDescription[i].nValue & GX_PIXEL_8BIT) == GX_PIXEL_8BIT)
			{
				emStatus = GXSetEnum(m_hDevices[nCameraID], GX_ENUM_PIXEL_FORMAT, pEnumDescription[i].nValue);
				break;
			}
		}	

		// �ͷ���Դ
		if(pEnumDescription != NULL)
		{
			delete []pEnumDescription;
			pEnumDescription = NULL;
		}
	}

	return emStatus;
}
