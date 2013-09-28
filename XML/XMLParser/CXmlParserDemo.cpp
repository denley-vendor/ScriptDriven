//
// CXmlParserDemo.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CXmlParserDemo.h"
#include "MiscUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char * kNode_MainWindow	= "MainWindow";
const char * kNode_ImageBox		= "ImageBox";
const char * kNode_Button		= "Button";
const char * kNode_Static		= "Static";
const char * kNode_Hyperlink	= "Hyperlink";

const char * kAttr_Version		= "version";
const char * kAttr_Skin			= "skin";
const char * kAttr_Name			= "name";
const char * kAttr_XPos			= "xpos";
const char * kAttr_YPos			= "ypos";
const char * kAttr_Width		= "width";
const char * kAttr_Height		= "height";

////////////////////////////////////////////////////////////////////////////
CXmlParserDemo::CXmlParserDemo() 
	: mXmlManager(0)
	, mMainWindowNode(0)
	, mCurrentNode(0)
{
}

CXmlParserDemo::~CXmlParserDemo()
{
	Finalize();
}

void CXmlParserDemo::Finalize()
{
	if (mCurrentNode)
	{
		mCurrentNode->Release();
		mCurrentNode = 0;
	}
	if (mMainWindowNode)
	{
		mMainWindowNode->Release();
		mMainWindowNode = 0;
	}
	if (mXmlManager)
	{
		delete mXmlManager;
		mXmlManager = 0;
	}
}

bool CXmlParserDemo::SetXmlFile(const char * inFile)
{
	Finalize();

	mXmlFile = inFile;
	mXmlManager = new CXmlWrapper(inFile);
	if (!mXmlManager->IsReady())
		return false;

	IXMLDOMNode* pRoot = mXmlManager->GetNodeFirstChild(NULL);
	if (pRoot)
	{
		// Retrieve the product version.
		CString version = mXmlManager->GetNodeAttr(pRoot, kAttr_Version);

#ifdef _DEBUG
		CString nodeName = mXmlManager->GetNodeName(pRoot);
#endif 

		// Get the first node under the root.
		// That is 'MainWindow' node.
		mMainWindowNode = mXmlManager->GetNodeFirstChild(pRoot);

		pRoot->Release();

		if (mMainWindowNode)
		{
			return true;
		}
	}
	return false;
}

CString CXmlParserDemo::GetMainWindowSkin()
{
	if (mMainWindowNode)
	{
		return mXmlManager->GetNodeAttr(mMainWindowNode, kAttr_Skin);
	}
	return CString("");
}

bool CXmlParserDemo::FindFirstNode()
{
	if (mMainWindowNode)
	{
		if (mCurrentNode)
			mCurrentNode->Release();

		mCurrentNode = mXmlManager->GetNodeFirstChild(mMainWindowNode);
		return (mCurrentNode != NULL);
	}
	return false;
}

bool CXmlParserDemo::FindNextNode()
{
	if (mCurrentNode)
	{
		IXMLDOMNode* newNode = mXmlManager->GetNodeNextSibling(mCurrentNode);
		mCurrentNode->Release();
		mCurrentNode = newNode;
		return (mCurrentNode != NULL);
	}
	return false;
}

bool CXmlParserDemo::FindButton(const char* inName)
{
	if (mCurrentNode)
	{
		mCurrentNode->Release();
		mCurrentNode = NULL;
	}

	if (mMainWindowNode)
	{
		IXMLDOMNode * pChild = mXmlManager->GetNodeFirstChild(mMainWindowNode);
		while (pChild && !mCurrentNode)
		{
			CString strName = mXmlManager->GetNodeAttr(pChild, kAttr_Name);
			if (mXmlManager->GetNodeName(pChild).Compare(kNode_Button) == 0 &&
				strName.Compare(inName) == 0)
			{
				mCurrentNode = pChild;
			}
			else
			{
				IXMLDOMNode * pNext = mXmlManager->GetNodeNextSibling(pChild);
				pChild->Release();
				pChild = pNext;
			}
		}
	}

	return (mCurrentNode != NULL);
}

// Get attributes of the current node
CString CXmlParserDemo::GetNodeName()
{
	if (mCurrentNode)
	{
		return mXmlManager->GetNodeName(mCurrentNode);
	}
	return CString("");
}

CString CXmlParserDemo::GetName()
{
	return GetAttribute(kAttr_Name);
}

CString CXmlParserDemo::GetSkin()
{
	return GetAttribute(kAttr_Skin);
}

long CXmlParserDemo::GetXPos()
{
	CString strValue = GetAttribute(kAttr_XPos);
	if (strValue != "")
	{
		return atol(strValue);
	}
	return -1;
}

long CXmlParserDemo::GetYPos()
{
	CString strValue = GetAttribute(kAttr_YPos);
	if (strValue != "")
	{
		return atol(strValue);
	}
	return -1;
}

long CXmlParserDemo::GetWidth()
{
	CString strValue = GetAttribute(kAttr_Width);
	if (strValue != "")
	{
		return atol(strValue);
	}
	return -1;
}

long CXmlParserDemo::GetHeight()
{
	CString strValue = GetAttribute(kAttr_Height);
	if (strValue != "")
	{
		return atol(strValue);
	}
	return -1;
}

void CXmlParserDemo::GetPosition(long& xpos, long& ypos, long& width, long& height)
{
	xpos = GetXPos();
	ypos = GetYPos();
	width  = GetWidth();
	height = GetHeight();
}

CString CXmlParserDemo::FullPath(CString const& inFileName)
{
	return CString(MiscUtils::GetSkinFolder() + "\\" + inFileName);
}
