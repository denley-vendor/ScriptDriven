//
// CUISkinXmlReader.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CUISkinXmlReader__
#define __H_CUISkinXmlReader__

/*
<?xml version="1.0" encoding="UTF-8"?>
<ImageViewer version="1.1">
	<MainWindow skin="main.jpg">
		<Button name="btn_Open" xpos="10" ypos="10" width="30" height="20" skin="btn_Open.jpg"/>
		<Button name="btn_Reload" xpos="50" ypos="10" width="30" height="20" skin="btn_Reload.jpg"/>
	</MainWindow>
</ImageViewer>
*/

#include "CXmlWrapper.h"

class CUISkinXmlReader
{
private:
	CString			mXmlFile;
	CXmlWrapper *	mXmlManager;

	IXMLDOMNode *	mMainWindowNode;
	IXMLDOMNode *	mCurrentButton;

	// Singleton instance
	static CUISkinXmlReader*	mInstance;

public:
	CUISkinXmlReader();
	virtual ~CUISkinXmlReader();

	// Singleton methods
	static void CreateInstance();
	static void DeleteInstance();
	static CUISkinXmlReader* GetInstance();

	bool SetXmlFile(const char * inFile);
	CString GetMainWindowSkin();
	
	bool FindButton(const char* inName);
	// Button attributes
	CString GetName();
	CString GetSkin();
	long GetXPos();
	long GetYPos();
	long GetWidth();
	long GetHeight();
	void GetPosition(long& xpos, long& ypos, long& width, long& height);
	
	// Preview Window for the image
	bool GetImageBox(long& xpos, long& ypos, long& width, long& height);

protected:
	// Get attributes of the current button
	inline CString GetAttribute(const char * inTag)
	{
		if (mCurrentButton)
		{
			return mXmlManager->GetNodeAttr(mCurrentButton, inTag);
		}
		return CString("");
	}

	void Finalize();
};

#endif // __H_CUISkinXmlReader__