//
// CDataCurve.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CDataCurve__
#define __H_CDataCurve__

#define POINT_COUNT 600

class CDataCurve
{
public:
	CString		mEngineType;
	CString		mExpression;

	double		mXMax;
	double		mYMax;
	double		mXValues[POINT_COUNT];
	double		mYValues[POINT_COUNT];

protected:
	CDataCurve();

public:
	static CDataCurve* GetInstance();
	~CDataCurve();

	void ResetCurveData();

protected:
	static CDataCurve*	mInstance;		// singleton instance of CDataCurve
};

extern CDataCurve* GetSettings();

#endif // __H_CDataCurve__