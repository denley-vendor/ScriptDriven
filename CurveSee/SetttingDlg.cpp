// SetttingDlg.cpp : implementation file
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CurveSee.h"
#include "SetttingDlg.h"
#include "CDataCurve.h"
#include ".\setttingdlg.h"

// CSetttingDlg dialog

IMPLEMENT_DYNAMIC(CSetttingDlg, CDialog)
CSetttingDlg::CSetttingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetttingDlg::IDD, pParent)
{
}

CSetttingDlg::~CSetttingDlg()
{
}

void CSetttingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_X_MAX, mEditXMax);
	DDX_Control(pDX, IDC_EDIT_Y_MAX, mEditYMax);
	DDX_Control(pDX, IDC_COMBO_ENGINES, mComboEngines);
	DDX_Control(pDX, IDC_EDIT_EXPRESSION, mEditExpression);
}


BEGIN_MESSAGE_MAP(CSetttingDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CSetttingDlg message handlers
BOOL CSetttingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	mComboEngines.ResetContent();
	mComboEngines.AddString("JScript");
	mComboEngines.AddString("VBScript");

	CDataCurve* pSettings = GetSettings();
	if (pSettings)
	{
		mComboEngines.SelectString(-1, pSettings->mEngineType);

		CString temp;
		temp.Format("%.2f", pSettings->mXMax);
		mEditXMax.SetWindowText(temp);
		temp.Format("%.2f", pSettings->mYMax);
		mEditYMax.SetWindowText(temp);

		mEditExpression.SetWindowText(pSettings->mExpression);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSetttingDlg::OnBnClickedOk()
{
	// Save settings
	CDataCurve* pSettings = GetSettings();
	if (pSettings)
	{
		CString temp;
		mEditXMax.GetWindowText(temp);
		pSettings->mXMax = atof(temp);
		mEditYMax.GetWindowText(temp);
		pSettings->mYMax = atof(temp);
		mEditExpression.GetWindowText(pSettings->mExpression);

		int index = mComboEngines.GetCurSel();
		if (CB_ERR != index)
		{
			mComboEngines.GetLBText(index, pSettings->mEngineType);
		}
	}

	OnOK();
}
