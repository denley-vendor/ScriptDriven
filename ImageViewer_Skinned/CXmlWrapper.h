//
// CXmlWrapper.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CXmlWrapper__
#define __H_CXmlWrapper__

#import <msxml.dll>
#include <afxtempl.h>

typedef CArray<IXMLDOMNode*, IXMLDOMNode*> DOMNodeList;

class CXmlWrapper
{
public:
	CXmlWrapper();
	CXmlWrapper(const char * inXmlFile);
	virtual ~CXmlWrapper();

public:
	bool LoadFile(const char * inXmlFile);
	bool SaveFile();
	bool SaveFile(const char * inXmlFile);
	bool IsReady();
	CString GetFileName() { return mXmlFile; }

public:
	//node: create, insert and remove
	IXMLDOMNode* CreateNode(BSTR inNodeName, DOMNodeType inNodeType = NODE_ELEMENT);
	IXMLDOMNode* CreateNode(const char * inNodeName, DOMNodeType inNodeType = NODE_ELEMENT);
	bool RemoveNode(IXMLDOMNode * inParentNode, IXMLDOMNode * inSonNode);
	bool InsertNode(IXMLDOMNode * inParentNode, IXMLDOMNode * inSonNode);
	bool InsertNodeEx(const char * inNodeName, const char * inNodeText, 
		IXMLDOMNode * inParentNode = NULL);//for one-level xml file easy to use

	//attributes
	CString GetNodeAttr(IXMLDOMNode * inNode, const char * inAttr);
	bool SetNodeAttr(IXMLDOMNode* inNode, const char * inAttr, char * szVal);
	bool SetNodeAttr(IXMLDOMNode* inNode, const char * inAttr, const char * szVal);
	bool SetNodeAttr(IXMLDOMNode* inNode, const char * inAttr, long nVal);
	bool SetNodeAttr(IXMLDOMNode* inNode, const char * inAttr, unsigned long nVal);
	bool SetNodeAttr(IXMLDOMNode* inNode, const char * inAttr, int nVal);
	bool SetNodeAttr(IXMLDOMNode* inNode, const char * inAttr, unsigned int nVal);
	bool SetNodeAttr(IXMLDOMNode* inNode, const char * inAttr, double dblVal);
	bool SetNodeAttr(IXMLDOMNode* inNode, const char * inAttr, _variant_t vVal);

	//name, text and data
	CString GetNodeName(IXMLDOMNode * inNode);
	CString GetNodeTextEx(const char * inNodeName, IXMLDOMNode * inParentNode = NULL);
	CString GetNodeText(IXMLDOMNode* inNode);
	bool SetNodeText(IXMLDOMNode* inNode, const char * inText);
	
	//navigation
	IXMLDOMNode* GetNodeFirstChild(IXMLDOMNode * inNode);
	IXMLDOMNode* GetNodeNextSibling(IXMLDOMNode * inNode);
	IXMLDOMNode* GetNodeLastChild(IXMLDOMNode * inNode);
	IXMLDOMNode* GetNodePreviousSibling(IXMLDOMNode * inNode);
	bool GetNodeSubNodeList(DOMNodeList * outNodeList, IXMLDOMNode * inParentNode, const char * inWhatName = NULL);
	IXMLDOMNode* GetNodeSubNode(IXMLDOMNode * inParentNode, const char * inWhatName);

protected:
	CString				mXmlFile;
	IXMLDOMDocument *	mDocNode;
	IXMLDOMNode *		mRootNode;

protected:
	bool SetFileName(const char * inXmlFile);
	void Clean();

	// helper functions
	inline bool Element2Node(IXMLDOMNode ** outNode, IXMLDOMElement * const inElement)
	{
		HRESULT hr = inElement->QueryInterface(IID_IXMLDOMNode, (void**)outNode);
		return SUCCEEDED(hr);
	}

	inline bool Node2Element(IXMLDOMElement ** outElement, IXMLDOMNode * const inNode)
	{
		HRESULT hr = inNode->QueryInterface(IID_IXMLDOMElement, (void**)outElement);
		return SUCCEEDED(hr);
	}
};

#endif // __H_CXmlWrapper__
