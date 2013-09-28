//
// CDebugger.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CDebugger.h"

CDebugger* GetDebugger() { return(CDebugger::GetInstance()); }

//	Singleton instance of CDebugger
CDebugger* CDebugger::mInstance = 0;

/////////////////////////////////////////////////////////////////////////////////////////
CDebugger::CDebugger(ITypeLib* ptl)
{
	LoadTypeInfo(ptl);
}

CDebugger::~CDebugger()
{
}

void CDebugger::CreateInstance(ITypeLib* ptl)
{
	if (mInstance == 0)
	{
		mInstance = new CDebugger(ptl);
	}
}

void CDebugger::DeleteInstance()
{
	if (mInstance)
	{
		mInstance->Release();
		mInstance = 0;
	}
}

CDebugger* CDebugger::GetInstance()
{
	return mInstance;
}


// APIs exposed to scripts
STDMETHODIMP CDebugger::trace(BSTR category, BSTR info)
{
	wprintf(L"[%s] %s\n", category, info);
	return S_OK;
}

STDMETHODIMP CDebugger::addBlankLine(UINT lineCount)
{
	for (UINT i = 0; i < lineCount; i++)
	{
		wprintf(L"\n");
	}
	return S_OK;
}