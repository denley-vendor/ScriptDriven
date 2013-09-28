//
// CSkinDialog.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CSkinDialog__
#define __H_CSkinDialog__

#include "CSkinObject.h"

class CSkinDialog : public CDialog, public CSkinObject
{
protected:
	bool			mInitializing;
	bool			mDragging;
	CPoint			mCursorPos;
	
public:
	virtual void LoadDialogImages() = 0;

	void MoveDialog(CPoint& inCursorPos);
	
// Construction
public:
	CSkinDialog(UINT nIDTemplate, CWnd * pParentWnd = NULL);
	CSkinDialog(LPCTSTR lpszTemplateName, CWnd * pParentWnd = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinDialog)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSkinDialog();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnNcDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct) ;
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __H_CSkinDialog__
