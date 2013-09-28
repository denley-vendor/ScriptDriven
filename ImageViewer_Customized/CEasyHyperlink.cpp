//
// CEasyHyperlink.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "stdafx.h"
#include "CEasyHyperlink.h"
#include "shellapi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define STATIC_HYPER_WINDOW_CLASS "STATIC_HYPER"

////////////////////////////////////////////////////////////////////////////
CEasyHyperlink::_autoinitializer::_autoinitializer() : hModule(NULL)
{
	// First, let's register the window class
	WNDCLASS hc;
	hc.style = 0;
	hc.lpfnWndProc = (WNDPROC)CEasyHyperlink::WndProc;
	hc.cbClsExtra = 0;
	hc.cbWndExtra = sizeof(CEasyHyperlink*);
	hc.hInstance = NULL;
	hc.hIcon = NULL;
	hc.hCursor = NULL;
	hc.hbrBackground = NULL;
	hc.lpszMenuName = NULL;
	hc.lpszClassName = STATIC_HYPER_WINDOW_CLASS;
	RegisterClass(&hc);

	// Now, try to find a hand icon...
	CEasyHyperlink::handcursor = ::LoadCursor(NULL, MAKEINTRESOURCE(32649)); //32649 == IDC_HAND
	if (CEasyHyperlink::handcursor == NULL)
	{
		TCHAR szWindowsDir[MAX_PATH];
		GetWindowsDirectory(szWindowsDir ,MAX_PATH);
		strcat(szWindowsDir,"\\winhlp32.exe");
		hModule = LoadLibrary(szWindowsDir);		
		if (hModule) 
			CEasyHyperlink::handcursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
	}
}

CEasyHyperlink::_autoinitializer::~_autoinitializer()
{
	if (hModule != NULL)
		FreeLibrary(hModule);
}


CEasyHyperlink::_autoinitializer CEasyHyperlink::__autoinitializer;
HCURSOR CEasyHyperlink::handcursor = NULL;
////////////////////////////////////////////////////////////////////////////
CEasyHyperlink::CEasyHyperlink() : m_hWnd(NULL)
{
}

CEasyHyperlink::~CEasyHyperlink()
{
	if (m_hWnd)
		::DestroyWindow(m_hWnd);
}


void CEasyHyperlink::ScreenToClient(HWND hwnd, RECT& rect) const
{
	POINT pos;
	pos.x = rect.left;
	pos.y = rect.top;		
	::ScreenToClient(hwnd, &pos);
	rect.left = pos.x; 
	rect.top  = pos.y;

	pos.x = rect.right;
	pos.y = rect.bottom;
	::ScreenToClient(hwnd, &pos);
	rect.right  = pos.x;
	rect.bottom = pos.y;
}

bool CEasyHyperlink::create(int resourceid, HWND parent)
{
	HWND old = ::GetDlgItem(parent, resourceid);
	if (old != NULL)
	{ 
		char url[256];
		::GetWindowText(old, url, sizeof(url));

		// GetWindowRect return bounding box in screen coordinates.
		RECT rect;
		::GetWindowRect(old, &rect);

		// calculate them down to client coordinates of the according dialog box...
		ScreenToClient(parent, rect);

		// finally, destroy the old label
		if (create(rect, url, parent))
			::DestroyWindow(old);
	}
	return (m_hWnd != NULL);
}

bool CEasyHyperlink::create(RECT& rect, const char *url, HWND parent)
{
	if (url != NULL)
		m_Url = url;

	m_hWnd = ::CreateWindow( STATIC_HYPER_WINDOW_CLASS, m_Url.c_str(), WS_CHILD | WS_VISIBLE, 
									rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 
									parent, NULL, NULL, NULL);

	::SetWindowLong( m_hWnd, GWL_USERDATA, (LONG)this);

	return (m_hWnd != NULL);
}

bool CEasyHyperlink::create(int x1, int y1, int x2, int y2, const char *url, HWND parent)
{
	RECT rect; 
	rect.left = x1;
	rect.top = y1;
	rect.right = x2;
	rect.bottom = y2;
	return create(rect, url, parent);
}


int CEasyHyperlink::WndProc(HWND hwnd, WORD wMsg, WPARAM wParam, LPARAM lParam)
{
	CEasyHyperlink *hl = (CEasyHyperlink*)GetWindowLong(hwnd, GWL_USERDATA);

	switch (wMsg)  
	{
	case WM_LBUTTONDOWN:
		if (((UINT)::ShellExecute(NULL, _T("open"), hl->m_Url.c_str(), NULL, NULL, SW_SHOWNORMAL)) <= 32)
			MessageBeep(0);
		break;

	case WM_MOUSEMOVE:
		break;

	case WM_PAINT:
		{
			HDC hDC; PAINTSTRUCT ps;
			hDC = ::BeginPaint(hwnd, &ps);
			if (hl == NULL)
				return 0;

			RECT rect;
			::GetClientRect(hwnd, &rect);

			HFONT font = ::CreateFont( 16, //height
										7, //average char width
										0, //angle of escapement
										0, //base-line orientation angle
										FW_NORMAL,	//font weight
										FALSE,		//italic
										TRUE,		//underline
										FALSE,		//strikeout
										ANSI_CHARSET,			//charset identifier
										OUT_DEFAULT_PRECIS,		//ouput precision
										CLIP_DEFAULT_PRECIS,	//clipping precision
										DEFAULT_QUALITY,	//output quality
										DEFAULT_PITCH,			//pitch and family
										"Arial");
				
			::SelectObject(hDC, font);
			::SetTextColor(hDC, RGB(0,0,200));
			::SetBkMode(hDC, TRANSPARENT);
			::DrawText(hDC, hl->m_Url.c_str(), int(hl->m_Url.length()), &rect, DT_VCENTER | DT_CENTER);
			::DeleteObject(font);

			::EndPaint(hwnd, &ps);
						
			return TRUE;
		}

		case WM_SETCURSOR:
		{
			if (CEasyHyperlink::handcursor)
				::SetCursor(CEasyHyperlink::handcursor);
			break;
		}

		default:
			DefWindowProc(hwnd, wMsg, wParam, lParam);
	}

	return TRUE;
}
