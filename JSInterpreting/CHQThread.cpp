// 
// CHQThread.cpp
// 
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CHQThread.h"

//////////////////////////////////////////////////////////////////////////////
CHQThread::CHQThread() : mhThread(0), mbWorking(false), mbExiting(false)
{
}

CHQThread::~CHQThread()
{
	Stop();
}

bool CHQThread::Start()
{
	if (mbWorking)
	{
		return true;
	}

	mbExiting = false;
	DWORD threadID = 0;
	mhThread = CreateThread(0, 0, ThreadProc, this, 0, &threadID);
	return (mhThread != 0);
}

void CHQThread::Stop()
{
	if (mbWorking)
	{
		mbExiting = true;

		if (mhThread) 
		{
			WaitForSingleObject(mhThread, INFINITE);
			mhThread = 0;
		}
	}
}

DWORD WINAPI CHQThread::ThreadProc(void * pParam)
{
	CHQThread * pThread = static_cast<CHQThread*> (pParam);
	if (pThread)
	{
		pThread->mbWorking = true;
		pThread->Process();
		pThread->mbWorking = false;
		return 1;
	}	
	return 0;
}