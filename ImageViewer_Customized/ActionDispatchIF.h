//
// ActionDispatchIF.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_ActionDispatchIF__
#define __H_ActionDispatchIF__

#include "global_defs.h"

class ActionDispatchIF
{
public:
	ActionDispatchIF() {};
	~ActionDispatchIF() {};

	virtual bool Invoke(IMAGE_FUNCTION funcId) = 0;
};

#endif // __H_ActionDispatchIF__