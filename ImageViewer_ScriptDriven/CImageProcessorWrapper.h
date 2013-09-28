//
// CImageProcessorWrapper.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CImageProcessorWrapper__
#define __H_CImageProcessorWrapper__

#include "IDispatchImpl.h"
#include "ImageViewer_h.h"

class ImageProcessor;

class CImageProcessorWrapper : public IDispatchImpl<CImageProcessorWrapper
												, IImageProcessor
												, &IID_IImageProcessor>
{
public:
	CImageProcessorWrapper(ITypeLib* ptl, ImageProcessor* processor);
	~CImageProcessorWrapper();

	// APIs exposed to scripts
	STDMETHODIMP load(BSTR filename);
	STDMETHODIMP saveAs(BSTR filename);
	STDMETHODIMP reload();
	STDMETHODIMP invert();
	STDMETHODIMP greyscale();	
	STDMETHODIMP isReady(BOOL* ready);

private:
	ImageProcessor*		mProcessor;
};

#endif // __H_CImageProcessorWrapper__