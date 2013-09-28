//
// CSkinObject.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CSkinObject__
#define __H_CSkinObject__

#include "ImageProcessor.h"

class CSkinObject
{
protected:
	ImageProcessor	mImageProcessor;
	CString			mSkinFileName;
	long			mStateCount;
	long			mImageCount;

	// Skin attributes
	CString			mTagName;
	long			mSkinXPos;
	long			mSkinYPos;
	long			mSkinWidth;
	long			mSkinHeight;

public:
	CSkinObject();
	virtual ~CSkinObject();

	// XML location operations
	void SetTagName(const char * inTagName);
	virtual void ReadXMLSettings();

	virtual void LoadImageMap(CRgn& outRegion);

protected:
	//COLORREF cTransparentColor = 0, COLORREF cTolerance = 0x101010)
	void LoadImageMap(CRgn& outRegion, long& outWidth, long& outHeight,
		long& outImageCount, const char* inFileName, char* pBuffer);
};

#endif //__H_CSkinObject__