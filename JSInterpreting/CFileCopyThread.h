//
// CFileCopyThread.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_CFileCopyThread__
#define __H_CFileCopyThread__

#include "CHQThread.h"
#include "DispatchHelper.h"
#include "CDebugger.h"
#include "CCallbackManager.h"

class CFileCopyThread : public CHQThread
{
public:
	CFileCopyThread(BSTR srcFile, BSTR dstFile, IDispatch* pCallback) 
		: mSourceFile(0), mDestFile(0), mpCallback(pCallback)

	{
		if (srcFile)
			mSourceFile = ::SysAllocString(srcFile);
		if (dstFile)
			mDestFile = ::SysAllocString(dstFile);
	}

	virtual ~CFileCopyThread()
	{
		if (mSourceFile)
			::SysFreeString(mSourceFile);
		if (mDestFile)
			::SysFreeString(mDestFile);
	}

protected:
	virtual void Process()
	{
		// Copying the source file to the destination file.
		// ...

//#define __CallbackInFileCopyThread__

		// callback(srcFile:string, dstFile:string, status:int);
#ifdef __CallbackInFileCopyThread__
		DispatchHelper dispatcher(mpCallback);
		dispatcher.CreateParameters(3);
		dispatcher.AddParameter(0, 1);
		dispatcher.AddParameter(1, mDestFile);
		dispatcher.AddParameter(2, mSourceFile);
		bool bRet = dispatcher.Invoke(0); // FAILED!!!
		if (bRet)
		{
			GetDebugger()->trace(L"Async Info", L"File Copy completed and Callback succeeded.");
		}
		else
		{
			GetDebugger()->trace(L"Async Info", L"File Copy completed but Callback failed!");
		}
#else 
		DispatchHelper* pDispatcher = new DispatchHelper(mpCallback);
		pDispatcher->CreateParameters(3);
		pDispatcher->AddParameter(0, 1);
		pDispatcher->AddParameter(1, mDestFile);
		pDispatcher->AddParameter(2, mSourceFile);
		GetCallbackMgr()->QueueCallback(pDispatcher); // Put callback into the queue
		GetDebugger()->trace(L"Async Info", L"File Copy callback has been put into the queue.");
#endif
	}

private:
	BSTR		mSourceFile;
	BSTR		mDestFile;
	IDispatch*	mpCallback;
};

#endif // __H_CFileCopyThread__