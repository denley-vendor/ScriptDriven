//
// IDispatchArrayImpl.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __IDispatchArrayImpl_H__
#define __IDispatchArrayImpl_H__

#include "IDispatchBaseImpl.h"
#include "global.h"
#include <vector>

template<class DERIVED_CLASS, class BASE_ITF, class ELEM_CLASS, const IID* BASE_IID>
class __declspec(novtable) IDispatchArrayImpl : public IDispatchBaseImpl<DERIVED_CLASS, BASE_ITF, BASE_IID>
{
protected:
	typedef std::vector< ELEM_CLASS* > ElemList;
	ElemList*		m_pElemList;
	UINT			m_nElemCount;
	DISPID			m_DispidBase;

public:
	IDispatchArrayImpl() : m_pElemList(0), m_nElemCount(0), m_DispidBase(200)
	{
	}

	virtual ~IDispatchArrayImpl()
	{
		DeleteElemList();
	}	
	
	// --- IDispatch methods --- 
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
		HRESULT hr = m_pti->GetIDsOfNames(rgszNames, cNames, rgdispid);
		if (DISP_E_UNKNOWNNAME == hr)
		{
			// Determine if it is a digital number...
			// If so, we recognize it as an array index.
			if (misc_utils::IsDigitals(*rgszNames))
			{
				int index = ::_wtoi(*rgszNames);
				*rgdispid = m_DispidBase + index;
				return S_OK;
			}
		}

		return hr;					
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
			hr = m_pti->Invoke(this, dispidMember, wFlags,		
				pdispparams, pvarResult, pexcepinfo, puArgErr);	
			if (DISP_E_MEMBERNOTFOUND == hr)
			{
				if (dispidMember >= m_DispidBase)
				{
					UINT index = dispidMember - m_DispidBase;
					if (m_nElemCount <= 0 || index >= m_nElemCount)
						return E_FAIL;
					if (m_pElemList == 0)
						return E_FAIL;

					//////////////////////////////////////////////////////////////////////
					// Create an element object if necessary.
					// Note: If you want to skip this step, please create element objects 
					// and build the list in the constructor of subclasses!
					ELEM_CLASS * pElem = (*m_pElemList)[index];
					if (pElem == 0)
					{
						// Note: subclasses must implement CreateElementInstance()!
						pElem = CreateElementInstance(index);
						(*m_pElemList)[index] = pElem;
					}
					//////////////////////////////////////////////////////////////////////

					// Note: subclasses must implement get_NthElement()!
					hr = getNthElement(index, pvarResult);
					return hr;
				}
			}
		}
		catch (...)
		{
			hr = E_UNEXPECTED;
		}

		return hr;
	}

protected:
	void CreateElemList()
	{
		if (m_nElemCount > 0)
		{
			m_pElemList = new ElemList(m_nElemCount);
			for (UINT i = 0; i < m_nElemCount; ++i)
			{
				(*m_pElemList)[i] = 0;
			}
		}
	}
	
	void DeleteElemList()
	{
		if (m_pElemList)
		{
			ElemList::iterator it, itEnd(m_pElemList->end());
			for (it = m_pElemList->begin(); it != itEnd; ++it)
			{
				if ((*it) != 0)
				{
					// Note: subclasses must implement DeleteElementInstance()!
					DeleteElementInstance(*it);
					(*it) = 0;
				}
			}

			delete m_pElemList;
			m_pElemList = 0;
		}
	}
	
	virtual HRESULT OnTypeInfoLoaded() 
	{
		// Determine the base DISPID for the array.
		DISPID maxDispid = 0;
		if (misc_utils::GetMaxDispid(m_pti, &maxDispid))
		{
			m_DispidBase = maxDispid + 200;
		}
		return S_OK;
	}

	virtual ELEM_CLASS* CreateElementInstance(UINT index) = 0;
	virtual void DeleteElementInstance(ELEM_CLASS* pElem) = 0;
	virtual HRESULT getNthElement(UINT index, VARIANT* value) = 0;
};

#endif	//__IDispatchArrayImpl_H__