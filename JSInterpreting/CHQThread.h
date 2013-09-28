//
// CHQThread.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CHQThread__
#define __H_CHQThread__

class CHQThread
{
protected:
	HANDLE		mhThread;		// The thread handle
	bool		mbWorking;		// Is thread in progress?
	bool		mbExiting;		// Is thread going to exit?

public:
	CHQThread();
	virtual ~CHQThread();

public:
	bool Start();
	void Stop();
	bool IsWorking() { return(mbWorking); }

protected:
	static DWORD WINAPI ThreadProc(void * pParam);

	virtual void Process() = 0;
};

#endif // __H_CHQThread__