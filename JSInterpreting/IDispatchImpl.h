//
// IDispatchImpl.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_IDispatchImpl__
#define __H_IDispatchImpl__

#include "CHQException.h"

template<class DERIVED_CLASS, class BASE_ITF, const IID* BASE_IID>
class __declspec(novtable) IDispatchImpl : public BASE_ITF
{
protected:
	long			m_cRef;
	ITypeInfo*		m_pti;
	ITypeLib*		m_ptl;
	
protected:
	virtual HRESULT OnTypeInfoLoaded() { return S_OK; }

public:
	IDispatchImpl() : m_pti(0), m_ptl(0), m_cRef(1) {};

	virtual ~IDispatchImpl() 
	{
		if (m_pti)
		{
			m_pti->Release();
			m_pti = 0;
		}
	}
	
	HRESULT LoadTypeInfo(ITypeLib* ptl)
	{
		// Note: we don't add extra reference to ptl!
		m_ptl = ptl;

		if (m_pti)
		{
			m_pti->Release();
			m_pti = 0;
		}

		HRESULT hr = m_ptl->GetTypeInfoOfGuid(*BASE_IID, &m_pti);
		if (SUCCEEDED(hr))
		{
			hr = OnTypeInfoLoaded();
		}
		return hr;
	}

	// Note: NOT outstanding!
	ITypeInfo* GetTypeInfoInterface() {return m_pti;}


	// --- IUnknown methods --- 
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) 
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
		else if (*BASE_IID == riid)
		{
			*ppvObject = static_cast<BASE_ITF*> (this);
			AddRef();
			return S_OK;
		}

		*ppvObject = 0;
		return E_NOINTERFACE;
	}

	ULONG STDMETHODCALLTYPE AddRef()
	{
		return InterlockedIncrement(&m_cRef);
	}

	ULONG STDMETHODCALLTYPE Release()
	{
		if (0 == InterlockedDecrement(&m_cRef))
		{
			delete this;
			return 0;
		}
		return m_cRef;
	}

	STDMETHODIMP GetTypeInfoCount(UINT * pctinfo)								
	{																			
		*pctinfo = 1;															
		return S_OK;															
	}	

	STDMETHODIMP GetTypeInfo(UINT itinfo, LCID /*lcid*/, ITypeInfo ** pptinfo)
	{
		if (0 == pptinfo)	
		{	
			return E_POINTER;	
		}	
		if (0 != itinfo)
		{			
			return TYPE_E_ELEMENTNOTFOUND;	
		}	
		if (0 == m_pti)	
		{
			return TYPE_E_ELEMENTNOTFOUND;	
		}	

		*pptinfo = m_pti;
		m_pti->AddRef();	
		return S_OK;	
	}

	STDMETHODIMP GetIDsOfNames(REFIID /*riid*/,
		OLECHAR  ** rgszNames,	
		UINT cNames,
		LCID /*lcid*/,
		DISPID * rgdispid)	
	{
		return m_pti->GetIDsOfNames(rgszNames, cNames, rgdispid);
	}

	STDMETHODIMP Invoke(DISPID dispidMember,
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
		try
		{
			hr = m_pti->Invoke((void*) this, dispidMember, wFlags,
				pdispparams, pvarResult, pexcepinfo, puArgErr);
		}
		catch (CHQException& ex)
		{
			// wCode: An error code identifying the error. 
			// Either this field or the return value field must be filled in; 
			// the other must be set to 0.
			// See winerror.h for more details on HRESULT error code.
			pexcepinfo->wCode = ex.GetNumber();
			pexcepinfo->scode = 0;

			wchar_t wszMsg[1000];
			::MultiByteToWideChar(CP_ACP, 0, ex.what(), -1, wszMsg, 1000);
			pexcepinfo->bstrDescription = ::SysAllocString(wszMsg);

			hr = DISP_E_EXCEPTION;
		}
		catch (...)
		{
			hr = E_UNEXPECTED;
		}

		return hr;
	}
};

#endif	//__H_IDispatchImpl__