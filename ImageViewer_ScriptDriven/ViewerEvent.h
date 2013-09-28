//
// ViewerEvent.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_ViewerEvent__
#define __H_ViewerEvent__

#include "DOM2Event.h"

class ViewerEvent : public DOM2Event
{
public:
	ViewerEvent(DOMString const& eventType, DOMString const& strID);
	virtual ~ViewerEvent();

	DOMString GetID() { return mID; }

protected:
	DOMString			mID;
};

#endif // __H_ViewerEvent__