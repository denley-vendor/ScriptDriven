//
// DispatchHelper.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "DispatchHelper.h"

DispatchHelper::DispatchHelper(IDispatch* pDisp) 
	: mpDisp(pDisp)
	, mpParams(0)
	, mParamCount(0)
{
}

DispatchHelper::~DispatchHelper()
{
	DeleteParameters();
}

bool DispatchHelper::CreateParameters(int paramCount)
{
	// Delete the parameters used last time.
	DeleteParameters();

	mParamCount = paramCount;
	if (paramCount > 0)
	{
		mpParams = new VARIANT[paramCount];
		if (mpParams == 0)
			return false;

		for (int i = 0; i < paramCount; i++)
		{
			VariantInit( &(mpParams[i]) );
		}
	}
	
	return true;
}

void DispatchHelper::DeleteParameters()
{
	if (mpParams)
	{
		for (UINT i = 0; i < mParamCount; i++)
		{
			VariantClear( &(mpParams[i]) );
		}

		delete[] mpParams;
		mpParams = 0;
		mParamCount = 0;
	}
}

bool DispatchHelper::AddParameter(UINT index, VARIANT* value)
{
	if (index >= mParamCount)
		return false;
	if (!mpParams || !value)
		return false;

	VariantCopy(&(mpParams[index]), value);
	return true;
}

bool DispatchHelper::AddParameter(UINT index, BSTR value)
{
	if (index >= mParamCount)
		return false;
	if (!mpParams)
		return false;

	mpParams[index].vt		= VT_BSTR;
	mpParams[index].bstrVal = SysAllocString(value);
	return true;
}

bool DispatchHelper::AddParameter(UINT index, INT value)
{
	if (index >= mParamCount)
		return false;
	if (!mpParams)
		return false;

	mpParams[index].vt		= VT_INT;
	mpParams[index].intVal	= value;
	return true;
}

bool DispatchHelper::AddParameter(UINT index, UINT value)
{
	if (index >= mParamCount)
		return false;
	if (!mpParams)
		return false;

	mpParams[index].vt		= VT_UINT;
	mpParams[index].uintVal	= value;
	return true;
}

bool DispatchHelper::AddParameter(UINT index, IDispatch* pDisp)
{
	if (index >= mParamCount)
		return false;
	if (!mpParams)
		return false;

	mpParams[index].vt		 = VT_DISPATCH;
	mpParams[index].pdispVal = pDisp;
	if (pDisp)
		pDisp->AddRef();
	return true;
}

bool DispatchHelper::AddNullParameter(UINT index)
{
	if (index >= mParamCount)
		return false;
	if (!mpParams)
		return false;

	mpParams[index].vt = VT_NULL;
	return true;
}

void DispatchHelper::SetDispatch(IDispatch* pDisp)
{
	// Note: we don't add reference count!
	mpDisp = pDisp;
}

bool DispatchHelper::GetDispid(BSTR functionName, DISPID* outDispid)
{
	HRESULT hr = E_FAIL;
	if (mpDisp && outDispid)
	{
		hr = mpDisp->GetIDsOfNames(IID_NULL, &functionName, 1, 0, outDispid);
	}
	return SUCCEEDED(hr);
}

bool DispatchHelper::Invoke(DISPID dispid)
{
	if (mpDisp == 0)
		return false;

	DISPPARAMS	dispParams;
	dispParams.rgvarg            = mpParams;
	dispParams.rgdispidNamedArgs = 0;
	dispParams.cArgs             = mParamCount;
	dispParams.cNamedArgs        = 0;

	HRESULT hr = mpDisp->Invoke(dispid, IID_NULL, 0, DISPATCH_METHOD, 
		&dispParams, 0, 0, 0);
	return SUCCEEDED(hr);
}

bool DispatchHelper::SetProperty(BSTR propName, VARIANT* inValue)
{
	if (mpDisp == 0)
		return false;

	// Get a dispid for the specified property name
	DISPID dispid = 0;
	if (!GetDispid(propName, &dispid))
		return false;

	DISPPARAMS dispParams = {0, 0, 1, 1};
	dispParams.rgvarg = inValue;
	DISPID dispidPut  = DISPID_PROPERTYPUT;
	dispParams.rgdispidNamedArgs = &dispidPut;

	HRESULT hr = S_OK;
	if (inValue->vt == VT_UNKNOWN || inValue->vt == VT_DISPATCH || 
		(inValue->vt & VT_ARRAY) || (inValue->vt & VT_BYREF))
	{
		hr = mpDisp->Invoke(dispid, IID_NULL, 0, DISPATCH_PROPERTYPUTREF,
			&dispParams, 0, 0, 0);
	}
	else
	{
		hr = mpDisp->Invoke(dispid, IID_NULL, 0, DISPATCH_PROPERTYPUT,
			&dispParams, 0, 0, 0);
	}
	return SUCCEEDED(hr);
}

bool DispatchHelper::GetProperty(BSTR propName, VARIANT* outValue)
{
	if (mpDisp == 0)
		return false;

	// Get a dispid for the specified property name
	DISPID dispid = 0;
	if (!GetDispid(propName, &dispid))
		return false;

	DISPPARAMS dispParams = {0, 0, 0, 0};
	HRESULT hr = mpDisp->Invoke(dispid, IID_NULL, 0, DISPATCH_PROPERTYGET,
		&dispParams, outValue, 0, 0);
	return SUCCEEDED(hr);
}
