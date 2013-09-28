//
// CCurvePainter.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CCurvePainter.h"
#include "CDataCurve.h"

////////////////////////////////////////////////////////////////////////
CCurvePainter::CCurvePainter(ITypeLib* ptl)
{
	LoadTypeInfo(ptl);
}

CCurvePainter::~CCurvePainter()
{
}

STDMETHODIMP CCurvePainter::Result(long i, double x, double y)
{
	if (i >= 0 && i < POINT_COUNT)
	{
		GetSettings()->mXValues[i] = x;
		GetSettings()->mYValues[i] = y;
		return S_OK;
	}
	return E_UNEXPECTED;
}