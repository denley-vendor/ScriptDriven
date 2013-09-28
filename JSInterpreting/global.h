//
// global.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_Global__
#define __H_Global__

namespace misc_utils
{

// Determine if all chars of the specified string are digitals. 
inline bool IsDigitals(OLECHAR * str)
{
	size_t length = ::wcslen(str);
	if (length <= 0)
		return false;

	for (size_t i = 0; i < length; i++)
	{
		if (!::iswdigit(str[i]))
			return false;
	}
	return true;
}

// Get the max DISPID defined in the type library.
inline bool GetMaxDispid(ITypeInfo * pTI, DISPID* outDispid)
{
	if (!pTI || !outDispid)
		return false;

	// Determine the number of methods (/properties) in the object
	TYPEATTR* pta = 0;
	HRESULT hr = pTI->GetTypeAttr(&pta);
	if (FAILED(hr))
		return false;

	*outDispid = 0;
	// Loop through getting the information on each member
	for (UINT index = 0; index < pta->cFuncs && SUCCEEDED(hr); ++index)
	{
		// Get the function description for this member
		FUNCDESC* pfd = 0;
		hr = pTI->GetFuncDesc(index, &pfd);
		if (SUCCEEDED(hr))
		{
			if (pfd->memid > *outDispid)
				*outDispid = pfd->memid;

#ifdef _DEBUG
			BSTR bstrName = 0;
			// Check the name of this member
			pTI->GetDocumentation(pfd->memid, &bstrName, 0, 0, 0);
			// Release the memory alloced on our behalf
			if (bstrName)
				::SysFreeString(bstrName);
#endif

			pTI->ReleaseFuncDesc(pfd);
		}
	}

	pTI->ReleaseTypeAttr(pta);
	return SUCCEEDED(hr);
}

}; // namespace misc_utils

#endif // __H_Global__
