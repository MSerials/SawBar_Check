////////////////////////////////////////////////////
#include "../stdafx.h"
#include "camera.h"

#if !defined(AFX_GXAUTOFUNCCTRLDLG_H__48794C28_712D_4E1F_97EA_58B5A231704F__INCLUDED_)
#define AFX_GXAUTOFUNCCTRLDLG_H__48794C28_712D_4E1F_97EA_58B5A231704F__INCLUDED_

#include "inc/GxIAPI.h"
#include "inc/DxImageProc.h"


#pragma comment(lib,"camera/lib/x64/GxIAPI.lib")
#pragma comment(lib,"camera/lib/x64/DxImageProc.lib")








#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <string>
#include <iostream>
using namespace std;


 
///< 错误提示函数宏定义
#define  GX_VERIFY(emStatus) \
if (emStatus != GX_STATUS_SUCCESS)\
{\
	ShowErrorString(emStatus); \
	return; \
}  ///< 错误提示函数宏定义

#define VERIFY_STATUS_RET(emStatus) \
if (emStatus != GX_STATUS_SUCCESS) \
{\
	return emStatus; \
}


class CDahengCam //: public Camera
{
public:
	CDahengCam();
	//CDahengCam(HWND hWnd);
	~CDahengCam();

	/// 回调函数
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// 获取设备的宽高等属性信息
	GX_STATUS GetDeviceParam();

	/// 设置相机的数据格式,使其输出图像位深为8-bit
	GX_STATUS SetPixelFormat8bit();

	/// 设备初始化
	GX_STATUS InitDevice();

	///  为彩色图像显示准备资源
	bool PrepareForShowColorImg();

	/// 为黑白图像显示准备资源
	bool PrepareForShowMonoImg();

	/// 为图像显示准备资源,分配Buffer
	bool PrepareForShowImg();

	/// 更新感兴趣区域的取值范围
	void UpdateROIRange();

	/// 释放为图像显示准备的资源
	void UnPrepareForShowImg();

	/// 获取枚举型信息并初始化其对应的Combox框
	void InitEnumUI(GX_FEATURE_ID emFeatureID, CComboBox *pComboBox, bool bIsImplemented);

	/// 显示图像
	void DrawImg(BYTE *pImageBuffer);

	/// 初始化UI界面
	void InitUI();

	/// 初始化曝光相关的控件
	void InitShutterUI();

	/// 初始化增益相关的控件
	void InitGainUI();

	/// 初始化期望灰度值相关的控件
	void InitGrayUI();

	/// 初始化自动增益Combox控件
	void InitAutoGainUI();

	/// 初始化感兴趣区域的宽、高、X坐标和Y坐标的值
	void InitROIUI();

	/// 初始化自动曝光Combox控件
	void InitAutoShutterUI();

	/// 初始化2A光照环境Combox控件
	void InitLightEnvironmentUI();

	/// 刷新界面
	void UpDateUI();

	/// 刷新显示曝光和增益当前值
	void RefreshCurValue();

	///获取错误信息
	void ShowErrorString(GX_STATUS emErrorStatus);





	int		m_nGray;               ///< 期望灰度值
	int		m_nRoiX;               ///< 2A感兴趣区域X坐标
	int		m_nRoiY;               ///< 2A感兴趣区域Y坐标
	int		m_nRoiH;               ///< 2A感兴趣区域高
	int		m_nRoiW;               ///< 2A感兴趣区域宽
	double	m_dAutoGainMin;        ///< 自动增益最小值
	double  m_dAutoGainMax;        ///< 自动增益最大值
	double	m_dAutoShutterMin;     ///< 自动曝光最小值
	double	m_dAutoShutterMax;     ///< 自动曝光最大值

	double	m_dShutter;            ///< 曝光值
	double  m_dGain;               ///< 增益值

	/*
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	*/

public:
	GX_DEV_HANDLE           m_hDevice;                ///< 设备句柄
	int64_t                 m_nImageWidth;            ///< 相机输出图像宽度
	int64_t                 m_nImageHeight;           ///< 相机输出图像高度
	int64_t                 m_nPayLoadSize;           ///< 设备输出原始图像大小
	int64_t                 m_nPixelColorFilter;      ///< 彩色相机的Bayer格式
	double                  m_dShutterInc;            ///< 曝光调节步长
	double                  m_dGainInc;               ///< 增益调节步长           

	BOOL                    m_bDevOpened;             ///< 标识设备是否已打开
	BOOL                    m_bIsSnap;                ///< 标识设备是否已开采
	bool                    m_bIsColorFilter;         ///< 判断相机是否支持Bayer格式
	bool                    m_bImplementAutoGain;     ///< 是否支持自动增益
	bool                    m_bImplementAutoShutter;  ///< 是否支持自动曝光
	bool                    m_bImplementLight;        ///< 是否支持2A光照环境功能


	char                    m_chBmpBuf[2048];         ///< BIMTAPINFO存储缓冲区，m_pBmpInfo即指向此缓冲区
	GX_EXPOSURE_AUTO_ENTRY  m_emAutoShutterMode;      ///< 自动曝光模式
	GX_GAIN_AUTO_ENTRY      m_emAutoGainMode;         ///< 自动增益模式      


	int CameraInit();

	void		ResetBOOL();
	void		OnOpenDevice();
	void		OnStartCapture();
	int			MainFunction(HWND hwnd);

	void		SetShutter(double shutter);
	BOOL		InitCam();
	BOOL		m_IsDevOpen;
	GX_STATUS	m_emStatus;
	HWND		m_hwnd;
	// 为了表示在ShowImg里面已经取得图像

	//相机拍摄延迟
	int m_SnapDelay;

	//用于连续拍摄的延时，每一次获得图片的时候，整个线程会被延迟，否则会很占用CPU
	int m_ContinueSnapDelay;
	int isOpen();

public:
	BITMAPINFO             *m_pBmpInfo;	              ///< BITMAPINFO 结构指针，显示图像时使用
	BYTE                   *m_pRawBuffer;             ///< 存放原始RAW图的Buffer
	BYTE                   *m_pImgBuffer;             ///< 指向经过处理后的图像数据缓冲区

	GX_FRAME_DATA GrabImage(int delay);
	GX_FRAME_DATA FRAME_DATA;
};

extern CDahengCam* daheng;



#endif


