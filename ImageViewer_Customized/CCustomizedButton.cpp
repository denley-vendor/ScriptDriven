//
// CCustomizedButton.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CCustomizedButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////////////////////
CCustomizedButton::CCustomizedButton(ActionDispatchIF* pDispatch, CString strFunc, bool bStatic)
	: mDispatch(pDispatch)
	, mFuncName(strFunc)
	, mFunction(IF_Unknown)
{
	if (bStatic)
	{
		// This is a special button. Its function is: 
		//	simply display an image.
		mStateCount = 1; 
	}
	else
	{
		mFunction = ConvertToFunctionNumber(strFunc);
	}
}

CCustomizedButton::~CCustomizedButton()
{
}

void CCustomizedButton::CreateSelf(CWnd* pParent)
{
	UINT ctrlID = kElementBaseID + mFunction;
	Create(mFuncName, WS_CHILD|WS_VISIBLE, CRect(0,0,1,1), pParent, ctrlID);

	LoadButtonImages();
}

void CCustomizedButton::SetSkinFile(CString const& inFile)
{
	mSkinFileName = inFile;
}

void CCustomizedButton::SetSkinPosition(long xpos, long ypos)
{
	mSkinXPos = xpos;
	mSkinYPos = ypos;
}

void CCustomizedButton::TakeClickAction()
{
	if (mDispatch)
	{
		mDispatch->Invoke(mFunction);

#ifdef _DEBUG
	//	::AfxMessageBox(mFuncName + " is executed.");
#endif 
	}
}

IMAGE_FUNCTION CCustomizedButton::ConvertToFunctionNumber(CString const & strFunc)
{
	if (strFunc.Compare("Minimize") == 0)
	{
		return IF_Minimize;
	}
	else if (strFunc.Compare("Exit") == 0)
	{
		return IF_Exit;
	}
	else if (strFunc.Compare("Open") == 0)
	{
		return IF_Open;
	}
	else if (strFunc.Compare("Reload") == 0)
	{
		return IF_Reload;
	}
	else if (strFunc.Compare("Clean") == 0)
	{
		return IF_Clean;
	}
	else if (strFunc.Compare("Invert") == 0)
	{
		return IF_Invert;
	}
	else if (strFunc.Compare("Logo") == 0)
	{
		return IF_Logo;
	}
	else if (strFunc.Compare("Save") == 0)
	{
		return IF_Save;
	}
	else if (strFunc.Compare("SystemTime") == 0)
	{
		return IF_SystemTime;
	}
	else if (strFunc.Compare("About") == 0)
	{
		return IF_About;
	}
	else if (strFunc.Compare("Greyscale") == 0)
	{
		return IF_Greyscale;
	}
	else
	{
		return IF_Unknown;
	}
}