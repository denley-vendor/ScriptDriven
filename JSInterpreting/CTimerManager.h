//
// CTimerManager.h
//

#ifndef __H_CTimerManager__
#define __H_CTimerManager__

#include <list>

class CTimer;

class CTimerManager
{
private:
	CTimerManager();
	
public:
	static void CreateInstance();
	static void DeleteInstance();
	static CTimerManager* GetInstance();
	~CTimerManager();

	void QueueTimer(CTimer* pTimer);
	void Process();

private:
	static CTimerManager*	mInstance;		// singleton instance of CTimerManager

	typedef std::list< CTimer* >	TimerList;
	TimerList		mTimerQueue;
};

extern CTimerManager* GetTimerMgr();

#endif // __H_CTimerManager__