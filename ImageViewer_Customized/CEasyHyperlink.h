//
// CEasyHyperlink.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CEasyHyperlink__
#define __H_CEasyHyperlink__

#include <string>

class CEasyHyperlink  
{
	class _autoinitializer
	{
	public:
		_autoinitializer();
		~_autoinitializer();

	protected:
		HMODULE hModule;
	};

	friend class _autoinitializer;

public:
	CEasyHyperlink();
	virtual ~CEasyHyperlink();

	bool create(int resourceid, HWND parent);
	bool create(RECT& rect, const char *url, HWND parent);
	bool create(int x1, int y1, int x2, int y2, const char *url, HWND parent);
	void ScreenToClient(HWND hwnd, RECT& rect) const;

protected:
	std::string		m_Url;
	HWND			m_hWnd;

	static _autoinitializer __autoinitializer;
	static HCURSOR handcursor;
	static int WndProc(HWND hwnd, WORD wMsg, WPARAM wParam, LPARAM lParam);
};

#endif // __H_CEasyHyperlink__
