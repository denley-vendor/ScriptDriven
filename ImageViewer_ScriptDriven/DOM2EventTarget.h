//
// DOM2EventTarget.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_DOM2EventTarget__
#define __H_DOM2EventTarget__

#include "DOM2_defs.h"
#include <vector>

class DOM2EventListener;
class DOM2Event;

class DOM2EventTarget
{
public:
	DOM2EventTarget();
	virtual ~DOM2EventTarget();

	virtual void addEventListener(DOMString type, DOM2EventListener & listener, bool useCapture);
	virtual void removeEventListener(DOMString type, DOM2EventListener & listener, bool useCapture);
	virtual bool dispatchEvent(DOM2Event const& pEvent);

protected:
	struct EventListenerStruct
	{
		DOMString			type;
		DOM2EventListener*	listener;
		bool				useCapture;

		EventListenerStruct() : type(), listener(0), useCapture(false) {}
		~EventListenerStruct() {}
	};

	virtual bool isSameListener(EventListenerStruct& pListener1, EventListenerStruct& pListener2);

	typedef std::vector<EventListenerStruct> EventListenerVector;
	EventListenerVector		mListeners;
};

#endif // __H_DOM2EventTarget__