#include "ActionCaculation.h"
#include "MotionCard.h"
#include "include\\Preferences.h"

Action *act = NULL;

Action::Action()
	: m_spinwise1(1)
	, m_spinwise2(1)
{

}

int Action::Clock180()
{
	int tmp = 2*spinwise1*ini->m_MotorSpinWaves;
	start_t_move(FIRST_MOTOR, tmp, 50, ini->m_MotorSpinSpeed, 50);
	return 1;
}


int Action::CounterClock180()
{
	int tmp = -2*spinwise1*ini->m_MotorSpinWaves;
	start_t_move(FIRST_MOTOR, tmp, 50, ini->m_MotorSpinSpeed, 50);
	return 1;
}


int Action::CounterClock90()
{
	int tmp = -1 * spinwise1*ini->m_MotorSpinWaves;
	start_t_move(FIRST_MOTOR, tmp, 50, ini->m_MotorSpinSpeed, 50);
	return 1;
}


int Action::Clock90()
{
	int tmp = spinwise1*ini->m_MotorSpinWaves;
	start_t_move(FIRST_MOTOR, ini->m_MotorSpinWaves, 50, ini->m_MotorSpinSpeed, 50);
	return 1;
}


void Action::SecondMotorRun()
{
	start_t_move(SECOND_MOTOR, ini->m_MotorSpinWavesPerTime1, 50, ini->m_MotorSpinSpeed1, 50);
}

int Action::FisrtMotorBackToOrigin()
{
	d1000_home_move(0, 50, -ini->m_MotorSpinSpeed / 10, 10);
	if (d1000_check_done(FIRST_MOTOR))
	{
		d1000_start_t_move(FIRST_MOTOR, ini->m_MotorSpinWaves, ini->m_MotorSpinSpeed / 100, ini->m_MotorSpinSpeed / 10, 10);
		while (!d1000_check_done(FIRST_MOTOR))
		{
			if (::WaitForSingleObject(g_evtEStop.evt, 0) == WAIT_OBJECT_0) break;
			continue;
		}
		d1000_home_move(0, 50, ini->m_MotorSpinSpeed / 10, 10);
		while (!d1000_check_done(FIRST_MOTOR))
		{
			if (::WaitForSingleObject(g_evtEStop.evt, 0) == WAIT_OBJECT_0) break;
			continue;
		}
	}
	else
	{
		while (!d1000_check_done(FIRST_MOTOR))
		{
			if (::WaitForSingleObject(g_evtEStop.evt, 0) == WAIT_OBJECT_0) break;
			continue;
		}
	}
	Sleep(100);
	d1000_start_t_move(FIRST_MOTOR, ini->m_MotorCompenstate, ini->m_MotorSpinSpeed / 100, ini->m_MotorSpinSpeed / 10, 10);

	while (!d1000_check_done(FIRST_MOTOR))
	{
		if (::WaitForSingleObject(g_evtEStop.evt, 0) == WAIT_OBJECT_0) break;
		continue;
	}
	d1000_set_command_pos(0,0);
	return 1;
}

int Action::SeconMotorBackToOrigin()
{
	d1000_home_move(SECOND_MOTOR, 50, ini->m_MotorSpinSpeed1/5, 10);
	if (d1000_check_done(SECOND_MOTOR))
	{
		d1000_start_t_move(SECOND_MOTOR, -2000, ini->m_MotorSpinSpeed1 / 40, ini->m_MotorSpinSpeed1 / 10, 50);
		while (!d1000_check_done(SECOND_MOTOR))
		{
			if (::WaitForSingleObject(g_evtEStop.evt, 0) == WAIT_OBJECT_0) break;
			continue;
		}
		d1000_home_move(SECOND_MOTOR, ini->m_MotorSpinSpeed1/10, ini->m_MotorSpinSpeed1/5, 10);
		while (!d1000_check_done(SECOND_MOTOR))
		{
			if (::WaitForSingleObject(g_evtEStop.evt, 0) == WAIT_OBJECT_0) break;
			continue;
		}
	}
	else
	{
		while (!d1000_check_done(SECOND_MOTOR))
		{
			if (::WaitForSingleObject(g_evtEStop.evt, 0) == WAIT_OBJECT_0) break;
			continue;
		}
	}

	Sleep(100);
	d1000_start_t_move(SECOND_MOTOR, ini->m_MotorCompenstate1, ini->m_MotorSpinSpeed / 100, ini->m_MotorSpinSpeed / 10, 10);

	while (!d1000_check_done(SECOND_MOTOR))
	{
		if (::WaitForSingleObject(g_evtEStop.evt, 0) == WAIT_OBJECT_0) break;
		continue;
	}

	d1000_set_command_pos(SECOND_MOTOR, 0);
	return 1;
}



int Action::ChopStickCYLPushOut()
{
	write_output(OUT_PUSHOUTCHOPSTIC, PUSH_OUT);
	return 1;
}


int Action::ChopStickCYLPushBack()
{
	write_output(OUT_PUSHOUTCHOPSTIC, PUSH_BACK);
	return 1;
}


int Action::PushCylPush()
{
	write_output(OUT_PUSHCHOPSTICALINE, PUSH_OUT);
	return 1;
}

int Action::PushCylPushBack()
{
	write_output(OUT_PUSHCHOPSTICALINE, PUSH_BACK);
	return 1;
}

int Action::TrapOpen()
{
	write_output(OUT_TRAP_CYL, PUSH_BACK);
	return 1;
}


int Action::TrapClose()
{
	write_output(OUT_TRAP_CYL, PUSH_OUT);
	return 1;
}



int Action::ContiuneRun()
{
	return 0;
}


int Action::FirstMotorStepRunCW()
{
	int tmp = m_spinwise1 * ini->m_MotorSpinWavesPerTime;
	start_t_move(FIRST_MOTOR, tmp, 100, ini->m_MotorSpinSpeed, 10);
	return 1;
}

int Action::FirstMotorStepRunCCW()
{
	int tmp = m_spinwise1 * ini->m_MotorSpinWavesPerTime;
	tmp = 0-tmp;
	start_t_move(FIRST_MOTOR, tmp, 100, ini->m_MotorSpinSpeed, 10);
	return 1;
}

int Action::SecondMotorStepRunCW()
{
	int tmp = m_spinwise2 * ini->m_MotorSpinWavesPerTime1;
	start_t_move(SECOND_MOTOR, tmp, 100, ini->m_MotorSpinSpeed1, 10);
//	CString str;
//	str.Format(_T("%d and speed is %d"), tmp, ini->m_MotorSpinSpeed1);
//	AfxMessageBox(str);
	return 1;
}

int Action::SecondMotorStepRunCCW()
{
	int tmp = m_spinwise2 * ini->m_MotorSpinWavesPerTime1;
	tmp = 0 - tmp;
	start_t_move(SECOND_MOTOR, tmp, 100, ini->m_MotorSpinSpeed1, 10);
	
	return 1;
}


int Action::ConveyorStepRun()
{
	int tmp = m_spinwise2 * ini->m_MotorSpinWaves1;
	start_t_move(SECOND_MOTOR, tmp, ini->m_MotorSpinSpeed1/10, ini->m_MotorSpinSpeed1, 10);
	return 1;
}


bool Action::wait_input_norm(short Bits, DWORD Time)
{
	DWORD startTick = GetTickCount();

	while (d1000_in_bit(Bits))
	{
		int endTick = GetTickCount();
		if ((endTick - startTick) > Time)
		{ 
			return false;
		}
		Sleep(1);
	}
	return true;
}

bool Action::wait_input_inverse(short Bits, DWORD Time)
{
	DWORD startTick = GetTickCount();

	while (!d1000_in_bit(Bits))
	{
		int endTick = GetTickCount();
		if ((endTick - startTick) > Time)
		{ 
			return false;
		}
		Sleep(1);
	}
	return true;
}

bool Action::WaitMotorTimeout(short No, DWORD time)
{
	DWORD startTick = GetTickCount();
	while (!d1000_check_done(No))
	{
		if ((GetTickCount() - startTick) > time)
		{
			return false;
		}
		Sleep(1);
	}
	return true;
	
}
