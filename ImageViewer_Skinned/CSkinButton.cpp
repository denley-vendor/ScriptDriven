//
// CSkinButton.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CSkinButton.h"
#include <windows.h>
#include <wingdi.h>
#include "CUISkinXmlReader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
CSkinButton::CSkinButton() 
	: m_bActAsTab(false)
	, m_bIsCancel(false)
	, mCaptureRegion(0)
{
	mStateCount = 4; // A botton has 4 status.

	mRgn.CreateRectRgn(0,0,2,2);
}

CSkinButton::~CSkinButton()
{	
	if (mCaptureRegion)
	{
		delete mCaptureRegion;
		mCaptureRegion = 0;
	}
	if (mRgn.m_hObject)
	{
		mRgn.DeleteObject();
	}
}

void CSkinButton::SetActAsTab(bool TabAct)
{
	m_bActAsTab = TabAct;
}

void CSkinButton::SetAsCancel(bool CancelAct)
{
    m_bIsCancel = CancelAct;
}

void CSkinButton::LoadImageMap(CRgn& outRegion)
{
	CSkinObject::LoadImageMap(outRegion);

	ModifyStyle(0, BS_OWNERDRAW);
}

void CSkinButton::ReadXMLSettings()
{
	// Locate the specified tag in the XML file.
	CUISkinXmlReader* pReader = CUISkinXmlReader::GetInstance();
	if (pReader->FindButton(mTagName))
	{
		mSkinFileName = pReader->GetSkin();
		pReader->GetPosition(mSkinXPos, mSkinYPos, mSkinWidth, mSkinHeight);
	}
}

void CSkinButton::LoadButtonImages()
{
	CString buttonText;
	GetWindowText(buttonText);
	SetTagName(buttonText);

	CRgn buttonRegion;
	LoadImageMap(buttonRegion);
	mRgn.CopyRgn(&buttonRegion);
	if ((HRGN)buttonRegion)
	{
		RECT buttonRect;
		buttonRegion.GetRgnBox(&buttonRect);
		SetWindowRgn((HRGN)buttonRegion, TRUE);
	//	RECT buttonRect2;
	//	buttonRegion.GetRgnBox(&buttonRect2);

		MoveWindow(mSkinXPos, mSkinYPos, mSkinWidth, mSkinHeight);
	}
}

// Attention: inSkinFileName is only file name, NOT full path!
void CSkinButton::LoadButtonImages(CString inSkinFileName)
{
	mSkinFileName = inSkinFileName; //change the bitmap file name

//	RECT rc;
//	GetWindowRect(&rc);
//	mSkinWidth  = rc.right - rc.left;
//	mSkinHeight = rc.bottom - rc.top;
	
	CRgn buttonRegion;
	LoadImageMap(buttonRegion);
	mRgn.CopyRgn(&buttonRegion);
	if ((HRGN)buttonRegion)
	{
		RECT buttonRect;
		buttonRegion.GetRgnBox(&buttonRect);
		SetWindowRgn((HRGN)buttonRegion, TRUE);
	}
}

int CSkinButton::GetDrawIndex(bool inSelected, bool inFocused, bool inDisabled)
{
	int imageIndex = 0;

	if (inSelected)
	{
		imageIndex = 2;
	}
	else if (GetCapture() == this)	// mouse over
	{
		imageIndex = 1;
	}
	else if (inDisabled)
	{
		imageIndex = 3;   // last image for disabled
	}
	return imageIndex;
}


BEGIN_MESSAGE_MAP(CSkinButton, CButton)
	//{{AFX_MSG_MAP(CSkinButton)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_CAPTURECHANGED()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_SHOWWINDOW()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinButton message handlers

int CSkinButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CSkinButton::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CPoint currentPoint;
	currentPoint.x = point.x;
	currentPoint.y = point.y;
	ClientToScreen(&currentPoint);

	CButton::OnRButtonUp(nFlags, point);
}

BOOL CSkinButton::PreTranslateMessage(MSG* pMsg)
{
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);
	
	return CButton::PreTranslateMessage(pMsg);
}

void CSkinButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	UINT state = lpDrawItemStruct->itemState;
	int imageIndex = GetDrawIndex((state & ODS_SELECTED) ? true : false,
		(state & ODS_FOCUS) ? true : false, (state & ODS_DISABLED) ? true : false);

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CDC* pImageDC = CDC::FromHandle(mImageProcessor.GetImageDC());
    if (mImageProcessor.IsReady())
		TransparentBitBlt(pDC,0,0,mSkinWidth,mSkinHeight,pImageDC,mSkinWidth *imageIndex,0);
}

void CSkinButton::TransparentBitBlt(CDC* pdc,int x,int y,int width,int height,CDC * mMemoryDC,int srcX,int srcY)
{
	pdc->SelectClipRgn(&mRgn);
	pdc->BitBlt(x,y,width,height,mMemoryDC,srcX,srcY,SRCCOPY);
	pdc->SelectClipRgn(0);
}

BOOL CSkinButton::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style ^= BS_OWNERDRAW;
	cs.style |= BS_OWNERDRAW;

	return CButton::PreCreateWindow(cs);
}

void CSkinButton::OnMouseMove(UINT nFlags, CPoint point) 
{	
	if (m_bIsCancel)
	{
		CWnd * pParent = GetParent();
		if (pParent != 0)
			pParent->SetFocus();
	}

	CButton::OnMouseMove(nFlags, point);
	if (nFlags & MK_LBUTTON)
		return ;
    CaptureMouseMove(point);   
}

BOOL CSkinButton::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CSkinButton::InitToolTip ()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		m_ToolTip.Create(this);
		m_ToolTip.Activate(FALSE);
	}
}

void CSkinButton::SetTooltipText(CString* spText, BOOL bActivate)
{
    if (spText == NULL) 
		return;

	InitToolTip();

	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, (LPCTSTR) *spText, rectBtn, 1);
	}
	m_ToolTip.UpdateTipText((LPCTSTR)*spText, this, 1);
	m_ToolTip.Activate(bActivate);
}

void CSkinButton::ActivateTooltip(BOOL bActivate)
{
	if (m_ToolTip.GetToolCount() == 0)
		return;

	m_ToolTip.Activate(bActivate);
}

void CSkinButton::OnCaptureChanged(CWnd *pWnd) 
{
	CButton::OnCaptureChanged(pWnd);
}

void CSkinButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CButton::OnLButtonUp(nFlags, point);
	ReleaseCapture();
//	SetFocus();
//	RedrawWindow();
}

void CSkinButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CButton::OnLButtonDown(nFlags, point);

	Invalidate(FALSE);
	UpdateWindow();
	if (m_bActAsTab)
	    PostMessage(WM_LBUTTONUP, 0, MAKELPARAM(point.x, point.y));
}

void CSkinButton::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CButton::OnShowWindow(bShow, nStatus);

	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	CaptureMouseMove(point);
}

void CSkinButton::CaptureMouseMove(CPoint inPoint)
{
	if (GetCapture() == this && mCaptureRegion)
	{
		if (mCaptureRegion->PtInRegion(inPoint) && IsWindowVisible())
		{		
		}
		else
		{
			ReleaseCapture();			
			Invalidate(FALSE);
			UpdateWindow();
		}
	}
	else
	{
		if (IsWindowVisible())
		{
			if (!mCaptureRegion)
			{
				mCaptureRegion = new CRgn();
				mCaptureRegion->CreateRectRgn(0, 0, mSkinWidth, mSkinHeight);
			}

			GetWindowRgn((HRGN)mCaptureRegion);
			SetCapture();
			Invalidate(FALSE);
			UpdateWindow();
		}
	}
}