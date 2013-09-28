//
// DispatchHelper.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __DispatchHelper_H__
#define __DispatchHelper_H__

class DispatchHelper
{
public:
	DispatchHelper(IDispatch* pDisp = 0);
	~DispatchHelper();

	bool CreateParameters(int paramCount);
	void DeleteParameters();

	// Note: Please add parameters with the sequence "Right to Left".
	// That is, the rightest parameter is the one with index 0!
	bool AddParameter(UINT index, VARIANT* value);
	// Easy-to-use functions 
	bool AddParameter(UINT index, BSTR value);
	bool AddParameter(UINT index, INT value);
	bool AddParameter(UINT index, UINT value);
	bool AddParameter(UINT index, IDispatch* pDisp);
	bool AddNullParameter(UINT index);

	void SetDispatch(IDispatch* pDisp); // NOT thread-safe at present!
	bool GetDispid(BSTR functionName, DISPID* outDispid);
	bool Invoke(DISPID dispid);

	bool SetProperty(BSTR propName, VARIANT* inValue);
	bool GetProperty(BSTR propName, VARIANT* outValue);
	
private:
	IDispatch*		mpDisp;			// The object to dispatch the JavaScript function.
									// Note: NOT keep a reference count!
	VARIANT*		mpParams;		// Parameters for the specified function.
	UINT			mParamCount;	// Parameter count
};

#endif //__DispatchHelper_H__