//
// DOM2EventTarget.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "DOM2EventTarget.h"
#include "DOM2Event.h"
#include "DOM2EventListener.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////////////
DOM2EventTarget::DOM2EventTarget()
{
}

DOM2EventTarget::~DOM2EventTarget()
{
	EventListenerVector::iterator itBegin(mListeners.begin()), itEnd(mListeners.end());
	for (EventListenerVector::iterator it = itBegin; it != itEnd; ++it)
	{
		EventListenerStruct& theEventLister(*it);
		if (theEventLister.listener)
			delete theEventLister.listener;
		
		mListeners.erase(it);
	}
}

void DOM2EventTarget::addEventListener(DOMString type, DOM2EventListener & listener, bool useCapture)
{
	EventListenerStruct theEventLister;
	theEventLister.type       = type;
	theEventLister.listener   = &listener;
	theEventLister.useCapture = useCapture;

	mListeners.push_back(theEventLister);
}

void DOM2EventTarget::removeEventListener(DOMString type, DOM2EventListener & listener, bool useCapture)
{
	EventListenerVector::iterator itBegin(mListeners.begin()), itEnd(mListeners.end());
	for (EventListenerVector::iterator it = itBegin; it != itEnd; ++it)
	{
		EventListenerStruct& listener1(*it);
		EventListenerStruct  listener2;
		listener2.type       = type;
		listener2.listener   = &listener;
		listener2.useCapture = useCapture;
		if (isSameListener(listener1, listener2))
		{
			if (listener1.listener)
				delete listener1.listener;
			mListeners.erase(it);
		}
	}
}

bool DOM2EventTarget::dispatchEvent(DOM2Event const& pEvent)
{
	EventListenerVector::iterator itBegin(mListeners.begin()), itEnd(mListeners.end());
	for (EventListenerVector::iterator it = itBegin; it != itEnd; ++it)
	{
		EventListenerStruct& theEventLister(*it);
		if (theEventLister.type == pEvent.getType())
		{
			theEventLister.listener->handleEvent(pEvent);
			if (pEvent.isStopPropagation())
				break;
		}
	}

	return pEvent.isPreventDefault();
}

bool DOM2EventTarget::isSameListener(EventListenerStruct& pListener1, EventListenerStruct& pListener2)
{
	if (pListener1.type == pListener2.type && 
		pListener1.listener == pListener2.listener && 
		pListener1.useCapture == pListener2.useCapture)
	{
		return true;
	}
	return false;
}