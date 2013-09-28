//
// CApplicationWrapper.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CApplicationWrapper__
#define __H_CApplicationWrapper__

#include "IDispatchImpl.h"
#include "ImageViewer_h.h"

class CImageProcessorWrapper;
class CMainWindowWrapper;

class CApplicationWrapper : public IDispatchImpl<CApplicationWrapper
												, IApplication
												, &IID_IApplication>
{
public:
	CApplicationWrapper(ITypeLib* ptl);
	~CApplicationWrapper();

	// APIs exposed to scripts
	STDMETHODIMP get_version(BSTR* value);
	STDMETHODIMP get_mainWindow(IDispatch** ppdisp);
	STDMETHODIMP get_imageProcessor(IDispatch** ppdisp);
	STDMETHODIMP put_onApplicationLoaded(IDispatch* pCallback);
	STDMETHODIMP getImageFile(IDispatch* pCallback);
	STDMETHODIMP saveImageFile(IDispatch* pCallback);
	STDMETHODIMP refreshDisplay();
	STDMETHODIMP showAboutDialog();
	STDMETHODIMP minimizeMainWindow();
	STDMETHODIMP exitApplication();

	// Constant values
	STDMETHODIMP get_SUCCEEDED(UINT* value);
	STDMETHODIMP get_FAILED(UINT* value);

	// DOM Level 2 EventTarget methods
	STDMETHODIMP addEventListener(BSTR type, IDispatch* pListener, BOOL useCapture);
	STDMETHODIMP removeEventListener(BSTR type, IDispatch* pListener, BOOL useCapture);
	STDMETHODIMP dispatchEvent(IDispatch* pEvent, BOOL* value);

private:
	CImageProcessorWrapper*		mImageProcessor;
	CMainWindowWrapper*			mMainWindow;
};

#endif // __H_CApplicationWrapper__