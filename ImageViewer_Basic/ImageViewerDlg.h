// ImageViewerDlg.h : header file
//

#pragma once
#include "afxwin.h"

class ImageProcessor;

// CImageViewerDlg dialog
class CImageViewerDlg : public CDialog
{
// Construction
public:
	CImageViewerDlg(CWnd* pParent = NULL);	// standard constructor
	~CImageViewerDlg();

// Dialog Data
	enum { IDD = IDD_IMAGEVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	CString		mImageFile;
	ImageProcessor*	mProcessor;

	void DisplayImage(void);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAbout();
	afx_msg void OnBnClickedButtonOpen();
	CEdit mEditImageFile;
	CStatic mPictureWnd;
	afx_msg void OnBnClickedButtonInvert();
	afx_msg void OnBnClickedButtonOverlayLogo();
	afx_msg void OnBnClickedButtonReload();
	afx_msg void OnBnClickedButtonOverlaySystime();
	afx_msg void OnBnClickedButtonClean();
	afx_msg void OnBnClickedButtonSaveAs();
	afx_msg void OnBnClickedButtonGreyscale();
};
