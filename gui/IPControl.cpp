#include "IPControl.h"
#include "Window.h"
#include "Static.h"
#include "drawing_tools.h"
#include "Debugger.h"
#include "wind_upg.h"

#include "leaker.h"

/**
 * Creates a new text field
 */
CIPControl::CIPControl(CWindow* _mainw, int _x, int _y, int _w, int _h, DWORD _id, const wchar_t* _statText, int _stw)
	: CControl(_mainw, _x, _y, _w, _h, L"", _id)
{
RECT a;
	if(_statText)
	{
		staticText = new CStatic(_mainw, _x, _y+ 2, _stw, _h, (wchar_t*)_statText);
		if(!staticText)
		{
			LOG_S("[ERROR] Could not create a new static ", "Edit");
		}
		GetClientRect(staticText->getHandle(), &a);
	}
	else
	{
		a.right = 0;
		staticText = NULL;
	}

	x += a.right;
	createWindow();
    SendMessage(hwnd, WM_SETFONT, (WPARAM)sysFont, TRUE);
}

/**
 * Destructor
 */
CIPControl::~CIPControl()
{
	if(staticText)
	{
		delete staticText;
	}
}

/**
 * Retrieves the value of the text field as a number
 */
long double CIPControl::asNumber()
{
wchar_t s[256];
	GetWindowText(hwnd, (LPWSTR)s, 256);
	return (long double)_wtof(s);
}

/**
 * A strange thing, just to make it easier to assign to an edit field a number
 */
void CIPControl::operator = (long double _v)
{
wchar_t s[256];
	wsprintf(s, L"%.4f", _v);
	setText(s);
}

/**
 * A strange thing, just to make it easier to assign to an edit field a number
 */
void CIPControl::operator = (int _v)
{
wchar_t s[256];
	wsprintf(s, L"%d", _v);
	setText(s);
}

/**
 * Sets the text of the text field
 */
void CIPControl::operator = (wstring _s)
{
	setText(_s.c_str());
}

/**
 * Hides this control
 */
void CIPControl::hide()
{
	ShowWindow(hwnd, SW_HIDE);
	if(staticText)
	{
		staticText->hide();
	}
}

/**
 * Shows this control
 */
void CIPControl::show()
{
	ShowWindow(hwnd, SW_SHOW);
	if(staticText)
	{
		staticText->show();
	}
}

/**
 * Sets the string of the label of this edit
 */
void CIPControl::setLabel(wstring labelText)
{
	if(staticText)
	{
		staticText->setText(labelText.c_str());
	}
}

/**
 * Supposed to create the window
 */
bool CIPControl::createWindow()
{
	style =  WS_OVERLAPPED | WS_CHILD | WS_TABSTOP;
	wcscpy(szClassName, WC_IPADDRESS);
	createControl();
	return true;
}
