#pragma once
#include "MotionCard.h"
#include <iostream>
#include "../stdafx.h"



#ifdef LEADTECH

_Product_information *ProductInfo = NULL;

//���̶���
int Motion::Procedure()
{

}

bool Motion::clear_running_state()
{
	Controller.FrontDeTectionTicks = GetTickCount();
	Controller.DelayForGetChainBar = GetTickCount();						//����һ��ŵ�������׼�ʱ����ֹʱ��̫��û�źþͽ����˶�
	Controller.isGettingBar = false;
	Controller.DelayForPutOffChainBar = GetTickCount();						//����һ��ŵ�������׼�ʱ����ֹʱ��̫��û�źþͽ����˶�
	Controller.isPuttingBar = false;
	Controller.DelayForPutBackChainBar = GetTickCount();						//����һ��ŵ�������׼�ʱ����ֹʱ��̫��û�������ͽ����˶�
	Controller.isBackBar = false;
	Controller.isMotionChange = 0;											//����ѭ������Ƿ��иı䶯��
	Controller.DelayForOutResult = GetTickCount();
	Controller.FrontDetecting = false;						//�������ڼ��
	Controller.BackDetecting = false;
	Controller.FHavingBar = false;
	Controller.BHavingBar = false;
	return true;
}



































Motion::~Motion()
{
	delete ProductInfo;
	ProductInfo = NULL;
	close();
}


Motion::Motion()
	:CardSelect(0)
	, isInitOK(false)
	, OldBit(0)
	//, NewBit(0)
{
	_MAX_KNODE = 32;
	ProductInfo = new _Product_information[_MAX_KNODE];
	clear_running_state();
}


void Motion::clear_index_data()
{
	//memcpy(ProductInfo, 0x0, _MAX_KNODE*sizeof(_Product_information));
	for (int i = 0; i < _MAX_KNODE; i++)
	{
		ProductInfo[i].fdata = -1;
		ProductInfo[i].bdata = -1;
	}
}

_Product_information Motion::get_index_data(long id_index)
{
	return ProductInfo[id_index];
}


long Motion::caculate_index(long index, long current_id_position)	//index��ʾ�����źţ�current ��ʾ��ǰ��λλ�ã����ص�����������źŸñ�������λ��Ӧ���ǵڼ�����Ʒ
{
	if (index%_MAX_KNODE < current_id_position)
		return _MAX_KNODE + index%_MAX_KNODE - current_id_position;
	else
		return (index - current_id_position) % _MAX_KNODE;// - position;
}



void Motion::ConfigureDevice()
{
	return;
#if 0
	ErrorCode	errorCode;
	DeviceInformation devInfo(m_confParam.deviceNumber);
	errorCode = m_instantDoCtrl->setSelectedDevice(devInfo);
	CheckError(errorCode);
	errorCode = m_instantDoCtrl->LoadProfile(m_confParam.profilePath);
	CheckError(errorCode);

	m_instantDoCtrl->getSelectedDevice(devInfo);
	CString str;
	TCHAR des[MAX_DEVICE_DESC_LEN];
	str.Format(_T("Static DO - Run( %s )"), WCHAR_TO_TCHAR((LPCWSTR)devInfo.Description, des));
	SetWindowText(str);
#endif
}


bool Motion::startHomeThread()
{
	if (pHomeThread) return false;
	pHomeThread = AfxBeginThread(HomeThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	return true;
}

UINT Motion::HomeThread(LPVOID lp)
{
	Motion *p = (Motion*)lp;
	p->BackToOrigin(FIRST_MOTOR,g.ini.m_FstMtr.InitSpeed,g.ini.m_FstMtr.MaxSpeed,g.ini.m_FstMtr.Tacc,g.ini.m_FstMtr.compansate);
	delete p->pHomeThread;
	p->pHomeThread = NULL;
	return 0;
}

bool Motion::startHomeThread1()
{
	if (pHomeThread1) return false;
	pHomeThread1 = AfxBeginThread(HomeThread1, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	return true;
}

UINT Motion::HomeThread1(LPVOID lp)
{
	Motion *p = (Motion*)lp;
	p->BackToOrigin(SECOND_MOTOR, g.ini.m_SndMtr.InitSpeed, g.ini.m_SndMtr.MaxSpeed, g.ini.m_SndMtr.Tacc, g.ini.m_SndMtr.compansate);
	delete p->pHomeThread1;
	p->pHomeThread1 = NULL;
	return 0;
}

bool Motion::WaitSensor(short Bits, long TimeOut)
{
	DWORD tk = GetTickCount();
	for (; d1000_in_bit(Bits);)		//�ߵ�ƽ״̬
	{
		Sleep(1);
		if ((GetTickCount() - tk) > TimeOut)
			return false;
	}
	return true;
}

bool Motion::WaitSensorInv(short Bits, long TimeOut)
{
	DWORD tk = GetTickCount();
	for (; !d1000_in_bit(Bits);)		//�ߵ�ƽ״̬
	{
		Sleep(1);
		if ((GetTickCount() - tk) > TimeOut)
			return false;
	}
	return true;
}

int Motion::SysToOrigin()
{
	//Write_Output(OUT_PRESSURE, OFF);//�ر�
	//if (!WaitSensor(IN_PRESSURE, 2000)) return SYSSENSORPRESSURE;
	Write_Output(OUT_CYL, OFF);//������
	if (!WaitSensor(IN_CYL_UP, 2000)) return SYSSENSORCYL;
	int res = BackToOrigin(FIRST_MOTOR,g.ini.m_FstMtr.InitSpeed,g.ini.m_FstMtr.MaxSpeed,g.ini.m_FstMtr.Tacc,g.ini.m_FstMtr.compansate);
	if (res) return res;
	res = BackToOrigin(SECOND_MOTOR, g.ini.m_SndMtr.InitSpeed, g.ini.m_SndMtr.MaxSpeed, g.ini.m_SndMtr.Tacc, g.ini.m_SndMtr.compansate);
	if (res) return res;
	return 0;
}


int Motion::BackToOrigin(short Axis, int initspeed, int speed, double Tacc, int compansate)
{
	//0��ʾ������ԭ��
	int UP_LIMIT = MOTOR1_UP_LIMIT;
	int DOWN_LIMIT = MOTOR1_DOWN_LIMIT;
	int ORGIN_SENSOR = MOTOR1_ORIGIN;
	switch (Axis)
	{
	case FIRST_MOTOR:break;
	case SECOND_MOTOR:
		UP_LIMIT = MOTOR2_UP_LIMIT;
		DOWN_LIMIT = MOTOR2_DOWN_LIMIT;
		ORGIN_SENSOR = MOTOR2_ORIGIN;
		break;
	default:
		break;
	}
	 
	DWORD StartTick = GetTickCount();
	//�����˼���λ�ã�
	if (read_input_state(UP_LIMIT))			//��Ӧ���ӷ�������bug//
	{
		for (; read_input_state(UP_LIMIT);)
		{
			d1000_start_tv_move(Axis, -1*initspeed, -1*speed, Tacc);
			if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
			{
				d1000_immediate_stop(Axis);
				return SYSEMGENCY;
			}
			if ((GetTickCount() - StartTick) > 6000)
			{
				d1000_immediate_stop(Axis);
				return SYSBACKTIMEOUT;
			}
		}
		d1000_immediate_stop(Axis);
	}
	else
	{	//���ڼ���λ�ã�������λ�õķ����ߣ�Ȼ��
		for (; !read_input_state(UP_LIMIT);)
		{
			d1000_start_tv_move(Axis, initspeed, speed, Tacc);
			if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
			{
				d1000_immediate_stop(Axis);
				return SYSEMGENCY;
			}
			if ((GetTickCount() - StartTick) > 6000)
			{
				d1000_immediate_stop(Axis);
				return SYSBACKTIMEOUT;
			}
		}
		//��ʵ������Զ�ֹͣ
		d1000_immediate_stop(Axis);
		for (; read_input_state(UP_LIMIT);)
		{
			d1000_start_tv_move(Axis, -1 * initspeed, -1 * speed, Tacc);
			if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
			{
				d1000_immediate_stop(Axis);
				return SYSEMGENCY;
			}
			if ((GetTickCount() - StartTick) > 8000)
			{
				d1000_immediate_stop(Axis);
				return SYSBACKTIMEOUT;
			}
		}
		d1000_immediate_stop(Axis);
	}
	d1000_start_t_move(Axis, compansate, initspeed, speed, Tacc);
	if (!Wait(Axis, 5000))
	{
		d1000_immediate_stop(Axis);
		return SYSBACKTIMEOUT;
	}
	d1000_set_command_pos(Axis, 0);
	return 0;
}

void Motion::SysOutOK()
{
	d1000_out_bit(OUT_OK, ON);
	d1000_out_bit(OUT_NG, OFF);
}

void Motion::SysOutNG()
{
	d1000_out_bit(OUT_OK, OFF);
	d1000_out_bit(OUT_NG, ON);
}

void Motion::SysOutVoid()
{
	d1000_out_bit(OUT_OK, OFF);
	d1000_out_bit(OUT_NG, OFF);
}
















void Motion::InitPorts()
{
	

#if 0
	CBitmap   bmp;
	byte portData;
	for (int i = 0; i < portCount; i++)
	{
		portData = 0;
		for (int j = 0; j < 12; j++)
		{
			// set the port number
			CString portNum;
			portNum.Format(_T("%d"), i);
			GetDlgItem(m_ports[i][0])->SetWindowText(portNum);

			// set the portValue
			CString portVal;
			portVal.Format(_T("%X"), portsValue[i]);
			GetDlgItem(m_ports[i][1])->SetWindowText(portVal);
			portData = drawButton(i, j, errorCode, i, portsValue);
		}

		/*if ( Success == errorCode )
		{
		CString portVal;
		portVal.Format(_T("%X"),portData);
		GetDlgItem(m_ports[i][1])->SetWindowText(portVal);
		}*/

		int *pPort = m_ports[i];
		for (int i = 0; i < 12; i++)
		{
			GetDlgItem(*(pPort++))->ShowWindow(SW_SHOW);
		}
	}
#endif
}

int Motion::CheckError(ErrorCode errorCode)
{
	return 0;
}

bool Motion::close()
{
	
	d1000_out_bit(g.ini.m_SrvEnable, OFF);
	if (!isInitOK) return false;
	d1000_board_close();
	return true;
}


bool Motion::init()
{
	int cardno = 0;
	int tmp = 0;
	isInitOK = false;
	if (!d1000_board_init())
	{
		return false;
	}
	d1000_in_enable(0, IN_ENABLE);
	d1000_set_pls_outmode(0, 1);
	d1000_set_pls_outmode(1, 0);
	d1000_set_pls_outmode(2, 0);
	d1000_out_bit(g.ini.m_SrvEnable, ON);
	d1000_out_bit(OUT_PRESSURE,ON);
	
	if (ProductInfo) delete ProductInfo;
	ProductInfo = NULL;
	ProductInfo = new _Product_information[_MAX_KNODE];
	if (!ProductInfo) return false;
	isInitOK = true;
	return true;
	
}

bool Motion::Read_Input(short x)
{
	if (!isInitOK) return false;
	return !d1000_in_bit(x);
}



bool Motion::Read_Output(short x)
{
	if (!isInitOK) return false;
	return !d1000_get_outbit(x);
}

void Motion::Write_Output(short x, short LowOrHigh)
{
	//OutStatusToPort(0,0x02);����
	//OutStatusToPort(0,0x04);����
	if (!isInitOK) return;
	d1000_out_bit(x, LowOrHigh);
}

bool Motion::Get_Output(short x)
{
	if (!isInitOK) return false;
	return !d1000_get_outbit(x);
	/*
	ErrorCode errorCode = m_instantDoCtrl->Write(x, LowOrHigh);
	CheckError(errorCode);
	*/
}

bool Motion::Triger()
{
	if (!isInitOK) return false;
	DWORD NewBit  = !d1000_in_bit(IN_START);
	bool res = false;
	if ((NewBit != OldBit) && (NewBit>OldBit))
	{
		res = true;
	}
	OldBit = NewBit;
	return res;
}

bool Motion::Triger(int &a, int &b)
{

	if (!isInitOK) return false;

	//int portcount = m_instantDoCtrl->getFeatures()->getPortCount();

	//m_instantDiCtrl->Read(0, 4, &NewBit);
	/*
	a = NewBit;

	b = OldBit;

	if ((NewBit != OldBit) && NewBit)
	{
		OldBit = NewBit;
		return true;
	}
	else
	{
		OldBit = NewBit;
		return false;
	}
	*/
	return false;
}

void Motion::Front()
{
	if (!isInitOK) return;
	//ErrorCode errorCode = m_instantDoCtrl->Write(0, 0x02);
}


void Motion::Back()
{
	if (!isInitOK) return;
	//ErrorCode errorCode = m_instantDoCtrl->Write(0, 0x04);


}

void Motion::Reset()
{
	if (!isInitOK) return;
	//ErrorCode errorCode = m_instantDoCtrl->Write(0, 0x00);
}


long Motion::Get_Position(short Axis)
{
	return d1000_get_command_pos(Axis);
}

void Motion::InValid()
{
	isInitOK = false;
}

void Motion::Valid()
{
	isInitOK = true;
}

void Motion::MoveToOrigin(short Axis, int MaxSpeed, double Tacc)
{
	if (!isInitOK) return;
	d1000_decel_stop(Axis);
	int initspeed = 100;
	long pos = -d1000_get_command_pos(Axis);
	d1000_start_t_move(Axis, pos, initspeed, MaxSpeed, Tacc);
}


void Motion::MoveForward(short Axis, long speed)
{
	if (!isInitOK) return;
	double Tacc = 3.0;
	//long speed = 2 * g.ini.m_FirstMotorSpeed;
	long initspeed = 100;
	d1000_start_tv_move(Axis, initspeed, speed, Tacc);

}


void Motion::MoveBackward(short Axis, long speed)
{
	if (!isInitOK) return;
	double Tacc = 3.0;
	//long speed = -2 * g.ini.m_FirstMotorSpeed;
	long initspeed = 100;
	d1000_start_tv_move(Axis, initspeed, speed, Tacc);
}

void Motion::Stop(short Axis)
{
	d1000_decel_stop(Axis);
}


void Motion::MoveToDes(short Axis,int initspeed, int speed,double Tacc, long MtrWaves)
{
	if (!isInitOK) return;
	long pos = -d1000_get_command_pos(Axis);
	pos = MtrWaves - pos;
	d1000_start_t_move(Axis, pos, initspeed, speed, Tacc);
}


bool Motion::Wait(short Axis, DWORD TimeOut)
{
	DWORD Tick = GetTickCount();
	while (!d1000_check_done(Axis))
	{
		if ((GetTickCount() - Tick) > TimeOut)
		{
			return false;
		}
		Sleep(1);
	}
	return true;
}




int Motion::Home(short Axis, int initspeed, int speed, double Tacc)
{
	int IN_ORIGIN = 1;
	int UP_LIMIT = 0;
	int DOWN_LIMIT = 0;
	switch (Axis)
	{
	case FIRST_MOTOR:break;
	case SECOND_MOTOR:
		UP_LIMIT = MOTOR2_UP_LIMIT;
		DOWN_LIMIT = MOTOR2_DOWN_LIMIT;
		IN_ORIGIN = 2;
		break;
	default:
		break;
	}
#if 0
	DWORD StartTick = GetTickCount();
	if (d1000_in_bit(UP_LIMIT))			//��Ӧ���ӷ�������bug
	{
		for (; d1000_in_bit(UP_LIMIT);)
		{
			d1000_start_tv_move(FIRST_MOTOR, -1 * initspeed, -1 * speed, 0.05);
			if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
			{
				d1000_immediate_stop(Axis);
				return EMERGENGCY;;
			}
			if ((GetTickCount() - StartTick) > 6000)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return BACKTIMEOUT;
			}
		}
		d1000_immediate_stop(FIRST_MOTOR);

		for (; read_input_state(MOTOR1_ORG);)
		{
			d1000_start_tv_move(FIRST_MOTOR, -1 * initspeed, -1 * speed, 0.05);
			if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return EMERGENGCY;
			}
			if ((GetTickCount() - StartTick) > 6000)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return BACKTIMEOUT;
			}
		}
		Sleep(100);
		d1000_decel_stop(FIRST_MOTOR);
	}
	else
	{	//���ڼ���λ�ã�������λ�õķ����ߣ�Ȼ��
		for (; !read_input_state(UP_LIMIT);)
		{
			d1000_start_tv_move(FIRST_MOTOR, initspeed, speed, 0.05);
			if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return EMERGENGCY;
			}
			if ((GetTickCount() - StartTick) > 6000)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return BACKTIMEOUT;
			}
		}
		//��ʵ������Զ�ֹͣ
		d1000_immediate_stop(FIRST_MOTOR);
		for (; read_input_state(MOTOR1_UP_LIMIT);)
		{
			d1000_start_tv_move(FIRST_MOTOR, -1 * initspeed, -1 * speed, 0.05);
			if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return EMERGENGCY;
			}
			if ((GetTickCount() - StartTick) > 8000)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return BACKTIMEOUT;
			}
		}
		d1000_immediate_stop(FIRST_MOTOR);


		for (; read_input_state(MOTOR1_ORG);)
		{
			d1000_start_tv_move(FIRST_MOTOR, -1 * initspeed, -1 * speed, 0.05);
			if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return EMERGENGCY;
			}
			if ((GetTickCount() - StartTick) > 6000)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return BACKTIMEOUT;
			}
		}
		Sleep(100);
		d1000_decel_stop(FIRST_MOTOR);
		//d1000_immediate_stop(FIRST_MOTOR);

	}

	StartTick = GetTickCount();

	for (; !read_input_state(MOTOR1_ORG);)
	{
		d1000_start_tv_move(FIRST_MOTOR, initspeed, speed / 10, 0.05);
		if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
		{
			d1000_immediate_stop(FIRST_MOTOR);
			return EMERGENGCY;
		}
		if ((GetTickCount() - StartTick) > 6000)
		{
			d1000_immediate_stop(FIRST_MOTOR);
			return BACKTIMEOUT;
		}
	}

	d1000_immediate_stop(FIRST_MOTOR);
	g.RunDisable &= ~NOT_ALREADY;
	d1000_set_command_pos(FIRST_MOTOR, 0);
	return 0;
	//�ҵ�����λ��
	StartTick = GetTickCount();
	d1000_start_tv_move(SECOND_MOTOR, backspeed, backspeed, 0.05); //���ܲ���bug����Ҫ��������?
	StartTick = GetTickCount();
	for (; d1000_in_bit(FIRST_ORIGIN_SENSOR);) //origin is for (; !d1000_in_bit(FIRST_ORIGIN_SENSOR);) 
	{
		if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
		{
			d1000_immediate_stop(FIRST_MOTOR);
			return EMERGENGCY;
		}
		if ((GetTickCount() - StartTick) > 2000)
		{
			d1000_immediate_stop(FIRST_MOTOR);
			return BACKTIMEOUT;
		}
	}
	for (; !d1000_in_bit(FIRST_ORIGIN_SENSOR);)
	{
		if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
		{
			d1000_immediate_stop(FIRST_MOTOR);
			return EMERGENGCY;
		}
		if ((GetTickCount() - StartTick) > 6000)
		{
			d1000_immediate_stop(FIRST_MOTOR);
			return BACKTIMEOUT;
		}
	}
	d1000_immediate_stop(FIRST_MOTOR);

#endif
	return 0;





}


#endif





#ifdef _ADVENTECH


void Motion::ConfigureDevice()
{
	return;
#if 0
	ErrorCode	errorCode;
	DeviceInformation devInfo(m_confParam.deviceNumber);
	errorCode = m_instantDoCtrl->setSelectedDevice(devInfo);
	CheckError(errorCode);
	errorCode = m_instantDoCtrl->LoadProfile(m_confParam.profilePath);
	CheckError(errorCode);

	m_instantDoCtrl->getSelectedDevice(devInfo);
	CString str;
	TCHAR des[MAX_DEVICE_DESC_LEN];
	str.Format(_T("Static DO - Run( %s )"), WCHAR_TO_TCHAR((LPCWSTR)devInfo.Description, des));
	SetWindowText(str);
#endif
}

void Motion::InitPorts()
{
	//SetAllPorts(false);  //false:hide; true:show
	//for (int k = 0; k < 64; k++)
	//{
	//	m_picBoxStatus[k] = -1;
	//}
	m_portNum0 = 0;
	int portCountMax = m_instantDoCtrl->getFeatures()->getPortCount();
	byte * portsValue = new byte[portCountMax];
	ErrorCode errorCode = m_instantDoCtrl->Read(0, portCountMax, portsValue);

	CheckError(errorCode);

	int portCount = 0;
	if (portCountMax < 9)
	{
		portCount = portCountMax;
	}
	else
	{
		if (portCountMax >= 9)
		{
			portCount = 8;
		}
	}

#if 0
	CBitmap   bmp;
	byte portData;
	for (int i = 0; i < portCount; i++)
	{
		portData = 0;
		for (int j = 0; j < 12; j++)
		{
			// set the port number
			CString portNum;
			portNum.Format(_T("%d"), i);
			GetDlgItem(m_ports[i][0])->SetWindowText(portNum);

			// set the portValue
			CString portVal;
			portVal.Format(_T("%X"), portsValue[i]);
			GetDlgItem(m_ports[i][1])->SetWindowText(portVal);
			portData = drawButton(i, j, errorCode, i, portsValue);
		}

		/*if ( Success == errorCode )
		{
		CString portVal;
		portVal.Format(_T("%X"),portData);
		GetDlgItem(m_ports[i][1])->SetWindowText(portVal);
		}*/

		int *pPort = m_ports[i];
		for (int i = 0; i < 12; i++)
		{
			GetDlgItem(*(pPort++))->ShowWindow(SW_SHOW);
		}
	}
#endif
}

int Motion::CheckError(ErrorCode errorCode)
{
	if (errorCode != Success)
	{
		return errorCode;
	}
	return Success;
}

bool Motion::init()
{
	int cardno = 0;
	int tmp = 0;

	m_instantDiCtrl = AdxInstantDiCtrlCreate();
	m_instantDoCtrl = AdxInstantDoCtrlCreate();
	if ((m_instantDiCtrl != NULL) && (NULL != m_instantDoCtrl))
	{
		cardno++;
		isInitOK = true;

		int deviceNum = 0;
		DeviceInformation devInfo(deviceNum);

		ErrorCode errorCode = m_instantDiCtrl->setSelectedDevice(devInfo);
		if (errorCode != Success)
		{
			//CheckError(errorCode);
			return false;
		}
		m_instantDiCtrl->getSelectedDevice(devInfo);
		errorCode = m_instantDoCtrl->setSelectedDevice(devInfo);
		if (errorCode != Success)
			return false;//CheckError(errorCode);
		m_instantDoCtrl->getSelectedDevice(devInfo);

		m_IportCount = m_instantDiCtrl->getFeatures()->getPortCount();
		m_portData = new byte[m_IportCount];
		memset(m_portData, 0, m_IportCount);


		int portcount = m_instantDoCtrl->getFeatures()->getPortCount();
		errorCode = m_instantDiCtrl->setSelectedDevice(devInfo);
		if (errorCode != Success)
		{
			return false;
		}
		m_instantDiCtrl->getSelectedDevice(devInfo);
		errorCode = m_instantDoCtrl->setSelectedDevice(devInfo);
		if (errorCode != Success)
		{
			return false;
		}
		m_instantDoCtrl->getSelectedDevice(devInfo);
		m_instantDiCtrl->Read(0, 4, &NewBit);
		OldBit = NewBit;
	}
	else
	{
		isInitOK = false;
		return false;
	}
	if (0 == cardno) return false;
	else return true;
}

bool Motion::Read_Input(short x)
{
	if (!isInitOK) return false;
	ErrorCode errorCode;
	uint8 outportValue;
	errorCode = m_instantDoCtrl->Read(0, x, &outportValue);
	return !outportValue;
}

void Motion::Write_Output(short x, short LowOrHigh)
{
	//OutStatusToPort(0,0x02);����
	//OutStatusToPort(0,0x04);����
	if (!isInitOK) return;
	uint32 port = (uint32)x;
	uint8 bValue = (uint8)LowOrHigh;
	ErrorCode errorCode = m_instantDoCtrl->Write(port, bValue);
	
}

bool Motion::Get_Output(short x)
{
	if (!isInitOK) return false;
	return false;
	/*
	ErrorCode errorCode = m_instantDoCtrl->Write(x, LowOrHigh);
	CheckError(errorCode);
	*/
}

bool Motion::Triger()
{

	if (!isInitOK) return false;

	int portcount = m_instantDoCtrl->getFeatures()->getPortCount();
	NewBit = 0;
	m_instantDiCtrl->Read(0,4, &NewBit);


	if ((NewBit != OldBit) && (NewBit>OldBit))
	{
		OldBit = NewBit ;
		return true;
	}
	else
	{
		OldBit = NewBit ;
		return false;
	}

}

bool Motion::Triger(int &a, int &b)
{

	if (!isInitOK) return false;

	int portcount = m_instantDoCtrl->getFeatures()->getPortCount();

	m_instantDiCtrl->Read(0, 4, &NewBit);

	a = NewBit;

	b = OldBit;

	if ((NewBit != OldBit) && NewBit)
	{
		OldBit = NewBit;
		return true;
	}
	else
	{
		OldBit = NewBit;
		return false;
	}

}

void Motion::Front()
{
	if (!isInitOK) return;
	ErrorCode errorCode = m_instantDoCtrl->Write(0, 0x02);
}


void Motion::Back()
{
	if (!isInitOK) return;
	ErrorCode errorCode = m_instantDoCtrl->Write(0, 0x04);


}

void Motion::Reset()
{
	if (!isInitOK) return;
	ErrorCode errorCode = m_instantDoCtrl->Write(0, 0x00);
}

#endif