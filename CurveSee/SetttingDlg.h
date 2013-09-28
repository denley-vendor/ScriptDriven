#pragma once
#include "afxwin.h"

// CSetttingDlg dialog

class CSetttingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetttingDlg)

public:
	CSetttingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetttingDlg();

// Dialog Data
	enum { IDD = IDD_DLG_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CEdit mEditXMax;
	CEdit mEditYMax;
	CComboBox mComboEngines;
	CEdit mEditExpression;
	afx_msg void OnBnClickedOk();
};
