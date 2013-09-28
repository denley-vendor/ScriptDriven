//
// CDataCurve.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CDataCurve.h"

CDataCurve* GetSettings() { return(CDataCurve::GetInstance()); }

//	Singleton instance of CDataCurve
CDataCurve* CDataCurve::mInstance = 0;

//////////////////////////////////////////////////////////////////////////////////////////
CDataCurve::CDataCurve()
	: mEngineType("JScript")
	, mExpression("sin(x)")
	, mXMax(6)
	, mYMax(2)
{
	ResetCurveData();
}

CDataCurve::~CDataCurve()
{
}

CDataCurve* CDataCurve::GetInstance()
{
	if (mInstance == 0)
	{
		static CDataCurve obj;
		mInstance = &obj;
	}
	return mInstance;
}

void CDataCurve::ResetCurveData()
{
	ZeroMemory(mXValues, sizeof(double)*POINT_COUNT);
	ZeroMemory(mYValues, sizeof(double)*POINT_COUNT);
}