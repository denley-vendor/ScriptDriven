//
// CCar.h
//

#ifndef __H_CCar__
#define __H_CCar__

#include "IDispatchImpl.h"
#include "js_demo_h.h"
#include "IDispatchArrayImpl.h"

class CEngine;
class CWheelArray;

class CCar : public IDispatchImpl<CCar
								, ICar
								, &IID_ICar>
{
public:
	CCar(ITypeLib* ptl);
	~CCar();

	// APIs exposed to scripts
	STDMETHODIMP get_engine(IDispatch** ppdisp);
	STDMETHODIMP get_wheels(IDispatch** ppdisp);

	STDMETHODIMP createWheel(BSTR brand, BSTR color, IDispatch** ppdisp);
	STDMETHODIMP installWheel(IDispatch* pdisp, UINT index);
	STDMETHODIMP installWheel2(VARIANT pVar, UINT index);
	STDMETHODIMP createEnhancedWheel(BSTR brand, BSTR color, IDispatch** ppdisp);
	STDMETHODIMP createDynamicWheel(BSTR brand, BSTR color, IDispatch** ppdisp);

private:
	CEngine*		mEngine;
	CWheelArray*	mWheelArray;
};


class CEngine : public IDispatchImpl<CEngine
									, IEngine
									, &IID_IEngine>
{
public:
	CEngine(ITypeLib* ptl);
	~CEngine();

	// APIs exposed to scripts
	STDMETHODIMP get_brand(BSTR* pVal);
	STDMETHODIMP run();
};


class CWheel : public IDispatchImpl<CWheel
								, IWheel
								, &IID_IWheel>
{
public:
	CWheel(ITypeLib* ptl, BSTR brand, BSTR color);
	~CWheel();

	// APIs exposed to scripts
	STDMETHODIMP get_brand(BSTR* pVal);
	STDMETHODIMP get_color(BSTR* pVal);

	STDMETHODIMP rotate();
	STDMETHODIMP adjustParameters(BSTR param1, UINT param2, UINT param3);

private:
	BSTR		mBrand;
	BSTR		mColor;
};


class CWheelArray : public IDispatchArrayImpl<CWheelArray
											, IJSArray
											, CWheel
											, &IID_IJSArray>
{
public:
	CWheelArray(ITypeLib* ptl);
	~CWheelArray();

	// --- IJSArray methods ---
	STDMETHODIMP get_length(UINT* value);

protected:
	virtual CWheel* CreateElementInstance(UINT index);
	virtual void DeleteElementInstance(CWheel* pElem);
	virtual HRESULT getNthElement(UINT index, VARIANT* value);

private:
	typedef CWheel* WheelPointer;
	WheelPointer*		mpWheels;	
};


#include "IDispatchImpl2.h"

class CEnhancedWheel : public IDispatchImpl2<CEnhancedWheel
										, IEnhancedWheel
										, &IID_IEnhancedWheel>
{
	typedef IDispatchImpl2<CEnhancedWheel, IEnhancedWheel, &IID_IEnhancedWheel> __base;

public:
	CEnhancedWheel(ITypeLib* ptl, BSTR brand, BSTR color);
	~CEnhancedWheel();

	// --- IJSObject methods ---
	
	#define __ENHANCED__

	// Give more detailed info
	STDMETHODIMP toString(BSTR* value)
	{
#ifdef __ENHANCED__
		*value = ::SysAllocString(L"[object EnhancedWheel]");
		return S_OK;
#else
		return __base::toString(value);
#endif
	}

	// APIs exposed to scripts
	STDMETHODIMP get_brand(BSTR* pVal);
	STDMETHODIMP get_color(BSTR* pVal);

	STDMETHODIMP rotate();
	STDMETHODIMP adjustParameters(BSTR param1, UINT param2, UINT param3);

private:
	BSTR		mBrand;
	BSTR		mColor;
};


#include "IDispatchExImpl.h"

class CDynamicWheel : public IDispatchExImpl<CDynamicWheel
										, IDynamicWheel
										, &IID_IDynamicWheel>
{
//	typedef IDispatchExImpl<CDynamicWheel, IDynamicWheel, &IID_IDynamicWheel> __base;

public:
	CDynamicWheel(ITypeLib* ptl, BSTR brand, BSTR color);
	~CDynamicWheel();

	// APIs exposed to scripts
	STDMETHODIMP get_brand(BSTR* pVal);
	STDMETHODIMP get_color(BSTR* pVal);

	STDMETHODIMP rotate();
	STDMETHODIMP adjustParameters(BSTR param1, UINT param2, UINT param3);

private:
	BSTR		mBrand;
	BSTR		mColor;
};

#endif // __H_CCar__