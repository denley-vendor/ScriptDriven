//
// ViewerEvent.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "ViewerEvent.h"

//////////////////////////////////////////////////////////////////////////////////////
ViewerEvent::ViewerEvent(DOMString const& eventType, DOMString const& strID)
	: DOM2Event(eventType)
	, mID(strID)
{
}

ViewerEvent::~ViewerEvent()
{
}

