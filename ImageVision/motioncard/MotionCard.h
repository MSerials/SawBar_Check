#pragma once
//#include "../stdafx.h"
#include <vector>
using namespace std;


#ifndef LEADTECH
#define LEADTECH
#endif

#ifdef _ADVENTECH

#ifndef __MOTION_CARD__
#define __MOTION_CARD__
#include "bdaqctrl.h"
using namespace Automation::BDaq;




class Motion
{
public:
	Motion();
	~Motion();

public:
	bool Read_Input(short x);
	void Write_Output(short x, short LowOrHigh);
	bool Get_Output(short x);
	bool init();

	void Front(); //output front
	void Back(); //output back
	void Reset(); //output reset
	bool Triger(); //triger for program
	bool Triger(int &a, int &b); //triger for program
	int isInitOK; //if not initok then nothing can do for card

private:
	vector<string> motioncardlist;
	int CardSelect;
	DWORD OldBit; //for read input
	//uint8 NewBit; //for read input


	int m_IportCount;
	int m_portNum0;
	byte* m_portData;


	InstantDiCtrl		*m_instantDiCtrl;
	InstantDoCtrl		*m_instantDoCtrl;
	int m_ports[8][12];
	void SetAllPorts(bool state);
	int CheckError(ErrorCode errorCode);
	void InitPorts();
	void ConfigureDevice();
#ifdef _BDAQ_TYPES_DEFINED

#define Error_Code ErrorCode 
#endif

};



#endif

#endif



#ifdef LEADTECH




#ifndef __MOTION_CARD__
#define __MOTION_CARD__


#include "afxwin.h"

#ifdef _WIN64
#include "DMC1380/x64/Dmc1380.h"
//#pragma comment(lib,"DMC1380\\x64\\Dmc1380.lib")
#else
#include "DMC1380/x86/Dmc1380.h"
//#pragma comment(lib,"DMC1380\\x86\\Dmc1380.lib")
#endif
//#include "../stdafx.h"

#define ON 0
#define OFF 1

#define FIRST_MOTOR 0
#define SECOND_MOTOR 1
#define THIRD_MOTOR 2

#define RUN_TIME_OUT 1
#define RUN_NOT_ALREADY (1<<1)

#define read_input_state(x)										!d1000_in_bit(x)


#define IN_ENABLE					123			//xy ���Զ�� ��λ��Ӧ���ܹ�ʹ�ò���ͨ�ø���

#define MOTOR1_ORIGIN				1			//��һ��ԭ���Ӧ�� �忨��ʶ9
#define MOTOR2_ORIGIN				2			//�ڶ���ԭ���Ӧ�� �忨��ʶ10

#define IN_PRESSURE					3			//��ѹ���
#define MOTOR1_UP_LIMIT				4			//��һ������ �忨��ʶ12
#define MOTOR1_DOWN_LIMIT			5			//��һ������ �忨��ʶ13

#define MOTOR2_UP_LIMIT				6			//�ڶ������� �忨��ʶ14
#define MOTOR2_DOWN_LIMIT			7			//�ڶ������� �忨��ʶ15

#define IN_CYL_UP					8			//��������
#define IN_CYL_DOWN					9			//��������

#define IN_START					10			//������ť�忨��ʶ18
#define IN_FIRST_DETECT_PRODUCT		11			//��һ��λ������� 19
#define IN_SECOND_DETECT_PRODUCT	12			//�ڶ���λ������ϰ忨��ʶ32
#define IN_EMGENCY					13			//��ͣ��ť �忨��ʾ33
#define IN_ALM1						14			//�������1 �忨��ʶ34
#define IN_ALM2						15			//�������2 �忨��ʶ35



#define OUT_DETECT_OVER				1			//�������ź�
#define OUT_DETECTING				2			//������ź�
#define OUT_RUN						3			//���������ź� 
#define OUT_SRV						4
#define OUT_NG						5
#define OUT_OK						6
#define OUT_CYL						7
#define OUT_PRESSURE				8			//��ѹ��ŷ�


#define EMERGENGCY  1
#define BACKTIMEOUT 2

#define MAX_PRODUCT 32

typedef int ErrorCode;
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned char byte;


//typedef void *lpvoid;

struct _Product_information
{
	int fdata;
	int bdata;
};

extern _Product_information *ProductInfo;

class Motion
{
public:
	Motion();
	~Motion();

public:

	//��������ʹ��
	struct _Controller
	{
		DWORD FrontDeTectionTicks;// = GetTickCount();
		DWORD DelayForGetChainBar;// r = GetTickCount();						//����һ��ŵ�������׼�ʱ����ֹʱ��̫��û�źþͽ����˶�
		bool isGettingBar;// = false;
		DWORD DelayForPutOffChainBar;// = GetTickCount();						//����һ��ŵ�������׼�ʱ����ֹʱ��̫��û�źþͽ����˶�
		bool isPuttingBar;// = false;
		DWORD DelayForPutBackChainBar;// = GetTickCount();						//����һ��ŵ�������׼�ʱ����ֹʱ��̫��û�������ͽ����˶�
		bool isBackBar;// = false;
		DWORD isMotionChange;// = 0;											//����ѭ������Ƿ��иı䶯��
		DWORD DelayForOutResult;

		bool FHavingBar;
		bool BHavingBar;
		bool FrontDetecting;// = false;
		bool BackDetecting;
}Controller;


	bool CylHaveProduct()
	{
		return (d1000_in_bit(IN_PRESSURE) && !d1000_get_outbit(OUT_PRESSURE));
	}
	//int BackToOrigin();
	bool clear_running_state();
	int Procedure();
	bool Read_Input(short x);
	bool Read_Output(short x);
	void Write_Output(short x, short LowOrHigh);
	bool Get_Output(short x);
	bool init();
	bool close();
	void Front(); //output front
	void Back(); //output back
	void Reset(); //output reset
	bool Triger(); //triger for program
	bool Triger(int &a, int &b); //triger for program
	int isInitOK; //if not initok then nothing can do for card
	long Get_Position(short Axis);

	bool WaitSensor(short Bits, long TimeOut);

	bool WaitSensorInv(short Bits, long TimeOut);

	void InValid();

	void Valid();

	//void MoveToOrigin(short Axis);
	void MoveToOrigin(short Axis, int MaxSpeed, double Tacc);

	void MoveBackward(short Axis, long speed);

	void MoveForward(short Axis, long speed);

	void Stop(short Axis);

	void MoveToDes(short Axis, int initspeed, int speed, double Tacc, long MtrWaves);

	void SetMotorZero(short Axis){ d1000_set_command_pos(Axis, 0); }

	bool Wait(short Axis, DWORD TimeOut);

	bool isRunning(short Axis){ return !d1000_check_done(Axis); }

	int Home(short Axis, int initspeed, int speed, double Tacc);
	
	int BackToOrigin(short Axis, int initspeed, int speed, double Tacc, int compansate);

	void SysOutOK();

	void SysOutNG();

	void SysOutVoid();

	int SysToOrigin();

	void clear_index_data();

	long caculate_index(long index, long current_id_position);	//index��ʾ�����źţ�current ��ʾ��ǰ��λλ�ã����ص�����������źŸñ�������λ��Ӧ���ǵڼ�����Ʒ

	_Product_information Motion::get_index_data(long id_index);

	bool startHomeThread();
	void *pHomeThread;
	static UINT HomeThread(LPVOID lp);

	bool startHomeThread1();
	void *pHomeThread1;
	static UINT HomeThread1(LPVOID lp);

	int get_max_knode(){ return _MAX_KNODE; }

private:

	int _MAX_KNODE;

	vector<string> motioncardlist;
	int CardSelect;
	DWORD OldBit; //for read input
	//uint8 NewBit; //for read input

	int m_IportCount;
	int m_portNum0;
	byte* m_portData;

	int m_ports[8][12];
	void SetAllPorts(bool state);
	int CheckError(ErrorCode errorCode);
	void InitPorts();
	void ConfigureDevice();



	

};



#endif


#endif



