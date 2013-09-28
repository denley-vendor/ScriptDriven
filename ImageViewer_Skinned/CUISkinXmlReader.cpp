//
// CUISkinXmlReader.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CUISkinXmlReader.h"
#include "MiscUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char * kNode_MainWindow	= "MainWindow";
const char * kNode_ImageBox		= "ImageBox";
const char * kNode_Button		= "Button";

const char * kAttr_Version		= "version";
const char * kAttr_Skin			= "skin";
const char * kAttr_Name			= "name";
const char * kAttr_XPos			= "xpos";
const char * kAttr_YPos			= "ypos";
const char * kAttr_Width		= "width";
const char * kAttr_Height		= "height";

// Singleton instance
CUISkinXmlReader* CUISkinXmlReader::mInstance = 0;
////////////////////////////////////////////////////////////////////////////
CUISkinXmlReader::CUISkinXmlReader() 
	: mXmlManager(0)
	, mMainWindowNode(0)
	, mCurrentButton(0)
{
}

CUISkinXmlReader::~CUISkinXmlReader()
{
	Finalize();
}

void CUISkinXmlReader::CreateInstance()
{
	if (mInstance == 0)
	{
		mInstance = new CUISkinXmlReader();

		CString filePath = MiscUtils::GetSkinFolder() + "\\ImageViewer.xml";
		mInstance->SetXmlFile(filePath);
	}
}

void CUISkinXmlReader::DeleteInstance()
{
	if (mInstance)
	{
		delete mInstance;
		mInstance = 0;
	}
}

CUISkinXmlReader* CUISkinXmlReader::GetInstance()
{
	if (mInstance == 0)
	{
		CreateInstance();
	}
	return mInstance;
}

void CUISkinXmlReader::Finalize()
{
	if (mCurrentButton)
	{
		mCurrentButton->Release();
		mCurrentButton = 0;
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

bool CUISkinXmlReader::SetXmlFile(const char * inFile)
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

CString CUISkinXmlReader::GetMainWindowSkin()
{
	if (mMainWindowNode)
	{
		return mXmlManager->GetNodeAttr(mMainWindowNode, kAttr_Skin);
	}
	return CString("");
}

bool CUISkinXmlReader::FindButton(const char* inName)
{
	// Release the previous used button.
	if (mCurrentButton)
	{
		mCurrentButton->Release();
		mCurrentButton = NULL;
	}

	if (mMainWindowNode)
	{
		IXMLDOMNode * pNode = mXmlManager->GetNodeFirstChild(mMainWindowNode);
		while (pNode && !mCurrentButton)
		{
			// Check:	1. Is it a <Button /> node?
			//			2. Does the button node have the specified name?
			CString strName = mXmlManager->GetNodeAttr(pNode, kAttr_Name);
			if (mXmlManager->GetNodeName(pNode) == kNode_Button && 
				strName.Compare(inName) == 0)
			{
				mCurrentButton = pNode;
			}
			else
			{
				IXMLDOMNode* pNextNode = mXmlManager->GetNodeNextSibling(pNode);
				pNode->Release();
				pNode = pNextNode;
			}
		}
	}

	return (mCurrentButton != NULL);
}

// Get attributes of the current button
CString CUISkinXmlReader::GetName()
{
	return GetAttribute(kAttr_Name);
}

CString CUISkinXmlReader::GetSkin()
{
	return GetAttribute(kAttr_Skin);
}

long CUISkinXmlReader::GetXPos()
{
	CString strValue = GetAttribute(kAttr_XPos);
	if (strValue != "")
	{
		return atol(strValue);
	}
	return -1;
}

long CUISkinXmlReader::GetYPos()
{
	CString strValue = GetAttribute(kAttr_YPos);
	if (strValue != "")
	{
		return atol(strValue);
	}
	return -1;
}

long CUISkinXmlReader::GetWidth()
{
	CString strValue = GetAttribute(kAttr_Width);
	if (strValue != "")
	{
		return atol(strValue);
	}
	return -1;
}

long CUISkinXmlReader::GetHeight()
{
	CString strValue = GetAttribute(kAttr_Height);
	if (strValue != "")
	{
		return atol(strValue);
	}
	return -1;
}

void CUISkinXmlReader::GetPosition(long& xpos, long& ypos, long& width, long& height)
{
	xpos = GetXPos();
	ypos = GetYPos();
	width  = GetWidth();
	height = GetHeight();
}

// Preview Window for the image
bool CUISkinXmlReader::GetImageBox(long& xpos, long& ypos, long& width, long& height)
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
