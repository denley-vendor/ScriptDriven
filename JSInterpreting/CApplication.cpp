//
// CApplication.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "safe_defs.h"
#include "CApplication.h"
#include "CCar.h"
#include "CDebugger.h"
#include "DispatchHelper.h"
#include "CScriptEngineWrapper.h"
#include "CFileCopyThread.h"
#include "CTimerManager.h"
#include "CTimer.h"

/////////////////////////////////////////////////////////////////////////////////////////
CApplication::CApplication(ITypeLib* ptl)
	: mCar(0)
	, mpFileThread(0)
{
	LoadTypeInfo(ptl);

	mCar = new CCar(ptl);
}

CApplication::~CApplication()
{
	SAFE_RELEASE(mCar);
	SAFE_DELETE(mpFileThread);
}

// APIs exposed to scripts
STDMETHODIMP CApplication::alert(BSTR msg)
{
	GetDebugger()->trace(L"alert", msg);

#ifndef _DEBUG
	int charCount = int(wcslen(msg) + 1); // including a null ternimator.
	char* szMsg = new char[charCount];
	::WideCharToMultiByte(CP_ACP, 0, msg, -1, szMsg, charCount, 0, 0);
	::MessageBox(0, szMsg, "JS Interpreting", MB_OK);
	delete[] szMsg;
#endif

	return S_OK;
}

STDMETHODIMP CApplication::get_car(IDispatch** ppdisp)
{
	if (mCar)
	{
		return mCar->QueryInterface(IID_IDispatch, (void**)ppdisp);
	}
	return E_FAIL;
}

STDMETHODIMP CApplication::triggerException()
{
	throw CHQException("Can you catch me?", 98);
}

// callback(key:string, value:string, errorCode:int);
STDMETHODIMP CApplication::getInformation(BSTR key, IDispatch* pCallback)
{
	// Look up the key and get its value...
	BSTR itsValue = 0;
	// ...
	itsValue = ::SysAllocString(L"sunny");

	// call back to scripts now.
	// Note: Please add parameters with the sequence "Right to Left".
	// That is, the rightest parameter is the one with index 0!
	DispatchHelper dispatcher(pCallback);
	dispatcher.CreateParameters(3);
	dispatcher.AddParameter(0, 1);
	dispatcher.AddParameter(1, itsValue);
	dispatcher.AddParameter(2, key);
	dispatcher.Invoke(0);

	if (itsValue)
		::SysFreeString(itsValue);

	return S_OK;
}

// retrieve variables and invoke functions of scripts from the C++ side
STDMETHODIMP CApplication::reviewScripts()
{
	CScriptEngineWrapper* pEngine = GetScriptEngine();
	if (pEngine)
	{
		IDispatch* pScriptDispatch = pEngine->GetDispatch();
		DispatchHelper dispatcher(pScriptDispatch);

		bool bRet = false;
		wchar_t wszInfo[200];
		
		// Get the value of global variable 'BOOK_TITLE'
		VARIANT var;
		VariantInit(&var);
		bRet = dispatcher.GetProperty(L"BOOK_TITLE", &var);
		swprintf(wszInfo, L"BOOK_TITLE: %s.", var.bstrVal);
		GetDebugger()->trace(L"Info", wszInfo);
		VariantClear(&var);

		// Get the value of global variable 'bookCount'
		bRet = dispatcher.GetProperty(L"bookCount", &var);
		swprintf(wszInfo, L"bookCount's value: %d.", var.uintVal);
		GetDebugger()->trace(L"Info", wszInfo);
		VariantClear(&var);

		// Set a new value to the variable 'bookCount'
		var.vt = VT_UINT;
		var.uintVal = 6000;
		bRet = dispatcher.SetProperty(L"bookCount", &var);
		VariantClear(&var);
		// Review the value of the variable 'bookCount'
		bRet = dispatcher.GetProperty(L"bookCount", &var);
		swprintf(wszInfo, L"bookCount's new value: %d.", var.uintVal);
		GetDebugger()->trace(L"Info", wszInfo);
		VariantClear(&var);

		// Invoke a script function
		dispatcher.CreateParameters(1);
		dispatcher.AddParameter(0, L"This is the parameter passed to scripts.");
		DISPID dispid = 0;
		if (dispatcher.GetDispid(L"showMessage", &dispid))
		{
			dispatcher.Invoke(dispid);
		}

		pScriptDispatch->Release();
	}

	return S_OK;
}

STDMETHODIMP CApplication::copyFile(BSTR srcFile, BSTR dstFile, IDispatch* pCallback)
{
	// Do file copy operation in another thread.
	SAFE_DELETE(mpFileThread);
	mpFileThread = new CFileCopyThread(srcFile, dstFile, pCallback);
	mpFileThread->Start();

	Sleep(1000);
	return S_OK;
}

STDMETHODIMP CApplication::createTimer(UINT interval, 
									   BOOL bAutoReset, 
									   IDispatch* pTimerProc, 
									   IDispatch** ppdisp)
{
	HRESULT hr = E_FAIL;
	CTimer* pTimer = new CTimer(m_ptl, interval, bAutoReset, pTimerProc);
	if (pTimer)
	{
		hr = pTimer->QueryInterface(IID_IDispatch, (void**)ppdisp);
		GetTimerMgr()->QueueTimer(pTimer); // Put the timer into the queue.
		pTimer->Release();
	}
	return hr;
}

STDMETHODIMP CApplication::pauseForDebugger()
{
	printf("Press the key 'C' on console to continue the script running.\n");
	while (GetAsyncKeyState('C') == 0) 
	{
		Sleep(100);
	}
	return S_OK;
}