//
// CButtonWrapper.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CButtonWrapper.h"
#include "CScriptButton.h"
#include "ImageViewerDlg.h"

/////////////////////////////////////////////////////////////////////////////////////////
CButtonWrapper::CButtonWrapper(ITypeLib* ptl, CScriptButton* button)
	: mButton(button)
{
	LoadTypeInfo(ptl);
}

CButtonWrapper::~CButtonWrapper()
{
}

// APIs exposed to scripts
STDMETHODIMP CButtonWrapper::get_id(BSTR* value)
{
	if (mButton)
	{
		*value = mButton->GetID().AllocSysString();
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CButtonWrapper::get_enabled(BOOL* value)
{
	if (mButton)
	{
		*value = mButton->IsWindowEnabled();
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CButtonWrapper::put_enabled(BOOL value)
{
	if (mButton)
	{
		mButton->EnableWindow(value);
		return S_OK;
	}
	return E_FAIL;
}


////////////////////////////////////////////////////////////////////////////////
CButtonSet::CButtonSet(ITypeLib* ptl)
{
	LoadTypeInfo(ptl);
}

CButtonSet::~CButtonSet()
{
}

HRESULT CButtonSet::IsKeyDefined(BSTR key)
{
	CImageViewerDlg* pApp = GetMainApp();
	if (!pApp)
		return E_UNEXPECTED;

	// Standard JavaScript Array has a property 'length'.
	if (wcscmp(key, L"length") == 0)
	{
		return S_OK;
	}

	CString strKey(key);
	ButtonList& buttons = pApp->GetButtonList();
	ButtonList::iterator itBegin(buttons.begin()), itEnd(buttons.end());
	for (ButtonList::iterator it = itBegin; it != itEnd; ++it)
	{
		if (strKey.Compare((*it)->GetID()) == 0)
		{
			return S_OK;
		}
	}
	return S_FALSE;
}

HRESULT CButtonSet::GetValueOfKey(BSTR key, VARIANT* value)
{
	CImageViewerDlg* pApp = GetMainApp();
	if (!pApp)
		return E_UNEXPECTED;

	ButtonList& buttons = pApp->GetButtonList();
	if (wcscmp(key, L"length") == 0)
	{
		value->vt		= VT_UINT;
		value->uintVal	= UINT(buttons.size());
		return S_OK;
	}

	CString strKey(key);
	ButtonList::iterator itBegin(buttons.begin()), itEnd(buttons.end());
	for (ButtonList::iterator it = itBegin; it != itEnd; ++it)
	{
		if (strKey.Compare((*it)->GetID()) == 0)
		{
			value->vt		= VT_DISPATCH;
			CButtonWrapper* pButton	= new CButtonWrapper(m_ptl, *it);
			value->pdispVal	= static_cast<IDispatch*> (pButton);
			return S_OK;
		}
	}

	return E_FAIL;
}