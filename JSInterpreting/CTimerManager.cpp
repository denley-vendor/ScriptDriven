//
// CTimerManager.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CTimerManager.h"
#include "CTimer.h"

CTimerManager* GetTimerMgr() { return(CTimerManager::GetInstance()); }

//	Singleton instance of CTimerManager
CTimerManager* CTimerManager::mInstance = 0;

/////////////////////////////////////////////////////////////////////////////////////////
CTimerManager::CTimerManager()
{
}

CTimerManager::~CTimerManager()
{
	TimerList::iterator itBegin(mTimerQueue.begin()), itEnd(mTimerQueue.end());
	for (TimerList::iterator it = itBegin; it != itEnd; ++it)
	{
		CTimer* pTimer = *it;
		pTimer->Release();
	}
}

void CTimerManager::CreateInstance()
{
	if (mInstance == 0)
	{
		mInstance = new CTimerManager();
	}
}

void CTimerManager::DeleteInstance()
{
	if (mInstance)
	{
		delete mInstance;
		mInstance = 0;
	}
}

CTimerManager* CTimerManager::GetInstance()
{
	return mInstance;
}

void CTimerManager::QueueTimer(CTimer* pTimer)
{
	if (pTimer)
	{
		pTimer->AddRef();
		mTimerQueue.push_back(pTimer);
	}
}

void CTimerManager::Process()
{
	static long sCounter = 0;

	TimerList::iterator itBegin(mTimerQueue.begin()), itEnd(mTimerQueue.end());
	for (TimerList::iterator it = itBegin; it != itEnd; ++it)
	{
		CTimer* pTimer = *it;
		if (pTimer)
		{
			pTimer->ProcessTicks(sCounter);
		}
	}

	++sCounter;
}
