//
// CDebugger.h
//

#ifndef __H_CDebugger__
#define __H_CDebugger__

#include "IDispatchImpl.h"
#include "js_demo_h.h"

class CDebugger : public IDispatchImpl<CDebugger
									, IDebugger
									, &IID_IDebugger>
{
private:
	CDebugger(ITypeLib* ptl);
	
public:
	static void CreateInstance(ITypeLib* ptl);
	static void DeleteInstance();
	static CDebugger* GetInstance();
	~CDebugger();

	// APIs exposed to scripts
	STDMETHODIMP trace(BSTR category, BSTR info);
	STDMETHODIMP addBlankLine(UINT lineCount);

private:
	static CDebugger*	mInstance;		// singleton instance of CDebugger
};

extern CDebugger* GetDebugger();

#endif // __H_CDebugger__