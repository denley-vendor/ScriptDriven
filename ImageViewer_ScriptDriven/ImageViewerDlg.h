// ImageViewerDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "CSkinDialog.h"
#include "DOM2EventTarget.h"
#include "CScriptButton.h"
#include "CEasyHyperlink.h"
#include <list>

class ImageProcessor;
class CMyJScriptHost;
class DispatchHelper;

typedef std::list<CScriptButton*> ButtonList;
typedef std::list<CWnd*> StaticList;

// CImageViewerDlg dialog
class CImageViewerDlg : public CSkinDialog, public DOM2EventTarget
{
	typedef CSkinDialog INHERITED;

// Construction
public:
	CImageViewerDlg(CWnd* pParent = NULL);	// standard constructor
	~CImageViewerDlg();

	// Load the skin for the main window.
	virtual void LoadDialogImages();

// Dialog Data
//	enum { IDD = IDD_IMAGEVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	CString		mImageFile;
	ImageProcessor*	mProcessor;
	CMyJScriptHost*	mScriptHost;
	DispatchHelper*	mCallbackHelper;

	// For global access!
	static CImageViewerDlg*	sMainApplication;

	void DisplayImage(void);
	void ParseUISkinXml(void);

	virtual bool isSameListener(EventListenerStruct& pListener1, EventListenerStruct& pListener2);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:

private:
	CStatic			mPictureWnd;
	CEasyHyperlink*	mpHyperlink;
	ButtonList		mButtons;
	StaticList		mStatics;

public:
	// Exposed APIs
	bool GetImageFile(CString& filename);
	bool SaveImageFile(CString& filename);
	void MinimizeMainWindow();
	void ExitApplication();
	void SetApplicationLoadedHandler(DispatchHelper* pHandler);
	void ShowAboutDialog();
	void RefreshDisplay();

	ImageProcessor* GetImageProcessor() { return mProcessor;}
	ButtonList& GetButtonList() { return mButtons; }

	// For global access!
	static CImageViewerDlg*	GetAppInstance();
};

// For global access!
extern CImageViewerDlg* GetMainApp();
