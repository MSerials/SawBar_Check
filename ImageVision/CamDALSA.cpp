// CamDALSA.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageVision.h"
#include "CamDALSA.h"
#include "ExampleUtils.h"

#include <iostream>
// CamDALSA

CamDALSA::CamDALSA()
{
	m_Acquisition = NULL;
	m_Buffers = NULL;
	m_Xfer = NULL;
	m_View = NULL;
	AcqDevice = NULL;
	pTransfer = NULL;
	initSuccess = false;
}

CamDALSA::~CamDALSA()
{
	_Destroy();
}


// CamDALSA 成员函数
BOOL CamDALSA::_Destroy()
{
	// Destroy FeatureInfo object
	//if (FeatureInfo && *FeatureInfo && !FeatureInfo->Destroy())
	//	return FALSE;

	// Destroy buffer object
	//if (Bayer && *Bayer && !Bayer->Destroy())
	//	return FALSE;

	// Destroy view object
	if (m_View && *m_View && !m_View->Destroy())
		return FALSE;

	// Destroy transfer object
	if (m_Xfer && *m_Xfer && !m_Xfer->Destroy())
		return FALSE;

	// Destroy buffer object
	if (m_Buffers && *m_Buffers && !m_Buffers->Destroy())
		return FALSE;


	if (AcqDevice && *AcqDevice && !AcqDevice->Destroy())
		return FALSE;

	// Destroy acquisition object
	//if (AcqDevice && *AcqDevice && !AcqDevice->Destroy())
	//	return FALSE;

	
	if (m_Acquisition) delete m_Acquisition;

	// Delete all objects
	if (m_View)	    delete m_View;//member
	if (m_Xfer)		delete m_Xfer;
	if (m_Buffers)	    delete m_Buffers;
	//if (Bayer)		delete Bayer;
	if (AcqDevice)	delete AcqDevice;
	//if (FeatureInfo) delete FeatureInfo;
	return TRUE;

}



std::vector<std::string>  CamDALSA::GetDeviceSituation1()
{
	vector<std::string> tmp1;
	//////// Ask questions to user to select acquisition board/device and config file ////////

	// Get total number of boards in the system
	int serverCount = SapManager::GetServerCount();
	char deviceIndexToPrint[STRING_LENGTH];
	char configFileIndexToPrint[STRING_LENGTH];

	if (serverCount == 0)
	{
		return tmp1;
	}

	int devicesToSkip = 0;
	for (int serverIndex = 0; serverIndex < serverCount; serverIndex++)
	{
		if (SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcqDevice) != 0)
		{
			char serverName[CORSERVER_MAX_STRLEN];
			SapManager::GetServerName(serverIndex, serverName, sizeof(serverName));
			printf("%s\n", serverName);
			//std::cout << serverName << std::endl;
			if (strstr(serverName, "CameraLink_") > 0)
			{
				devicesToSkip++;


			}
		}
	}

	// Scan the boards to find those that support acquisition
	BOOL serverFound = FALSE;
	BOOL cameraFound = FALSE;

	for (int serverIndex = 0; serverIndex < serverCount; serverIndex++)
	{
		if (SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcq) != 0)
		{
			char serverName[CORSERVER_MAX_STRLEN];
			SapManager::GetServerName(serverIndex, serverName, sizeof(serverName));
			std::string s = (std::string)serverName;
			tmp1.push_back(s);
			printf("Server %d: %s\n", serverIndex, serverName);
			serverFound = TRUE;
		}
		if (SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcqDevice) != 0)
		{
			char serverName[CORSERVER_MAX_STRLEN];
			SapManager::GetServerName(serverIndex, serverName, sizeof(serverName));
			if (strstr(serverName, "CameraLink_") > 0)
				continue;
			printf("Camera %d: %s\n", serverIndex, serverName);
			cameraFound = TRUE;
		}
	}

	// At least one acquisition server must be available
	if (!serverFound && !cameraFound)
	{
		printf("No acquisition server found!\n");

	}
	return tmp1;
}





vector <vector<std::string> > CamDALSA::GetDeviceSituation()
{
	vector<std::string> tmp1;
	vector<vector<std::string>> tmp2;
	
	//////// Ask questions to user to select acquisition board/device and config file ////////

	// Get total number of boards in the system
	int serverCount = SapManager::GetServerCount();
	char deviceIndexToPrint[STRING_LENGTH];
	char configFileIndexToPrint[STRING_LENGTH];
	
	if (serverCount == 0)
	{
		return tmp2;
	}

	int devicesToSkip = 0;
	for (int serverIndex = 0; serverIndex < serverCount; serverIndex++)
	{
		if (SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcqDevice) != 0)
		{
			char serverName[CORSERVER_MAX_STRLEN];
			SapManager::GetServerName(serverIndex, serverName, sizeof(serverName));
			printf("%s\n",serverName);
			//std::cout << serverName << std::endl;
			if (strstr(serverName, "CameraLink_") > 0)
			{
				devicesToSkip++;
				

			}
		}
	}

	// Scan the boards to find those that support acquisition
	BOOL serverFound = FALSE;
	BOOL cameraFound = FALSE;

	for (int serverIndex = 0; serverIndex < serverCount; serverIndex++)
	{
		if (SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcq) != 0)
		{
			char serverName[CORSERVER_MAX_STRLEN];
			SapManager::GetServerName(serverIndex, serverName, sizeof(serverName));
			std::string s = (std::string)serverName;
			tmp1.push_back(s);
			tmp2.push_back(tmp1);
			printf("Server %d: %s\n", serverIndex, serverName);
			serverFound = TRUE;
		}
		if (SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcqDevice) != 0)
		{
			char serverName[CORSERVER_MAX_STRLEN];
			SapManager::GetServerName(serverIndex, serverName, sizeof(serverName));
			if (strstr(serverName, "CameraLink_") > 0)
				continue;
			printf("Camera %d: %s\n", serverIndex, serverName);
			cameraFound = TRUE;
		}
	}

	// At least one acquisition server must be available
	if (!serverFound && !cameraFound)
	{
		printf("No acquisition server found!\n");

	}
	return tmp2;
}


int CamDALSA::Init(CString serverName, CString configFile, int index)
{
	//相机的初始化
	if (-1 == index) return NoSelect;


	if (0 == serverName.GetLength())
	{
		cout << "error in serverName" << (char*)(LPCSTR)(CStringA)serverName << endl;
		return ServerName;
	}


	OFSTRUCT of = { 0 };
	LPCSTR ConfigFile = (LPCSTR)(CStringA)configFile;
	if (OpenFile(ConfigFile, &of, OF_EXIST) == HFILE_ERROR)
	{
		return OpenConfigFail;
	}	

	SapLocation loc((char*)(LPCSTR)(CStringA)serverName, (UINT)index);
	if (SapManager::GetResourceCount((char*)(LPCSTR)(CStringA)serverName, SapManager::ResourceAcq) > 0)
	{
		m_Acquisition = new SapAcquisition(loc, (char*)(LPCSTR)(CStringA)configFile);
		m_Buffers = new SapBuffer(1, m_Acquisition);
	//	m_View = new SapView(m_Buffers, SapHwndAutomatic);
		m_Xfer = new SapAcqToBuf(m_Acquisition, m_Buffers);// , m_AcqCallback, View);

		// Create acquisition object
		if (m_Acquisition && !*m_Acquisition && !m_Acquisition->Create())
			return CanNotInitCam;
	}
	
	/*
	if (SapManager::GetResourceCount(AcqServerName, SapManager::ResourceAcqDevice) > 0)
	{
		if (strcmp(ConfigFileName, "NoFile") == 0)
			AcqDevice = new SapAcqDevice(loc, FALSE);
		else
			AcqDevice = new SapAcqDevice(loc, ConfigFileName);

		m_Buffers = new SapBufferWithTrash(2, AcqDevice);
		m_View = new SapView(m_Buffers, SapHwndAutomatic);
		m_Xfer = new SapAcqDeviceToBuf(AcqDevice, m_Buffers);// , AcqCallback, View);

		// Create acquisition object
		if (AcqDevice && !*AcqDevice && !AcqDevice->Create())
			return ErrorResourceAcqDevice;

	}
	*/

	if (m_Buffers && !*m_Buffers && !m_Buffers->Create())
		return 18;

	if (m_Xfer && !*m_Xfer && !m_Xfer->Create())
		return 19;


	if (m_View && !*m_View && !m_View->Create())
		return 20;

	return 0;

#if 0
	std::string severName = "Xcelera-CL_PX4_1";// (std::string)(CStringA)serverName;
	const char *_AcqServerName = severName.c_str();
	const char *_ConfigFileName = (char*)(LPCSTR)(CStringA)configFile;
	char AcqServerName[_MAX_PATH] = { 0 };
	char ConfigFileName[_MAX_PATH] = { 0 };
	strcpy_s(AcqServerName, _AcqServerName);
	strcpy_s(ConfigFileName, _ConfigFileName);

#endif
	return FALSE;

	

//	if (!GetOptions(argc, argv, acqServerName, &acqDeviceNumber, configFilename))
//	{
//		AfxMessageBox(L"没有检测到任何采集卡");
//	}


//	m_Acq->SetConfigFile(FileName);

#if 0
	m_Acq = new SapAcquisition(SapLocation("X64 - CL_1", index), configFileName);
	if (!m_Acq->Create()) return FALSE;





	SapBuffer *pBuffer = new SapBuffer(1, m_Acq);
	if (!pBuffer) return FALSE;
	SapView *pView = new SapView(pBuffer, SapHwndDesktop);

	// Allocate transfer object to link acquisition and buffer  
	SapTransfer *pTransfer = new SapTransfer(XferCallback, pView);
	pTransfer->AddPair(SapXferPair(m_Acq, pBuffer));

	// Create resources for all objects  
	BOOL success;
	success = pBuffer->Create();
	success = pView->Create();
	success = pTransfer->Create();
	if (!success) return FALSE;
	return TRUE;
#endif

}

void CamDALSA::XferCallback(SapXferCallbackInfo *pInfo)
{
	SapView *pView = (SapView *)pInfo->GetContext();
	//pView->Show();
}

void CamDALSA::Grab()
{
	BOOL success = m_Xfer->Grab();
}

void CamDALSA::Snap()
{
	BOOL success = m_Xfer->Snap();
}




BOOL CamDALSA::GetOptions(int argc, char *argv[], char *acqServerName, UINT32 *pAcqDeviceIndex, char *configFileName)
{
	// Check if arguments were passed
	if (argc > 1)
		return GetOptionsFromCommandLine(argc, argv, acqServerName, pAcqDeviceIndex, configFileName);
	else
		return GetOptionsFromQuestions(acqServerName, pAcqDeviceIndex, configFileName);
	return FALSE;
}

#if 0
BOOL CamDALSA::SelectOptions(char *acqServerName, UINT32 *pAcqDeviceIndex, char *configFileName)
{
	if (SapManager::GetServerIndex(acqServerName) < 0)
	{
		printf("Invalid acquisition server name!\n");
		return FALSE;
	}

	// Does the server support acquisition?
	int deviceCount = SapManager::GetResourceCount(acqServerName, SapManager::ResourceAcq);
	int cameraCount = SapManager::GetResourceCount(acqServerName, SapManager::ResourceAcqDevice);
	if (deviceCount + cameraCount == 0)
	{
		printf("This server does not support acquisition!\n");
		return FALSE;
	}

	CorStrncpy(acqServerName, argv[1], CORSERVER_MAX_STRLEN);
	*pAcqDeviceIndex = atoi(argv[2]);
	if (cameraCount == 0)
		CorStrncpy(configFileName, argv[3], MAX_PATH);

	return TRUE;

}
#endif

BOOL CamDALSA::GetOptionsFromCommandLine(int argc, char *argv[], char *acqServerName, UINT32 *pAcqDeviceIndex, char *configFileName)
{
	// Check the command line for user commands
	if ((strcmp(argv[1], "/?") == 0) || (strcmp(argv[1], "-?") == 0))
	{
		// print help
		printf("Usage:\n");
		printf("GrabCPP [<acquisition server name> <acquisition device index> <config filename>]\n");
		return FALSE;
	}

	// Check if enough arguments were passed
	if (argc < 4)
	{
		printf("Invalid command line!\n");
		return FALSE;
	}

	// Validate server name
	if (SapManager::GetServerIndex(argv[1]) < 0)
	{
		printf("Invalid acquisition server name!\n");
		return FALSE;
	}

	// Does the server support acquisition?
	int deviceCount = SapManager::GetResourceCount(argv[1], SapManager::ResourceAcq);
	int cameraCount = SapManager::GetResourceCount(argv[1], SapManager::ResourceAcqDevice);

	if (deviceCount + cameraCount == 0)
	{
		printf("This server does not support acquisition!\n");
		return FALSE;
	}

	// Validate device index
	if (atoi(argv[2]) < 0 || atoi(argv[2]) >= deviceCount + cameraCount)
	{
		printf("Invalid acquisition device index!\n");
		return FALSE;
	}

	if (cameraCount == 0)
	{
		// Verify that the specified config file exist
		OFSTRUCT of = { 0 };
		if (OpenFile(argv[3], &of, OF_EXIST) == HFILE_ERROR)
		{
			printf("The specified config file (%s) is invalid!\n", argv[3]);
			return FALSE;
		}
	}

	// Fill-in output variables
	CorStrncpy(acqServerName, argv[1], CORSERVER_MAX_STRLEN);
	*pAcqDeviceIndex = atoi(argv[2]);
	if (cameraCount == 0)
		CorStrncpy(configFileName, argv[3], MAX_PATH);

	return TRUE;
}