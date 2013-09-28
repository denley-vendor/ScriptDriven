//
// CTimer.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CTimer.h"

/////////////////////////////////////////////////////////////////////////////////////////
CTimer::CTimer(ITypeLib* ptl, UINT interval, BOOL autoReset, IDispatch* pProc)
	: mInterval(interval)
	, mAutoReset(autoReset)
	, mDispatcher(pProc)
	, mEnabled(true)
	, mInited(false)
	, mHeadTick(0)
{
	LoadTypeInfo(ptl);
}

CTimer::~CTimer()
{
}

// APIs exposed to scripts
STDMETHODIMP CTimer::get_enabled(BOOL* pRet)
{
	*pRet = mEnabled;
	return S_OK;
}

STDMETHODIMP CTimer::put_enabled(BOOL pVal)
{
	mEnabled = pVal;
	if (mEnabled)
	{
		mInited = false; // Need re-initialization.
	}
	return S_OK;
}

STDMETHODIMP CTimer::get_interval(UINT* pVal)
{
	*pVal = mInterval;
	return S_OK;
}

// Note: A tick is generated every 100ms.
void CTimer::ProcessTicks(long ticks)
{
	if (!mEnabled)
		return;

	// Do initialization.
	if (mInited)
	{
		mInited = true;
		mHeadTick = ticks; // Save the first tick.
		return;
	}

	if (UINT(ticks - mHeadTick) * 100 >= mInterval)
	{
		// Execute the timer procedure.
		mDispatcher.Invoke(0);

		// Update the head tick for next interval.
		mHeadTick = ticks;

		if (!mAutoReset)
			mEnabled = false;
	}
}