//
// DOM2EventListener.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_DOM2EventListener__
#define __H_DOM2EventListener__

class DOM2Event;

class DOM2EventListener
{
public:
	DOM2EventListener();
	virtual ~DOM2EventListener();

	virtual void handleEvent(DOM2Event const& pEvent);
};

#endif // __H_DOM2EventListener__