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
#include "CUISkinXmlParser.h"
#include "CMyJScriptHost.h"
#include "JSEventHandling.h"
#include "DispatchHelper.h"

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


// For global access!
CImageViewerDlg* CImageViewerDlg::sMainApplication = 0;
CImageViewerDlg* CImageViewerDlg::GetAppInstance() { return(sMainApplication); }
CImageViewerDlg* GetMainApp() { return CImageViewerDlg::GetAppInstance(); }

// CImageViewerDlg dialog
CImageViewerDlg::CImageViewerDlg(CWnd* pParent /*=NULL*/)
	: INHERITED(IDD_IMAGEVIEWER_DIALOG, pParent)
	, mImageFile("")
	, mProcessor(new ImageProcessor())
	, mScriptHost(new CMyJScriptHost())
	, mCallbackHelper(0)
	, mpHyperlink(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	sMainApplication = this; // For global access!

	// Create internal components.
	CUISkinXmlParser::CreateInstance();
}

CImageViewerDlg::~CImageViewerDlg()
{
	SAFE_DELETE(mProcessor);
	SAFE_DELETE(mCallbackHelper);
	SAFE_DELETE(mScriptHost);

	// Delete all buttons
	ButtonList::iterator itBegin(mButtons.begin()), itEnd(mButtons.end());
	for (ButtonList::iterator it = itBegin; it != itEnd; ++it)
	{
		if (*it)
		{
			delete (*it);
			*it = 0;
		}
	}

	// Delete all statics
	StaticList::iterator itBegin2(mStatics.begin()), itEnd2(mStatics.end());
	for (StaticList::iterator it = itBegin2; it != itEnd2; ++it)
	{
		if (*it)
		{
			delete (*it);
			*it = 0;
		}
	}

	SAFE_DELETE(mpHyperlink);

	// Delete internal components.
	CUISkinXmlParser::DeleteInstance();

	sMainApplication = 0;
}

void CImageViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	INHERITED::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImageViewerDlg, INHERITED)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CImageViewerDlg message handlers

BOOL CImageViewerDlg::OnInitDialog()
{
	INHERITED::OnInitDialog();
	SetWindowText("ImageViewer");

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

	///////////////////////////////////////////////////////////////////////
	ParseUISkinXml();

	// Send a notification to scripts: 'The application is loaded!'
	if (mCallbackHelper)
		mCallbackHelper->Invoke(0);

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
		INHERITED::OnSysCommand(nID, lParam);
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
		INHERITED::OnPaint();

		DisplayImage();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CImageViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CImageViewerDlg::ParseUISkinXml(void)
{
	CUISkinXmlParser* pParser = CUISkinXmlParser::GetInstance();

	bool found = pParser->FindFirstNode();
	while (found)
	{
		switch (pParser->GetNodeTypeNumber())
		{
		case NT_ImageBox:
			{
				long x, y, width, height;
				pParser->GetPosition(x, y, width, height);
				CRect rect(x, y, x+width, y+height);
				mPictureWnd.Create("", WS_CHILD|WS_BORDER|WS_VISIBLE|SS_BITMAP, rect, this, 1234);
			}
			break;

		case NT_Button:
			{
				CScriptButton* pButton = new CScriptButton(this, pParser->GetID());
				pButton->SetSkinFile(pParser->GetSkin());
				pButton->SetSkinPosition(pParser->GetXPos(), pParser->GetYPos());
				pButton->CreateSelf(this);
				mButtons.push_back(pButton);
			}
			break;

		case NT_Static:
			{
				CString strSkin = pParser->GetSkin();
				if (strSkin != "")
				{
					// Static with a bitmap
					CScriptButton* pButton = new CScriptButton(NULL, "", true);
					pButton->SetSkinFile(strSkin);
					pButton->SetSkinPosition(pParser->GetXPos(), pParser->GetYPos());
					pButton->CreateSelf(this);
					mStatics.push_back(pButton);
				}
				else
				{
					// TODO: Static with text
				}
			}
			break;

		case NT_Hyperlink:
			{
				SAFE_DELETE(mpHyperlink);
				mpHyperlink = new CEasyHyperlink();

				RECT rect;
				rect.left   = pParser->GetXPos();
				rect.top    = pParser->GetYPos();
				rect.right  = rect.left + pParser->GetWidth();
				rect.bottom = rect.top + pParser->GetHeight();
				mpHyperlink->create(rect, pParser->GetName(), this->GetSafeHwnd());
			}
			break;

		case NT_Script:
			{
				if (mScriptHost && mScriptHost->Initialize())
				{
					wchar_t * wszScripts = 0;
					CString jsSrcFile = MiscUtils::GetSkinFolder() + "\\" + pParser->GetSource();
					if (MiscUtils::LoadScriptFile(jsSrcFile, &wszScripts))
					{
						mScriptHost->Execute(wszScripts);
						delete[] wszScripts;
					}
				}
			}
			break;

		default:
			break;
		}

		found = pParser->FindNextNode();
	}
}

void CImageViewerDlg::LoadDialogImages()
{
	// Get the skin for the main window.
	CUISkinXmlParser* pParser = CUISkinXmlParser::GetInstance();
	mSkinFileName = pParser->GetMainWindowSkin();

	CRgn windowRegion;
	LoadImageMap(windowRegion);
	if ((HRGN)windowRegion)
	{
		SetWindowRgn((HRGN)windowRegion, TRUE);
	}
}

void CImageViewerDlg::DisplayImage(void)
{
	if (mProcessor && mPictureWnd.GetSafeHwnd())
	{
		mProcessor->Display(&mPictureWnd);
	}
}

bool CImageViewerDlg::isSameListener(EventListenerStruct& pListener1, EventListenerStruct& pListener2)
{
	if (DOM2EventTarget::isSameListener(pListener1, pListener2))
	{
		return true;
	}

	JSEventListener* pJSListener1 = dynamic_cast<JSEventListener*>(pListener1.listener);
	JSEventListener* pJSListener2 = dynamic_cast<JSEventListener*>(pListener2.listener);
	if (pJSListener1 && pJSListener1->isSame(pJSListener2))
	{
		return true;
	}
	return false;
}

// Exposed APIs
bool CImageViewerDlg::GetImageFile(CString& filename)
{
	return MiscUtils::BrowseImageFile(filename);
}

bool CImageViewerDlg::SaveImageFile(CString& filename)
{
	return MiscUtils::SaveAsFile(filename);
}

void CImageViewerDlg::MinimizeMainWindow()
{
	ShowWindow(SW_MINIMIZE);
}

void CImageViewerDlg::ExitApplication()
{
	INHERITED::OnOK();
}

void CImageViewerDlg::ShowAboutDialog()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CImageViewerDlg::RefreshDisplay()
{
	DisplayImage();
}

void CImageViewerDlg::SetApplicationLoadedHandler(DispatchHelper* pHandler)
{
	SAFE_DELETE(mCallbackHelper);
	mCallbackHelper = pHandler;
}