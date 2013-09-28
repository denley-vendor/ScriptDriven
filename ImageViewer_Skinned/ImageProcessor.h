//
// ImageProcessor.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_ImageProcessor__
#define __H_ImageProcessor__

class ImageProcessor
{
public:
	ImageProcessor();
	~ImageProcessor();

	// File load / save
	bool IsSupported(const char * inSrcFile);
	bool LoadFile(const char * inSrcFile);
	bool Reload();
	bool SaveFile(const char * inDestFile);
	CString GetFile() { return mSourceFile; }
	HDC GetImageDC() { return mMemDC; }
	HBITMAP GetImage() { return mSrcImage; }
	bool IsReady() { return mIsReady; }
	bool GetInfo(long* outWidth, long* outHeight = 0, long* outBitcount = 0);
	
	// Image processing
	bool OverlayLogo(int inX, int inY, const char * inLogoFile);
	bool OverlaySystemTime(int inX, int inY);
	bool DrawText(int inX, int inY, const char * inText);
	void Invert();
	void Greyscale();
	void Clean();

	// Displaying
	void Display(CWnd* inDestWnd);

	// Options
	void KeepAspectRatio(bool enabled) { mKeepAspectRatio = enabled; }

protected:
	bool Decode(const char * inSrcFile, IPicture** outPic);
	void PaintColor(HDC inDC, RECT* inRect, COLORREF inColor);

private:
	CString			mSourceFile;	// File path for the source image
	IPicture *		mIPicture;		// For image decoding
	HDC				mMemDC;			// For image mixing
	HBITMAP			mSrcImage;		// The source bitmap
	HBITMAP			mOldImage;		// The original image in the memory DC
	
	bool			mIsReady;
	bool			mKeepAspectRatio;
};

#endif // __H_ImageProcessor__