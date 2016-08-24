#include "ComboBox.h"
#include "Window.h"
#include "drawing_tools.h"
#include "leaker.h"
#include "wind_upg.h"

/**
 * Creates a new Combo Box
 */
CComboBox::CComboBox(CWindow* _parent, int _x, int _y, int _w, int _h, DWORD _id):CControl(_parent, _x, _y, _w, _h, L"", _id)
{
	wcscpy(szClassName, WC_COMBOBOX);
	hwnd = CreateWindowEx(
		0,
		szClassName,					        // predefined class
		L"",								    // button text
		WS_CHILD| WS_BORDER | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS ,
		_x,										// starting x position
		_y,										// starting y position
		_w,										// button width
		_h,										// button height
		mainHandle,								// parent window
		(HMENU)_id,								// No menu
		(HINSTANCE) GetWindowLong(mainHandle, GWL_HINSTANCE),
		NULL);									// pointer not needed

    SendMessage(hwnd, WM_SETFONT, (WPARAM)sysFont, TRUE);
}

/**
 * Destructor
 */
CComboBox::~CComboBox(void)
{
}

/**
 * Adds a new entry to the Combo
 */
void CComboBox::addString(std::string s)
{
	SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)(s.c_str()));
	strings.push_back(s);
}
