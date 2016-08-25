#include <float.h>
#include <iostream>
#include "wind_upg.h"

// Windows used in the application
#include "MainWindow.h"
#include "AboutWindow.h"

#include "leaker.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

CMainWindow* mainWindow = NULL;
CAboutWindow* aboutWindow = NULL;

/*
 * Windows of the application
 */
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR args, int style)
{

	std::cout << "aaa";
	// firstly: Init the Common Controls
	INITCOMMONCONTROLSEX icex;

	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC   = ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_WIN95_CLASSES | ICC_USEREX_CLASSES | ICC_INTERNET_CLASSES;
	InitCommonControlsEx(&icex);

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	// the winsock
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) 
	{
		return 1;
	}

	if ( LOBYTE( wsaData.wVersion ) != 2 || 	HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		WSACleanup( );
		return 1; 
	}

	/* The WinSock DLL is acceptable. Proceed. */


	mainWindow = new CMainWindow(hInstance);
	mainWindow->createWindow();

    aboutWindow = new CAboutWindow(hInstance);
    aboutWindow->mainWindow = mainWindow;

    mainWindow->aboutWindow = aboutWindow;

    mainWindow->show();
    mainWindow->setFocus();

	// program main loop
	MSG msg;
	bool bQuit = false;
	while (!bQuit) 
	{
		// check for messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			// handle or dispatch messages
			if (msg.message == WM_QUIT) 
			{
				bQuit = true;
			} 
			else 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} 
		else 
		{
			Sleep(10);		// so.. that we don't kill the processor by keeping it 100% all the time
		}
	}

	delete mainWindow;
	
	return 0;
}
