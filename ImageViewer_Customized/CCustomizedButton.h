//
// CCustomizedButton.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CCustomizedButton__
#define __H_CCustomizedButton__

#include "CSkinButton.h"
#include "ActionDispatchIF.h"

class CCustomizedButton : public CSkinButton
{
public:
	CCustomizedButton(ActionDispatchIF* pDispatch, CString strFunc, bool bStatic = false);
	~CCustomizedButton();

	void CreateSelf(CWnd* pParent);
	virtual void TakeClickAction();

	void SetSkinFile(CString const& inFile);
	void SetSkinPosition(long xpos, long ypos);

private:
	ActionDispatchIF*	mDispatch;
	CString				mFuncName;
	IMAGE_FUNCTION		mFunction;

	IMAGE_FUNCTION ConvertToFunctionNumber(CString const & strFunc);
};

#endif // __H_CCustomizedButton__