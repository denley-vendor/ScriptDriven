//
// CApplication.h
//

#ifndef __H_CApplication__
#define __H_CApplication__

#include "IDispatchImpl.h"
#include "js_demo_h.h"

class CCar;
class CFileCopyThread;

class CApplication : public IDispatchImpl<CApplication
										, IApplication
										, &IID_IApplication>
{
public:
	CApplication(ITypeLib* ptl);
	~CApplication();

	// APIs exposed to scripts
	STDMETHODIMP get_car(IDispatch** ppdisp);

	STDMETHODIMP alert(BSTR msg);
	STDMETHODIMP triggerException();
	STDMETHODIMP getInformation(BSTR key, IDispatch* pCallback);
	STDMETHODIMP reviewScripts();
	STDMETHODIMP copyFile(BSTR srcFile, BSTR dstFile, IDispatch* pCallback);
	STDMETHODIMP createTimer(UINT interval, BOOL bAutoReset, IDispatch* pTimerProc, IDispatch** ppdisp);
	STDMETHODIMP pauseForDebugger();

private:
	CCar*				mCar;
	CFileCopyThread*	mpFileThread;
};

#endif // __H_CApplication__