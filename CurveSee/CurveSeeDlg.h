// CurveSeeDlg.h : header file
//

#pragma once

#include "CWinScriptHost.h"

// CCurveSeeDlg dialog
class CCurveSeeDlg : public CDialog
{
// Construction
public:
	CCurveSeeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CURVESEE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CWinScriptHost	mScriptHost;

	void DrawXYAxes(CPaintDC& dc);
	void DrawCurve(CPaintDC& dc);
	void Repaint();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnFunctionsSettings();
	afx_msg void OnFunctionsRepaint();
	afx_msg void OnFunctionsAbout();
	afx_msg void OnFunctionsExit();
};
