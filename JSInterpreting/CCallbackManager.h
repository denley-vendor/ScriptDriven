//
// CCallbackManager.h
//

#ifndef __H_CCallbackManager__
#define __H_CCallbackManager__

#include <list>

class DispatchHelper;

class CCallbackManager
{
private:
	CCallbackManager();
	
public:
	static CCallbackManager* GetInstance();
	~CCallbackManager();

	void QueueCallback(DispatchHelper* pDispatcher);
	void Process();

private:
	static CCallbackManager*	mInstance;		// singleton instance of CCallbackManager

	typedef std::list< DispatchHelper* >	CallbackList;
	CallbackList		mCallbackQueue;
};

extern CCallbackManager* GetCallbackMgr();

#endif // __H_CCallbackManager__