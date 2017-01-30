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


 
///< ������ʾ�����궨��
#define  GX_VERIFY(emStatus) \
if (emStatus != GX_STATUS_SUCCESS)\
{\
	ShowErrorString(emStatus); \
	return; \
}  ///< ������ʾ�����궨��

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

	/// �ص�����
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// ��ȡ�豸�Ŀ�ߵ�������Ϣ
	GX_STATUS GetDeviceParam();

	/// ������������ݸ�ʽ,ʹ�����ͼ��λ��Ϊ8-bit
	GX_STATUS SetPixelFormat8bit();

	/// �豸��ʼ��
	GX_STATUS InitDevice();

	///  Ϊ��ɫͼ����ʾ׼����Դ
	bool PrepareForShowColorImg();

	/// Ϊ�ڰ�ͼ����ʾ׼����Դ
	bool PrepareForShowMonoImg();

	/// Ϊͼ����ʾ׼����Դ,����Buffer
	bool PrepareForShowImg();

	/// ���¸���Ȥ�����ȡֵ��Χ
	void UpdateROIRange();

	/// �ͷ�Ϊͼ����ʾ׼������Դ
	void UnPrepareForShowImg();

	/// ��ȡö������Ϣ����ʼ�����Ӧ��Combox��
	void InitEnumUI(GX_FEATURE_ID emFeatureID, CComboBox *pComboBox, bool bIsImplemented);

	/// ��ʾͼ��
	void DrawImg(BYTE *pImageBuffer);

	/// ��ʼ��UI����
	void InitUI();

	/// ��ʼ���ع���صĿؼ�
	void InitShutterUI();

	/// ��ʼ��������صĿؼ�
	void InitGainUI();

	/// ��ʼ�������Ҷ�ֵ��صĿؼ�
	void InitGrayUI();

	/// ��ʼ���Զ�����Combox�ؼ�
	void InitAutoGainUI();

	/// ��ʼ������Ȥ����Ŀ��ߡ�X�����Y�����ֵ
	void InitROIUI();

	/// ��ʼ���Զ��ع�Combox�ؼ�
	void InitAutoShutterUI();

	/// ��ʼ��2A���ջ���Combox�ؼ�
	void InitLightEnvironmentUI();

	/// ˢ�½���
	void UpDateUI();

	/// ˢ����ʾ�ع�����浱ǰֵ
	void RefreshCurValue();

	///��ȡ������Ϣ
	void ShowErrorString(GX_STATUS emErrorStatus);





	int		m_nGray;               ///< �����Ҷ�ֵ
	int		m_nRoiX;               ///< 2A����Ȥ����X����
	int		m_nRoiY;               ///< 2A����Ȥ����Y����
	int		m_nRoiH;               ///< 2A����Ȥ�����
	int		m_nRoiW;               ///< 2A����Ȥ�����
	double	m_dAutoGainMin;        ///< �Զ�������Сֵ
	double  m_dAutoGainMax;        ///< �Զ��������ֵ
	double	m_dAutoShutterMin;     ///< �Զ��ع���Сֵ
	double	m_dAutoShutterMax;     ///< �Զ��ع����ֵ

	double	m_dShutter;            ///< �ع�ֵ
	double  m_dGain;               ///< ����ֵ

	/*
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	*/

public:
	GX_DEV_HANDLE           m_hDevice;                ///< �豸���
	int64_t                 m_nImageWidth;            ///< ������ͼ����
	int64_t                 m_nImageHeight;           ///< ������ͼ��߶�
	int64_t                 m_nPayLoadSize;           ///< �豸���ԭʼͼ���С
	int64_t                 m_nPixelColorFilter;      ///< ��ɫ�����Bayer��ʽ
	double                  m_dShutterInc;            ///< �ع���ڲ���
	double                  m_dGainInc;               ///< ������ڲ���           

	BOOL                    m_bDevOpened;             ///< ��ʶ�豸�Ƿ��Ѵ�
	BOOL                    m_bIsSnap;                ///< ��ʶ�豸�Ƿ��ѿ���
	bool                    m_bIsColorFilter;         ///< �ж�����Ƿ�֧��Bayer��ʽ
	bool                    m_bImplementAutoGain;     ///< �Ƿ�֧���Զ�����
	bool                    m_bImplementAutoShutter;  ///< �Ƿ�֧���Զ��ع�
	bool                    m_bImplementLight;        ///< �Ƿ�֧��2A���ջ�������


	char                    m_chBmpBuf[2048];         ///< BIMTAPINFO�洢��������m_pBmpInfo��ָ��˻�����
	GX_EXPOSURE_AUTO_ENTRY  m_emAutoShutterMode;      ///< �Զ��ع�ģʽ
	GX_GAIN_AUTO_ENTRY      m_emAutoGainMode;         ///< �Զ�����ģʽ      


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
	// Ϊ�˱�ʾ��ShowImg�����Ѿ�ȡ��ͼ��

	//��������ӳ�
	int m_SnapDelay;

	//���������������ʱ��ÿһ�λ��ͼƬ��ʱ�������̻߳ᱻ�ӳ٣�������ռ��CPU
	int m_ContinueSnapDelay;
	int isOpen();

public:
	BITMAPINFO             *m_pBmpInfo;	              ///< BITMAPINFO �ṹָ�룬��ʾͼ��ʱʹ��
	BYTE                   *m_pRawBuffer;             ///< ���ԭʼRAWͼ��Buffer
	BYTE                   *m_pImgBuffer;             ///< ָ�򾭹�������ͼ�����ݻ�����

	GX_FRAME_DATA GrabImage(int delay);
	GX_FRAME_DATA FRAME_DATA;
};

extern CDahengCam* daheng;



#endif


