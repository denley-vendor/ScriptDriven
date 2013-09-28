// XMLParser.cpp : Defines the entry point for the console application.
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "XMLParser.h"
#include "CXmlParserDemo.h"
#include "MiscUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	CoInitialize(0);

	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		CXmlParserDemo skinParser;
		if (skinParser.SetXmlFile("ImageViewer.xml"))
		{
			printf("\nThe skin folder <<< %s >>>\n\n", MiscUtils::GetSkinFolder());
			printf("The main window's skin: %s.\n", skinParser.GetMainWindowSkin());
			
			printf("\nLoop through all elements.\n");
			bool found = skinParser.FindFirstNode();
			while (found)
			{
				CString btnName = skinParser.GetName();
				CString btnSkin = skinParser.GetSkin();
				long x, y, width, height;
				skinParser.GetPosition(x, y, width, height);
				printf("%s <name:%s, skin:%s, xpos:%d, ypos:%d, width:%d, height:%d>\n",
					skinParser.GetNodeName(), btnName, btnSkin, x, y, width, height);

				found = skinParser.FindNextNode();
			}

			printf("\nTry to search a specified button: 'btn_Reload'.\n");
			if (skinParser.FindButton("btn_Reload"))
			{
				printf("%s <name:%s, skin:%s, xpos:%d, ypos:%d>\n",
					skinParser.GetNodeName(), 
					skinParser.GetName(), 
					skinParser.GetSkin(), 
					skinParser.GetXPos(), 
					skinParser.GetYPos());
			}
			else
			{
				printf("NOT FOUND!\n");
			}
		}
	}

	CoUninitialize();

	printf("\nPress any key on console to exit.\n");
	getchar();

	return nRetCode;
}
