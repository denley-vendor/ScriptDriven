//
// CWinScriptHost.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CWinScriptHost__
#define __H_CWinScriptHost__

#include "activscp.h"

class CCurvePainter;

class CWinScriptHost : public IActiveScriptSite
{
public:
	CWinScriptHost();
	~CWinScriptHost();

	void UseScriptEngine(CString& inEngineType);
	bool Initialize();
	void Finalize();
	void Execute(CString& scripts);

	// IUnknown methods 
	HRESULT STDMETHODCALLTYPE QueryInterface( /* [in] */ REFIID riid,
		/* [out] */ void  **ppvObject ); 
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	// IActiveScriptSite methods
	STDMETHODIMP GetLCID(LCID * plcid);
	STDMETHODIMP GetItemInfo(LPCOLESTR pstrName, DWORD dwReturnMask, 
		IUnknown **ppunkItem, ITypeInfo **ppTypeInfo);
	STDMETHODIMP GetDocVersionString(BSTR *pbstrVersionString);
	STDMETHODIMP OnScriptTerminate(const VARIANT *pvarResult, const EXCEPINFO *pexcepinfo);
	STDMETHODIMP OnStateChange(SCRIPTSTATE ssScriptState);
	STDMETHODIMP OnScriptError(IActiveScriptError *pase);
	STDMETHODIMP OnEnterScript(void);
	STDMETHODIMP OnLeaveScript(void);

private:
	long				m_cRef;				// COM reference count

	bool				mInited;
	BSTR				mEngineType;

	ITypeLib *			mTypeLib;			// Type library for scripting APIs' dispatch
	IActiveScript *		mActiveScript;		// Script Engine
	IActiveScriptParse*	mActiveScriptParse;	// Script parser

	CCurvePainter*		mPainter;
};

#endif // __H_CWinScriptHost__
