//
// CMediaPlayer.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CMediaPlayer.h"
#include "CDebugger.h"

/////////////////////////////////////////////////////////////////////////////////////////
CMediaPlayer::CMediaPlayer(ITypeLib* ptl) : mCurrentPosition(0)
{
	LoadTypeInfo(ptl);
}

CMediaPlayer::~CMediaPlayer()
{
}

// APIs exposed to scripts
STDMETHODIMP CMediaPlayer::get_majorVersion(UINT *pVal)
{
	*pVal = 1;
	return S_OK;
}

STDMETHODIMP CMediaPlayer::get_minorVersion(UINT *pVal)
{
	*pVal = 0;
	return S_OK;
}

STDMETHODIMP CMediaPlayer::get_vendor(BSTR *pVal)
{
	*pVal = ::SysAllocString(L"HQ Tech");
	return S_OK;
}

STDMETHODIMP CMediaPlayer::get_currentPosition(UINT *pVal)
{
	*pVal = mCurrentPosition;
	return S_OK;
}

STDMETHODIMP CMediaPlayer::put_currentPosition(UINT pVal)
{
	mCurrentPosition = pVal;
	wchar_t wszInfo[100];
	swprintf(wszInfo, L"Seek to a new position: %d.", pVal);
	GetDebugger()->trace(L"Function", wszInfo);
	return S_OK;
}

STDMETHODIMP CMediaPlayer::play(BSTR mediaFile)
{
	wchar_t wszInfo[300];
	swprintf(wszInfo, L"MediaPlayer::play( %s )", mediaFile);
	GetDebugger()->trace(L"Function", wszInfo);
	return S_OK;
}

STDMETHODIMP CMediaPlayer::pauseOn()
{
	GetDebugger()->trace(L"Function", L"MediaPlayer::pauseOn");
	return S_OK;
}

STDMETHODIMP CMediaPlayer::pauseOff()
{
	GetDebugger()->trace(L"Function", L"MediaPlayer::pauseOff");
	return S_OK;
}

STDMETHODIMP CMediaPlayer::stop()
{
	GetDebugger()->trace(L"Function", L"MediaPlayer::stop");
	return S_OK;
}