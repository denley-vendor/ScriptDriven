//
// DOM2Event.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_DOM2Event__
#define __H_DOM2Event__

#include "DOM2_defs.h"

class DOM2EventTarget;

class DOM2Event
{
public:
	DOM2Event();
	DOM2Event(DOMString const& eventType);
	virtual ~DOM2Event();

	enum EventPhase
	{
		UNDEFINED = 0,
		CAPTURING_PHASE = 1,
		AT_TARGET       = 2,
		BUBBLING_PHASE  = 3
	};

	virtual DOMString getType() const {return mEventType;}
	virtual DOM2EventTarget* getTarget() const {return mTarget;}
	virtual DOM2EventTarget* getCurrentTarget() const {return mCurrentTarget;}
	virtual EventPhase getEventPhase() const {return mEventPhase;}
	virtual bool getBubbles() const {return mCanBubble;}
	virtual bool getCancelable() const {return mCancelable;}
	virtual DOMTimeStamp getTimeStamp() const {return mTimeStamp;}

	virtual void stopPropagation();
	virtual void preventDefault();
	virtual void initEvent(const DOMString& eventTypeArg, bool canBubbleArg, bool cancelableArg);

	void setTarget(DOM2EventTarget* p){mTarget=p;}
	void setCurrentTarget(DOM2EventTarget* p){mCurrentTarget=p;}
	void setEventPhase(EventPhase eventPhase){mEventPhase=eventPhase;}
	void setTimeStamp(DOMTimeStamp timeStamp){mTimeStamp=timeStamp;}

	bool isStopPropagation() const {return mStopPropagation;}
	bool isPreventDefault() const {return mPreventDefault;}

protected:
	DOM2EventTarget*	mTarget;
	DOM2EventTarget*	mCurrentTarget;
	EventPhase			mEventPhase;
	DOMTimeStamp		mTimeStamp;
	DOMString			mEventType;
	bool				mCanBubble;
	bool				mCancelable;
	bool				mStopPropagation;
	bool				mPreventDefault;
};

#endif // __H_DOM2Event__