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

	//��ʼ���豸
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
	BITMAPINFO             *m_pBmpInfo;	              ///< BITMAPINFO �ṹָ�룬��ʾͼ��ʱʹ��
	BYTE                   *m_pRawBuffer;             ///< ���ԭʼRAWͼ��Buffer
	BYTE                   *m_pImgBuffer;             ///< ָ�򾭹�������ͼ�����ݻ�����





};

