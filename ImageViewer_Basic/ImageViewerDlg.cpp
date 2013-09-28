// ImageViewerDlg.cpp : implementation file
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "ImageViewer.h"
#include "ImageViewerDlg.h"
#include ".\imageviewerdlg.h"
#include "safe_defs.h"
#include "ImageProcessor.h"
#include "MiscUtils.h"

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


// CImageViewerDlg dialog



CImageViewerDlg::CImageViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageViewerDlg::IDD, pParent)
	, mImageFile("")
	, mProcessor(new ImageProcessor())
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CImageViewerDlg::~CImageViewerDlg()
{
	SAFE_DELETE(mProcessor);
}

void CImageViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SOURCE_FILE, mEditImageFile);
	DDX_Control(pDX, IDC_PICTURE, mPictureWnd);
}

BEGIN_MESSAGE_MAP(CImageViewerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, OnBnClickedButtonAbout)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_INVERT, OnBnClickedButtonInvert)
	ON_BN_CLICKED(IDC_BUTTON_OVERLAY_LOGO, OnBnClickedButtonOverlayLogo)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD, OnBnClickedButtonReload)
	ON_BN_CLICKED(IDC_BUTTON_OVERLAY_SYSTIME, OnBnClickedButtonOverlaySystime)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN, OnBnClickedButtonClean)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_AS, OnBnClickedButtonSaveAs)
	ON_BN_CLICKED(IDC_BUTTON_GREYSCALE, OnBnClickedButtonGreyscale)
END_MESSAGE_MAP()


// CImageViewerDlg message handlers

BOOL CImageViewerDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CImageViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CImageViewerDlg::OnPaint() 
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
		CDialog::OnPaint();

		DisplayImage();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CImageViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CImageViewerDlg::OnBnClickedButtonAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CImageViewerDlg::OnBnClickedButtonOpen()
{
	CString imageFile;
	if (MiscUtils::BrowseImageFile(imageFile))
	{
		mImageFile = imageFile;
		mEditImageFile.SetWindowText(mImageFile);

		if (mProcessor->LoadFile(mImageFile))
		{
			DisplayImage();
		}
	}
}

void CImageViewerDlg::OnBnClickedButtonSaveAs()
{
	CString destFile;
	if (MiscUtils::SaveAsFile(destFile))
	{
		if (mProcessor)
		{
			mProcessor->SaveFile(destFile);
		}
	}
}

void CImageViewerDlg::OnBnClickedButtonReload()
{
	if (mProcessor)
	{
		mProcessor->Reload();
		DisplayImage();
	}
}

void CImageViewerDlg::OnBnClickedButtonClean()
{
	if (mProcessor)
	{
		mProcessor->Clean();
		DisplayImage();
	}
}

void CImageViewerDlg::OnBnClickedButtonInvert()
{
	if (mProcessor)
	{
		mProcessor->Invert();
		DisplayImage();
	}
}

void CImageViewerDlg::OnBnClickedButtonGreyscale()
{
	if (mProcessor)
	{
		mProcessor->Greyscale();
		DisplayImage();
	}
}

void CImageViewerDlg::OnBnClickedButtonOverlayLogo()
{
	if (mProcessor)
	{
		CString logoFile = MiscUtils::GetAppFolder() + "\\logo.gif";
		mProcessor->OverlayLogo(0, 0, logoFile);
		DisplayImage();
	}
}

void CImageViewerDlg::OnBnClickedButtonOverlaySystime()
{
	if (mProcessor)
	{
		char* szTime = "0000-00-00(00:00:00)";
		SIZE txtSize;
		::GetTextExtentPoint32(::GetWindowDC(NULL), szTime, (int)strlen(szTime), &txtSize);

		long startX = 0;
		long StartY = 0;
		long imageWidth, imageHeight;
		if (mProcessor->GetInfo(&imageWidth, &imageHeight))
		{
			startX = imageWidth - txtSize.cx;
		}

		mProcessor->OverlaySystemTime(startX, StartY);
		DisplayImage();
	}
}

void CImageViewerDlg::DisplayImage(void)
{
	if (mProcessor)
	{
		mProcessor->Display(&mPictureWnd);
	}
}
