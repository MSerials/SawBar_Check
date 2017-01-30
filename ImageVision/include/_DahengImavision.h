#pragma once

///��������ṹ��
typedef struct CAMER_INFO
{
	BITMAPINFO					*pBmpInfo;		  ///< BITMAPINFO �ṹָ�룬��ʾͼ��ʱʹ��
	BYTE						*pImageBuffer;	  ///< ָ�򾭹�������ͼ�����ݻ�����
	BYTE						*pRawBuffer;      ///< ָ��ԭʼRAWͼ������
	char						chBmpBuf[2048];	  ///< BIMTAPINFO �洢��������m_pBmpInfo��ָ��˻�����	
	int64_t                     nPayLoadSise;     ///< ͼ����С
	int64_t					    nImageWidth;	  ///< ͼ����
	int64_t					    nImageHeight;	  ///< ͼ��߶�	
	int64_t					    nBayerLayout;	  ///< Bayer�Ų���ʽ
	//int64_t						m_nPixelColorFilter; ///< Bayer��ʽ
	bool						bIsColorFilter;	  ///< �ж��Ƿ�Ϊ��ɫ���
	BOOL						bIsOpen;		  ///< ����Ѵ򿪱�־
	BOOL						bIsSnap;		  ///< ������ڲɼ���־
	float						fFps;			  ///< ֡��
}CAMER_INFO;


///��ȡ����ID��Ӧ����
char* GetFName(GX_FEATURE_ID emID);

///��ȡö�������ṹ��
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

	/// ������Ϣ��ʾ
	void ShowErrorString(GX_STATUS emErrorStatus);


public:
	GX_FRAME_DATA*      m_stFrameDatas;       ///< GetImage��ȡ��ͼ���ַ
	GX_DEV_HANDLE*	    m_hDevices;			///< �豸���ָ��
	CAMER_INFO*	  	    m_pstCam;			///< ������ݽṹ��		
	int				    m_nOperateID;		///< �����豸ID
	uint32_t		    m_nCamNum;			///< �����Ŀ
	BOOL			    m_bViewID;			///< �Ƿ���ʾ������
	int*		 	    m_npfpsCont;		///< ֡�ʼ���
	GX_DEVICE_BASE_INFO *m_pBaseinfo; 	    ///< �豸��Ϣ�ṹ��

	int m_nSnapTimeDelay;

};

