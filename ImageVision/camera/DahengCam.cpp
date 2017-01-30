
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

	//初始化设备库
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		AfxMessageBox(L"摄像头状态读取错误");
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

	// 获取感兴趣区域参数的范围并显示到界面
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
		AfxMessageBox(L"GXGetLastError接口调用失败！");
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

	// 获取图像大小
	emStatus = GXGetInt(m_hDevice, GX_INT_PAYLOAD_SIZE, &m_nPayLoadSize);
	VERIFY_STATUS_RET(emStatus);

	// 获取宽度
	emStatus = GXGetInt(m_hDevice, GX_INT_WIDTH, &m_nImageWidth);
	VERIFY_STATUS_RET(emStatus);

	// 获取高度
	emStatus = GXGetInt(m_hDevice, GX_INT_HEIGHT, &m_nImageHeight);
	VERIFY_STATUS_RET(emStatus);

	// 判断相机是否支持bayer格式
	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &m_bIsColorFilter);
	VERIFY_STATUS_RET(emStatus);

	// 支持表示输出彩色图像
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

	// 获取像素点大小
	emStatus = GXGetEnum(m_hDevice, GX_ENUM_PIXEL_SIZE, &nPixelSize);
	VERIFY_STATUS_RET(emStatus);

	// 判断为8bit时直接返回,否则设置为8bit
	if (nPixelSize == GX_PIXEL_SIZE_BPP8)
	{
		return GX_STATUS_SUCCESS;
	}
	else
	{
		// 获取设备支持的像素格式枚举数
		emStatus = GXGetEnumEntryNums(m_hDevice, GX_ENUM_PIXEL_FORMAT, &nEnmuEntry);
		VERIFY_STATUS_RET(emStatus);

		// 为获取设备支持的像素格式枚举值准备资源
		nBufferSize = nEnmuEntry * sizeof(GX_ENUM_DESCRIPTION);
		pEnumDescription = new GX_ENUM_DESCRIPTION[nEnmuEntry];

		// 获取支持的枚举值
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

		// 遍历设备支持的像素格式,设置像素格式为8bit,
		// 如设备支持的像素格式为Mono10和Mono8则设置其为Mono8
		for (uint32_t i = 0; i<nEnmuEntry; i++)
		{
			if ((pEnumDescription[i].nValue & GX_PIXEL_8BIT) == GX_PIXEL_8BIT)
			{
				emStatus = GXSetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, pEnumDescription[i].nValue);
				break;
			}
		}

		// 释放资源
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

	// 为存储原始图像分配Buffer
	m_pRawBuffer = new BYTE[(size_t)m_nPayLoadSize];
	if (m_pRawBuffer == NULL)
	{
		bRet = false;
	}

	if (m_bIsColorFilter)
	{
		// 为彩色图像显示准备资源,分配Buffer
		bRet = PrepareForShowColorImg();
	}
	else
	{
		// 为黑白图像显示准备资源,分配Buffer
		bRet = PrepareForShowMonoImg();
	}

	// 若分配失败则释放已分配的资源
	if (!bRet)
	{
		UnPrepareForShowImg();
	}

	return bRet;
}


bool CDahengCam::PrepareForShowColorImg()
{
	//--------------------------------------------------------------------
	//---------------------------初始化bitmap头---------------------------
	m_pBmpInfo = (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth = (LONG)m_nImageWidth;
	m_pBmpInfo->bmiHeader.biHeight = (LONG)m_nImageHeight;

	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biBitCount = 24; // 彩色图像为24,黑白图像为8
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage = 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biClrUsed = 0;
	m_pBmpInfo->bmiHeader.biClrImportant = 0;


	//---------------------------------------------------------------------
	//----------------------------为图像数据分配Buffer---------------------

	// 为经过RGB转换后的图像分配Buffer
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
	//---------------------------初始化bitmap头---------------------------
	m_pBmpInfo = (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth = (LONG)m_nImageWidth;
	m_pBmpInfo->bmiHeader.biHeight = (LONG)m_nImageHeight;

	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biBitCount = 8;  // 彩色图像为24,黑白图像为8
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage = 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biClrUsed = 0;
	m_pBmpInfo->bmiHeader.biClrImportant = 0;

	if (!m_bIsColorFilter)
	{
		// 黑白相机需要进行初始化调色板操作
		for (int i = 0; i<256; i++)
		{
			m_pBmpInfo->bmiColors[i].rgbBlue = i;
			m_pBmpInfo->bmiColors[i].rgbGreen = i;
			m_pBmpInfo->bmiColors[i].rgbRed = i;
			m_pBmpInfo->bmiColors[i].rgbReserved = i;
		}
	}

	//---------------------------------------------------------------------
	//----------------------------为图像数据分配Buffer---------------------
	// 黑白图像Buffer分配
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
			// 彩色图像需要将Raw8图像经过RGB转换后显示
			DxRaw8toRGB24(pDlg->m_pRawBuffer, pDlg->m_pImgBuffer, nImageWidth, nImageHeight, RAW2RGB_NEIGHBOUR,
				DX_PIXEL_COLOR_FILTER(pDlg->m_nPixelColorFilter), TRUE);
			pDlg->DrawImg(pDlg->m_pImgBuffer);
		}
		else
		{
			// 黑白相机需要翻转数据后显示
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

	//// 初始化BMP头信息、分配Buffer为图像采集做准备
	if (!PrepareForShowImg())
	{
		AfxMessageBox(L"为图像显示准备资源失败!");
		return;
	}

	//注册回调
	emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		UnPrepareForShowImg();
		ShowErrorString(emStatus);
		return;
	}

	//发开始采集命令
	emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_START);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		UnPrepareForShowImg();
		ShowErrorString(emStatus);
		AfxMessageBox(L"请确认没有其他程序占用该摄像头！");
		return;
	}
	m_bIsSnap = TRUE;
	// 更新界面UI
	return;
}

void CDahengCam::OnOpenDevice()
{

	// TODO: Add your control notification handler code here
	GX_STATUS     emStatus = GX_STATUS_SUCCESS;
	uint32_t      nDevNum = 0;
	GX_OPEN_PARAM stOpenParam;

	// 初始化设备打开参数
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";

	// 枚举设备
	emStatus = GXUpdateDeviceList(&nDevNum, 1000);
	GX_VERIFY(emStatus);

	// 判断当前连接设备个数
	if (nDevNum <= 0)
	{
		AfxMessageBox(L"没有发现摄像头!");
		return;
	}

	// 打开设备
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	GX_VERIFY(emStatus);
	m_bDevOpened = TRUE;

	// 设置相机的默认参数:采集模式:连续采集,数据格式:8-bit
	emStatus = InitDevice();
	GX_VERIFY(emStatus);

	// 获取设备的宽、高等属性信息
	emStatus = GetDeviceParam();
	GX_VERIFY(emStatus);

	m_IsDevOpen = TRUE;

	return;

}



GX_STATUS CDahengCam::InitDevice()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//设置采集模式连续采集
	emStatus = GXSetEnum(m_hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);
	VERIFY_STATUS_RET(emStatus);

	// 设置触发开关为关
	emStatus = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
	VERIFY_STATUS_RET(emStatus);

	// 已知当前相机支持哪个8位图像数据格式可以直接设置
	// 例如设备支持数据格式GX_PIXEL_FORMAT_BAYER_GR8,则可按照以下代码实现
	// emStatus = GXSetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, GX_PIXEL_FORMAT_BAYER_GR8);
	// VERIFY_STATUS_RET(emStatus);

	// 不清楚当前相机的数据格式时，可以调用以下函数将图像数据格式设置为8Bit
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
	//初始化设备库

	GX_STATUS emStatus = GX_STATUS_ERROR;
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		AfxMessageBox(L"文件库初始化错误");
		exit(0);
	}

	OnOpenDevice();
	if (m_IsDevOpen != TRUE) {
		AfxMessageBox(L"未能正确打开相机");
		return -1;
	}

	OnStartCapture();
	if (m_bIsSnap != TRUE) {
		AfxMessageBox(L"未能正确采集图片");
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