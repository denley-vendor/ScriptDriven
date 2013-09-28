//
// MiscUtils.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#pragma once

class MiscUtils
{
private:
	static CString     mAppPath;

public:
	static bool IsSupportedImageFile(const char * inFilename);
	static bool BrowseImageFile(CString & outFilename);
	static bool SaveAsFile(CString & outFilename);
	static bool IsFileExist(const char * inFilename);

	static CString& GetAppFolder(void);
	static CString GetSkinFolder(void);
};