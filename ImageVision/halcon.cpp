
#pragma once
#include "stdafx.h"
#include "halcon.h"

_CMutexLock::_CMutexLock()
{

#ifndef _MSC_VER 
	m_mutex = NULL;
	m_condition = NULL;
#else  
	m_mutex = NULL;
	m_event = NULL;
#endif  
}
_CMutexLock::~_CMutexLock()
{
	release();
}
void _CMutexLock::release()
{
#ifdef _MSC_VER  
	CloseHandle(m_mutex);//�����ں˶��󣬻�����������ʽ�����ģ���������closeHandle�����ü�����1��  
	m_mutex = NULL;
#else  
	pthread_mutexattr_destroy(&m_mutexAttr);
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_condition);
	m_mutex = NULL;
	m_condition = NULL;
#endif  
}

void _CMutexLock::init()
{
#ifdef _MSC_VER 
	// arg1 ��NULL,��������Ĭ�ϵİ�ȫ������Ϣ,���ʱ���ӽ��̲��ܼ̳иû�����.  
	// arg2 �ǵ�ǰָ��������ָ����߳�Ϊ��,�ұ����õĴ�����0��û���߳�/����ӵ�иû�����������ǰ�߳�ӵ�иû�������  
	// arg3 ������������  
	m_mutex = CreateMutex(NULL, FALSE, NULL);
	DWORD dwLastError = GetLastError();
	if (dwLastError == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(m_mutex);
		m_mutex = NULL;
	}
	//�¼������ź��̲߳����������ź������߳�˯�ߡ�  
	// arg1���¼����ԡ�  
	// arg2���ֶ������Զ�����ResetEvent���¼�����Ϊ���ź�,SetEvent�ǽ��¼�����Ϊ���ź�  
	// ResetEvent�Ƿ��ֶ�����Ϊ���ź�,WaitForSingleObject��������Զ���ʽ��ô���Զ�����ResetEvent���¼�����Ϊ���źš�  
	// arg3�ǳ�ʼ״̬�źţ�һ������ΪFALSE���źţ����̹߳���������  
	// arg4���̵߳����֡�  
	m_event = CreateEvent(NULL, FALSE, FALSE, NULL);
#else  
	// arg1�ǳ�ʼ���Ļ�������arg2��pthread_mutexattr_t����ָ�룬�����NULL,��ôû���߳�ӵ�иó�ʼ���õĻ�������  
	int nResult = pthread_mutex_init(&m_mutex, NULL);
	if (nResult == 0)
	{
		printf("pthread_mutex_init result OK.\n");
	}
	else
	{
		printf("pthread_mutex_init result error:%d\n", nResult);
	}
	pthread_mutexattr_init(&m_mutexAttr);
	// ���� recursive ����,ʹ��linux�¿��Եݹ����������ݹ����������  
	pthread_mutexattr_settype(&m_mutexAttr, PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(&m_mutex, &m_mutexAttr);

	pthread_cond_init(&m_condition, NULL);
#endif  
}

void _CMutexLock::lock()
{
#ifdef _MSC_VER 
	// arg2�ǵȴ�����ʱ�䣬INFINITE����Զ�ȴ���ֱ�����ں˶��󱻴������ã��ú�����һ���첽���ú�����������ӵ���߳�id��0��  
	// ��ô�ú������������������ͷŵ�ǰCPUӵ��Ȩ�����������߳��ͷŻ�����ӵ���߳�idΪ0�����ỽ�ѵ�ǰ�������߳����»�ȡ��������  
	WaitForSingleObject(m_mutex, INFINITE);
	/*if(WaiteforSingleObject(m_hMutex, dwMilliSec) == WAIT_OBJECT_0)
	{
	return true;
	}
	return false;
	*/
#else  
	// ����������������û������������߳�ӵ�У���ô��������������ָ�����òŻص����أ�  
	// �߳��Լ�����������ᵼ�������������߳���Ҫ����������໥�ȴ��Է��Ļ�������Ҳ�ᵼ��������  
	pthread_mutex_lock(&m_mutex);
#endif  
}

void  _CMutexLock::waite()
{
#ifdef _MSC_VER 
	WaitForSingleObject(m_event, INFINITE);// �ȴ����¼�����ʱ��  
#else  
	//���Զ�����pthread_mutex_unlock(&m_mutex)�ͷŻ�����������ǰ�̹߳����������ȴ��Է��߳�pthread_cond_signal֪ͨ���ѣ�  
	// ���Ѻ�pthread_cond_wait�����pthread_mutex_lock����������������  
	// pthread_cond_timedwait������һ��ʱ�䡣  
	pthread_cond_wait(&m_condition, &m_mutex);
#endif  
}

void _CMutexLock::unwaite()
{
#ifdef _MSC_VER 
	SetEvent(m_event);//����Ϊ���źţ����ѵȴ��¼�������̡߳�  
#else  
	pthread_cond_signal(&m_condition);//pthread_cond_broadcast(pthread_cond_t * cond)�����������ϵȴ��������̡߳�  
#endif  

}

void _CMutexLock::unlock()
{
#ifdef _MSC_VER  
	ReleaseMutex(m_mutex);// ���������ͷ�,��֪ͨ��WaitForSingleObject.  

#else  
	pthread_mutex_unlock(&m_mutex);
#endif  
}
