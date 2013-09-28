//
// CHQException.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CHQException__
#define __H_CHQException__

#include <exception>

class CHQException : public exception
{
public:
	CHQException(const char* szInfo, int number)
		: exception(szInfo)
		, mNumber(number)
	{
	}

	int GetNumber() { return mNumber; }

protected:
	int		mNumber;
};

#endif // __H_CHQException__