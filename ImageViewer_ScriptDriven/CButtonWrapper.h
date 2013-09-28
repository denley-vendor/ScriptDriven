//
// CButtonWrapper.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CButtonWrapper__
#define __H_CButtonWrapper__

#include "IDispatchImpl.h"
#include "ImageViewer_h.h"
#include "IDispatchExImpl2.h"

class CScriptButton;

class CButtonWrapper : public IDispatchImpl<CButtonWrapper
										, IButton
										, &IID_IButton>
{
public:
	CButtonWrapper(ITypeLib* ptl, CScriptButton* button);
	~CButtonWrapper();

	// APIs exposed to scripts
	STDMETHODIMP get_id(BSTR* value);
	STDMETHODIMP get_enabled(BOOL* value);
	STDMETHODIMP put_enabled(BOOL value);

private:
	CScriptButton*		mButton;
};

////////////////////////////////////////////////////////////////////////////////
class CButtonSet : public IDispatchExImpl2<CButtonSet
										, IButtonSet
										, &IID_IButtonSet>
{
public:
	CButtonSet(ITypeLib* ptl);
	~CButtonSet();

	virtual HRESULT IsKeyDefined(BSTR key);
	virtual HRESULT GetValueOfKey(BSTR key, VARIANT* value);
};

#endif // __H_CButtonWrapper__