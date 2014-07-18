#include "Static.h"
#include "Window.h"
#include "drawing_tools.h"

#include "leaker.h"

/**
 * Constructor
 */
CStatic::CStatic(CWindow* _mainw, int _x, int _y, int _w, int _h, wchar_t* _text):CControl(_mainw, _x, _y, _w, _h, _text, 0)
{
	//style = SS_LEFT | SS_SIMPLE;
	
	createWindow();
	SetWindowText(hwnd, (LPCWSTR)_text);
    SendMessage(hwnd, WM_SETFONT, (WPARAM)sysFont, TRUE);
}

/**
 * Destructor
 */
CStatic::~CStatic()
{
}

/**
 * Creates the window for the control
 */
bool CStatic::createWindow()
{
	wcscpy(szClassName, L"STATIC");
	createControl();
	return true;
}
