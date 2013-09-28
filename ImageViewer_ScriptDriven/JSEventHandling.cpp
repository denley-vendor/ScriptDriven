//
// JSEventHandling.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "JSEventHandling.h"
#include "DOM2EventTarget.h"
#include "DispatchHelper.h"
#include "ViewerEvent.h"

//////////////////////////////////////////////////////////////////////////////////////////////
JSEventListener::JSEventListener(ITypeLib* ptl, BSTR type, IDispatch* pListener, BOOL useCapture) 
	: mptl(ptl)
	, mpListener(pListener)
{
	if (mpListener)
		mpListener->AddRef();

	setUseCapture(useCapture ? true : false);
	if (type)
	{
		CString strType(type);
		setEventType((LPCSTR)strType);
	}
}

JSEventListener::~JSEventListener()
{
	if (mpListener)
		mpListener->Release();
}

bool JSEventListener::isSame(JSEventListener* pListener2)
{
	if (mEventType == pListener2->getEventType() &&
		mpListener == pListener2->mpListener &&
		mbUseCapture == pListener2->getUseCapture())
	{
		return true;
	}
	return false;
}

// DOM Level 2 EventListener methods
void JSEventListener::handleEvent(DOM2Event const& pEvent)
{
	// Construct an event object which scripts can refer to,
	// and dispatch it as a parameter of JavaScript event handling function.
	DOM2Event* pDOM2Event = const_cast<DOM2Event*>(&pEvent);
	ViewerEvent* pViewerEvent = dynamic_cast<ViewerEvent*> (pDOM2Event);
	if (pViewerEvent)
	{
		ViewerEventWrapper * pEventWrapper = new ViewerEventWrapper(mptl, *pDOM2Event);
		if (pEventWrapper)
		{
			IDispatch* pDisp = 0;
			pEventWrapper->QueryInterface(IID_IDispatch, (void**)&pDisp);
			if (pDisp)
			{
				DispatchHelper	dispatcher(mpListener);
				dispatcher.CreateParameters(1);
				dispatcher.AddParameter(0, pDisp);
				dispatcher.Invoke(0);

				pDisp->Release();
			}

			pEventWrapper->Release();
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////
ViewerEventWrapper::ViewerEventWrapper(ITypeLib* ptl, DOM2Event & pEvent)
{
	SetEvent(pEvent);

	LoadTypeInfo(ptl);
}

ViewerEventWrapper::~ViewerEventWrapper()
{
}

STDMETHODIMP ViewerEventWrapper::get_id(BSTR* value)
{
	ViewerEvent* pViewerEvent = dynamic_cast<ViewerEvent*>(mpEvent);
	if (!pViewerEvent)
		return DISP_E_MEMBERNOTFOUND;

	CString strID(pViewerEvent->GetID().c_str());
	*value = strID.AllocSysString();
	return S_OK;
}


////////////////////////////////////////////////////////////////////////////////////
EventTargetWrapper::EventTargetWrapper(ITypeLib* ptl, DOM2EventTarget* pTarget)
	: mpTarget(pTarget)
{
	LoadTypeInfo(ptl);
}

EventTargetWrapper::~EventTargetWrapper()
{
}

// --- IDOM2EventTarget methods ---
STDMETHODIMP EventTargetWrapper::addEventListener(BSTR type, IDispatch* pListener, BOOL useCapture)
{
	if (mpTarget)
	{
		JSEventListener* pJSListener = new JSEventListener(m_ptl, type, pListener, useCapture);
		mpTarget->addEventListener(pJSListener->getEventType(), *pJSListener, pJSListener->getUseCapture());
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP EventTargetWrapper::removeEventListener(BSTR type, IDispatch* pListener, BOOL useCapture)
{
	if (mpTarget)
	{
		JSEventListener jsListener(m_ptl, type, pListener, useCapture);
		mpTarget->removeEventListener(jsListener.getEventType(), jsListener, jsListener.getUseCapture());
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP EventTargetWrapper::dispatchEvent(IDispatch* pEvent, BOOL* value)
{
	*value = false;

	if (mpTarget)
	{
		ViewerEventWrapper* pEventWrapper = static_cast<ViewerEventWrapper*>(pEvent);
		DOM2Event* pEvent = pEventWrapper->GetEvent();
		*value = mpTarget->dispatchEvent(*pEvent);
		return S_OK;
	}
	return E_FAIL;
}
