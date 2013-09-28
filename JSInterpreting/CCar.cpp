//
// CCar.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "safe_defs.h"
#include "CDebugger.h"
#include "CCar.h"
#include "DispatchHelper.h"

/////////////////////////////////////////////////////////////////////////////////////////
CCar::CCar(ITypeLib* ptl)
	: mEngine(0)
	, mWheelArray(0)
{
	LoadTypeInfo(ptl);

	mEngine = new CEngine(ptl);
	mWheelArray = new CWheelArray(ptl);
}

CCar::~CCar()
{
	SAFE_RELEASE(mEngine);
	SAFE_RELEASE(mWheelArray);
}

// APIs exposed to scripts
STDMETHODIMP CCar::get_engine(IDispatch** ppdisp)
{
	if (mEngine)
	{
		return mEngine->QueryInterface(IID_IDispatch, (void**)ppdisp);
	}
	return E_FAIL;
}

STDMETHODIMP CCar::get_wheels(IDispatch** ppdisp)
{
	if (mWheelArray)
	{
		return mWheelArray->QueryInterface(IID_IDispatch, (void**)ppdisp);
	}
	return E_FAIL;
}

STDMETHODIMP CCar::createWheel(BSTR brand, BSTR color, IDispatch** ppdisp)
{
	HRESULT hr = E_FAIL;
	CWheel* pWheel = new CWheel(m_ptl, brand, color);
	if (pWheel)
	{
		hr = pWheel->QueryInterface(IID_IDispatch, (void**)ppdisp);

		// Let JSCript Engine to manage this object's lifecycle. 
		// So we can release a reference count safely here.
		pWheel->Release();
	}
	return hr;
}

STDMETHODIMP CCar::createEnhancedWheel(BSTR brand, BSTR color, IDispatch** ppdisp)
{
	HRESULT hr = E_FAIL;
	CEnhancedWheel* pWheel = new CEnhancedWheel(m_ptl, brand, color);
	if (pWheel)
	{
		hr = pWheel->QueryInterface(IID_IDispatch, (void**)ppdisp);
		pWheel->Release();
	}
	return hr;
}

STDMETHODIMP CCar::createDynamicWheel(BSTR brand, BSTR color, IDispatch** ppdisp)
{
	HRESULT hr = E_FAIL;
	CDynamicWheel* pWheel = new CDynamicWheel(m_ptl, brand, color);
	if (pWheel)
	{
		hr = pWheel->QueryInterface(IID_IDispatch, (void**)ppdisp);
		pWheel->Release();
	}
	return hr;
}


#define __SAFE_MODE__

STDMETHODIMP CCar::installWheel(IDispatch* pdisp, UINT index)
{
	//TODO: install the Nth wheel.
	//...

	// Retrieve the properties of the wheel object,
	// and invoke some functions on this object.

#ifdef __SAFE_MODE__
	// Works fine on any conditions

	// Retrieve properties
	DispatchHelper dispatcher(pdisp);
	VARIANT var;
	VariantInit(&var);
	dispatcher.GetProperty(L"brand", &var);
	ASSERT(var.vt == VT_BSTR);
	VariantClear(&var);

	// Invoke functions
	dispatcher.CreateParameters(3);
	dispatcher.AddParameter(0, 8);
	dispatcher.AddParameter(1, 6);
	dispatcher.AddParameter(2, L"max");
	DISPID dispid = 0;
	if (dispatcher.GetDispid(L"adjustParameters", &dispid))
	{
		dispatcher.Invoke(dispid);
	}
	
#else
	// If the wheel object is created by C++ side...
	CWheel* pWheelObj = static_cast<CWheel*> (pdisp);
	if (pWheelObj)
	{
		// Retrieve properties
		BSTR bstrBrand = 0;
		pWheelObj->get_brand(&bstrBrand);
		if (bstrBrand)
			::SysFreeString(bstrBrand);

		// Invoke functions
		pWheelObj->adjustParameters(L"max", 6, 8);
	}
#endif

	wchar_t wszMsg[100];
	swprintf(wszMsg, L"The %dth wheel is installed.", index);
	GetDebugger()->trace(L"Function", wszMsg);

	return S_OK;
}

STDMETHODIMP CCar::installWheel2(VARIANT pVar, UINT index)
{
	if (pVar.vt == VT_NULL)
	{
		//TODO: uninstall the Nth wheel.
		//...

		wchar_t wszMsg[100];
		swprintf(wszMsg, L"The %dth wheel is uninstalled.", index);
		GetDebugger()->trace(L"Function", wszMsg);
		return S_OK;
	}
	else if (pVar.vt == VT_DISPATCH)
	{
		return installWheel(pVar.pdispVal, index);
	}
	else 
	{
		return E_UNEXPECTED;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
CEngine::CEngine(ITypeLib* ptl)
{
	LoadTypeInfo(ptl);
}

CEngine::~CEngine()
{
}

// APIs exposed to scripts
STDMETHODIMP CEngine::get_brand(BSTR* pVal)
{
	*pVal = ::SysAllocString(L"ACTECO");
	return S_OK;
}

STDMETHODIMP CEngine::run()
{
	GetDebugger()->trace(L"Function", L"The car engine starts to run...");
	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////////////////
CWheel::CWheel(ITypeLib* ptl, BSTR brand, BSTR color)
	: mBrand(0)
	, mColor(0)
{
	LoadTypeInfo(ptl);

	if (brand)
		mBrand = ::SysAllocString(brand);
	if (color)
		mColor = ::SysAllocString(color);
}

CWheel::~CWheel()
{
	if (mBrand)
		::SysFreeString(mBrand);
	if (mColor)
		::SysFreeString(mColor);
}

// APIs exposed to scripts
STDMETHODIMP CWheel::get_brand(BSTR* pVal)
{
	if (mBrand)
	{
		*pVal = ::SysAllocString(mBrand);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CWheel::get_color(BSTR* pVal)
{
	if (mColor)
	{
		*pVal = ::SysAllocString(mColor);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CWheel::rotate()
{
	GetDebugger()->trace(L"Function", L"The car wheel starts to rotate...");
	return S_OK;
}

STDMETHODIMP CWheel::adjustParameters(BSTR param1, UINT param2, UINT param3)
{
	wchar_t wszInfo[100];
	swprintf(wszInfo, L"Parameters adjusted to: %s, %d, %d.", param1, param2, param3);
	GetDebugger()->trace(L"Function", wszInfo);
	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////////////////
static const int kWheelCount = 4;
CWheelArray::CWheelArray(ITypeLib* ptl) : mpWheels(0)
{
	LoadTypeInfo(ptl);

	// Retrieve the total element count and initialize the element list.
	// get_length() is a method of IJSArray.
	HRESULT hr = get_length(&m_nElemCount);
	if (SUCCEEDED(hr))
	{
		CreateElemList();
	}

	// Testing: build 4 wheels internally!
	mpWheels = new WheelPointer[kWheelCount];
	mpWheels[0] = new CWheel(ptl, L"KUMHO-100", L"black1");
	mpWheels[1] = new CWheel(ptl, L"KUMHO-200", L"black2");
	mpWheels[2] = new CWheel(ptl, L"KUMHO-300", L"black3");
	mpWheels[3] = new CWheel(ptl, L"KUMHO-400", L"black4");
}

CWheelArray::~CWheelArray()
{
	if (mpWheels)
	{
		delete[] mpWheels;
		mpWheels = 0;
	}
}

// --- IJSArray methods ---
STDMETHODIMP CWheelArray::get_length(UINT* value)
{
	*value = kWheelCount;
	return S_OK;
}

CWheel* CWheelArray::CreateElementInstance(UINT index)
{
	return mpWheels[index];
}

void CWheelArray::DeleteElementInstance(CWheel* pElem)
{
	if (pElem)
	{
		pElem->Release();
	}
}

HRESULT CWheelArray::getNthElement(UINT index, VARIANT* value)
{
	if (index >= m_nElemCount)
		return E_FAIL;

	CWheel* pElem = (*m_pElemList)[index];
	if (pElem)
	{
		value->vt = VT_DISPATCH;
		pElem->QueryInterface(IID_IDispatch, (void**)&value->pdispVal);
		return S_OK;
	}
	return E_FAIL;
}


/////////////////////////////////////////////////////////////////////////////////////////
CEnhancedWheel::CEnhancedWheel(ITypeLib* ptl, BSTR brand, BSTR color)
	: mBrand(0)
	, mColor(0)
{
	LoadTypeInfo(ptl);

	if (brand)
		mBrand = ::SysAllocString(brand);
	if (color)
		mColor = ::SysAllocString(color);
}

CEnhancedWheel::~CEnhancedWheel()
{
	if (mBrand)
		::SysFreeString(mBrand);
	if (mColor)
		::SysFreeString(mColor);
}

// APIs exposed to scripts
STDMETHODIMP CEnhancedWheel::get_brand(BSTR* pVal)
{
	if (mBrand)
	{
		*pVal = ::SysAllocString(mBrand);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CEnhancedWheel::get_color(BSTR* pVal)
{
	if (mColor)
	{
		*pVal = ::SysAllocString(mColor);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CEnhancedWheel::rotate()
{
	GetDebugger()->trace(L"Function", L"The car wheel starts to rotate...");
	return S_OK;
}

STDMETHODIMP CEnhancedWheel::adjustParameters(BSTR param1, UINT param2, UINT param3)
{
	wchar_t wszInfo[100];
	swprintf(wszInfo, L"Parameters adjusted to: %s, %d, %d.", param1, param2, param3);
	GetDebugger()->trace(L"Function", wszInfo);
	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////////////////
CDynamicWheel::CDynamicWheel(ITypeLib* ptl, BSTR brand, BSTR color)
	: mBrand(0)
	, mColor(0)
{
	LoadTypeInfo(ptl);

	if (brand)
		mBrand = ::SysAllocString(brand);
	if (color)
		mColor = ::SysAllocString(color);
}

CDynamicWheel::~CDynamicWheel()
{
	if (mBrand)
		::SysFreeString(mBrand);
	if (mColor)
		::SysFreeString(mColor);
}

// APIs exposed to scripts
STDMETHODIMP CDynamicWheel::get_brand(BSTR* pVal)
{
	if (mBrand)
	{
		*pVal = ::SysAllocString(mBrand);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CDynamicWheel::get_color(BSTR* pVal)
{
	if (mColor)
	{
		*pVal = ::SysAllocString(mColor);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CDynamicWheel::rotate()
{
	GetDebugger()->trace(L"Function", L"The car wheel starts to rotate...");
	return S_OK;
}

STDMETHODIMP CDynamicWheel::adjustParameters(BSTR param1, UINT param2, UINT param3)
{
	wchar_t wszInfo[100];
	swprintf(wszInfo, L"Parameters adjusted to: %s, %d, %d.", param1, param2, param3);
	GetDebugger()->trace(L"Function", wszInfo);
	return S_OK;
}

