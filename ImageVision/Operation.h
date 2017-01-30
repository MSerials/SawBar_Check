#pragma once


// COperation
#ifndef __OPERATION__
#define __OPERATION__

#include <thread>
#include <mutex>
class Lock
{
private:
	std::mutex g_i_mutex;
public:
	//Lock(std::mutex lck) :g_i_mutex(lck)
	Lock()
	{
		g_i_mutex.lock();
	}
	~Lock()
	{
		g_i_mutex.unlock();
	}
};



class COperation : public CDockablePane
{
	DECLARE_DYNAMIC(COperation)
private:
	int m_state;
public:
	COperation();
	virtual ~COperation();
	int isVisible(){ return (0x1 & m_state); }
	void Show(){ ShowPane(TRUE, FALSE, TRUE); Lock lck; m_state |= 0x1; }
	
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	virtual void OnPressCloseButton();
	virtual void OnPressButtons(UINT id) { return; }

	

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#endif
