// JSInterpreting.cpp : Defines the entry point for the console application.
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CJScriptHost.h"
#include "CCallbackManager.h"
#include "CTimerManager.h"

// NOTE: caller is responsible for memory release referenced by outBuf.
bool LoadScriptFile(const char * inFile, wchar_t** outBuf)
{
	// Open the .js file and read out the scripts.
	FILE * fp = fopen(inFile, "rb");
	if (fp)
	{
		// First to determine the file size.
		fseek(fp, 0, SEEK_END);
		long fileSize = ftell(fp);
		if (fileSize <= 0)
		{
			fclose(fp);
			return false;
		}

		// Read scripts out of the source file.
		long  charCount = fileSize + 1; // including a null ternimator.
		char * pScripts = new char[charCount];
		memset(pScripts, 0, charCount);
		fseek(fp, 0, SEEK_SET);
		fread(pScripts, 1, fileSize, fp);
		fclose(fp);

		// Convert scripts to wide characters.
		*outBuf = new wchar_t[charCount];
		::MultiByteToWideChar(CP_ACP, 0, pScripts, -1, *outBuf, charCount);
		delete[] pScripts;
		return true;
	}

	return false;
}

void OnIdle()
{
	// Process timers.
	GetTimerMgr()->Process();

	// Process callbacks.
	GetCallbackMgr()->Process();
}

int _tmain(int argc, _TCHAR* argv[])
{
	CoInitialize(0);

	// Create internal components.
	CTimerManager::CreateInstance();

	// Create a Script Host to execute scripts...
	CJScriptHost jsHost;
	if (jsHost.Initialize())
	{
		const int kCaseCount = 14;
		for (int i = 0; i < kCaseCount; i++)
		{
			char szFilename[100];
			sprintf(szFilename, "TestingCase%02d.js", i+1);

			wchar_t * wszScripts = 0;
			if (LoadScriptFile(szFilename, &wszScripts))
			{
				jsHost.Execute(wszScripts);
				delete[] wszScripts;
			}
		}
	}

	// Wait for Exit and process some tasks in the idle time.
	wprintf(L"\nPress <Enter> on console to exit.\n");
	while (GetAsyncKeyState(VK_RETURN) == 0) 
	{
		OnIdle();
		Sleep(100);
	}

	// Delete internal components.
	CTimerManager::DeleteInstance();

	jsHost.Finalize();
	CoUninitialize();

	return 0;
}