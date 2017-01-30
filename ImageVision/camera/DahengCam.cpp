
#ifdef __AFXWIN_H__
	#include "../stdafx.h"
#endif
#include "../stdafx.h"
#include "DahengCam.h"

#pragma once


CDahengCam* daheng;

CDahengCam::CDahengCam()
: m_nGray(0)
, m_nRoiX(0)
, m_nRoiY(0)
, m_nRoiH(0)
, m_nRoiW(0)
, m_dAutoGainMin(0)
, m_dAutoGainMax(0)
, m_dAutoShutterMin(0)
, m_dAutoShutterMax(0)
, m_dShutter(0)
, m_dGain(0)
, m_bDevOpened(FALSE)
, m_bIsSnap(FALSE)
, m_IsDevOpen(FALSE)
, m_SnapDelay(0)
, m_ContinueSnapDelay(0)


{
	//MainFunction
}


CDahengCam::~CDahengCam()
{

}


BOOL CDahengCam::InitCam()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;

	//��ʼ���豸��
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		AfxMessageBox(L"����ͷ״̬��ȡ����");
		exit(0);
	}

	return 0;
}

void CDahengCam::InitROIUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	int64_t   nValue = 0;

	emStatus = GXGetInt(m_hDevice, GX_INT_AAROI_OFFSETY, &nValue);
	GX_VERIFY(emStatus);
	m_nRoiY = (int)nValue;

	emStatus = GXGetInt(m_hDevice, GX_INT_AAROI_OFFSETX, &nValue);
	GX_VERIFY(emStatus);
	m_nRoiX = (int)nValue;

	emStatus = GXGetInt(m_hDevice, GX_INT_AAROI_WIDTH, &nValue);
	GX_VERIFY(emStatus);
	m_nRoiW = (int)nValue;

	emStatus = GXGetInt(m_hDevice, GX_INT_AAROI_HEIGHT, &nValue);
	GX_VERIFY(emStatus);
	m_nRoiH = (int)nValue;

	// ��ȡ����Ȥ��������ķ�Χ����ʾ������
	//	UpdateROIRange();

	//	UpdateData(FALSE);
}

void CDahengCam::ShowErrorString(GX_STATUS emErrorStatus)
{
	char      chErrorInof[512] = { 0 };
	size_t    nSize = 512;
	GX_STATUS emStatus = GX_STATUS_ERROR;

	emStatus = GXGetLastError(&emErrorStatus, chErrorInof, &nSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		AfxMessageBox(L"GXGetLastError�ӿڵ���ʧ�ܣ�");
	}
	else
	{
		AfxMessageBox((CString)chErrorInof);
	}
}


GX_STATUS CDahengCam::GetDeviceParam()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	bool      bIsImplemented = false;

	// ��ȡͼ���С
	emStatus = GXGetInt(m_hDevice, GX_INT_PAYLOAD_SIZE, &m_nPayLoadSize);
	VERIFY_STATUS_RET(emStatus);

	// ��ȡ���
	emStatus = GXGetInt(m_hDevice, GX_INT_WIDTH, &m_nImageWidth);
	VERIFY_STATUS_RET(emStatus);

	// ��ȡ�߶�
	emStatus = GXGetInt(m_hDevice, GX_INT_HEIGHT, &m_nImageHeight);
	VERIFY_STATUS_RET(emStatus);

	// �ж�����Ƿ�֧��bayer��ʽ
	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &m_bIsColorFilter);
	VERIFY_STATUS_RET(emStatus);

	// ֧�ֱ�ʾ�����ɫͼ��
	if (m_bIsColorFilter)
	{
		emStatus = GXGetEnum(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &m_nPixelColorFilter);
	}

	return emStatus;
}

GX_STATUS CDahengCam::SetPixelFormat8bit()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	int64_t   nPixelSize = 0;
	uint32_t  nEnmuEntry = 0;
	size_t    nBufferSize = 0;
	BOOL      bIs8bit = TRUE;

	GX_ENUM_DESCRIPTION  *pEnumDescription = NULL;
	GX_ENUM_DESCRIPTION  *pEnumTemp = NULL;

	// ��ȡ���ص��С
	emStatus = GXGetEnum(m_hDevice, GX_ENUM_PIXEL_SIZE, &nPixelSize);
	VERIFY_STATUS_RET(emStatus);

	// �ж�Ϊ8bitʱֱ�ӷ���,��������Ϊ8bit
	if (nPixelSize == GX_PIXEL_SIZE_BPP8)
	{
		return GX_STATUS_SUCCESS;
	}
	else
	{
		// ��ȡ�豸֧�ֵ����ظ�ʽö����
		emStatus = GXGetEnumEntryNums(m_hDevice, GX_ENUM_PIXEL_FORMAT, &nEnmuEntry);
		VERIFY_STATUS_RET(emStatus);

		// Ϊ��ȡ�豸֧�ֵ����ظ�ʽö��ֵ׼����Դ
		nBufferSize = nEnmuEntry * sizeof(GX_ENUM_DESCRIPTION);
		pEnumDescription = new GX_ENUM_DESCRIPTION[nEnmuEntry];

		// ��ȡ֧�ֵ�ö��ֵ
		emStatus = GXGetEnumDescription(m_hDevice, GX_ENUM_PIXEL_FORMAT, pEnumDescription, &nBufferSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			if (pEnumDescription != NULL)
			{
				delete[]pEnumDescription;
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
				emStatus = GXSetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, pEnumDescription[i].nValue);
				break;
			}
		}

		// �ͷ���Դ
		if (pEnumDescription != NULL)
		{
			delete[]pEnumDescription;
			pEnumDescription = NULL;
		}
	}

	return emStatus;
}


bool  CDahengCam::PrepareForShowImg()
{
	bool bRet = true;

	// Ϊ�洢ԭʼͼ�����Buffer
	m_pRawBuffer = new BYTE[(size_t)m_nPayLoadSize];
	if (m_pRawBuffer == NULL)
	{
		bRet = false;
	}

	if (m_bIsColorFilter)
	{
		// Ϊ��ɫͼ����ʾ׼����Դ,����Buffer
		bRet = PrepareForShowColorImg();
	}
	else
	{
		// Ϊ�ڰ�ͼ����ʾ׼����Դ,����Buffer
		bRet = PrepareForShowMonoImg();
	}

	// ������ʧ�����ͷ��ѷ������Դ
	if (!bRet)
	{
		UnPrepareForShowImg();
	}

	return bRet;
}


bool CDahengCam::PrepareForShowColorImg()
{
	//--------------------------------------------------------------------
	//---------------------------��ʼ��bitmapͷ---------------------------
	m_pBmpInfo = (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth = (LONG)m_nImageWidth;
	m_pBmpInfo->bmiHeader.biHeight = (LONG)m_nImageHeight;

	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biBitCount = 24; // ��ɫͼ��Ϊ24,�ڰ�ͼ��Ϊ8
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage = 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biClrUsed = 0;
	m_pBmpInfo->bmiHeader.biClrImportant = 0;


	//---------------------------------------------------------------------
	//----------------------------Ϊͼ�����ݷ���Buffer---------------------

	// Ϊ����RGBת�����ͼ�����Buffer
	m_pImgBuffer = new BYTE[(size_t)(m_nImageWidth * m_nImageHeight * 3)];
	if (m_pImgBuffer == NULL)
	{
		return false;
	}

	return true;
}

bool  CDahengCam::PrepareForShowMonoImg()
{
	//--------------------------------------------------------------------
	//---------------------------��ʼ��bitmapͷ---------------------------
	m_pBmpInfo = (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth = (LONG)m_nImageWidth;
	m_pBmpInfo->bmiHeader.biHeight = (LONG)m_nImageHeight;

	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biBitCount = 8;  // ��ɫͼ��Ϊ24,�ڰ�ͼ��Ϊ8
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage = 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biClrUsed = 0;
	m_pBmpInfo->bmiHeader.biClrImportant = 0;

	if (!m_bIsColorFilter)
	{
		// �ڰ������Ҫ���г�ʼ����ɫ�����
		for (int i = 0; i<256; i++)
		{
			m_pBmpInfo->bmiColors[i].rgbBlue = i;
			m_pBmpInfo->bmiColors[i].rgbGreen = i;
			m_pBmpInfo->bmiColors[i].rgbRed = i;
			m_pBmpInfo->bmiColors[i].rgbReserved = i;
		}
	}

	//---------------------------------------------------------------------
	//----------------------------Ϊͼ�����ݷ���Buffer---------------------
	// �ڰ�ͼ��Buffer����
	m_pImgBuffer = new BYTE[(size_t)(m_nImageWidth * m_nImageHeight)];
	if (m_pImgBuffer == NULL)
	{
		return false;
	}

	return true;
}

void CDahengCam::UnPrepareForShowImg()
{
	if (m_pRawBuffer != NULL)
	{
		delete[]m_pRawBuffer;
		m_pRawBuffer = NULL;
	}

	if (m_pImgBuffer != NULL)
	{
		delete[]m_pImgBuffer;
		m_pImgBuffer = NULL;
	}
}


void CDahengCam::DrawImg(BYTE *pImageBuffer)
{
	//g_evtGetVideoCapture.SetEvent();
}

void __stdcall CDahengCam::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame){
	CDahengCam *pDlg = (CDahengCam*)(pFrame->pUserParam);
	VxUint32 nImageHeight = (VxUint32)pDlg->m_nImageHeight;
	VxUint32 nImageWidth = (VxUint32)pDlg->m_nImageWidth;

	if (pFrame->status == 0)
	{
		memcpy(pDlg->m_pRawBuffer, pFrame->pImgBuf, pFrame->nImgSize);

		if (pDlg->m_bIsColorFilter)
		{
			// ��ɫͼ����Ҫ��Raw8ͼ�񾭹�RGBת������ʾ
			DxRaw8toRGB24(pDlg->m_pRawBuffer, pDlg->m_pImgBuffer, nImageWidth, nImageHeight, RAW2RGB_NEIGHBOUR,
				DX_PIXEL_COLOR_FILTER(pDlg->m_nPixelColorFilter), TRUE);
			pDlg->DrawImg(pDlg->m_pImgBuffer);
		}
		else
		{
			// �ڰ������Ҫ��ת���ݺ���ʾ
			for (VxUint32 i = 0; i <nImageHeight; ++i)
			{
				memcpy(pDlg->m_pImgBuffer + i*nImageWidth, pDlg->m_pRawBuffer + (nImageHeight - i - 1)*nImageWidth, (size_t)nImageWidth);
			}

			pDlg->DrawImg(pDlg->m_pImgBuffer);

		}
	}
#ifdef _DEBUG

#endif

#ifdef _OPENCV
		g.CameraMat_Mutex.Lock();
		g.Opencv.CameramatOK = g.Opencv.BmpToMat(daheng->m_pBmpInfo, daheng->m_pImgBuffer, g.Opencv.Cameramat);
		g.CameraMat_Mutex.Unlock();
		if (!g.evt_detect.EventState())
		g.evt_detect.SetEvent();
#endif



}

void CDahengCam::OnStartCapture()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_ERROR;

	//// ��ʼ��BMPͷ��Ϣ������BufferΪͼ��ɼ���׼��
	if (!PrepareForShowImg())
	{
		AfxMessageBox(L"Ϊͼ����ʾ׼����Դʧ��!");
		return;
	}

	//ע��ص�
	emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		UnPrepareForShowImg();
		ShowErrorString(emStatus);
		return;
	}

	//����ʼ�ɼ�����
	emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_START);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		UnPrepareForShowImg();
		ShowErrorString(emStatus);
		AfxMessageBox(L"��ȷ��û����������ռ�ø�����ͷ��");
		return;
	}
	m_bIsSnap = TRUE;
	// ���½���UI
	return;
}

void CDahengCam::OnOpenDevice()
{

	// TODO: Add your control notification handler code here
	GX_STATUS     emStatus = GX_STATUS_SUCCESS;
	uint32_t      nDevNum = 0;
	GX_OPEN_PARAM stOpenParam;

	// ��ʼ���豸�򿪲���
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";

	// ö���豸
	emStatus = GXUpdateDeviceList(&nDevNum, 1000);
	GX_VERIFY(emStatus);

	// �жϵ�ǰ�����豸����
	if (nDevNum <= 0)
	{
		AfxMessageBox(L"û�з�������ͷ!");
		return;
	}

	// ���豸
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	GX_VERIFY(emStatus);
	m_bDevOpened = TRUE;

	// ���������Ĭ�ϲ���:�ɼ�ģʽ:�����ɼ�,���ݸ�ʽ:8-bit
	emStatus = InitDevice();
	GX_VERIFY(emStatus);

	// ��ȡ�豸�Ŀ��ߵ�������Ϣ
	emStatus = GetDeviceParam();
	GX_VERIFY(emStatus);

	m_IsDevOpen = TRUE;

	return;

}



GX_STATUS CDahengCam::InitDevice()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//���òɼ�ģʽ�����ɼ�
	emStatus = GXSetEnum(m_hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);
	VERIFY_STATUS_RET(emStatus);

	// ���ô�������Ϊ��
	emStatus = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
	VERIFY_STATUS_RET(emStatus);

	// ��֪��ǰ���֧���ĸ�8λͼ�����ݸ�ʽ����ֱ������
	// �����豸֧�����ݸ�ʽGX_PIXEL_FORMAT_BAYER_GR8,��ɰ������´���ʵ��
	// emStatus = GXSetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, GX_PIXEL_FORMAT_BAYER_GR8);
	// VERIFY_STATUS_RET(emStatus);

	// �������ǰ��������ݸ�ʽʱ�����Ե������º�����ͼ�����ݸ�ʽ����Ϊ8Bit
	emStatus = SetPixelFormat8bit();

	return emStatus;
}

void CDahengCam::ResetBOOL()
{
	m_IsDevOpen = FALSE;
	m_bIsSnap = FALSE;
}

int CDahengCam::isOpen()
{
#ifdef _DEBUG
	
#endif
	return m_IsDevOpen;
}


int CDahengCam::CameraInit()
{
	//��ʼ���豸��

	GX_STATUS emStatus = GX_STATUS_ERROR;
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		AfxMessageBox(L"�ļ����ʼ������");
		exit(0);
	}

	OnOpenDevice();
	if (m_IsDevOpen != TRUE) {
		AfxMessageBox(L"δ����ȷ�����");
		return -1;
	}

	OnStartCapture();
	if (m_bIsSnap != TRUE) {
		AfxMessageBox(L"δ����ȷ�ɼ�ͼƬ");
		return -1;
	}

	return 1;
}

//GX_API GXGetImage(GX_DEV_HANDLE hDevice, GX_FRAME_DATA *pFrameData, uint32_t nTimeout)

GX_FRAME_DATA CDahengCam::GrabImage(int delay)
{
	GXGetImage(m_hDevice, &FRAME_DATA, delay);
	return FRAME_DATA;
}

void	CDahengCam::SetShutter(double shutter)
{
	GXSetFloat(m_hDevice, GX_FLOAT_EXPOSURE_TIME, shutter);
}