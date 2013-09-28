//
// CHQException.h
//

#ifndef __H_CHQException__
#define __H_CHQException__

#include <exception>

class CHQException : public std::exception
{
public:
	CHQException(const char* szInfo, int number)
		: std::exception(szInfo)
		, mNumber(number)
	{
	}

	int GetNumber() { return mNumber; }

protected:
	int		mNumber;
};

#endif // __H_CHQException__