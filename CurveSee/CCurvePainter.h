//
// CCurvePainter.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CCurvePainter__
#define __H_CCurvePainter__

#include "IDispatchImpl.h"
#include "curves_h.h"

class CCurvePainter : public IDispatchImpl<CCurvePainter
										, ICurvePainter
										, &IID_ICurvePainter>
{
public:
	CCurvePainter(ITypeLib* ptl);
	~CCurvePainter();

	// APIs exposed to scripts
	STDMETHODIMP Result(long i, double x, double y);
};

#endif // __H_CCurvePainter__