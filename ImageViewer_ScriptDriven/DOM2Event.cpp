//
// DOM2Event.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "DOM2Event.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////////////
DOM2Event::DOM2Event()
	: mTarget(0)
	, mCurrentTarget(0)
	, mEventPhase(UNDEFINED)
	, mTimeStamp(0)
	, mEventType()
	, mCanBubble(false)
	, mCancelable(false)
	, mStopPropagation(false)
	, mPreventDefault(false)
{
}

DOM2Event::DOM2Event(DOMString const& eventType)
	: mTarget(0)
	, mCurrentTarget(0)
	, mEventPhase(UNDEFINED)
	, mTimeStamp(0)
	, mEventType(eventType)
	, mCanBubble(false)
	, mCancelable(false)
	, mStopPropagation(false)
	, mPreventDefault(false)
{
}

DOM2Event::~DOM2Event()
{
}

void DOM2Event::stopPropagation()
{
	mStopPropagation = true;
}

void DOM2Event::preventDefault()
{
	if (mCancelable)
		mPreventDefault = true;
}

void DOM2Event::initEvent(const DOMString& eventTypeArg, bool canBubbleArg, bool cancelableArg)
{
	mEventType = eventTypeArg;
	mCanBubble = canBubbleArg;
	mCancelable = cancelableArg;
}