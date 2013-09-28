//
// CUISkinXmlParser.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CUISkinXmlParser__
#define __H_CUISkinXmlParser__

/*
<?xml version="1.0" encoding="UTF-8"?>
<ImageViewer version="1.2">
	<MainWindow skin="main.jpg">
		<ImageBox xpos="20" ypos="85" width="582" height="446"/>
		<Button name="btn_Open" function="Open" xpos="21" ypos="40" skin="btn_Open.jpg"/>
		<Hyperlink name="http://www.chery.cn" xpos="400" ypos="6" width="160" height="20"/>
	</MainWindow>
</ImageViewer>
*/

#include "CXmlWrapper.h"

typedef enum NODE_TYPE
{
	NT_Unknown = 0,
	NT_ImageBox,
	NT_Button,
	NT_Static,
	NT_Hyperlink,
	NT_Script
} NODE_TYPE;

class CUISkinXmlParser
{
private:
	CString			mXmlFile;
	CXmlWrapper *	mXmlManager;

	IXMLDOMNode *	mMainWindowNode;
	IXMLDOMNode *	mCurrentNode;

	// Singleton instance
	static CUISkinXmlParser*	mInstance;

public:
	CUISkinXmlParser();
	virtual ~CUISkinXmlParser();
	
	// Singleton methods
	static void CreateInstance();
	static void DeleteInstance();
	static CUISkinXmlParser* GetInstance();

	bool SetXmlFile(const char * inFile);
	CString GetMainWindowSkin();

	// Loop through all nodes
	bool FindFirstNode();
	bool FindNextNode(); 
	bool FindButton(const char* inName);

	// Node attributes
	CString GetNodeType();
	NODE_TYPE GetNodeTypeNumber();

	CString GetName();
	CString GetSkin();
	long GetXPos();
	long GetYPos();
	long GetWidth();
	long GetHeight();
	void GetPosition(long& xpos, long& ypos, long& width, long& height);
	CString GetFunction();
	CString GetID();
	CString GetSource();
	
	// Preview Window for the image
	bool GetImageBox(long& xpos, long& ypos, long& width, long& height);

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

#endif // __H_CUISkinXmlParser__