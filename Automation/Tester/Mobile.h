//
// Mobile.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_Mobile__
#define __H_Mobile__

#include "Mobile_h.h"

class Mobile : public IMobile
{
public:
	Mobile();
	~Mobile();

	// --- IUnknown methods --- 
	STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject); 
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	// --- IDispatch methods --- 
	STDMETHODIMP GetTypeInfoCount(UINT * pctinfo);
	STDMETHODIMP GetTypeInfo(UINT itinfo, LCID /*lcid*/, ITypeInfo ** pptinfo);
	STDMETHODIMP GetIDsOfNames(REFIID /*riid*/,	
							OLECHAR  ** rgszNames,
							UINT cNames,
							LCID /*lcid*/,
							DISPID * rgdispid);
	STDMETHODIMP Invoke(DISPID dispidMember,
						REFIID riid,
						LCID /*lcid*/,	
						WORD wFlags,		
						DISPPARAMS * pdispparams,	
						VARIANT * pvarResult,	
						EXCEPINFO * pexcepinfo,	
						UINT * puArgErr);

	// --- IMobile methods --- 
	STDMETHODIMP get_type(BSTR *pVal);
	STDMETHODIMP put_type(BSTR newVal);
	STDMETHODIMP get_color(long *pVal);
	STDMETHODIMP put_color(long newVal);
	STDMETHODIMP Call(BSTR phoneNumber);

protected:
	ITypeInfo* GetTypeInfo();

private:
	long			m_cRef;			// Reference count
	ITypeLib*		m_ptl;			// Type library

	BSTR			m_Type;
	long			m_Color;
};

#endif // __H_Mobile__