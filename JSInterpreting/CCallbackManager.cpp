//
// CCallbackManager.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CCallbackManager.h"
#include "DispatchHelper.h"

CCallbackManager* GetCallbackMgr() { return(CCallbackManager::GetInstance()); }

//	Singleton instance of CCallbackManager
CCallbackManager* CCallbackManager::mInstance = 0;

/////////////////////////////////////////////////////////////////////////////////////////
CCallbackManager::CCallbackManager()
{
}

CCallbackManager::~CCallbackManager()
{
}

CCallbackManager* CCallbackManager::GetInstance()
{
	if (mInstance == 0)
	{
		static CCallbackManager callbackMgr;
		mInstance = &callbackMgr; 
	}
	return mInstance;
}

void CCallbackManager::QueueCallback(DispatchHelper* pDispatcher)
{
	mCallbackQueue.push_back(pDispatcher);
}

void CCallbackManager::Process()
{
	size_t count = mCallbackQueue.size();
	if (count > 0) 
	{
		DispatchHelper* pDispatcher = mCallbackQueue.front();
		if (pDispatcher)
		{
			pDispatcher->Invoke(0);
			delete pDispatcher;
			mCallbackQueue.pop_front();
		}
	}
}
