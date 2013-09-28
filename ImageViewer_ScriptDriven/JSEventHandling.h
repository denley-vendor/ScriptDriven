//
// JSEventHandling.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_JSEventHandling__
#define __H_JSEventHandling__

#include "IDispatchImpl.h"
#include "ImageViewer_h.h"
#include "DOM2EventListener.h"
#include "DOM2Event.h"

class DOM2EventTarget;

class JSEventListener : public DOM2EventListener
{
public:
	JSEventListener(ITypeLib* ptl, BSTR type, IDispatch* pListener, BOOL useCapture);
	virtual ~JSEventListener();

	DOMString getEventType() { return mEventType; }
	void setEventType(DOMString const& inType) { mEventType = inType; }

	bool getUseCapture() { return mbUseCapture; }
	void setUseCapture(bool useCapture) { mbUseCapture = useCapture; } 

	bool isSame(JSEventListener* pListener2);

	// DOM Level 2 EventListener methods
	virtual void handleEvent(DOM2Event const& pEvent);

private:
	ITypeLib*		mptl;			// Type library.
	IDispatch*		mpListener;		// Use it to call JavaScript event handling method.

	DOMString		mEventType;
	bool			mbUseCapture;
};


////////////////////////////////////////////////////////////////////////////////////
class EventTargetWrapper : public IDispatchImpl<EventTargetWrapper
											, IDOM2EventTarget
											, &IID_IDOM2EventTarget>
{
public:
	EventTargetWrapper(ITypeLib* ptl, DOM2EventTarget* pTarget);
	~EventTargetWrapper();

	// --- IDOM2EventTarget methods ---
	STDMETHODIMP addEventListener(BSTR type, IDispatch* pListener, BOOL useCapture);
	STDMETHODIMP removeEventListener(BSTR type, IDispatch* pListener, BOOL useCapture);
	STDMETHODIMP dispatchEvent(IDispatch* pEvent, BOOL* value);

private:
	DOM2EventTarget*		mpTarget;
};


////////////////////////////////////////////////////////////////////////////////////
template<class DERIVED_CLASS, class BASE_ITF, const IID* BASE_IID>
class __declspec(novtable) DOM2EventImpl : public IDispatchImpl<DERIVED_CLASS, BASE_ITF, BASE_IID>
{
public:
	DOM2EventImpl() {};

	void SetEvent(DOM2Event & pEvent) { mpEvent = &pEvent; }
	DOM2Event* GetEvent() { return mpEvent; }


	// --- IDOM2Event methods ---
	STDMETHODIMP get_type(BSTR* value)
	{
		if (mpEvent)
		{
			CString str(mpEvent->getType().c_str());
			*value = str.AllocSysString();
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHODIMP get_target(IDispatch** ppDisp)
	{
		if (mpEvent)
		{
			EventTargetWrapper * pTarget = new EventTargetWrapper(m_ptl, mpEvent->getTarget());
			if (pTarget)
			{
				pTarget->QueryInterface(IID_IDispatch, (void**)ppDisp);
				pTarget->Release();
				return S_OK;
			}
		}
		return E_FAIL;		
	}

	STDMETHODIMP get_currentTarget(IDispatch** ppDisp)
	{
		if (mpEvent)
		{
			EventTargetWrapper * pTarget = new EventTargetWrapper(m_ptl, mpEvent->getCurrentTarget());
			if (pTarget)
			{
				pTarget->QueryInterface(IID_IDispatch, (void**)ppDisp);
				pTarget->Release();
				return S_OK;
			}
		}
		return E_FAIL;
	}

	STDMETHODIMP get_eventPhase(USHORT* value)
	{
		if (mpEvent)
		{
			*value = USHORT(mpEvent->getEventPhase());
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHODIMP get_bubbles(BOOL* value)
	{
		if (mpEvent)
		{
			*value = mpEvent->getBubbles() ? TRUE : FALSE;
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHODIMP get_cancelable(BOOL* value)
	{
		if (mpEvent)
		{
			*value = mpEvent->getCancelable() ? TRUE : FALSE;
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHODIMP get_timeStamp(DATE* value)
	{
		if (mpEvent)
		{
			// Need conversion here?
			*value = DATE(mpEvent->getTimeStamp());
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHODIMP stopPropagation()
	{
		if (mpEvent)
		{
			mpEvent->stopPropagation();
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHODIMP preventDefault()
	{
		if (mpEvent)
		{
			mpEvent->preventDefault();
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHODIMP initEvent(BSTR typeArg, BOOL canBubbleArg, BOOL cancelableArg)
	{
		if (mpEvent)
		{
			CString strType(typeArg);
			mpEvent->initEvent((LPCSTR)strType,
				canBubbleArg ? true : false,
				cancelableArg ? true : false);
			return S_OK;
		}
		return E_FAIL;
	}

	// value properties
	STDMETHODIMP get_CAPTURING_PHASE(USHORT* value)
	{
		*value = DOM2Event::CAPTURING_PHASE;
		return S_OK;
	}

	STDMETHODIMP get_AT_TARGET(USHORT* value)
	{
		*value = DOM2Event::AT_TARGET;
		return S_OK;
	}

	STDMETHODIMP get_BUBBLING_PHASE(USHORT* value)
	{
		*value = DOM2Event::BUBBLING_PHASE;
		return S_OK;
	}	

protected:
	DOM2Event*		mpEvent;
};


////////////////////////////////////////////////////////////////////////////////////
class ViewerEventWrapper : public DOM2EventImpl<ViewerEventWrapper
											, IViewerEvent
											, &IID_IViewerEvent>
{
public:
	ViewerEventWrapper(ITypeLib* ptl, DOM2Event & pEvent);
	~ViewerEventWrapper();

	STDMETHODIMP get_id(BSTR* value);
};

#endif // __H_JSEventHandling__