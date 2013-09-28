//
// CSkinObject.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CSkinObject.h"
#include "MiscUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
CSkinObject::CSkinObject() 
	: mStateCount(1)
	, mImageCount(0)
	, mTagName("")
	, mSkinXPos(0)
	, mSkinYPos(0)
	, mSkinWidth(0)
	, mSkinHeight(0)
{
}

CSkinObject::~CSkinObject()
{
}

void CSkinObject::SetTagName(const char* inTagName)
{
	if (mTagName != inTagName)
	{
		mTagName = inTagName;
		ReadXMLSettings();		
	}
}

void CSkinObject::ReadXMLSettings()
{
}

void CSkinObject::LoadImageMap(CRgn& outRegion)
{
	LoadImageMap(outRegion, mSkinWidth, mSkinHeight, mImageCount, mSkinFileName, 0);
}

// The transparent color is RGB(255, 0, 255).
void CSkinObject::LoadImageMap(CRgn& outRegion, long& outWidth, long& outHeight,
							   long& outImageCount, const char* inFileName, char* pBuffer)
{
	HRGN hRgn = NULL; 

/*	COLORREF crTransparentColor = RGB(255,0,255);
	int rTol = 10;
	int gTol = 10;
	int bTol = 10;*/

	CString skinFilePath = MiscUtils::GetSkinFolder() + "\\" + inFileName;
//	HBITMAP bitmapUsed = NULL;
//	bitmapUsed = (HBITMAP)::LoadImage(0, skinFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (mImageProcessor.LoadFile(skinFilePath))
	{
		long bmpWidth, bmpHeight, bmpBitcount;
		mImageProcessor.GetInfo(&bmpWidth, &bmpHeight, &bmpBitcount);

		//get the bitmap dimension.
		outWidth  = bmpWidth / mStateCount;
		outHeight = bmpHeight;
		outImageCount = bmpWidth /outWidth;

		//create a new region for the window.
		if (outRegion.m_hObject)
		{
			outRegion.DeleteObject();
		}
		outRegion.CreateRectRgn(0, 0, outWidth, outHeight);

        //--------------------------------------------------------------------------
		CBitmap bitmap;
		bitmap.Attach(mImageProcessor.GetImage());
		if (bitmap.m_hObject)
		{
			//Create the memory DC inside which we will scan the bitmap content
			HDC hMemDC = CreateCompatibleDC(0);
			if (hMemDC)
			{
				//create a 32 bits depth bitmap map and Select it into the memory dc
				BITMAPINFOHEADER RGB32BITSBITMAPINFO = {	
					    sizeof(BITMAPINFOHEADER),	// biSize 
					    bmpWidth,					// biWidth
					    bmpHeight,				    // biHeight
					    1,							// biPlanes
					    32,							// biBitCount 
					    BI_RGB,						// biCompression
					    0,							// biSizeImage
					    0,							// biXPelsPerMeter
					    0,							// biYPelsPerMeter
					    0,							// biClrUsed
					    0							// biClrImportant
				};

				void * pbits32 = 0;
				HBITMAP hbm32 = CreateDIBSection(hMemDC,(BITMAPINFO*)&RGB32BITSBITMAPINFO,DIB_RGB_COLORS,&pbits32,NULL,0);
				if (hbm32)
				{
					HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);
					
					//get how many bytes per row we have for the bitmap bits (round up to 32 bits)
					BITMAP bm32;
					GetObject(hbm32, sizeof(bm32), &bm32);
					while (bm32.bmWidthBytes % 4)
						bm32.bmWidthBytes++;

					//copy the bitmap into the memory DC
					::BitBlt(hMemDC, 0, 0, bmpWidth, bmpHeight, mImageProcessor.GetImageDC(), 0, 0, SRCCOPY);
					
					//for the better performance ,we use the ExtCreateRegion() function to create the window Region
					#define ALLOC_UNIT  100
					DWORD maxRects = ALLOC_UNIT;
					HANDLE hData = GlobalAlloc(GMEM_MOVEABLE,sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
					RGNDATA * pData   = (RGNDATA*)GlobalLock(hData);
					pData->rdh.dwSize = sizeof(RGNDATAHEADER);
					pData->rdh.iType  = RDH_RECTANGLES;
					pData->rdh.nCount = pData->rdh.nRgnSize = 0;
					SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

					//the transparent color is R > 246, G < 9, B > 246, please remember it.
					BYTE R = 246;
					BYTE G = 9;
					BYTE B = 246;
					
					//scan each the bitmap row from bottom to the top for the transparent pixels.
					BYTE * p32 = (BYTE*)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
					for (int y = 0; y < outHeight; y++)
					{
						//scan for the bitmap from left to right 
						for (int x = 0; x < outWidth; x++)
						{
							//scan for the continuous range of non transparetn pixel
							int  x0 = x;
							LONG * p = (LONG*)p32 + x;
							while (x < outWidth)
							{ 
								BYTE tmp = GetRValue(*p);
								if (tmp > R)
								{ 
									tmp = GetGValue(*p);
									if (tmp < G)
									{ 
										tmp = GetBValue(*p);
										if (tmp > B)
											break;
									}
								}
								p++;
								x++;
							}

							if (x > x0)
							{
								//add the pixels (x0, y) to (x, y+1) as a new rectangle in the region.
								if (pData->rdh.nCount >= maxRects)
								{
									GlobalUnlock(hData);
									maxRects += ALLOC_UNIT;
									hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
									pData = (RGNDATA *)GlobalLock(hData);
								}
								RECT *pr = (RECT *)&pData->Buffer;
								SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
								if (x0 < pData->rdh.rcBound.left)
									pData->rdh.rcBound.left = x0;
								if (y < pData->rdh.rcBound.top)
									pData->rdh.rcBound.top = y;
								if (x > pData->rdh.rcBound.right)
									pData->rdh.rcBound.right = x;
								if (y+1 > pData->rdh.rcBound.bottom)
									pData->rdh.rcBound.bottom = y + 1;
								pData->rdh.nCount++;

								// On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
								// large (ie: > 4000). Therefore, we have to create the region by multiple steps.
								if (pData->rdh.nCount == 2000)
								{
									HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
									if (hRgn)
									{
										CombineRgn(hRgn, hRgn, h, RGN_OR);
										DeleteObject(h);
									}
									else
									{
										hRgn = h;
									}

									pData->rdh.nCount = 0;
									SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
								}
							}
						}

						// Go to next row (remember, the bitmap is inverted vertically)
						p32 -= bm32.bmWidthBytes; 
					}

					//create or extend the region with the remaining rectangle
					HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
					if (hRgn)
					{
						CombineRgn(hRgn, hRgn, h, RGN_OR);
						DeleteObject(h);
					}
					else
					{
						hRgn = h;
					}

					// Clean up
					GlobalFree(hData);
					DeleteObject(SelectObject(hMemDC, holdBmp));
				}

				DeleteDC(hMemDC);
			}

			outRegion.CopyRgn(CRgn::FromHandle(hRgn));
			::DeleteObject(hRgn);
		}

		bitmap.Detach();
	}
}

