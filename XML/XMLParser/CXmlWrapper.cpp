//
// CXmlWrapper.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CXmlWrapper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////////////////
CXmlWrapper::CXmlWrapper() : mDocNode(0), mRootNode(0)
{
	// CXmlWrapper serves as xml writer...
	HRESULT hr = CoCreateInstance(__uuidof(DOMDocument), NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IXMLDOMDocument), (void**)&mDocNode);
	if (SUCCEEDED(hr))
	{
		// Create a root node.
		mRootNode = CreateNode("myRoot");
		if (mRootNode)
		{
			SetNodeAttr(mRootNode, "specVersion", 0.1);
			InsertNode(mDocNode, mRootNode);
		}
	}
}

CXmlWrapper::CXmlWrapper(const char * inXmlFile) : mDocNode(0), mRootNode(0)
{
	// CXmlWrapper serves as xml reader...
	LoadFile(inXmlFile);
}

CXmlWrapper::~CXmlWrapper()
{
	Clean();
}

void CXmlWrapper::Clean()
{
	if (mRootNode)
	{
		mRootNode->Release();
		mRootNode = 0;
	}
	if (mDocNode)
	{
		mDocNode->Release();
		mDocNode = 0;
	}
}

bool CXmlWrapper::LoadFile(const char * inXmlFile)
{
	Clean();
	SetFileName(inXmlFile);

	HRESULT hr = CoCreateInstance(__uuidof(DOMDocument), NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IXMLDOMDocument), (void**)&mDocNode);
	if (!SUCCEEDED(hr) || !mDocNode)
	{
		Clean();
		return false;
	}

	VARIANT_BOOL     vt_b;
	hr = mDocNode->load(_variant_t(mXmlFile), &vt_b);
	if (!SUCCEEDED(hr) || !vt_b)
	{
		Clean();
		return false;
	}

	// Retrieve the root node.
	IXMLDOMElement * pElem = 0;
	hr = mDocNode->get_documentElement(&pElem);
	if (!SUCCEEDED(hr) || !pElem)
	{
		Clean();
		return false;
	}
	if (!Element2Node(&mRootNode, pElem))
	{
		Clean();
		pElem->Release();
		return false;
	}

	pElem->Release();
	return true;
}

bool CXmlWrapper::SaveFile()
{
	// If xml file does not exist, here create one first.
	HANDLE hFile = CreateFile(mXmlFile,            // The filename
							GENERIC_WRITE,         // File access
							(DWORD) 0,             // Share access
							NULL,                  // Security
							OPEN_ALWAYS,           // Open flags
							(DWORD) 0,             // More flags
							NULL);                 // Template
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
	}

	if (mDocNode)
	{
		HRESULT hr = mDocNode->save(_variant_t(mXmlFile));
		return SUCCEEDED(hr);
	}
	return false;
}

bool CXmlWrapper::SaveFile(const char * inXmlFile)
{
	SetFileName(inXmlFile);
	return SaveFile();
}

bool CXmlWrapper::SetFileName(const char * inXmlFile)
{
	mXmlFile = inXmlFile;
	return true;
}

bool CXmlWrapper::IsReady()
{
	if (mRootNode && mDocNode)
		return true;
	return false;
}

IXMLDOMNode* CXmlWrapper::CreateNode(BSTR inNodeName, DOMNodeType inNodeType)
{
	if (inNodeType == NODE_TEXT)
	{
		IXMLDOMText * t = NULL;
		mDocNode->createTextNode(inNodeName, &t);
		return t;
	}
	else if (inNodeType == NODE_CDATA_SECTION)
	{
		IXMLDOMCDATASection * t = NULL;
		mDocNode->createCDATASection(inNodeName, &t);
		return t;
	}
	else if (inNodeType == NODE_PROCESSING_INSTRUCTION)
	{
		_bstr_t bstr1("xml");
		IXMLDOMProcessingInstruction * ins = NULL;
		mDocNode->createProcessingInstruction(bstr1, inNodeName, &ins);
		return ins;
	}
	else
	{
		ASSERT(inNodeType == NODE_ELEMENT);

		VARIANT var;
		var.vt     = VT_I4;
		V_I4(&var) = (int)inNodeType;

		IXMLDOMNode * node = NULL;
 		mDocNode->createNode(var, inNodeName, NULL, &node);
		return node;
	}
}

IXMLDOMNode* CXmlWrapper::CreateNode(const char * inNodeName, DOMNodeType inNodeType)
{
	IXMLDOMNode * node = NULL;

	CString nodeName(inNodeName);
	BSTR bstrName = nodeName.AllocSysString();
	if (bstrName)
	{
		node = CreateNode(bstrName, inNodeType);
		::SysFreeString(bstrName);
	}
	return node;
}

bool CXmlWrapper::InsertNode(IXMLDOMNode * inParentNode, IXMLDOMNode * inSonNode)
{
	IXMLDOMNode * tmp = NULL;
	HRESULT hr = inParentNode->appendChild(inSonNode, &tmp);
	if (tmp)
	{
		tmp->Release();
	}
	return SUCCEEDED(hr);
}

bool CXmlWrapper::RemoveNode(IXMLDOMNode * inParentNode, IXMLDOMNode * inSonNode)
{
	IXMLDOMNode * tmp = NULL;
	HRESULT hr = inParentNode->removeChild(inSonNode, &tmp);
	if (tmp)
	{
		tmp->Release();
	}
	return SUCCEEDED(hr);
}

// Insert a node under root, which named inNodeName and has inNodeText as text
bool CXmlWrapper::InsertNodeEx(const char * inNodeName, const char * inNodeText, IXMLDOMNode * inParentNode /*= NULL*/)
{
	IXMLDOMNode* pNode = CreateNode(inNodeName, NODE_ELEMENT);
	if (!pNode)
	{
		return false;
	}	

	if (!SetNodeText(pNode, inNodeText))
	{
		pNode->Release();
		return false;
	}

	if (!InsertNode(mRootNode, pNode))
	{
		pNode->Release();
		return false;
	}
	return true;
}

CString CXmlWrapper::GetNodeAttr(IXMLDOMNode * inNode, const char * inAttr)
{
	IXMLDOMElement* pEle = NULL;
	if (!Node2Element(&pEle, inNode))
	{
		return CString("");
	}

	_variant_t vt;
	CString attrName(inAttr), strValue("");
	BSTR bstrName = attrName.AllocSysString();
	if (pEle->getAttribute(bstrName, &vt) == S_OK)
	{
		strValue = CString(vt.bstrVal);
	}
	pEle->Release();
	::SysFreeString(bstrName); 
	return strValue;
}

bool CXmlWrapper::SetNodeAttr(IXMLDOMNode* inNode, const char * inAttr, _variant_t vVal)
{
	IXMLDOMElement* pEle = NULL;
	if (!Node2Element(&pEle, inNode))
	{
		return false;
	}

	CString attrName(inAttr);
	BSTR bstrName = attrName.AllocSysString();
	HRESULT hr = pEle->setAttribute(bstrName, vVal);
	pEle->Release();
	::SysFreeString(bstrName); 
	return SUCCEEDED(hr);
}

bool CXmlWrapper::SetNodeAttr(IXMLDOMNode* inNode, const char * inAttr, char * szVal)
{
	return SetNodeAttr(inNode, inAttr, _variant_t(szVal));
}

bool CXmlWrapper::SetNodeAttr(IXMLDOMNode* inNode, const char * inAttr, const char * szVal)
{
	return SetNodeAttr(inNode, inAttr, (char *)szVal);
}

bool CXmlWrapper::SetNodeAttr(IXMLDOMNode* inNode, const char * inAttr, double dblVal)
{
	VARIANT var;
	var.vt     = VT_R8;
	var.dblVal = dblVal;
	return SetNodeAttr(inNode, inAttr, var);
}

bool CXmlWrapper::SetNodeAttr(IXMLDOMNode* inNode, const char * inAttr, long nVal)
{
	VARIANT var;
	var.vt     = VT_I4;
	var.intVal = nVal;
	return SetNodeAttr(inNode, inAttr, var);
}

bool CXmlWrapper::SetNodeAttr(IXMLDOMNode* inNode, const char * inAttr, unsigned int nVal)
{
	return SetNodeAttr(inNode, inAttr, (long)nVal);
}

bool CXmlWrapper::SetNodeAttr(IXMLDOMNode* inNode, const char * inAttr, int nVal)
{
	return SetNodeAttr(inNode, inAttr, (long)nVal);
}

bool CXmlWrapper::SetNodeAttr(IXMLDOMNode* inNode, const char * inAttr, unsigned long nVal)
{
	return SetNodeAttr(inNode, inAttr, (long)nVal);
}

CString CXmlWrapper::GetNodeName(IXMLDOMNode * inNode)
{
	CString strName("");
	if (inNode)
	{	
		BSTR bstrName = 0;
		inNode->get_nodeName(&bstrName);
		if (bstrName)
		{
			strName = CString(bstrName);
			SysFreeString(bstrName); 
		}
	}
	return strName;
}

bool CXmlWrapper::SetNodeText(IXMLDOMNode* inNode, const char * inText)
{
	IXMLDOMNode * pNode = CreateNode(inText, NODE_TEXT);
	if (pNode)
	{
		if (InsertNode(inNode, pNode))
		{
			return true;
		}
	}
	return false;
}

CString CXmlWrapper::GetNodeText(IXMLDOMNode* inNode)
{
	IXMLDOMElement* pEle = NULL;
	if (!Node2Element(&pEle, inNode))
	{
		return CString("");
	}

	CString strText("");
	BSTR bstrText = 0;
	if (pEle->get_text(&bstrText) == S_OK)
	{
		strText = CString(bstrText);
	}
	SysFreeString(bstrText); 
	pEle->Release();
	return strText;
}

CString CXmlWrapper::GetNodeTextEx(const char * inNodeName, IXMLDOMNode * inParentNode)
{
	if (!inParentNode)
	{
		inParentNode = mRootNode;
	}

	IXMLDOMNode * pNode = GetNodeSubNode(inParentNode, inNodeName);
	return GetNodeText(pNode);
}

IXMLDOMNode* CXmlWrapper::GetNodeFirstChild(IXMLDOMNode * inNode)
{
	if (!inNode)
	{
		mRootNode->AddRef();
		return mRootNode;
	}

	IXMLDOMNode * pNode = NULL;
	inNode->get_firstChild(&pNode);
	return pNode;
}

IXMLDOMNode* CXmlWrapper::GetNodeNextSibling(IXMLDOMNode * inNode)
{
	IXMLDOMNode * pNode = NULL;
	if (inNode)
	{
		inNode->get_nextSibling(&pNode);
	}
	return pNode;
}

IXMLDOMNode* CXmlWrapper::GetNodeLastChild(IXMLDOMNode * inNode)
{
	IXMLDOMNode * pNode = NULL;
	if (inNode)
	{
		inNode->get_lastChild(&pNode);
	}
	return pNode;
}

IXMLDOMNode* CXmlWrapper::GetNodePreviousSibling(IXMLDOMNode * inNode)
{
	IXMLDOMNode * pNode = NULL;
	if (inNode)
	{
		inNode->get_previousSibling(&pNode);
	}
	return pNode;
}

bool CXmlWrapper::GetNodeSubNodeList(DOMNodeList * outNodeList,
									 IXMLDOMNode * inParentNode, 
									 const char * inWhatName)
{
	bool ret = false;
	IXMLDOMNodeList * tmpList = NULL;
	if (SUCCEEDED(inParentNode->get_childNodes(&tmpList)))
	{
		IXMLDOMNode * pNode = NULL;
		while ((tmpList->nextNode(&pNode) == S_OK) && pNode)
		{
			if ((!inWhatName) || (GetNodeName(pNode) == inWhatName))
			{
				outNodeList->Add(pNode);
				ret = true;
			}
		}

		tmpList->Release();
	}
	return ret;
}

IXMLDOMNode* CXmlWrapper::GetNodeSubNode(IXMLDOMNode * inParentNode, const char * inWhatName)
{
	IXMLDOMNodeList * tmpList = NULL;
	if (SUCCEEDED(inParentNode->get_childNodes(&tmpList)))
	{
		IXMLDOMNode * pNode = NULL;
		while (tmpList->nextNode(&pNode) == S_OK)
		{
			if (GetNodeName(pNode) == inWhatName)
			{
				tmpList->Release();
				return pNode;
			}
		}

		tmpList->Release();
	}
	return NULL;
}
