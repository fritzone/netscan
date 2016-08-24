#include "StatusBar.h"
#include "Debugger.h"
#include "Window.h"
#include "wind_upg.h"

#include "leaker.h"

/**
 * Constructor
 */
CStatusBar::CStatusBar(CWindow* _parent, int _id, int _parts, int* _partPos):CControl( _parent, 0, 0,0 ,0, L"", _id)
{
HWND mainHandle = _parent->getHandle();
	hwnd = CreateWindowEx(
		0,
		(LPCWSTR)STATUSCLASSNAME ,   		// predefined class
		L"Welcome",       		//  text
		WS_VISIBLE | WS_CHILD | SBARS_SIZEGRIP,  // styles
		0,         			// starting x position
		0,         			// starting y position
		0,        			// width
		0,        			// height
		mainHandle,       	// parent window
		(HMENU)id,       	// No menu
		(HINSTANCE) GetWindowLong(mainHandle, GWL_HINSTANCE),
		NULL);      		// pointer not needed
	 
	
	SendMessage(hwnd, SB_SETPARTS, _parts, (LPARAM)_partPos);
	SendMessage(hwnd, WM_SIZE, 0, 0);
	show();
}


void CStatusBar::autoSize()
{
	SendMessage(hwnd, WM_SIZE, 0, 0);
}

void CStatusBar::setText(LPCWSTR text, int idx)
{
	SendMessage(hwnd, SB_SETTEXT, (WPARAM)idx, (LPARAM)text);
}
