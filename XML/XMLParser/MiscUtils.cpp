//
// MiscUtils.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "MiscUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString MiscUtils::mAppPath = "";
//////////////////////////////////////////////////////////////////////////////////////////////////
bool MiscUtils::IsSupportedImageFile(const char * inFilename)
{
	CString  strFile = inFilename;
	int nIndex = strFile.ReverseFind('.');
	if (nIndex != -1)
	{
		CString  strExt = strFile.Right(strFile.GetLength() - nIndex);
		if (strExt.CompareNoCase(".bmp") == 0 ||
			strExt.CompareNoCase(".jpg") == 0 ||
			strExt.CompareNoCase(".jpeg") == 0 ||
			strExt.CompareNoCase(".gif") == 0)
		{
			return true;
		}
	}
	return false;
}

bool MiscUtils::IsFileExist(const char * inFilename)
{
	WIN32_FIND_DATA sWin32FindData;
	HANDLE hFindFile = FindFirstFile(inFilename, &sWin32FindData);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		FindClose(hFindFile);
		return true;
	}
	return false;
}

// NOT contain the back-slash!
CString& MiscUtils::GetAppFolder(void)
{
	if (mAppPath == "")
	{
		TCHAR appPath[1024];
		::GetModuleFileName(0, appPath, 1024);
		TCHAR * backSlash = strrchr(appPath, '\\');
		*backSlash = '\0';
		mAppPath = appPath;
	}
	return mAppPath;
}

CString MiscUtils::GetSkinFolder(void)
{
	return CString(GetAppFolder() + "\\Skin");
}

bool MiscUtils::BrowseImageFile(CString & outFilename)
{
	CString   strFilter = "JPEG File (*.jpg;*.jpeg)|*.jpg;*.jpeg|";
	strFilter += "GIF File (*.gif)|*.gif|";
	strFilter += "BMP File (*.bmp)|*.bmp|";
	strFilter += "All Files (*.*)|*.*|";
	CFileDialog dlgOpen(TRUE, NULL, NULL, OFN_HIDEREADONLY, strFilter, NULL);
	if (IDOK == dlgOpen.DoModal()) 
	{
		outFilename = dlgOpen.GetPathName();
		return true;
	}
	return false;
}

bool MiscUtils::SaveAsFile(CString & outFilename)
{
	CString   strFilter = "BMP File (*.bmp)|*.bmp|";
	CFileDialog dlgSave(FALSE, NULL, NULL, OFN_HIDEREADONLY, strFilter, NULL);
	if (IDOK == dlgSave.DoModal()) 
	{
		outFilename = dlgSave.GetPathName();
		if (-1 == outFilename.ReverseFind('.'))
		{
			outFilename += ".bmp";
		}
		return true;
	}
	return false;
}