//
// CUISkinXmlParser.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CUISkinXmlParser.h"
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
const char * kAttr_Function		= "function";
const char * kAttr_ID			= "id";

// Singleton instance
CUISkinXmlParser* CUISkinXmlParser::mInstance = 0;
////////////////////////////////////////////////////////////////////////////
CUISkinXmlParser::CUISkinXmlParser() 
	: mXmlManager(0)
	, mMainWindowNode(0)
	, mCurrentNode(0)
{
}

CUISkinXmlParser::~CUISkinXmlParser()
{
	Finalize();
}

void CUISkinXmlParser::CreateInstance()
{
	if (mInstance == 0)
	{
		mInstance = new CUISkinXmlParser();

		CString filePath = MiscUtils::GetSkinFolder() + "\\myViewer.xml";
		mInstance->SetXmlFile(filePath);
	}
}

void CUISkinXmlParser::DeleteInstance()
{
	if (mInstance)
	{
		delete mInstance;
		mInstance = 0;
	}
}

CUISkinXmlParser* CUISkinXmlParser::GetInstance()
{
	if (mInstance == 0)
	{
		CreateInstance();
	}
	return mInstance;
}

void CUISkinXmlParser::Finalize()
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

bool CUISkinXmlParser::SetXmlFile(const char * inFile)
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

CString CUISkinXmlParser::GetMainWindowSkin()
{
	if (mMainWindowNode)
	{
		return mXmlManager->GetNodeAttr(mMainWindowNode, kAttr_Skin);
	}
	return CString("");
}

bool CUISkinXmlParser::FindFirstNode()
{
	if (mMainWindowNode)
	{
		// Release the current node.
		if (mCurrentNode)
		{
			mCurrentNode->Release();
			mCurrentNode = 0;
		}

		mCurrentNode = mXmlManager->GetNodeFirstChild(mMainWindowNode);
		return (mCurrentNode != NULL);
	}
	return false;
}

bool CUISkinXmlParser::FindNextNode()
{
	if (mCurrentNode)
	{
		IXMLDOMNode* pNode = mXmlManager->GetNodeNextSibling(mCurrentNode);
		mCurrentNode->Release();
		mCurrentNode = pNode;
		return (mCurrentNode != NULL);
	}
	return false;
}

bool CUISkinXmlParser::FindButton(const char* inName)
{
	// Release the previous used button.
	if (mCurrentNode)
	{
		mCurrentNode->Release();
		mCurrentNode = NULL;
	}

	if (mMainWindowNode)
	{
		IXMLDOMNode * pNode = mXmlManager->GetNodeFirstChild(mMainWindowNode);
		while (pNode && !mCurrentNode)
		{
			// Check:	1. Is it a <Button /> node?
			//			2. Does the button node have the specified name?
			CString strName = mXmlManager->GetNodeAttr(pNode, kAttr_Name);
			if (mXmlManager->GetNodeName(pNode) == kNode_Button && 
				strName.Compare(inName) == 0)
			{
				mCurrentNode = pNode;
			}
			else
			{
				IXMLDOMNode* pNextNode = mXmlManager->GetNodeNextSibling(pNode);
				pNode->Release();
				pNode = pNextNode;
			}
		}
	}

	return (mCurrentNode != NULL);
}

// Get attributes of the current node
CString CUISkinXmlParser::GetNodeType()
{
	if (mCurrentNode)
	{
		return mXmlManager->GetNodeName(mCurrentNode);
	}
	return CString("");
}

NODE_TYPE CUISkinXmlParser::GetNodeTypeNumber()
{
	CString strType = GetNodeType();
	if (strType.Compare(kNode_ImageBox) == 0)
	{
		return NT_ImageBox;
	}
	else if (strType.Compare(kNode_Button) == 0) 
	{
		return NT_Button;
	}
	else if (strType.Compare(kNode_Static) == 0)
	{
		return NT_Static;
	}
	else if (strType.Compare(kNode_Hyperlink) == 0)
	{
		return NT_Hyperlink;
	}
	else
	{
		return NT_Unknown;
	}
}

CString CUISkinXmlParser::GetName()
{
	return GetAttribute(kAttr_Name);
}

CString CUISkinXmlParser::GetSkin()
{
	return GetAttribute(kAttr_Skin);
}

long CUISkinXmlParser::GetXPos()
{
	CString strValue = GetAttribute(kAttr_XPos);
	if (strValue != "")
	{
		return atol(strValue);
	}
	return -1;
}

long CUISkinXmlParser::GetYPos()
{
	CString strValue = GetAttribute(kAttr_YPos);
	if (strValue != "")
	{
		return atol(strValue);
	}
	return -1;
}

long CUISkinXmlParser::GetWidth()
{
	CString strValue = GetAttribute(kAttr_Width);
	if (strValue != "")
	{
		return atol(strValue);
	}
	return -1;
}

long CUISkinXmlParser::GetHeight()
{
	CString strValue = GetAttribute(kAttr_Height);
	if (strValue != "")
	{
		return atol(strValue);
	}
	return -1;
}

void CUISkinXmlParser::GetPosition(long& xpos, long& ypos, long& width, long& height)
{
	xpos = GetXPos();
	ypos = GetYPos();
	width  = GetWidth();
	height = GetHeight();
}

CString CUISkinXmlParser::GetFunction()
{
	return GetAttribute(kAttr_Function);
}

// Preview Window for the image
bool CUISkinXmlParser::GetImageBox(long& xpos, long& ypos, long& width, long& height)
{
	if (mMainWindowNode)
	{
		bool found = false;
		IXMLDOMNode * pNode = mXmlManager->GetNodeFirstChild(mMainWindowNode);
		while (pNode && !found)
		{
			if (mXmlManager->GetNodeName(pNode) == kNode_ImageBox)
			{
				found = true;
			}
			else
			{
				IXMLDOMNode* pNextNode = mXmlManager->GetNodeNextSibling(pNode);
				pNode->Release();
				pNode = pNextNode;
			}
		}

		// If we found a <ImageBox /> node, read its attributes out.
		if (found)
		{
			CString strXPos = mXmlManager->GetNodeAttr(pNode, kAttr_XPos);
			CString strYPos = mXmlManager->GetNodeAttr(pNode, kAttr_YPos);
			CString strWidth  = mXmlManager->GetNodeAttr(pNode, kAttr_Width);
			CString strHeight = mXmlManager->GetNodeAttr(pNode, kAttr_Height);
			pNode->Release();

			xpos = atol(strXPos);
			ypos = atol(strYPos);
			width  = atol(strWidth);
			height = atol(strHeight);
			return true;
		}
	}
	return false;
}
