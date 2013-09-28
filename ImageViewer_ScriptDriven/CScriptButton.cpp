//
// CScriptButton.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CScriptButton.h"
#include "DOM2EventTarget.h"
#include "ViewerEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define kElementBaseID 8800

UINT CScriptButton::sButtonCount = 0;
///////////////////////////////////////////////////////////////////////////////////////////////
CScriptButton::CScriptButton(DOM2EventTarget* pTarget, CString strID, bool bStatic)
	: mTarget(pTarget)
	, mID(strID)
{
	if (bStatic)
	{
		// This is a special button. Its function is: 
		//	simply display an image.
		mStateCount = 1; 
	}
}

CScriptButton::~CScriptButton()
{
}

void CScriptButton::CreateSelf(CWnd* pParent)
{
	UINT ctrlID = kElementBaseID + sButtonCount;
	++sButtonCount;
	Create(mID, WS_CHILD|WS_VISIBLE, CRect(0,0,1,1), pParent, ctrlID);

	LoadButtonImages();
}

void CScriptButton::SetSkinFile(CString const& inFile)
{
	mSkinFileName = inFile;
}

void CScriptButton::SetSkinPosition(long xpos, long ypos)
{
	mSkinXPos = xpos;
	mSkinYPos = ypos;
}

void CScriptButton::TakeClickAction()
{
	if (mTarget)
	{
		ViewerEvent clickEvent("ev_ButtonClicked", (LPCSTR)mID);
		mTarget->dispatchEvent(clickEvent);

#ifdef _DEBUG
	//	::AfxMessageBox("Button <" + mID + "> clicked.");
#endif 
	}
}