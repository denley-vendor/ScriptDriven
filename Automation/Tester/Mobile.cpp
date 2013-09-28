//
// Mobile.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "safe_defs.h"
#include "Mobile.h"

/////////////////////////////////////////////////////////////////////////////////////////
Mobile::Mobile()
	: m_cRef(0)
	, m_ptl(0)
	, m_Type(0)
	, m_Color(0)
{
}

Mobile::~Mobile()
{
	if (m_Type)
	{
		::SysFreeString(m_Type);
		m_Type = 0;
	}
	SAFE_RELEASE(m_ptl);
}

// --- IUnknown methods --- 
STDMETHODIMP Mobile::QueryInterface(REFIID riid, void **ppvObject) 
{
	if (IID_IUnknown == riid)
	{
		*ppvObject = static_cast<IUnknown*> (this);
		AddRef();
		return S_OK;
	}
	else if (IID_IDispatch == riid)
	{
		*ppvObject = static_cast<IDispatch*> (this);
		AddRef();
		return S_OK;
	}
	else if (IID_IMobile == riid)
	{
		*ppvObject = static_cast<IMobile*> (this);
		AddRef();
		return S_OK;
	}

	*ppvObject = 0;
	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) Mobile::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) Mobile::Release()
{
	if (0 == InterlockedDecrement(&m_cRef))
	{
		delete this;
		return 0;
	}
	return m_cRef;
}


// --- IDispatch methods --- 
STDMETHODIMP Mobile::GetTypeInfoCount(UINT * pctinfo)
{
	*pctinfo = 1;
	return S_OK;
}

STDMETHODIMP Mobile::GetTypeInfo(UINT itinfo, LCID /*lcid*/, ITypeInfo ** pptinfo)
{
	if (0 == pptinfo)
	{
		return E_POINTER;	
	}
	if (0 != itinfo)
	{
		return TYPE_E_ELEMENTNOTFOUND;	
	}

	*pptinfo = GetTypeInfo();
	return (*pptinfo == 0) ? E_FAIL : S_OK;	
}			

STDMETHODIMP Mobile::GetIDsOfNames(REFIID /*riid*/,	
						   OLECHAR  ** rgszNames,
						   UINT cNames,	
						   LCID /*lcid*/,	
						   DISPID * rgdispid)
{
	HRESULT hr = E_FAIL;

	ITypeInfo* pTypeInfo = GetTypeInfo();
	if (pTypeInfo)
	{
		hr = pTypeInfo->GetIDsOfNames(rgszNames, cNames, rgdispid);
		pTypeInfo->Release();
	}
	return hr;				
}		

STDMETHODIMP Mobile::Invoke(DISPID dispidMember,
					REFIID riid,
					LCID /*lcid*/,
					WORD wFlags,
					DISPPARAMS * pdispparams,
					VARIANT * pvarResult,	
					EXCEPINFO * pexcepinfo,
					UINT * puArgErr)
{
	if (IID_NULL != riid)
	{		
		return DISP_E_UNKNOWNINTERFACE;	
	}	

	HRESULT hr = NOERROR;
	ITypeInfo* pTypeInfo = 0;

	try
	{
		pTypeInfo = GetTypeInfo();
		if (pTypeInfo)
		{
			hr = pTypeInfo->Invoke((void*)this, dispidMember, wFlags,
				pdispparams, pvarResult, pexcepinfo, puArgErr);
		}
	}
	catch (...)
	{
		hr = E_FAIL;
	}

	SAFE_RELEASE(pTypeInfo);
	return hr;	
}

ITypeInfo* Mobile::GetTypeInfo()
{
	// Load the type library
	HRESULT hr = NOERROR;
	if (m_ptl == 0)
	{	
		hr = ::LoadTypeLib(L"Mobile.tlb", &m_ptl);
	}

	ITypeInfo* pTypeInfo = 0;
	if (SUCCEEDED(hr) && m_ptl)
	{
		m_ptl->GetTypeInfoOfGuid(IID_IMobile, &pTypeInfo);
	}
	return pTypeInfo;
}


// --- IMobile methods --- 
STDMETHODIMP Mobile::get_type(BSTR *pVal)
{
	if (m_Type)
	{
		*pVal = ::SysAllocString(m_Type);
	}
	else
	{
		*pVal = ::SysAllocString(L"unknown");
	}
	return S_OK;
}

STDMETHODIMP Mobile::put_type(BSTR newVal)
{
	if (m_Type)
	{
		::SysFreeString(m_Type);
		m_Type = 0;
	}
	m_Type = ::SysAllocString(newVal);
	return S_OK;
}

STDMETHODIMP Mobile::get_color(long *pVal)
{
	*pVal = m_Color;
	return S_OK;
}

STDMETHODIMP Mobile::put_color(long newVal)
{
	m_Color = newVal;
	return S_OK;
}

STDMETHODIMP Mobile::Call(BSTR phoneNumber)
{
	// calling sb.
	// ...

#ifdef _DEBUG
	wchar_t wszMsg[100];
	swprintf(wszMsg, L"You're calling %s ...", phoneNumber);
	::MessageBoxW(0, wszMsg, L"COM Automation Tester", MB_OK);
#endif

	return S_OK;
}