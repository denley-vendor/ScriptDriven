//
// CSkinButton.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CSkinButton__
#define __H_CSkinButton__

#include "CSkinObject.h"

/********************************************************
usage:::
   CSkinButton SkinButton;
   SkinButton.Create("name",WS_CHILD,rect,this,ID_BUTTON);
   SkinButton.SetTooltipText("ToolBox");
*******************************************************/

class CSkinButton : public CButton, public CSkinObject
{
protected:
	CRgn            mRgn;
	CRgn *			mCaptureRegion;
	bool            m_bActAsTab;
    bool            m_bIsCancel;

public:
	void LoadButtonImages();
	void LoadButtonImages(CString inSkinFileName);
	virtual void ReadXMLSettings();

	virtual int GetDrawIndex(bool inSelected, bool inFocused, bool inDisabled);
    void CaptureMouseMove(CPoint inPoint);
	void SetActAsTab(bool TabAct = true);
	void SetAsCancel(bool CancelAct = true);

// Construction
public:
	CSkinButton();
   
// Attributes
public:
	///tool tip routine
	void InitToolTip();
	CToolTipCtrl m_ToolTip;

	void SetTooltipText(CString* spText, BOOL bActivate = TRUE);
	void ActivateTooltip(BOOL bEnable = TRUE);
	virtual void LoadImageMap(CRgn& outRegion);
	
	void TransparentBitBlt(CDC* pdc,int x,int y,int width,int height,CDC * mMemoryDC,int srcX,int srcY);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSkinButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinButton)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __H_CSkinButton__

