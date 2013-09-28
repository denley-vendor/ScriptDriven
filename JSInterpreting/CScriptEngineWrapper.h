//
// CScriptEngineWrapper.h
//

#ifndef __H_CScriptEngineWrapper__
#define __H_CScriptEngineWrapper__

#include "activscp.h"

class CScriptEngineWrapper
{
private:
	CScriptEngineWrapper(IActiveScript* pEngine);
	
public:
	static void CreateInstance(IActiveScript* pEngine);
	static void DeleteInstance();
	static CScriptEngineWrapper* GetInstance();
	~CScriptEngineWrapper();

	IDispatch* GetDispatch(BSTR pItemName = 0);

private:
	static CScriptEngineWrapper*	mInstance;		// singleton instance of CScriptEngineWrapper

	IActiveScript*		mpEngine; // NOT add an extra reference count!
};

extern CScriptEngineWrapper* GetScriptEngine();

#endif // __H_CScriptEngineWrapper__