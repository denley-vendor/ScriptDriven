//
// CXmlParserDemo.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CXmlParserDemo__
#define __H_CXmlParserDemo__

/*
<?xml version="1.0" encoding="UTF-8"?>
<ImageViewer version="1.2">
	<MainWindow skin="main.jpg">
		<ImageBox xpos="20" ypos="50" width="582" height="446"/>
		<Button name="btn_Open" xpos="10" ypos="10" skin="btn_Open.jpg"/>
		<Button name="btn_Reload" xpos="50" ypos="10" skin="btn_Reload.jpg"/>
		<Hyperlink name="http://www.chery.cn" xpos="100" ypos="10"/>
	</MainWindow>
</ImageViewer>
*/

#include "CXmlWrapper.h"

class CXmlParserDemo
{
private:
	CString			mXmlFile;
	CXmlWrapper *	mXmlManager;

	IXMLDOMNode *	mMainWindowNode;
	IXMLDOMNode *	mCurrentNode;

public:
	CXmlParserDemo();
	virtual ~CXmlParserDemo();

	bool SetXmlFile(const char * inFile);
	CString GetMainWindowSkin();

	bool FindFirstNode();
	bool FindNextNode(); 
	bool FindButton(const char* inName);

	// Button attributes
	CString GetNodeName();
	CString GetName();
	CString GetSkin();
	long GetXPos();
	long GetYPos();
	long GetWidth();
	long GetHeight();
	void GetPosition(long& xpos, long& ypos, long& width, long& height);
	
	CString FullPath(CString const& inFileName);

protected:
	// Get attributes of the current button
	inline CString GetAttribute(const char * inTag)
	{
		if (mCurrentNode)
		{
			return mXmlManager->GetNodeAttr(mCurrentNode, inTag);
		}
		return CString("");
	}

	void Finalize();
};

#endif // __H_CXmlParserDemo__