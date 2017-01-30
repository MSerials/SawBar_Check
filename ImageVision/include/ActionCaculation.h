#include "stdafx.h"
#include "stdlib.h"
#include <vector>
using namespace std;

#ifndef __Action__
#define	__Action__

#define 步进电机回原点						act->FisrtMotorBackToOrigin()

#define 伺服电机回原点						act->SeconMotorBackToOrigin()

#define 高电平等待感应器(bit,time)			act->wait_input_inverse(bit,time)

#define 低电平等待感应器(bit,time)			act->wait_input_norm(bit,time)

#define wait_sensor_timeout(bit,time)		act->wait_input_norm(bit,time)

#define wait_motor_timeout(No,time)			act->WaitMotorTimeout(No,time)

#define first_motor_backs_to_origin			act->FisrtMotorBackToOrigin()

#define second_motor_backs_to_origin		act->SeconMotorBackToOrigin()

#define counter_clock_180					act->CounterClock180()

#define clock_180							act->Clock180()

#define counter_clock_90					act->CounterClock90()

#define clock_90							act->Clock90()

#define trap_open							act->TrapOpen()

#define trap_close							act->TrapClose()

#define conveyor_step_run					act->ConveyorStepRun()

#define continue_run						act->ContiuneRun()

#define chopstick_cyl1_push_out				act->ChopStickCYLPushOut()

#define chopstick_cyl1_push_back			act->ChopStickCYLPushBack()

#define chopstick_cyl2_push_out				act->PushCylPush()

#define chopstick_cyl2_push_back			act->PushCylPushBack()

#define second_motor_run()					act->SecondMotorRun()

#define first_motor_CCW						act->FirstMotorStepRunCCW()

#define first_motor_CW						act->FirstMotorStepRunCW()

#define second_motor_CCW					act->SecondMotorStepRunCCW()

#define second_motor_CW						act->SecondMotorStepRunCW()

#define spinwise1							act->m_spinwise1

#define spinwise2							act->m_spinwise2

class Action
{
public:
	Action();

	int CounterClock180();
	int CounterClock90();
	int Clock90();
	int FisrtMotorBackToOrigin();
	int SeconMotorBackToOrigin();
	int PushCylPushBack();
	int PushCylPush();
	int TrapOpen();
	int TrapClose();
	int ContiuneRun();
	int FirstMotorStepRunCW();
	int FirstMotorStepRunCCW();
	int SecondMotorStepRunCW();
	int SecondMotorStepRunCCW();
	int ChopStickCYLPushOut();
	int ChopStickCYLPushBack();
	void SecondMotorRun();
	int Clock180();
	int m_spinwise1;
	int m_spinwise2;
	int ConveyorStepRun();
	bool wait_input_norm(short Bits, DWORD Time);
	bool wait_input_inverse(short Bits, DWORD Time);
	bool WaitMotorTimeout(short No, DWORD time);
};

extern Action *act;

#endif