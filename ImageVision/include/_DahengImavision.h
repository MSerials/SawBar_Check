#pragma once

///相机参数结构体
typedef struct CAMER_INFO
{
	BITMAPINFO					*pBmpInfo;		  ///< BITMAPINFO 结构指针，显示图像时使用
	BYTE						*pImageBuffer;	  ///< 指向经过处理后的图像数据缓冲区
	BYTE						*pRawBuffer;      ///< 指向原始RAW图缓冲区
	char						chBmpBuf[2048];	  ///< BIMTAPINFO 存储缓冲区，m_pBmpInfo即指向此缓冲区	
	int64_t                     nPayLoadSise;     ///< 图像块大小
	int64_t					    nImageWidth;	  ///< 图像宽度
	int64_t					    nImageHeight;	  ///< 图像高度	
	int64_t					    nBayerLayout;	  ///< Bayer排布格式
	//int64_t						m_nPixelColorFilter; ///< Bayer格式
	bool						bIsColorFilter;	  ///< 判断是否为彩色相机
	BOOL						bIsOpen;		  ///< 相机已打开标志
	BOOL						bIsSnap;		  ///< 相机正在采集标志
	float						fFps;			  ///< 帧率
}CAMER_INFO;


///获取命令ID对应描述
char* GetFName(GX_FEATURE_ID emID);

///获取枚举描述结构体
GX_ENUM_DESCRIPTION* GetEnumDsb(GX_DEV_HANDLE hDevice, GX_FEATURE_ID emID, size_t* pNum);


class CDahengImavision
{
public:
	CDahengImavision(void);
	~CDahengImavision(void);
private:
	GX_STATUS OpenDeviceByID(int nCameraID);

public:
	GX_STATUS CloseCamer();
	void ClearBuffer();
	GX_STATUS AllocBuffere();

	GX_STATUS InitDevice(int nCameraID);
	GX_STATUS CloseDevice(int nCameraID);
	GX_STATUS SetPixelFormat8bit(int nCameraID);

	GX_STATUS CloseCameraByID(int nCameraID);
	GX_STATUS GetDeviceInitParam(int nCamID); //Get Camera Parameter
	void UpdateDeviceList();
	//Get Image from Camera
	GX_STATUS GetImageFromeCamera(int n_CamerID);

	/// 错误信息提示
	void ShowErrorString(GX_STATUS emErrorStatus);


public:
	GX_FRAME_DATA*      m_stFrameDatas;       ///< GetImage获取的图像地址
	GX_DEV_HANDLE*	    m_hDevices;			///< 设备句柄指针
	CAMER_INFO*	  	    m_pstCam;			///< 相机数据结构体		
	int				    m_nOperateID;		///< 操作设备ID
	uint32_t		    m_nCamNum;			///< 相机数目
	BOOL			    m_bViewID;			///< 是否显示相机编号
	int*		 	    m_npfpsCont;		///< 帧率计数
	GX_DEVICE_BASE_INFO *m_pBaseinfo; 	    ///< 设备信息结构体

	int m_nSnapTimeDelay;

};

