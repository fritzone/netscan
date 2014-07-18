#include "ToolTip.h"

#include "leaker.h"

/**
 * Creates a new object
 */
CToolTip::CToolTip(HINSTANCE _hInstance, HWND _hwnd, LPCTSTR _text):CControl(NULL, 0,0,0,0, _text, 0)
{
TOOLINFO ti;
unsigned int uid = 0;       // for ti initialization

RECT rect;                  // for client area coordinates

    handle = CreateWindowEx(WS_EX_TOPMOST,
        TOOLTIPS_CLASS,
        NULL,
        WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        _hwnd,
        NULL,
        _hInstance,
        NULL
        );

    SetWindowPos(handle,
        HWND_TOPMOST,
        0,
        0,
        0,
        0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

    GetClientRect (_hwnd, &rect);

    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_SUBCLASS;
    ti.hwnd = _hwnd;
    ti.hinst = _hInstance;
    ti.uId = uid;
    ti.lpszText = const_cast<wchar_t*>(_text);

    ti.rect.left = rect.left;
    ti.rect.top = rect.top;
    ti.rect.right = rect.right;
    ti.rect.bottom = rect.bottom;

	SendMessage(handle, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
}

/**
 * Destructor
 */
CToolTip::~CToolTip(void)
{
}

/**
 * Creates the window (does nothing)
 */
bool CToolTip::createWindow()
{
	return true;
}
