// Tester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Tester.h"

#include <initguid.h>
#include "Mobile_i.c"

#include "safe_defs.h"
#include "Mobile.h"
#include "DispatchHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	CoInitialize(0);

	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		HRESULT hr = S_OK;

		// create an automation object for testing...
		Mobile* pMobileObj = new Mobile();
		IDispatch* pDispMobile = 0;
		hr = pMobileObj->QueryInterface(IID_IDispatch, (void**)&pDispMobile);

		bool ret = false;
		DispatchHelper dispatcher(pDispMobile);

		// set/get properties on the automation object
		VARIANT var;
		VariantInit(&var);

		ret = dispatcher.GetProperty(L"type", &var); // get the original value
		VariantClear(&var);
		
		var.vt = VT_BSTR;
		var.bstrVal = ::SysAllocString(L"NOKIA");
		ret = dispatcher.SetProperty(L"type", &var); // set a new value
		VariantClear(&var);

		ret = dispatcher.GetProperty(L"type", &var); // verify the current value
		ASSERT(var.vt == VT_BSTR && wcscmp(var.bstrVal, L"NOKIA") == 0);
		VariantClear(&var);

		ret = dispatcher.GetProperty(L"color", &var); // get the current color
		VariantClear(&var);

		var.vt = VT_I4;
		var.lVal = 0x00808080;
		ret = dispatcher.SetProperty(L"color", &var); // set a new color value
		VariantClear(&var);

		// call methods of the automation object
		DISPID dispid = 0;
		if (dispatcher.GetDispid(L"Call", &dispid))
		{
			dispatcher.CreateParameters(1);
			dispatcher.AddParameter(0, L"13988888888");
			dispatcher.Invoke(dispid);
		}

		////////////////////////////////////////////////////////////////////////
		// test dual interface mechanism
		////////////////////////////////////////////////////////////////////////
		IMobile* pMobile = 0;
		hr = pMobileObj->QueryInterface(IID_IMobile, (void**)&pMobile);
		if (pMobile)
		{
			BSTR bstrType = 0;
			pMobile->get_type(&bstrType);
			if (bstrType)
				::SysFreeString(bstrType);

			pMobile->put_type(L"SAMSUNG");

			long curColor = 0;
			pMobile->get_color(&curColor);

			pMobile->put_color(0x00FFFFFFFF);

			pMobile->Call(L"13666666666");

			pMobile->Release();
		}

		// delete the automation object
		SAFE_RELEASE(pDispMobile);
	}

	CoUninitialize();

	return nRetCode;
}
