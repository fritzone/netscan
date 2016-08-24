#include "checkbox.h"
#include "ToolTip.h"
#include "Window.h"
#include "drawing_tools.h"
#include "leaker.h"

/**
 * Creates a new control
 */
CCheckBox::CCheckBox(CWindow* _parent, int _x, int _y, int _w, int _h, int _id, wchar_t* _text, LPCWSTR _tooltip):CControl(_parent, _x, _y, _w, _h, _text, _id)
{

	createWindow();
	SendMessage(hwnd, WM_SETFONT, (WPARAM)sysFont, TRUE);

	// create the tooltip for the button
	if(_tooltip != NULL)
	{
		toolTip = new CToolTip((HINSTANCE)GetWindowLong(mainHandle, GWL_HINSTANCE), hwnd, _tooltip);
	}
}

/**
 * Destructor
 */
CCheckBox::~CCheckBox(void)
{
	if(toolTip)
	{
		delete toolTip;
	}
}

/**
 * Returns if the control is checked or not
 */
bool CCheckBox::isChecked()
{
	return SendMessage(hwnd, BM_GETCHECK, 0, 0) == BST_CHECKED;
}

/**
 * Creates the window
 */
bool CCheckBox::createWindow()
{
	style = BS_AUTOCHECKBOX | BS_FLAT;
	wcscpy(szClassName, L"BUTTON");
	return createControl();
}

/**
 * Actually checks if this is checked or not
 */
void CCheckBox::operator =(bool _v)
{
	SendMessage(hwnd, BM_SETCHECK, _v?BST_CHECKED:BST_UNCHECKED, 0);
}
