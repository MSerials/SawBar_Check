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


#define IN_ENABLE					123			//xy 轴的远点 限位感应器能够使用不做通用隔离

#define MOTOR1_ORIGIN				1			//第一轴原点感应器 板卡标识9
#define MOTOR2_ORIGIN				2			//第二轴原点感应器 板卡标识10

#define IN_PRESSURE					3			//负压检测
#define MOTOR1_UP_LIMIT				4			//第一轴上限 板卡标识12
#define MOTOR1_DOWN_LIMIT			5			//第一轴下限 板卡标识13

#define MOTOR2_UP_LIMIT				6			//第二轴上限 板卡标识14
#define MOTOR2_DOWN_LIMIT			7			//第二轴下限 板卡标识15

#define IN_CYL_UP					8			//气缸上限
#define IN_CYL_DOWN					9			//气缸下限

#define IN_START					10			//启动按钮板卡标识18
#define IN_FIRST_DETECT_PRODUCT		11			//第一工位检测物料 19
#define IN_SECOND_DETECT_PRODUCT	12			//第二工位检测物料板卡标识32
#define IN_EMGENCY					13			//急停按钮 板卡表示33
#define IN_ALM1						14			//电机报警1 板卡标识34
#define IN_ALM2						15			//电机报警2 板卡标识35



#define OUT_DETECT_OVER				1			//检测完成信号
#define OUT_DETECTING				2			//检测中信号
#define OUT_RUN						3			//程序运行信号 
#define OUT_SRV						4
#define OUT_NG						5
#define OUT_OK						6
#define OUT_CYL						7
#define OUT_PRESSURE				8			//气压电磁阀


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

	//参数传递使用
	struct _Controller
	{
		DWORD FrontDeTectionTicks;// = GetTickCount();
		DWORD DelayForGetChainBar;// r = GetTickCount();						//给第一块放导板的气缸计时，防止时间太短没放好就进行运动
		bool isGettingBar;// = false;
		DWORD DelayForPutOffChainBar;// = GetTickCount();						//给第一块放导板的气缸计时，防止时间太短没放好就进行运动
		bool isPuttingBar;// = false;
		DWORD DelayForPutBackChainBar;// = GetTickCount();						//给第一块放导板的气缸计时，防止时间太短没缩回来就进行运动
		bool isBackBar;// = false;
		DWORD isMotionChange;// = 0;											//用于循环检测是否有改变动作
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

	long caculate_index(long index, long current_id_position);	//index表示索引信号，current 表示当前工位位置，返回的是这个索引信号该表后这个工位对应的是第几个物品

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



