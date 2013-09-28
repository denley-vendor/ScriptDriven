// CurveSeeDlg.cpp : implementation file
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CurveSee.h"
#include "CurveSeeDlg.h"
#include ".\curveseedlg.h"
#include "SetttingDlg.h"
#include "CDataCurve.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCurveSeeDlg dialog



CCurveSeeDlg::CCurveSeeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCurveSeeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCurveSeeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCurveSeeDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_FUNCTIONS_SETTINGS, OnFunctionsSettings)
	ON_COMMAND(ID_FUNCTIONS_REPAINT, OnFunctionsRepaint)
	ON_COMMAND(ID_FUNCTIONS_ABOUT, OnFunctionsAbout)
	ON_COMMAND(ID_FUNCTIONS_EXIT, OnFunctionsExit)
END_MESSAGE_MAP()


// CCurveSeeDlg message handlers

BOOL CCurveSeeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCurveSeeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCurveSeeDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		DrawXYAxes(dc);
		DrawCurve(dc);

		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCurveSeeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCurveSeeDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// Popup a menu
	CMenu menu;
	menu.LoadMenu(IDR_MENU_FUNC);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, pWnd);
}

void CCurveSeeDlg::OnFunctionsSettings()
{
	CSetttingDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		// Reset the previous curve data
		GetSettings()->ResetCurveData();

		mScriptHost.UseScriptEngine(GetSettings()->mEngineType);
		mScriptHost.Initialize();
		
		double strideX = 2 * GetSettings()->mXMax / POINT_COUNT;
		if (GetSettings()->mEngineType == "JScript")
		{
			for (long i = 0; i < POINT_COUNT; i++)
			{
				/************************************************************
				JavaScript codes:

				i = 8;
				x = -6.0;
				y = Math.sin(x);
				Result(i, x, y);
				*************************************************************/

				CString scripts;
				double x = i * strideX - GetSettings()->mXMax;
				scripts.Format("i=%d;\nx=%.5f;\ny=Math.%s;\nResult(i,x,y);",
					i, x, GetSettings()->mExpression);

				mScriptHost.Execute(scripts);
			}
		}
		else
		{
			for (long i = 0; i < POINT_COUNT; i++)
			{
				/************************************************************
				VBScript codes:

				i = 8
				x = -6.0
				y = sin(x)
				call Result(i, x, y)
				*************************************************************/

				CString scripts;
				double x = i * strideX - GetSettings()->mXMax;
				scripts.Format("i=%d\r\nx=%.5f\r\ny=%s\r\ncall Result(i,x,y)",
					i, x, GetSettings()->mExpression);

				mScriptHost.Execute(scripts);
			}
		}

		Repaint();
	}
}

void CCurveSeeDlg::Repaint()
{
	CPaintDC dc(this);
	DrawXYAxes(dc);
	DrawCurve(dc);

	Invalidate();
}

void CCurveSeeDlg::OnFunctionsRepaint()
{
	Repaint();
}

void CCurveSeeDlg::OnFunctionsAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CCurveSeeDlg::OnFunctionsExit()
{
	CDialog::OnOK();
}

void CCurveSeeDlg::DrawXYAxes(CPaintDC& dc)
{
	RECT rc;
	GetClientRect(&rc);
	long centerX = rc.left + (rc.right - rc.left) / 2;
	long centerY = rc.top + (rc.bottom - rc.top) / 2;

	// Draw X-axis
	dc.MoveTo(rc.left, centerY);
	dc.LineTo(rc.right, centerY);

	// Draw Y-axis
	dc.MoveTo(centerX, rc.top);
	dc.LineTo(centerX, rc.bottom);
}

void CCurveSeeDlg::DrawCurve(CPaintDC& dc)
{
	RECT rc;
	GetClientRect(&rc);
	long centerX = rc.left + (rc.right - rc.left) / 2;
	long centerY = rc.top + (rc.bottom - rc.top) / 2;

	COLORREF clr = RGB(255, 0, 0); // color for curve drawing

	double strideX = (rc.right - rc.left) / (2 * GetSettings()->mXMax);
	double strideY = (rc.bottom - rc.top) / (2 * GetSettings()->mYMax);
	for (long i = 0; i < POINT_COUNT; i++)
	{
		long xPos = (long)(centerX + strideX * GetSettings()->mXValues[i]);
		long yPos = (long)(centerY - strideY * GetSettings()->mYValues[i]);

		dc.SetPixel(xPos, yPos, clr);
	}
}
