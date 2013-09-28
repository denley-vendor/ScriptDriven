//
// CJScriptHost.h
//

#ifndef __H_CJScriptHost__
#define __H_CJScriptHost__

#include "activscp.h"

#define __Script_Debug__

#ifdef __Script_Debug__
#include "activdbg.h"
#endif 

class CApplication;
class CMediaPlayer;

class CJScriptHost : public IActiveScriptSite
			#ifdef __Script_Debug__
				   , public IActiveScriptSiteDebug
			#endif
{
public:
	CJScriptHost();
	~CJScriptHost();

	bool Initialize();
	void Finalize();
	void Execute(wchar_t* scripts);

	// --- IUnknown methods ---
	HRESULT STDMETHODCALLTYPE QueryInterface( /* [in] */ REFIID riid,
		/* [out] */ void  **ppvObject ); 
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	// --- IActiveScriptSite methods ---
	STDMETHODIMP GetLCID(LCID * plcid);
	STDMETHODIMP GetItemInfo(LPCOLESTR pstrName, DWORD dwReturnMask, 
		IUnknown **ppunkItem, ITypeInfo **ppTypeInfo);
	STDMETHODIMP GetDocVersionString(BSTR *pbstrVersionString);
	STDMETHODIMP OnScriptTerminate(const VARIANT *pvarResult, const EXCEPINFO *pexcepinfo);
	STDMETHODIMP OnStateChange(SCRIPTSTATE ssScriptState);
	STDMETHODIMP OnScriptError(IActiveScriptError *pase);
	STDMETHODIMP OnEnterScript(void);
	STDMETHODIMP OnLeaveScript(void);

#ifdef __Script_Debug__
	// --- IActiveScriptSiteDebug methods ---
	STDMETHODIMP GetDocumentContextFromPosition(/*[in]*/ DWORD dwSourceContext,
		/*[in]*/ ULONG uCharacterOffset, /*[in]*/ ULONG uNumChars,
		/*[out]*/ IDebugDocumentContext** ppsc); 
	STDMETHODIMP GetApplication(/*[out]*/ IDebugApplication **ppda);
	STDMETHODIMP GetRootApplicationNode(/*[out]*/ IDebugApplicationNode** ppdanRoot); 
	STDMETHODIMP OnScriptErrorDebug(/*[in]*/ IActiveScriptErrorDebug *pErrorDebug,
		/*[out]*/ BOOL *pfEnterDebugger,/*[out]*/ BOOL *pfCallOnScriptErrorWhenContinuing);
#endif 

private:
	long				m_cRef;				// COM reference count

	bool				mInited;
	ITypeLib *			mTypeLib;			// Type library for scripting APIs' dispatch
	IActiveScript *		mActiveScript;		// JScript Engine
	IActiveScriptParse*	mActiveScriptParse;	// Script parser

	// Components
	CApplication*		mApplication;
	CMediaPlayer*		mPlayer;


	// For script debugging...
#ifdef __Script_Debug__
	IProcessDebugManager*	mpDebugMgr;
	IDebugApplication*		mpDebugApp;
	IDebugDocumentHelper*	mpDebugDoc;
	DWORD					mAppCookie;

	HRESULT CreateScriptDebugger();
	void ReleaseScriptDebugger();
	HRESULT CreateDocumentForDebugger(BSTR scripts);
	void ReleaseDebugDocument();
#endif
};

#endif // __H_CJScriptHost__
