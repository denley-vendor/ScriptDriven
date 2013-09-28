//
// CMainWindowWrapper.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "safe_defs.h"
#include "CMainWindowWrapper.h"
#include "CButtonWrapper.h"

/////////////////////////////////////////////////////////////////////////////////////////
CMainWindowWrapper::CMainWindowWrapper(ITypeLib* ptl)
	: mButtons(0)
{
	LoadTypeInfo(ptl);
}

CMainWindowWrapper::~CMainWindowWrapper()
{
	SAFE_RELEASE(mButtons);
}

// APIs exposed to scripts
STDMETHODIMP CMainWindowWrapper::get_buttons(IDispatch** ppdisp)
{
	if (mButtons == 0)
		mButtons = new CButtonSet(m_ptl);
	if (mButtons)
		return mButtons->QueryInterface(IID_IDispatch, (void**)ppdisp);
	return E_FAIL;
}