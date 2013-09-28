//
// CMainWindowWrapper.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CMainWindowWrapper__
#define __H_CMainWindowWrapper__

#include "IDispatchImpl.h"
#include "ImageViewer_h.h"

class CButtonSet;

class CMainWindowWrapper : public IDispatchImpl<CMainWindowWrapper
											, IMainWindow
											, &IID_IMainWindow>
{
public:
	CMainWindowWrapper(ITypeLib* ptl);
	~CMainWindowWrapper();

	// APIs exposed to scripts
	STDMETHODIMP get_buttons(IDispatch** ppdisp);

private:
	CButtonSet*		mButtons;
};

#endif // __H_CMainWindowWrapper__