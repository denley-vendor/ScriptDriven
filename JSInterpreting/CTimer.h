//
// CTimer.h
//

#ifndef __H_CTimer__
#define __H_CTimer__

#include "IDispatchImpl.h"
#include "js_demo_h.h"
#include "DispatchHelper.h"

class CTimer : public IDispatchImpl<CTimer
								, ITimer
								, &IID_ITimer>
{
public:
	CTimer(ITypeLib* ptl, UINT interval, BOOL autoReset, IDispatch* pProc);
	~CTimer();

	// APIs exposed to scripts
	STDMETHODIMP get_enabled(BOOL* pRet);
	STDMETHODIMP put_enabled(BOOL pVal);
	STDMETHODIMP get_interval(UINT* pVal);

	void ProcessTicks(long ticks);

private:
	BOOL			mAutoReset;
	BOOL			mEnabled;
	UINT			mInterval;	 // In ms
	DispatchHelper	mDispatcher;

	// Timer runtime data
	bool			mInited;
	long			mHeadTick;
};

#endif // __H_CTimer__