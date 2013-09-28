//
// CScriptButton.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CScriptButton__
#define __H_CScriptButton__

#include "CSkinButton.h"

class DOM2EventTarget;

class CScriptButton : public CSkinButton
{
public:
	CScriptButton(DOM2EventTarget* pTarget, CString strID, bool bStatic = false);
	~CScriptButton();

	void CreateSelf(CWnd* pParent);
	virtual void TakeClickAction();

	void SetSkinFile(CString const& inFile);
	void SetSkinPosition(long xpos, long ypos);

	CString GetID() { return mID; }

private:
	DOM2EventTarget*	mTarget;
	CString				mID;

	static UINT sButtonCount;
};

#endif // __H_CScriptButton__