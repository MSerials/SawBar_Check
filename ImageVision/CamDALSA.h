#pragma once
#include "stdafx.h"
#include <vector>
// CamDALSA 
#define _DALSA
#ifdef _DALSA


#if !defined(__DALSA__CAMERA__INCLUDE__)
#define  __DALSA__CAMERA__INCLUDE__

typedef enum tagError_Code{
//	OpenConfigFail= 0x00000001,
//	BottomInvalid = 0x00000002,
}CamErrorCode;

enum{
	OpenConfigFail = 0x1,
	CanNotInitCam = 0x2,
	ServerName = 0x4,
	ErrorResourceAcqDevice = 0x8,
	NoSelect = 16
};


#include "SapClassBasic.h"

#ifdef _WIN64
#pragma comment(lib,"lib/dalsa/Win64/SapClassBasic.lib")
#else
#pragma comment(lib,"lib/dalsa/Win32/SapClassBasic.lib")
#endif



class CamDALSA : public CObject
{
public:
	CamDALSA();
	virtual ~CamDALSA();
	BOOL _Destroy();
	bool initSuccess;
	int Init(CString serverName, CString configFile, int index = 0);
	static void XferCallback(SapXferCallbackInfo *pInfo);
	void Grab();
	void Snap();

	vector<vector<std::string> > GetDeviceSituation();
	std::vector<std::string>  GetDeviceSituation1();
	static BOOL GetOptions(int argc, char *argv[], char *acqServerName, UINT32 *pAcqDeviceIndex, char *configFileName);
	static BOOL GetOptionsFromCommandLine(int argc, char *argv[], char *acqServerName, UINT32 *pAcqDeviceIndex, char *configFileName);




#ifdef _HALCON
	Halcon::Hobject ToHobject();
#endif // _HALCON


	/*
	CImageExWnd		m_ImageWnd;
	*/
	SapAcquisition	*m_Acquisition;
	SapBuffer		*m_Buffers;
	SapTransfer		*m_Xfer;
	SapView         *m_View;
	SapTransfer		*pTransfer;
	SapAcqDevice			*AcqDevice;
};

#endif

#endif