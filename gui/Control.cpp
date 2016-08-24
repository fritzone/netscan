#include "Control.h"

#include "Debugger.h"
#include "Container.h"

#include "leaker.h"
#include <assert.h>

map<HWND,CControl*> CControl::allControls;

/**
 * Creates a new control.
 */
CControl::CControl(DWORD _id):id(_id), x(0), y(0), w(0), h(0), visible(true) 
{
}

/**
 * Creates a new object
 */
CControl::CControl(CControl* _parent, int _x, int _y, int _w, int _h, wstring _text, DWORD _id):id(_id), x(_x), y(_y), w(_w), h(_h), visible(true) 
{
	parent = _parent;
	//tabbedParent = NULL;
	mainHandle = HWND_DESKTOP;
	style = 0;
	styleEx = 0;
	wcscpy(text, _text.c_str());
	hwnd = NULL;
	wcscpy(szClassName, L"BASIC_CONTROL");

	if(_parent)
	{
		mainHandle = _parent->getHandle();
		progInstance = (HINSTANCE) GetWindowLong(mainHandle, GWL_HINSTANCE);

		if( dynamic_cast<CContainer*>(_parent) != NULL)
		{
			dynamic_cast<CContainer*>(_parent)->addControl(this);
		}
	}
}

/**
 * Hides this control
 */
void CControl::hide()
{
	if(!hwnd)
	{
		return;
	}
	ShowWindow(hwnd, SW_HIDE);
	visible = false;
}

/**
 * Shows this control
 */
void CControl::show()
{
	if(!hwnd)
	{
		return;
	}
	ShowWindow(hwnd, SW_SHOW);
	visible = true;
}

/**
 * Sets the text of the control to the parameter
 */
void CControl::setText(LPCWSTR _text)
{
    assert(wcslen(_text) < 255);

	SetWindowText(hwnd, _text);
	wcscpy(text, _text);
}

/**
 * Returns the text of the control
 */
wstring CControl::getText()
{
wchar_t s[4096];
	GetWindowText(hwnd, s, 4095);
wstring g (s);
	if(g.length() == 0)
	{
		return wstring(text);
	}
	else
	{
		return g;
	}
}

/**
 * Creates the window class, and registers it for this object
 */
bool CControl::createWindowClass(WNDPROC _windowProcedure)
{
	windowClass.cbSize					= sizeof (WNDCLASSEX);

    windowClass.hInstance				= progInstance;
	windowClass.lpszClassName			= szClassName;
    windowClass.lpfnWndProc				= (WNDPROC)_windowProcedure;			// This function is called by windows
    windowClass.style					= CS_DBLCLKS | CS_SAVEBITS;				// Catch double-clicks 

    windowClass.hIcon					= LoadIcon (NULL, IDI_APPLICATION);
    windowClass.hIconSm					= LoadIcon (NULL, IDI_APPLICATION);
    windowClass.hCursor					= LoadCursor (NULL, IDC_ARROW);
    windowClass.lpszMenuName			= NULL;									// No menu
    windowClass.cbClsExtra				= 0;									// No extra bytes after the window class
    windowClass.cbWndExtra				= 0;									// structure or the window instance
	windowClass.hbrBackground			= (HBRUSH)(COLOR_BTNFACE + 1);

    if (!RegisterClassEx (&windowClass))
    {
    	MessageBox(NULL, L"Cannot register window class", L"Error", MB_OK);
        return false;
    }

	return true;
}

/**
 * Creates the control 
 */
bool CControl::createControl()
{
	if(mainHandle != HWND_DESKTOP)
	{
		style |= WS_CHILD;
	}

	if(visible)
	{
		style |= WS_VISIBLE;
	}

	hwnd = CreateWindowEx(
		styleEx,									// Extended
		szClassName,   								// predefined class
		text,			      						// text on the control
		style,										// styles
		x,         									// starting x position
		y,         									// starting y position
		w,        									// button width
		h,        									// button height
		mainHandle,       							// parent window
		(HMENU)id,       							// No menu
		progInstance,								// The instance of the program
		NULL);      								// pointer not needed

	if(hwnd == NULL)
	{

		TCHAR szBuf[1024]; 
		LPVOID lpMsgBuf;
		DWORD dw = GetLastError(); 

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,NULL,	dw,	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0, NULL );
		wsprintf(szBuf, L"%s failed with error %d: %s", L"createControl", dw, lpMsgBuf); 
	 
		MessageBox(NULL, szBuf, L"Error", MB_OK); 

		LocalFree(lpMsgBuf);
		return false;
	}
	
	allControls.insert(std::make_pair(hwnd, this));
	return true;
}

/**
 * Returns the ID of the control
 */
DWORD CControl::getId() const
{
	return id;
}

/**
 * Returns the handle of the control
 */
HWND CControl::getHandle() const
{
	return hwnd;
}

/**
 * Disables the control
 */
void CControl::disable()
{
	EnableWindow(hwnd, FALSE);
}

/**
 * Enables the control
 */
void CControl::enable()
{
	EnableWindow(hwnd, TRUE);
}

/**
 * Sets the focus to this window
 */
void CControl::setFocus()
{
	SetFocus(hwnd);
}

/**
 * Updates the window
 */
void CControl::update()
{
	UpdateWindow(hwnd);
}

/**
 * Brings the control to the front
 */
void CControl::bringToFront()
{
	BringWindowToTop(hwnd);
}

/**
 * Sends the control to the back
 */
void CControl::sendToBack()
{
	SetWindowPos(hwnd, HWND_BOTTOM, 0,0,0,0, SWP_NOSIZE|SWP_NOSENDCHANGING|SWP_NOREDRAW|SWP_NOMOVE|SWP_NOCOPYBITS|SWP_NOACTIVATE|SWP_HIDEWINDOW); 
}

/**
 * Returns if the control is visible
 */
bool CControl::isVisible()
{
	return (IsWindowVisible(hwnd) == TRUE);
}

/**
 * Repaints the control
 */
void CControl::repaint()
{
	RedrawWindow(hwnd, NULL, NULL, RDW_ALLCHILDREN | RDW_ERASENOW);
}


void CControl::move(RECT& rc)
{
	MoveWindow(hwnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 1);
}