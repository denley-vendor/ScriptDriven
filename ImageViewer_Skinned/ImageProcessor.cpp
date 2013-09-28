//
// ImageProcessor.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "ImageProcessor.h"
#include "safe_defs.h"
#include "MiscUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
ImageProcessor::ImageProcessor()
	: mSourceFile("")
	, mIPicture(0)
	, mMemDC(0)
	, mSrcImage(0)
	, mOldImage(0)
	, mIsReady(false)
	, mKeepAspectRatio(false)
{
	mMemDC = ::CreateCompatibleDC(0);
}

ImageProcessor::~ImageProcessor()
{
	if (mMemDC)
	{
		// Select out the source bitmap
		if (mOldImage)
		{
			::SelectObject(mMemDC, mOldImage);
			mOldImage = 0;
		}
		::DeleteDC(mMemDC);
		mMemDC = 0;
	}
	SAFE_DELETEOBJECT(mSrcImage);
	SAFE_RELEASE(mIPicture);
}

// Determine if the given file is supported by this image processor.
bool ImageProcessor::IsSupported(const char * inSrcFile)
{
	return MiscUtils::IsSupportedImageFile(inSrcFile);
}

// Load the source image file, then decode the data and store the bitmap to a memory DC.
bool ImageProcessor::LoadFile(const char * inSrcFile)
{
	mIsReady = false;

	IPicture* pPicture = 0;
	if (Decode(inSrcFile, &pPicture))
	{
		// Save the source file path
		mSourceFile = inSrcFile;

		// Release the previously loaded image
		if (mMemDC && mOldImage)
		{
			::SelectObject(mMemDC, mOldImage);
			mOldImage = 0;
		}
		SAFE_DELETEOBJECT(mSrcImage);
		SAFE_RELEASE(mIPicture);
		mIPicture = pPicture;

		// If keep the bitmap object for later using, 
		// please keep IPicture interface too!
		OLE_HANDLE oleBitmap = 0;
		mIPicture->get_Handle(&oleBitmap);

		SAFE_DELETEOBJECT(mSrcImage);
		mSrcImage = (HBITMAP)oleBitmap;

		// Load source image to the memory DC
		if (mMemDC && mSrcImage)
		{
			mOldImage = (HBITMAP) ::SelectObject(mMemDC, mSrcImage);
			mIsReady  = true;
		}
	}

	return mIsReady;	
}

// Reload the current source image file.
bool ImageProcessor::Reload()
{
	return LoadFile(mSourceFile);
}

// Save the current image to a BMP file.
bool ImageProcessor::SaveFile(const char * inDestFile)
{
	if (!mMemDC)
		return false;

	CBitmap* pImage = CBitmap::FromHandle((HBITMAP)GetCurrentObject(mMemDC, OBJ_BITMAP));
	if (!pImage)
		return false;

	// Get the descriptions of the bitmap to save
	BITMAP  bmp;
	pImage->GetBitmap(&bmp);
	long imageSize = bmp.bmWidthBytes * bmp.bmHeight;
	
	// Set up the bitmap info structure
	BITMAPINFOHEADER info;
	ZeroMemory(&info, sizeof(info));
	info.biSize = sizeof(BITMAPINFOHEADER);
	info.biBitCount = bmp.bmBitsPixel;
	info.biWidth  = bmp.bmWidth;
	info.biHeight = bmp.bmHeight;
	info.biPlanes = bmp.bmPlanes;
	info.biCompression = BI_RGB;
	// If 16-bpp bitmap, RGB565 or RGB555?
	long rgb16Format = 0; // 0:non-rgb16, 1:rgb565, 2:rgb555
	if (bmp.bmBitsPixel == 16)
	{
		info.biCompression = BI_BITFIELDS;
		rgb16Format = 1; // ?
	}

	int nColors = 0;
	if (bmp.bmBitsPixel <= 8)
		nColors = 1 << bmp.bmBitsPixel;

	const DWORD bits555[] = {0x7C00,0x03E0,0x001F};
	const DWORD bits565[] = {0xF800,0x07E0,0x001F};

	// Bitmap file header
	BITMAPFILEHEADER	hdr;
	hdr.bfType		= ((WORD) ('M' << 8) | 'B');	// always is "BM"
	hdr.bfSize		= imageSize + sizeof(hdr) + sizeof(BITMAPINFOHEADER);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	hdr.bfOffBits	= (DWORD) (sizeof(BITMAPFILEHEADER) + info.biSize + nColors * sizeof(RGBQUAD));
	if (rgb16Format) // Modify some fields if 16-bpp format
	{
		hdr.bfSize    += 12;
		hdr.bfOffBits += 12;
	}

	FILE * fp = fopen(inDestFile, "wb");
	if (fp)
	{
		fwrite(&hdr, sizeof(char), sizeof(BITMAPFILEHEADER), fp);
		fwrite(&info, sizeof(char), sizeof(BITMAPINFOHEADER), fp);
		if (rgb16Format == 1) // RGB565
		{
			fwrite(bits565, sizeof(DWORD), 3, fp);
		}
		else if (rgb16Format == 2) // RGB555
		{
			fwrite(bits555, sizeof(DWORD), 3, fp);
		}

		// Here, if nColors less than 256, need write RGBQUAD
		// ...

		fwrite(bmp.bmBits, sizeof(char), imageSize, fp);
		fclose(fp);
		return true;
	}
	
	return false;
}

// Get the bitmap information of the source image.
bool ImageProcessor::GetInfo(long* outWidth, long* outHeight, long* outBitcount)
{
	if (!mSrcImage)
		return false;

	BITMAP  bmp;
	CBitmap* pImage = CBitmap::FromHandle(mSrcImage);
	pImage->GetBitmap(&bmp);
	if (outWidth)
		*outWidth = bmp.bmWidth;
	if (outHeight)
		*outHeight = bmp.bmHeight;
	if (outBitcount)
		*outBitcount = bmp.bmBitsPixel;
	return true;
}

// Load the source image file, then decode it.
bool ImageProcessor::Decode(const char * inSrcFile, IPicture** outPic)
{
	*outPic = 0;
	// Open the image source file.
	HANDLE hFile = CreateFile(inSrcFile, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	// Determine the file size.
	DWORD dwFileSize = GetFileSize(hFile, 0);
	if (dwFileSize == -1)
	{
		CloseHandle(hFile);
		return false;
	}

	// Read the file data out to a global memory block.
	LPVOID pvData      = 0;
	DWORD  dwBytesRead = 0;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
	pvData = GlobalLock(hGlobal);
	BOOL bRead = ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, 0);
	GlobalUnlock(hGlobal);
	CloseHandle(hFile);
	if (!bRead)
	{
		GlobalFree(hGlobal);
		return false;
	}

	// Decode the source image and retrieve a Win32 HBITMAP handle.
	LPSTREAM pstm = 0;
	HRESULT  hr   = ::CreateStreamOnHGlobal(hGlobal, TRUE, &pstm);
	if (FAILED(hr))
	{
		GlobalFree(hGlobal);
		return false;
	}

	hr = ::OleLoadPicture(pstm, dwFileSize, FALSE, IID_IPicture, (void**)outPic);
	if (FAILED(hr))
	{
		GlobalFree(hGlobal);
		return false;
	}
	pstm->Release();
	GlobalFree(hGlobal);

	return (*outPic != 0);
}

// Overlay the given logo image to the source image at the specified coordinate.
bool ImageProcessor::OverlayLogo(int inX, int inY, const char * inLogoFile)
{
	// Decode the logo image file
	IPicture* pLogoPic = 0;
	if (!Decode(inLogoFile, &pLogoPic))
		return false;
	
	// Retrieve the logo image and get its dimension
	OLE_HANDLE oleBitmap = 0;
	pLogoPic->get_Handle(&oleBitmap);
	HBITMAP logoImage = (HBITMAP)oleBitmap;
	BITMAP  bmp;
	CBitmap tmpBmp;
	tmpBmp.Attach(logoImage);
	tmpBmp.GetBitmap(&bmp);
	tmpBmp.Detach();

	HDC tmpDC = ::CreateCompatibleDC(0);
	HBITMAP oldBmp = (HBITMAP) ::SelectObject(tmpDC, logoImage);
	
	::StretchBlt(
		mMemDC,							// Destination DC
		inX,							// X sink position
		inY,							// Y sink position
		bmp.bmWidth,					// Destination width
		bmp.bmHeight,					// Destination height
		tmpDC,							// Source DC
		0,								// X source position
		0,								// Y source position
		bmp.bmWidth,					// Source width
		bmp.bmHeight,					// Source height
		SRCCOPY							// Raster operation code
		);

	::SelectObject(tmpDC, oldBmp);
	SAFE_DELETEOBJECT(logoImage);
	SAFE_RELEASE(pLogoPic);
	::DeleteDC(tmpDC);
	return true;
}

// Overlay the current system time to the image surface at the specified coordinate.
bool ImageProcessor::OverlaySystemTime(int inX, int inY)
{
	// Update the current system time
	SYSTEMTIME    systemTime, localTime;
	GetSystemTime(&systemTime);   // This is Coordinated Universal Time (UTC)
	SystemTimeToTzSpecificLocalTime(0, &systemTime, &localTime);
	char szTime[30];
	sprintf(szTime, "%4d-%02d-%02d(%02d:%02d:%02d)", localTime.wYear, localTime.wMonth, 
		localTime.wDay, localTime.wHour, localTime.wMinute, localTime.wSecond);
	return DrawText(inX, inY, szTime);
}

// Draw a text to the current image at the specified coordinate.
bool ImageProcessor::DrawText(int inX, int inY, const char * inText)
{
	if (!mMemDC)
		return false;

/*	HDC tmpDC = ::CreateCompatibleDC(0);
	if (!tmpDC)
		return false;

	// Get the dimension of the text (if drawing to the bitmap)
	SIZE txtSize;
	::GetTextExtentPoint32(tmpDC, inText, (int)strlen(inText), &txtSize);

	HBITMAP txtBmp = ::CreateCompatibleBitmap(tmpDC, txtSize.cx, txtSize.cy);
	HBITMAP oldBmp = (HBITMAP) ::SelectObject(tmpDC, txtBmp);
	// Paint with the white background
	CRect rect(0, 0, txtSize.cx, txtSize.cy);
	PaintColor(tmpDC, &rect, RGB(255, 255, 255));
	::ExtTextOut(tmpDC, 0, 0, ETO_OPAQUE | ETO_CLIPPED, 0, inText, (UINT)strlen(inText), 0);

	// Copy the text bitmap to the internal memory DC
	::StretchBlt(
		mMemDC,							// Destination DC
		inX,							// X sink position
		inY,							// Y sink position
		txtSize.cx,						// Destination width
		txtSize.cy,						// Destination height
		tmpDC,							// Source DC
		0,								// X source position
		0,								// Y source position
		txtSize.cx,						// Source width
		txtSize.cy,						// Source height
		SRCCOPY							// Raster operation code
		);

	::SelectObject(tmpDC, oldBmp);
	::DeleteObject(txtBmp);
	::DeleteDC(tmpDC);
	return true;*/
	
	int oldMode = ::SetBkMode(mMemDC, TRANSPARENT);
	::ExtTextOut(mMemDC, inX, inY, 0, 0, inText, (UINT)strlen(inText), 0);
	::SetBkMode(mMemDC, oldMode);
	return true;
}

// Invert the current image.
void ImageProcessor::Invert()
{
	if (mMemDC && mSrcImage)
	{
		// Get the dimension of the source image.
		BITMAP  bmp;
		CBitmap srcBmp;
		srcBmp.Attach(mSrcImage);
		srcBmp.GetBitmap(&bmp);
		srcBmp.Detach();

		// Create a background bitmap (with white color)
		HBITMAP backBmp = ::CreateCompatibleBitmap(mMemDC, bmp.bmWidth, bmp.bmHeight);
		if (!backBmp)
			return;
		HDC tmpDC = ::CreateCompatibleDC(0);
		HBITMAP oldBmp = (HBITMAP) ::SelectObject(tmpDC, backBmp);
		CRect rect(0, 0, bmp.bmWidth, bmp.bmHeight);
		PaintColor(tmpDC, &rect, RGB(255, 255, 255));

		// Invert the source image on the temp DC
		::StretchBlt(
			tmpDC,							// Destination DC
			0,								// X sink position
			0,								// Y sink position
			bmp.bmWidth,					// Destination width
			bmp.bmHeight,					// Destination height
			mMemDC,							// Source DC
			0,								// X source position
			0,								// Y source position
			bmp.bmWidth,					// Source width
			bmp.bmHeight,					// Source height
			SRCINVERT						// Raster operation code
			);

		// Copy the inverted image from the temp DC to the internal memory DC
		::StretchBlt(
			mMemDC,							// Destination DC
			0,								// X sink position
			0,								// Y sink position
			bmp.bmWidth,					// Destination width
			bmp.bmHeight,					// Destination height
			tmpDC,							// Source DC
			0,								// X source position
			0,								// Y source position
			bmp.bmWidth,					// Source width
			bmp.bmHeight,					// Source height
			SRCCOPY							// Raster operation code
			);

		::SelectObject(tmpDC, oldBmp);
		::DeleteObject(backBmp);
		::DeleteDC(tmpDC);
	}
}

// Convert the colorful image to a greyscale image.
//	Y = 0.299 * R + 0.587 * G + 0.11 * B
void ImageProcessor::Greyscale()
{
	if (!mSrcImage)
		return;

	BITMAP  bmp;
	CBitmap* pImage = CBitmap::FromHandle(mSrcImage);
	pImage->GetBitmap(&bmp);

	// RGB32: BGRA BGRA ...
	// RGB24: BGR BGR ...
	if (bmp.bmBitsPixel == 32 || bmp.bmBitsPixel == 24)
	{
		int pixelBytes = bmp.bmBitsPixel / 8;
		
		BYTE* pLine = (BYTE*) bmp.bmBits;
		for (int i = 0; i < bmp.bmHeight; i++)
		{
			BYTE* pPixel = pLine;
			for (int j = 0; j < bmp.bmWidth; j++)
			{
				BYTE y = (BYTE) (0.11 * pPixel[0] + 0.587 * pPixel[1] + 0.299 * pPixel[2]);
				pPixel[0] = pPixel[1] = pPixel[2] = y;
				pPixel += pixelBytes;
			}

			pLine += bmp.bmWidthBytes;
		}
	}
	else
	{
		// NOT implemented!
	}
}

void ImageProcessor::PaintColor(HDC inDC, RECT* inRect, COLORREF inColor)
{
	HBRUSH backBrush = ::CreateSolidBrush(inColor);
	if (backBrush)
	{
		::FillRect(inDC, inRect, backBrush);
		::DeleteObject(backBrush);
	}
}

// Clean the current image content (with the background color).
void ImageProcessor::Clean()
{
	if (mMemDC && mSrcImage)
	{
		// Get the dimension of the source image.
		BITMAP  bmp;
		CBitmap srcBmp;
		srcBmp.Attach(mSrcImage);
		srcBmp.GetBitmap(&bmp);
		srcBmp.Detach();

		::StretchBlt(
			mMemDC,							// Destination DC
			0,								// X sink position
			0,								// Y sink position
			bmp.bmWidth,					// Destination width
			bmp.bmHeight,					// Destination height
			mMemDC,							// Source DC
			0,								// X source position
			0,								// Y source position
			bmp.bmWidth,					// Source width
			bmp.bmHeight,					// Source height
			WHITENESS						// Raster operation code
			);
	}
}

// Draw the current image to the destination window.
void ImageProcessor::Display(CWnd* inDestWnd)
{
	if (mMemDC && mSrcImage && inDestWnd)
	{
		// Get the dimension of the source image.
		BITMAP  bmp;
		CBitmap srcBmp;
		srcBmp.Attach(mSrcImage);
		srcBmp.GetBitmap(&bmp);
		srcBmp.Detach();

		// Get the target displaying window.
		RECT dstRect;
		inDestWnd->GetClientRect(&dstRect);
		int dstX = dstRect.left;
		int dstY = dstRect.top;
		int dstWidth  = dstRect.right - dstRect.left;
		int dstHeight = dstRect.bottom - dstRect.top;
		HDC dstDC = inDestWnd->GetWindowDC()->GetSafeHdc();

		// Should we keep the same aspect ratio as the original image?
		if (mKeepAspectRatio)
		{
			double xScale = 1.0 * dstWidth / bmp.bmWidth;
			double yScale = 1.0 * dstHeight / bmp.bmHeight;
			if (xScale > yScale)
			{
				int newWidth = int(yScale * bmp.bmWidth);
				dstX = (dstWidth - newWidth) / 2;
				dstWidth = newWidth;
			}
			else
			{
				int newHeight = int(xScale * bmp.bmHeight);
				dstY = (dstHeight - newHeight) / 2;
				dstHeight = newHeight;
			}

			// Clean the screen first!
			PaintColor(dstDC, &dstRect, RGB(0,0,0));
		}

		::StretchBlt(
			dstDC,							// Destination DC
			dstX,							// X sink position
			dstY,							// Y sink position
			dstWidth,						// Destination width
			dstHeight,						// Destination height
			mMemDC,							// Source DC
			0,								// X source position
			0,								// Y source position
			bmp.bmWidth,					// Source width
			bmp.bmHeight,					// Source height
			SRCCOPY							// Raster operation code
			);
	}
}