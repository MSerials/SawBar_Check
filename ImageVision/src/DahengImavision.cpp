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
\brief  遍历关闭所有相机

\return 无返回值
*/
//----------------------------------------------------------------------------------
GX_STATUS CDahengImavision::CloseCamer()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	uint32_t  i = 0;

	//if(m_nCamNum==0)
	//	return GX_STATUS_NOT_FOUND_DEVICE;
	//如果相机没关闭，则关闭相机
	for(i = 0; i < (uint32_t)m_nCamNum; i++)
	{
		if(m_pstCam[i].bIsSnap)
		{
			//停止采集
			emStatus = GXSendCommand(m_hDevices[i], GX_COMMAND_ACQUISITION_STOP);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				return emStatus;
			}

			//注销回调
			emStatus = GXUnregisterCaptureCallback(m_hDevices[i]);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				return emStatus;
			}
			m_pstCam[i].bIsSnap = FALSE;
		}
		if(m_pstCam[i].bIsOpen)
		{
			//关闭相机
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
\brief  清空为保存设备相关信息分配的资源

\return 无
*/
//----------------------------------------------------------------------------------
void CDahengImavision::ClearBuffer()
{

	// 清理设备句柄
	if(m_hDevices != NULL)
	{
		delete[]m_hDevices;
		m_hDevices = NULL;
	}

	// 清理设备信息
	if(m_pBaseinfo != NULL)
	{
		delete[]m_pBaseinfo;
		m_pBaseinfo = NULL;
	}

	// 自定义设备信息结构体
	if(m_pstCam != NULL)
	{
		delete[]m_pstCam;
		m_pstCam = NULL;
	}

	// 帧率信息
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
\brief  弹出错误信息
\param  status  错误码

\return 无
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
		AfxMessageBox(L"GXGetLastError接口调用失败！");
	}
	else
	{
		AfxMessageBox((A_to_W(chErrorInof)).c_str());
	}	
}

//----------------------------------------------------------------------------------
/**
\brief  枚举设备,获取设备个数

\return 无
*/
//----------------------------------------------------------------------------------
void CDahengImavision::UpdateDeviceList()
{
	GX_STATUS  emStatus   = GX_STATUS_SUCCESS;
	uint32_t   nDeviceNum = 0;
	bool       bRet       = true;
	size_t     nSize      = 0;

	//枚举设备
	emStatus = GXUpdateDeviceList(&nDeviceNum, 1000);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		return;
	}

	//设备个数大于0,分配资源获取设备信息
	if (nDeviceNum > 0)
	{
		// 为获取设备信息准备资源
		m_pBaseinfo = new GX_DEVICE_BASE_INFO[nDeviceNum];
		if (m_pBaseinfo == NULL)
		{
			AfxMessageBox(L"为获取设备信息分配资源失败");

			// 将主窗口的设备个数置为0
			nDeviceNum = 0;
			m_nCamNum = nDeviceNum;
			return ;
		}

		//获取所有枚举相机的信息
		nSize = nDeviceNum * sizeof(GX_DEVICE_BASE_INFO);  //计算机构体大小
		emStatus = GXGetAllDeviceBaseInfo(m_pBaseinfo, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			ShowErrorString(emStatus);
			delete []m_pBaseinfo;
			m_pBaseinfo = NULL;

			// 将设备个数置为0
			nDeviceNum = 0;
			m_nCamNum = nDeviceNum;
			return ;
		}
	}

	m_nCamNum = nDeviceNum;
}

//----------------------------------------------------------------------------------
/**
\brief  为主窗口分配资源

\return 无
*/
//----------------------------------------------------------------------------------
GX_STATUS CDahengImavision::AllocBuffere()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	uint32_t  nMinDevMum = 0;

	// 停止显示设备信息
	m_nOperateID = -1;

	// 若设备被打开则先关闭
	emStatus = CloseCamer();
	VERIFY_STATUS_RET(emStatus);

	
	ClearBuffer();

	// 枚举设备
	UpdateDeviceList();

	// 设备个数为0则直接返回
	if (m_nCamNum <= 0)
	{
		return 0;
	}

	uint32_t i = 0;
	
	//---------------------申请空间-----------------------------
	//创建相机句柄
	
	m_hDevices = new GX_DEV_HANDLE[m_nCamNum];
	if (m_hDevices == NULL)
	{
		ClearBuffer();
		return GX_STATUS_INVALID_PARAMETER;
	}

	//创建相机数据结构体
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
	//---------------------初始化资源信息-----------------------------
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
		//打开并读取参数
		emStatus=InitDevice(i);
		VERIFY_STATUS_RET(emStatus);
	}

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  获取设备的宽高等信息

\return 无
*/
//----------------------------------------------------------------------------------
GX_STATUS CDahengImavision::GetDeviceInitParam(int nCamID)
{
	GX_STATUS emStatus       = GX_STATUS_ERROR;
	bool      bIsImplemented = false;
	if(nCamID>m_nCamNum)
		return 0;
	// 查询当前相机是否支持GX_ENUM_PIXEL_COLOR_FILTER
	emStatus = GXIsImplemented(m_hDevices[nCamID], GX_ENUM_PIXEL_COLOR_FILTER, &bIsImplemented);
	VERIFY_STATUS_RET(emStatus);
	m_pstCam[nCamID].bIsColorFilter = bIsImplemented;

	// 支持表示输出彩色图像
	if(bIsImplemented)
	{
		emStatus = GXGetEnum(m_hDevices[nCamID], GX_ENUM_PIXEL_COLOR_FILTER, &m_pstCam[nCamID].nBayerLayout);
		VERIFY_STATUS_RET(emStatus);
	}

	// 获取宽度
	emStatus = GXGetInt(m_hDevices[nCamID], GX_INT_WIDTH, &m_pstCam[nCamID].nImageWidth);
	VERIFY_STATUS_RET(emStatus);

	// 获取高度
	emStatus = GXGetInt(m_hDevices[nCamID], GX_INT_HEIGHT, &m_pstCam[nCamID].nImageHeight);
	VERIFY_STATUS_RET(emStatus);

	// 获取图像大小
	emStatus = GXGetInt(m_hDevices[nCamID],GX_INT_PAYLOAD_SIZE,&m_pstCam[nCamID].nPayLoadSise);
	//为GetImage参数分配空间
	m_stFrameDatas[nCamID].pImgBuf = new BYTE[(size_t)m_pstCam[nCamID].nPayLoadSise];
	if (m_stFrameDatas[nCamID].pImgBuf == NULL)
	{
		emStatus=GX_STATUS_ERROR;
	}

		//---------------------------初始化bitmap头---------------------------
	m_pstCam[nCamID].pBmpInfo								= (BITMAPINFO *)(m_pstCam[nCamID].chBmpBuf);
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biSize				= sizeof(BITMAPINFOHEADER);
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biWidth			= (LONG)m_pstCam[nCamID].nImageWidth;
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biHeight			= (LONG)m_pstCam[nCamID].nImageHeight;	

	m_pstCam[nCamID].pBmpInfo->bmiHeader.biPlanes			= 1;
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biBitCount			= 8;  // 彩色图像为24,黑白图像为8
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pstCam[nCamID].pBmpInfo->bmiHeader.biClrImportant	= 0;

	// 黑白相机需要进行初始化调色板操作
	for(int i=0;i<256;i++)
	{
		m_pstCam[nCamID].pBmpInfo->bmiColors[i].rgbBlue	 = i;
		m_pstCam[nCamID].pBmpInfo->bmiColors[i].rgbGreen	 = i;
		m_pstCam[nCamID].pBmpInfo->bmiColors[i].rgbRed		 = i;
		m_pstCam[nCamID].pBmpInfo->bmiColors[i].rgbReserved = i;
	}

	//---------------------------------------------------------------------
	//----------------------------为图像数据分配Buffer---------------------
	// 黑白图像Buffer分配
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
		//停止采集
		emStatus = GXSendCommand(m_hDevices[nCameraID], GX_COMMAND_ACQUISITION_STOP);
		VERIFY_STATUS_RET(emStatus);

		//注销回调
		emStatus = GXUnregisterCaptureCallback(m_hDevices[nCameraID]);
		VERIFY_STATUS_RET(emStatus);

		//释放资源
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

	//关闭设备
	emStatus = GXCloseDevice(m_hDevices[nCameraID]);
	VERIFY_STATUS_RET(emStatus);

	m_hDevices[nCameraID]       = NULL;
	m_pstCam[nCameraID].bIsOpen = FALSE;

	return emStatus;

}

//----------------------------------------------------------------------------------
/**
\brief  将获取到的图像数据依据黑白彩色的不同进行处理并显示到界面
\param  pFrameData  [in] 指向用户传入的图像数据的地址指针

\return 无
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
	//发送软触发命令
	emStatus = GXSendCommand(m_hDevices[nCameraID],GX_COMMAND_TRIGGER_SOFTWARE);
	VERIFY_STATUS_RET(emStatus);

	Sleep(m_nSnapTimeDelay);
	//获取图像
	int m_nEditTimeOut = 500;
	emStatus = GXGetImage(m_hDevices[nCameraID], &m_stFrameDatas[nCameraID], m_nEditTimeOut);


	//判断GXGetImage返回值
	VERIFY_STATUS_RET(emStatus);

	// 获取到的图像是否为完整的图像
	if (m_stFrameDatas[nCameraID].nStatus != 0)    
	{
		return GX_STATUS_ERROR;
	}
	
/*
	//清除图像数据
	memset(m_stFrameDatas[nCameraID].pImgBuf,0,(size_t)m_pstCam[nCameraID].nPayLoadSise);

	//发送开始采集命令
	BOOL bGetImageOK;
	bGetImageOK=FALSE;
	int nRetryCounter=0;
	while(bGetImageOK==FALSE)
	{
		//发送软触发命令
		emStatus = GXSendCommand(m_hDevices[nCameraID],GX_COMMAND_TRIGGER_SOFTWARE);
		VERIFY_STATUS_RET(emStatus);
		Sleep(m_nSnapTimeDelay);
		//获取图像
		int m_nEditTimeOut = 500;
		emStatus = GXGetImage(m_hDevices[nCameraID], &m_stFrameDatas[nCameraID], m_nEditTimeOut);
		//判断GXGetImage返回值
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
	//若支持彩色,转换为RGB图像后输出
	if (m_pstCam[nCameraID].bIsColorFilter)
	{
		//将Raw8图像转换为RGB图像以供显示
		DxRaw8toRGB24((BYTE*)m_stFrameDatas[nCameraID].pImgBuf, m_pstCam[nCameraID].pImageBuffer, (VxUint32)m_pstCam[nCameraID].nImageWidth, (VxUint32)m_pstCam[nCameraID].nImageHeight, RAW2RGB_NEIGHBOUR,
			DX_PIXEL_COLOR_FILTER(m_pstCam[nCameraID].nBayerLayout), TRUE);

		//将转换后的RGB图像显示到窗口
		//DrawImg(m_pImgBuffer, m_nImageWidth, m_nImageHeight);
	}
	else   
	{
		// 黑白相机需要翻转数据后显示
		for(int i =0;i <m_pstCam[nCameraID].nImageHeight;i++)
		{
			memcpy(m_pstCam[nCameraID].pImageBuffer+i*m_pstCam[nCameraID].nImageWidth, (BYTE *)(m_stFrameDatas[nCameraID].pImgBuf)+(m_pstCam[nCameraID].nImageHeight-i-1)*m_pstCam[nCameraID].nImageWidth,(size_t)m_pstCam[nCameraID].nImageWidth);
		}

		//在指定窗口上一帧一帧的画图
		//DrawImg(m_pImgBuffer, m_nImageWidth, m_nImageHeight);
	}
	return GX_STATUS_SUCCESS;
}

//----------------------------------------------------------------------------------
/**
\brief  打开设备

\return 打开设备成功则返回GX_STATUS_SUCCESS，否则返回错误码
*/
//----------------------------------------------------------------------------------
GX_STATUS CDahengImavision::OpenDeviceByID(int nCameraID)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	bool      bRet     = true;
	uint32_t  nDevNum  = 0;

	// 枚举设备
	emStatus = GXUpdateDeviceList(&nDevNum, 1000);
	VERIFY_STATUS_RET(emStatus);

	// 判断当前连接设备个数
	if (nDevNum <= 0)
	{
		//MessageBox("未发现设备!");
		return GX_STATUS_NOT_FOUND_DEVICE;
	}

	//================================

	GX_OPEN_PARAM  stOpenParam;

	// 初始化设备打开参数,默认打开序号为1的设备
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_INDEX;
	CStringA strCameraID;
	strCameraID.Format("%d",nCameraID);
	stOpenParam.pszContent = strCameraID.GetBuffer();

	// 打开枚举列表中的一台设备
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevices[nCameraID-1]);

	VERIFY_STATUS_RET(emStatus);
	// 更新设备打开标识
	m_pstCam[nCameraID-1].bIsOpen = true;


	return emStatus;
}
//----------------------------------------------------------------------------------
/**
\brief  关闭设备

\return 关闭设备成功则返回GX_STATUS_SUCCESS，否则返回错误码
*/
//----------------------------------------------------------------------------------
GX_STATUS CDahengImavision::CloseDevice(int nCameraID)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// 关闭设备
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
\brief   相机初始化

\return  无
*/
// ----------------------------------------------------------------------------------
GX_STATUS CDahengImavision::InitDevice(int nCameraID)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	//打开相机
	emStatus = OpenDeviceByID(nCameraID+1);
	VERIFY_STATUS_RET(emStatus);

	//设置设备的默认参数，采集模式:连续采集,触发开关:开,触发源:软触发,数据格式:8-bit

	//设置采集模式连续采集
	emStatus = GXSetEnum(m_hDevices[nCameraID],GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);
	VERIFY_STATUS_RET(emStatus);

	//设置触发模式为开
	emStatus = GXSetEnum(m_hDevices[nCameraID],GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_ON);
	VERIFY_STATUS_RET(emStatus);
	

	// 使用GxGetImage获取图像,只需要发送开采命令
	// 发送开采命令
	emStatus = GXSendCommand(m_hDevices[nCameraID],GX_COMMAND_ACQUISITION_START);
	VERIFY_STATUS_RET(emStatus);

	// 更新开采标识
	m_pstCam[nCameraID].bIsSnap = true;
	// 已知当前相机支持哪个8位图像数据格式可以直接设置
	// 例如设备支持数据格式GX_PIXEL_FORMAT_BAYER_GR8,则可按照以下代码实现
	// emStatus = GXSetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, GX_PIXEL_FORMAT_BAYER_GR8);
	// VERIFY_STATUS_RET(emStatus);

	// 不清楚当前相机的数据格式时，可以调用SetPixelFormat8bit函数将图像数据格式设置为8Bit
	emStatus = SetPixelFormat8bit(nCameraID); 
	VERIFY_STATUS_RET(emStatus);

	//选择触发源为软触发
	emStatus = GXSetEnum(m_hDevices[nCameraID],GX_ENUM_TRIGGER_SOURCE, GX_TRIGGER_SOURCE_SOFTWARE);
	VERIFY_STATUS_RET(emStatus);

	//获取设备的宽高等信息
	emStatus = GetDeviceInitParam(nCameraID);
	VERIFY_STATUS_RET(emStatus);
	return emStatus;
}


// ---------------------------------------------------------------------------------
/**
\brief   设置相机的数据格式为8bit

\return  emStatus GX_STATUS_SUCCESS:设置成功，其它:设置失败
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

	// 获取像素点大小
	emStatus = GXGetEnum(m_hDevices[nCameraID], GX_ENUM_PIXEL_SIZE, &nPixelSize);
	VERIFY_STATUS_RET(emStatus);

	// 判断为8bit时直接返回,否则设置为8bit
	if (nPixelSize == GX_PIXEL_SIZE_BPP8)
	{
		return GX_STATUS_SUCCESS;
	}
	else
	{
		// 获取设备支持的像素格式枚举数
		emStatus = GXGetEnumEntryNums(m_hDevices[nCameraID], GX_ENUM_PIXEL_FORMAT, &nEnmuEntry);
		VERIFY_STATUS_RET(emStatus);

		// 为获取设备支持的像素格式枚举值准备资源
		nBufferSize      = nEnmuEntry * sizeof(GX_ENUM_DESCRIPTION);
		pEnumDescription = new GX_ENUM_DESCRIPTION[nEnmuEntry];

		// 获取支持的枚举值
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

		// 遍历设备支持的像素格式,设置像素格式为8bit,
		// 如设备支持的像素格式为Mono10和Mono8则设置其为Mono8
		for (uint32_t i = 0; i<nEnmuEntry; i++)
		{
			if ((pEnumDescription[i].nValue & GX_PIXEL_8BIT) == GX_PIXEL_8BIT)
			{
				emStatus = GXSetEnum(m_hDevices[nCameraID], GX_ENUM_PIXEL_FORMAT, pEnumDescription[i].nValue);
				break;
			}
		}	

		// 释放资源
		if(pEnumDescription != NULL)
		{
			delete []pEnumDescription;
			pEnumDescription = NULL;
		}
	}

	return emStatus;
}
