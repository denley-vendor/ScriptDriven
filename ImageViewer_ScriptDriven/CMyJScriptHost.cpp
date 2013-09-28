//
// CMyJScriptHost.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"

#include <initguid.h>
#include "ImageViewer_i.c"

#include "safe_defs.h"
#include "CMyJScriptHost.h"
#include "CApplicationWrapper.h"

/////////////////////////////////////////////////////////////////////////////////////////
CMyJScriptHost::CMyJScriptHost()
	: mInited(false)
	, m_cRef(1)
	, mTypeLib(0)
	, mActiveScript(0)
	, mActiveScriptParse(0)
	, mApplication(0)
#ifdef __Script_Debug__
	, mpDebugMgr(0)
	, mpDebugApp(0)
	, mpDebugDoc(0)
	, mAppCookie(0)
#endif
{
	// Load the type library
	HRESULT hr = S_OK;
	hr = ::LoadTypeLib(L"ImageViewer.tlb", &mTypeLib);

	// Create implementation components (for named items)
	mApplication = new CApplicationWrapper(mTypeLib);
}

CMyJScriptHost::~CMyJScriptHost()
{
	SAFE_RELEASE(mApplication);

	Finalize();
}

bool CMyJScriptHost::Initialize()
{
	if (mInited)
		Finalize(); // finalize for re-initialzing...

#ifdef __Script_Debug__
	CreateScriptDebugger();
#endif
	// Create an instance of JScript Engine
	CLSID clsid;
	HRESULT hr = ::CLSIDFromProgID(L"JScript", &clsid);
	if (SUCCEEDED(hr))
	{
		hr = ::CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, 
			IID_IActiveScript, (void**)&mActiveScript);
	}
	if (mActiveScript == 0) 
		return false;

	// Retrieve the script parser interface
	hr = mActiveScript->QueryInterface(IID_IActiveScriptParse, (void**)&mActiveScriptParse);

	// Configurations on JScript Engine
	hr = mActiveScript->SetScriptSite(this);
	hr = mActiveScript->AddNamedItem(L"Application", SCRIPTITEM_ISVISIBLE|SCRIPTITEM_GLOBALMEMBERS);
	hr = mActiveScriptParse->InitNew();

	mInited = SUCCEEDED(hr);
	return true;
}

void CMyJScriptHost::Finalize()
{
#ifdef __Script_Debug__
	ReleaseScriptDebugger();
#endif

	// Release JScript Engine interfaces
	SAFE_RELEASE(mActiveScriptParse);
	if (mActiveScript)
	{
		mActiveScript->Close();
		mActiveScript->Release();
		mActiveScript = 0;
	}

	mInited = false;
}

void CMyJScriptHost::Execute(wchar_t* scripts)
{
	HRESULT hr = S_OK;

#ifdef __Script_Debug__
	CreateDocumentForDebugger(scripts);
#endif

	EXCEPINFO ei;
	hr = mActiveScriptParse->ParseScriptText(scripts, 0,
		0, 0, 0, 0, 0, 0, &ei);

	hr = mActiveScript->SetScriptState(SCRIPTSTATE_CONNECTED);
}


// --- IUnknown methods --- 
HRESULT STDMETHODCALLTYPE CMyJScriptHost::QueryInterface(REFIID riid,
														void **ppvObject) 
{
	if (IID_IUnknown == riid)
	{
		*ppvObject = static_cast<IUnknown*>((void*)(this));
		AddRef();
		return S_OK;
	}
	else if (IID_IActiveScriptSite == riid)
	{
		*ppvObject = static_cast<IActiveScriptSite*> (this);
		AddRef();
		return S_OK;
	}
#ifdef __Script_Debug__
	else if (IID_IActiveScriptSiteDebug == riid)
	{
		*ppvObject = static_cast<IActiveScriptSiteDebug*> (this);
		AddRef();
		return S_OK;
	}
#endif	

	*ppvObject = 0;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE CMyJScriptHost::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

ULONG STDMETHODCALLTYPE CMyJScriptHost::Release()
{
	if (0 == InterlockedDecrement(&m_cRef))
	{
		delete this;
		return 0;
	}
	return m_cRef;
}


// --- IActiveScriptSite methods ---
STDMETHODIMP CMyJScriptHost::GetLCID(LCID * plcid)
{
	plcid;
	return E_NOTIMPL;
}

STDMETHODIMP CMyJScriptHost::GetItemInfo(LPCOLESTR pstrName, 
										DWORD dwReturnMask,
										IUnknown **ppunkItem,
										ITypeInfo **ppTypeInfo)
{
	if (ppTypeInfo)
	{
		*ppTypeInfo = 0;
		if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
		{
			if (0 == wcscmp(L"Application", pstrName))
			{
				mTypeLib->GetTypeInfoOfGuid(IID_IApplication, ppTypeInfo);
			}
			else
			{
				return E_UNEXPECTED;
			}
		}
	}

	if (ppunkItem)
	{
		*ppunkItem = 0;
		if (dwReturnMask & SCRIPTINFO_IUNKNOWN)
		{
			if (0 == wcscmp(L"Application", pstrName))
			{
				*ppunkItem = static_cast<IUnknown*> (mApplication);
				mApplication->AddRef();
			}
			else
			{
				return E_UNEXPECTED;
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CMyJScriptHost::GetDocVersionString(BSTR *pbstrVersionString)
{
	pbstrVersionString;
	return E_NOTIMPL;
}

STDMETHODIMP CMyJScriptHost::OnScriptTerminate(const VARIANT *pvarResult, const EXCEPINFO *pexcepinfo)
{
	pvarResult;
	pexcepinfo;
	return S_OK;
}

STDMETHODIMP CMyJScriptHost::OnStateChange(SCRIPTSTATE ssScriptState)
{
	ssScriptState;
	return S_OK;
}

STDMETHODIMP CMyJScriptHost::OnScriptError(IActiveScriptError *pase)
{
	HRESULT hr = NOERROR;

//	BSTR szSrcLine;
//	hr = pase->GetSourceLineText(&szSrcLine);

	DWORD	srcContext = 0;		
	ULONG	lineNumber = 0;
	LONG	charPosition = 0; 
	hr = pase->GetSourcePosition(&srcContext, &lineNumber, &charPosition);

	EXCEPINFO ei;
	ZeroMemory(&ei, sizeof(ei));
	hr = pase->GetExceptionInfo(&ei);
	if (SUCCEEDED(hr))
	{
		if (ei.bstrSource)
			::SysFreeString(ei.bstrSource);
		if (ei.bstrDescription)
			::SysFreeString(ei.bstrDescription);
		if (ei.bstrHelpFile)
			::SysFreeString(ei.bstrHelpFile);
	}

	return hr;
}

STDMETHODIMP CMyJScriptHost::OnEnterScript(void)
{
	return S_OK;
}

STDMETHODIMP CMyJScriptHost::OnLeaveScript(void)
{
	return S_OK;
}

// For script debugging...
#ifdef __Script_Debug__
STDMETHODIMP CMyJScriptHost::GetDocumentContextFromPosition(/*[in]*/ DWORD dwSourceContext,
														  /*[in]*/ ULONG uCharacterOffset,
														  /*[in]*/ ULONG uNumChars,
														  /*[out]*/ IDebugDocumentContext** ppsc) 
{ 	
	dwSourceContext;
	if (!ppsc) 
		return E_POINTER;

	*ppsc = 0; 
	if (!mpDebugDoc) 
		return E_UNEXPECTED;

	ULONG ulStartPos=0; 
	HRESULT hr = mpDebugDoc->GetScriptBlockInfo(mAppCookie, 0, &ulStartPos, 0); 
	if (SUCCEEDED(hr))
	{
		return mpDebugDoc->CreateDebugDocumentContext(ulStartPos+uCharacterOffset, uNumChars, ppsc); 
	}
	return hr; 
} 

STDMETHODIMP CMyJScriptHost::GetApplication(/*[out]*/ IDebugApplication **ppda) 
{ 
	if (!ppda) 
		return E_POINTER; 

	*ppda = 0; 
	if (!mpDebugApp) 
		return E_UNEXPECTED; 

	*ppda = mpDebugApp; 
	mpDebugApp->AddRef();
	return S_OK; 
} 

STDMETHODIMP CMyJScriptHost::GetRootApplicationNode(/*[out]*/ IDebugApplicationNode** ppdanRoot) 
{ 
	// If we have only one document, we can safely return NULL here (this is the root). 
	if (!ppdanRoot) 
		return E_POINTER; 

	*ppdanRoot = 0; 
	return S_OK; 
}

STDMETHODIMP CMyJScriptHost::OnScriptErrorDebug(/*[in]*/ IActiveScriptErrorDebug* pErrorDebug,
											  /*[out]*/ BOOL *pfEnterDebugger,
											  /*[out]*/ BOOL *pfCallOnScriptErrorWhenContinuing) 
{ 
	pErrorDebug;
	// Runtime errors get here before go into IActiveScriptSite::OnScriptError 
	// Query the IActiveScriptErrorDebug interface for more 
	// info on what kind of error occurred. 
	*pfEnterDebugger = FALSE; 
	*pfCallOnScriptErrorWhenContinuing = TRUE;
	return S_OK; 
}

HRESULT CMyJScriptHost::CreateScriptDebugger()
{
	// Create script debugger interfaces.
	HRESULT hr = ::CoCreateInstance(CLSID_ProcessDebugManager, NULL, CLSCTX_INPROC_SERVER, 
		IID_IProcessDebugManager, (void**)&mpDebugMgr);
	if (SUCCEEDED(hr))
	{
		hr = mpDebugMgr->CreateApplication(&mpDebugApp);
	}
	if (SUCCEEDED(hr))
	{
		hr = mpDebugApp->SetName(L"HQ JScript Testing");
	}
	if (SUCCEEDED(hr))
	{
		hr = mpDebugMgr->AddApplication(mpDebugApp, &mAppCookie);
	}

	if (FAILED(hr))
	{
		ReleaseScriptDebugger();
	}
	return hr;
}

HRESULT CMyJScriptHost::CreateDocumentForDebugger(BSTR scripts)
{
	if (!mpDebugMgr)
		return E_UNEXPECTED;

	// Release previous used document.
	ReleaseDebugDocument();

	HRESULT hr = mpDebugMgr->CreateDebugDocumentHelper(0, &mpDebugDoc);
	if (SUCCEEDED(hr))
	{
		hr = mpDebugDoc->Init(mpDebugApp, L"Sample Codes", L"Sample Codes", TEXT_DOC_ATTR_READONLY);
	}
	if (SUCCEEDED(hr))
	{
		hr = mpDebugDoc->Attach(0);
	}
	if (SUCCEEDED(hr))
	{
		hr = mpDebugDoc->AddUnicodeText(scripts);
	}
	if (SUCCEEDED(hr))
	{
		DWORD_PTR  pdwSourceContext;
		hr = mpDebugDoc->DefineScriptBlock(0, (ULONG)(wcslen(scripts)), mActiveScript, FALSE, &pdwSourceContext);
	}
	return hr;
}

void CMyJScriptHost::ReleaseDebugDocument()
{
	if (mpDebugDoc)
	{
		mpDebugDoc->Detach();
		mpDebugDoc->Release();
		mpDebugDoc = 0;
	}
}

void CMyJScriptHost::ReleaseScriptDebugger()
{
	// Release script debugger interfaces.
	ReleaseDebugDocument();
	if (mpDebugMgr && mAppCookie)
	{
		mpDebugMgr->RemoveApplication(mAppCookie);
		mAppCookie = 0;
	}
	SAFE_RELEASE(mpDebugApp);
	SAFE_RELEASE(mpDebugMgr);
}

#endif // __Script_Debug__
