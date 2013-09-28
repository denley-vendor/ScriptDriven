//
// IDispatchBaseImpl.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __IDispatchBaseImpl_H__
#define __IDispatchBaseImpl_H__

template<class DERIVED_CLASS, class BASE_ITF, const IID* BASE_IID>
class __declspec(novtable) IDispatchBaseImpl : public BASE_ITF
{
protected:
	ITypeInfo*		m_pti;
	ITypeLib*		m_ptl;
	long			m_cRef;

	IDispatch*		mPrototype;

public:
	IDispatchBaseImpl() : m_pti(0), m_ptl(0), m_cRef(1), mPrototype(0)
	{
	}

	virtual ~IDispatchBaseImpl() 
	{
		if (mPrototype)
		{
			mPrototype->Release();
			mPrototype = 0;
		}

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


	// --- IJSObject methods --- 
	STDMETHODIMP get_DefaultValue(VARIANT* value)
	{
		if (value)
		{
			value->vt = VT_BSTR;
			return toString(&value->bstrVal); 
		}
		return E_FAIL;
	}

	STDMETHODIMP get_prototype(VARIANT* value)
	{
		if (value)
		{
			if (mPrototype)
			{
				mPrototype->AddRef();
				value->vt = VT_DISPATCH;
				value->pdispVal = mPrototype;
			}
			else
			{
				value->vt = VT_NULL; // Default to null
			}
			return S_OK;			
		}
		return E_FAIL;
	}

	STDMETHODIMP put_prototype(VARIANT value)
	{
		if (mPrototype)
		{
			mPrototype->Release();
			mPrototype = 0;
		}
		if (value.vt == VT_DISPATCH)
			mPrototype = value.pdispVal;
		return S_OK;
	}

	STDMETHODIMP toString(BSTR* value)
	{
		*value = ::SysAllocString(L"[object]");
		return S_OK;
	}


	// --- IUnknown methods --- 
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,
		void **ppvObject) 
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

protected:
	virtual HRESULT OnTypeInfoLoaded() { return S_OK; }
};

#endif	//__IDispatchBaseImpl_H__