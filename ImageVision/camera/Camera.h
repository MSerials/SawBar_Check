#pragma once
#include "../stdafx.h"
#include <iostream>
using namespace std;
//#include "DahengCam.h"


class Camera
{

public:
	struct tagCameraInfo
	{
		std::string CameraBrand;
		std::string CameraSerial;
		int CameraNo;

	}*CameraInfo, InitInfo;

	//初始化设备
	virtual int CameraInit()
	{
#ifdef _DEBUG
		cout << "no camera found" << endl;
#endif 
		return 0;
	};
	int CameraNum;


public:
	Camera();
	~Camera();







public:
	BITMAPINFO             *m_pBmpInfo;	              ///< BITMAPINFO 结构指针，显示图像时使用
	BYTE                   *m_pRawBuffer;             ///< 存放原始RAW图的Buffer
	BYTE                   *m_pImgBuffer;             ///< 指向经过处理后的图像数据缓冲区





};

