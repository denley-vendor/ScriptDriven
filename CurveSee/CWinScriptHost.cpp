//
// CWinScriptHost.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"

#include <initguid.h>
#include "curves_i.c"

#include "safe_defs.h"
#include "CWinScriptHost.h"
#include "CDataCurve.h"
#include "CCurvePainter.h"

/////////////////////////////////////////////////////////////////////////////////////////
CWinScriptHost::CWinScriptHost()
	: mInited(false)
	, mEngineType(0)
	, m_cRef(1)
	, mTypeLib(0)
	, mActiveScript(0)
	, mActiveScriptParse(0)
	, mPainter(0)
{
	// Load the type library
	HRESULT hr = S_OK;
	hr = ::LoadTypeLib(L"curves.tlb", &mTypeLib);

	// Create implementation components (for named items)
	mPainter = new CCurvePainter(mTypeLib);
}

CWinScriptHost::~CWinScriptHost()
{
	SAFE_RELEASE(mPainter);
	if (mEngineType)
	{
		::SysFreeString(mEngineType);
		mEngineType = 0;
	}

	Finalize();
}

void CWinScriptHost::UseScriptEngine(CString& inEngineType)
{
	if (mEngineType)
	{
		::SysFreeString(mEngineType);
		mEngineType = 0;
	}

	mEngineType = inEngineType.AllocSysString();
}

bool CWinScriptHost::Initialize()
{
	if (mInited)
		Finalize(); // finalize for re-initialzing...

	// Create an instance of Script Engine
	CLSID clsid;
	HRESULT hr = ::CLSIDFromProgID(mEngineType, &clsid);
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
	hr = mActiveScript->AddNamedItem(L"CurvePainter", SCRIPTITEM_ISVISIBLE);
	hr = mActiveScriptParse->InitNew();

	mInited = SUCCEEDED(hr);
	return true;
}

void CWinScriptHost::Finalize()
{
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

void CWinScriptHost::Execute(CString& scripts)
{
	HRESULT hr = S_OK;

	EXCEPINFO ei;
	BSTR wszScripts = scripts.AllocSysString();
	hr = mActiveScriptParse->ParseScriptText(wszScripts, L"CurvePainter",
		0, 0, 0, 0, 0, 0, &ei);
	if (wszScripts)
		::SysFreeString(wszScripts);

	hr = mActiveScript->SetScriptState(SCRIPTSTATE_CONNECTED);
}


// --- IUnknown methods --- 
HRESULT STDMETHODCALLTYPE CWinScriptHost::QueryInterface(REFIID riid,
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

	*ppvObject = 0;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE CWinScriptHost::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

ULONG STDMETHODCALLTYPE CWinScriptHost::Release()
{
	if (0 == InterlockedDecrement(&m_cRef))
	{
		delete this;
		return 0;
	}
	return m_cRef;
}


// --- IActiveScriptSite methods ---
STDMETHODIMP CWinScriptHost::GetLCID(LCID * plcid)
{
	plcid;
	return E_NOTIMPL;
}

STDMETHODIMP CWinScriptHost::GetItemInfo(LPCOLESTR pstrName, 
										DWORD dwReturnMask,
										IUnknown **ppunkItem,
										ITypeInfo **ppTypeInfo)
{
	if (ppTypeInfo)
	{
		*ppTypeInfo = 0;
		if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
		{
			if (0 == wcscmp(L"CurvePainter", pstrName))
			{
				mTypeLib->GetTypeInfoOfGuid(IID_ICurvePainter, ppTypeInfo);
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
			if (0 == wcscmp(L"CurvePainter", pstrName))
			{
				*ppunkItem = static_cast<IUnknown*> (mPainter);
				mPainter->AddRef();
			}
			else
			{
				return E_UNEXPECTED;
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CWinScriptHost::GetDocVersionString(BSTR *pbstrVersionString)
{
	pbstrVersionString;
	return E_NOTIMPL;
}

STDMETHODIMP CWinScriptHost::OnScriptTerminate(const VARIANT *pvarResult, const EXCEPINFO *pexcepinfo)
{
	pvarResult;
	pexcepinfo;
	return S_OK;
}

STDMETHODIMP CWinScriptHost::OnStateChange(SCRIPTSTATE ssScriptState)
{
	ssScriptState;
	return S_OK;
}

STDMETHODIMP CWinScriptHost::OnScriptError(IActiveScriptError *pase)
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

STDMETHODIMP CWinScriptHost::OnEnterScript(void)
{
	return S_OK;
}

STDMETHODIMP CWinScriptHost::OnLeaveScript(void)
{
	return S_OK;
}
