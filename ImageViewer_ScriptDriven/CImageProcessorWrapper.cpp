//
// CImageProcessorWrapper.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CImageProcessorWrapper.h"
#include "ImageProcessor.h"

/////////////////////////////////////////////////////////////////////////////////////////
CImageProcessorWrapper::CImageProcessorWrapper(ITypeLib* ptl, ImageProcessor* processor)
	: mProcessor(processor)
{
	LoadTypeInfo(ptl);
}

CImageProcessorWrapper::~CImageProcessorWrapper()
{
}

// APIs exposed to scripts
STDMETHODIMP CImageProcessorWrapper::load(BSTR filename)
{
	if (mProcessor)
	{
		CString strFile(filename);
		mProcessor->LoadFile(strFile);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CImageProcessorWrapper::saveAs(BSTR filename)
{
	if (mProcessor)
	{
		CString strFile(filename);
		mProcessor->SaveFile(strFile);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CImageProcessorWrapper::reload()
{
	if (mProcessor)
	{
		mProcessor->Reload();
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CImageProcessorWrapper::invert()
{
	if (mProcessor)
	{
		mProcessor->Invert();
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CImageProcessorWrapper::greyscale()
{
	if (mProcessor)
	{
		mProcessor->Greyscale();
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CImageProcessorWrapper::isReady(BOOL* ready)
{
	if (mProcessor)
	{
		*ready = mProcessor->IsReady() ? TRUE : FALSE;
		return S_OK;
	}
	return E_FAIL;
}

