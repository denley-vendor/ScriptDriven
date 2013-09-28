//
// CScriptEngineWrapper.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CScriptEngineWrapper.h"

CScriptEngineWrapper* GetScriptEngine() { return(CScriptEngineWrapper::GetInstance()); }

//	Singleton instance of CScriptEngineWrapper
CScriptEngineWrapper* CScriptEngineWrapper::mInstance = 0;

/////////////////////////////////////////////////////////////////////////////////////////
CScriptEngineWrapper::CScriptEngineWrapper(IActiveScript* pEngine) : mpEngine(pEngine)
{
}

CScriptEngineWrapper::~CScriptEngineWrapper()
{
}

void CScriptEngineWrapper::CreateInstance(IActiveScript* pEngine)
{
	if (mInstance == 0)
	{
		mInstance = new CScriptEngineWrapper(pEngine);
	}
}

void CScriptEngineWrapper::DeleteInstance()
{
	if (mInstance)
	{
		delete mInstance;
		mInstance = 0;
	}
}

CScriptEngineWrapper* CScriptEngineWrapper::GetInstance()
{
	return mInstance;
}

IDispatch* CScriptEngineWrapper::GetDispatch(BSTR pItemName)
{
	IDispatch* pDispatch = 0;
	if (mpEngine)
	{
		mpEngine->GetScriptDispatch(pItemName, &pDispatch);
	}
	return pDispatch;
}