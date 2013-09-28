//
// CApplicationWrapper.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "safe_defs.h"
#include "global_defs.h"
#include "CApplicationWrapper.h"
#include "ImageViewerDlg.h"
#include "JSEventHandling.h"
#include "DispatchHelper.h"
#include "CImageProcessorWrapper.h"
#include "CMainWindowWrapper.h"

/////////////////////////////////////////////////////////////////////////////////////////
CApplicationWrapper::CApplicationWrapper(ITypeLib* ptl)
	: mImageProcessor(0)
	, mMainWindow(0)
{
	LoadTypeInfo(ptl);
}

CApplicationWrapper::~CApplicationWrapper()
{
	SAFE_RELEASE(mImageProcessor);
	SAFE_RELEASE(mMainWindow);
}

// APIs exposed to scripts
STDMETHODIMP CApplicationWrapper::get_version(BSTR* value)
{
	*value = ::SysAllocString(L"1.3");
	return S_OK;
}

STDMETHODIMP CApplicationWrapper::get_mainWindow(IDispatch** ppdisp)
{
	if (mMainWindow == 0)
		mMainWindow = new CMainWindowWrapper(m_ptl);
	if (mMainWindow)
		return mMainWindow->QueryInterface(IID_IDispatch, (void**)ppdisp);
	return E_FAIL;	
}

STDMETHODIMP CApplicationWrapper::get_imageProcessor(IDispatch** ppdisp)
{
	if (mImageProcessor == 0)
		mImageProcessor = new CImageProcessorWrapper(m_ptl, GetMainApp()->GetImageProcessor());
	if (mImageProcessor)
		return mImageProcessor->QueryInterface(IID_IDispatch, (void**)ppdisp);
	return E_FAIL;
}

STDMETHODIMP CApplicationWrapper::put_onApplicationLoaded(IDispatch* pCallback)
{
	CImageViewerDlg* pApp = GetMainApp();
	if (pApp)
	{
		DispatchHelper* pHandler = new DispatchHelper(pCallback);
		pApp->SetApplicationLoadedHandler(pHandler);
		return S_OK;
	}
	return E_FAIL;
}

// callback(status:UINT, filename:string);
STDMETHODIMP CApplicationWrapper::getImageFile(IDispatch* pCallback)
{
	DispatchHelper dispatcher(pCallback);
	dispatcher.CreateParameters(2);

	CString srcFile;
	CImageViewerDlg* pApp = GetMainApp();
	if (pApp && pApp->GetImageFile(srcFile))
	{
		BSTR bstrFile = srcFile.AllocSysString();
		dispatcher.AddParameter(0, bstrFile);
		dispatcher.AddParameter(1, SUCCEEDED);
		::SysFreeString(bstrFile);
	}
	else
	{
		dispatcher.AddParameter(0, L"");
		dispatcher.AddParameter(1, FAILED);
	}

	dispatcher.Invoke(0);
	return S_OK;
}

// callback(status:UINT, filename:string);
STDMETHODIMP CApplicationWrapper::saveImageFile(IDispatch* pCallback)
{
	DispatchHelper dispatcher(pCallback);
	dispatcher.CreateParameters(2);

	CString dstFile;
	CImageViewerDlg* pApp = GetMainApp();
	if (pApp && pApp->SaveImageFile(dstFile))
	{
		BSTR bstrFile = dstFile.AllocSysString();
		dispatcher.AddParameter(0, bstrFile);
		dispatcher.AddParameter(1, SUCCEEDED);
		::SysFreeString(bstrFile);
	}
	else
	{
		dispatcher.AddParameter(0, L"");
		dispatcher.AddParameter(1, FAILED);
	}

	dispatcher.Invoke(0);
	return S_OK;
}

STDMETHODIMP CApplicationWrapper::refreshDisplay()
{
	CImageViewerDlg* pApp = GetMainApp();
	if (pApp)
	{
		pApp->RefreshDisplay();
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CApplicationWrapper::showAboutDialog()
{
	CImageViewerDlg* pApp = GetMainApp();
	if (pApp)
	{
		pApp->ShowAboutDialog();
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CApplicationWrapper::minimizeMainWindow()
{
	CImageViewerDlg* pApp = GetMainApp();
	if (pApp)
	{
		pApp->MinimizeMainWindow();
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CApplicationWrapper::exitApplication()
{
	CImageViewerDlg* pApp = GetMainApp();
	if (pApp)
	{
		pApp->ExitApplication();
		return S_OK;
	}
	return E_FAIL;
}

// Constant values
STDMETHODIMP CApplicationWrapper::get_SUCCEEDED(UINT* value)
{
	*value = SUCCEEDED;
	return S_OK;
}

STDMETHODIMP CApplicationWrapper::get_FAILED(UINT* value)
{
	*value = FAILED;
	return S_OK;
}

// DOM Level 2 EventTarget methods
STDMETHODIMP CApplicationWrapper::addEventListener(BSTR type, IDispatch* pListener, BOOL useCapture)
{
	CImageViewerDlg* pApp = GetMainApp();
	if (pApp)
	{
		EventTargetWrapper tmpTarget(m_ptl, pApp);
		return tmpTarget.addEventListener(type, pListener, useCapture);
	}
	return E_FAIL;
}

STDMETHODIMP CApplicationWrapper::removeEventListener(BSTR type, IDispatch* pListener, BOOL useCapture)
{
	CImageViewerDlg* pApp = GetMainApp();
	if (pApp)
	{
		EventTargetWrapper tmpTarget(m_ptl, pApp);
		return tmpTarget.removeEventListener(type, pListener, useCapture);
	}
	return E_FAIL;
}

STDMETHODIMP CApplicationWrapper::dispatchEvent(IDispatch* pEvent, BOOL* value)
{
	CImageViewerDlg* pApp = GetMainApp();
	*value = FALSE;
	if (pApp)
	{
		EventTargetWrapper tmpTarget(m_ptl, pApp);
		return tmpTarget.dispatchEvent(pEvent, value);
	}
	return E_FAIL;
}