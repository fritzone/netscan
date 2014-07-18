#include "PushButton.h"
#include "ToolTip.h"
#include "Window.h"
#include "Debugger.h"
#include "drawing_tools.h"
#include "leaker.h"

const COLORREF COLOR_VERYDARKGRAY	= RGB(24,24,24);
const COLORREF COLOR_WHITE			= RGB(255,255,255);

/**
 * Creates a new Push Button. Can be owner draw or not ownerdraw
 */
CPushButton::CPushButton(CWindow* _parent, int _x, int _y, int _w, int _h, DWORD _id, const wchar_t* _text, const wchar_t* _tooltip, int _style):COwnerDrawControl(_parent, _x, _y, _w, _h, _text, _id)
{
	style = ((_style == BTN_BMP||_style==BTN_COLOR)?BS_OWNERDRAW:0);
	style |= WS_TABSTOP;
	styleEx |= WS_EX_CONTROLPARENT;
	createWindow();

	if(_style == BTN_TXT)
	{
		SendMessage(hwnd, WM_SETFONT, (WPARAM)sysFont, TRUE);
	}
	else
	if(_style == BTN_BMP)
	{
	wchar_t* sns = new wchar_t[wcslen(_text) + 5];	// +5 = ".bmp" + 1 for safety
		wcscpy(sns, _text);
		wcscat(sns, L".bmp");
		bmpNoSel = (HBITMAP)LoadImage(NULL, (LPCWSTR)sns, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADTRANSPARENT);
		if(bmpNoSel == NULL)
		{
			_parent->ShowMessage(wstring(wstring(L"Cannot load a bitmap:") + sns).c_str());
			delete [] sns;
			return;
		}
		delete [] sns;
	}
	else
	{
		color = COLOR_3DFACE;
	}

    // create the tooltip for the button
    if(_tooltip != NULL)
    {
        toolTip = new CToolTip((HINSTANCE)GetWindowLong(mainHandle, GWL_HINSTANCE), hwnd, _tooltip);
    }
	else
	{
		toolTip = NULL;
	}

    // add the control
	//_parent->addControl(this);
	style = _style;
}


/**
 * Destructor, just frees the resources that were allocated
 */
CPushButton::~CPushButton()
{
	if(toolTip != NULL)
	{
		delete toolTip;
	}
}

/**
 * Draws the given button using the draw item struct provided
 */
bool CPushButton::draw(LPDRAWITEMSTRUCT lpdis)
{
HDC hdcMem = CreateCompatibleDC(lpdis->hDC);
	if(hdcMem == NULL)
	{
		MessageBox(hwnd, (LPCWSTR)"CreateCompatibleDC failed", (LPCWSTR)"Error", MB_OK);
		return false;
	}

	if (lpdis->itemState & ODS_SELECTED)  // if selected
	{
		SelectObject(hdcMem, bmpNoSel);
	}
	else
    {
		SelectObject(hdcMem, bmpNoSel);
    }

	if(style == BTN_BMP)
	{
		FillRect(lpdis->hDC, &lpdis->rcItem, (HBRUSH)GetStockObject(GRAY_BRUSH));

		StretchBlt(
			lpdis->hDC,         // destination DC
			lpdis->rcItem.left + ((lpdis->itemState & ODS_SELECTED)?1:0), // x upper left
			lpdis->rcItem.top + ((lpdis->itemState & ODS_SELECTED)?1:0),  // y upper left

			// The next two lines specify the width and
			// height.
			lpdis->rcItem.right - lpdis->rcItem.left,
			lpdis->rcItem.bottom - lpdis->rcItem.top,
			hdcMem,		// source device context
			0,
			0,			// x and y upper left
			w,			// source bitmap width
			h,			// source bitmap height
			SRCCOPY);	// raster operation

	}
	else
	{
		LOG("This is a BTN_COLOR button???");
	HBRUSH hb = CreateSolidBrush(color);
		FillRect(lpdis->hDC, &lpdis->rcItem, hb);
		DeleteObject(hb);
	}

	if(lpdis->itemState & ODS_SELECTED)
	{
		SelectObject(lpdis->hDC, CreatePen(PS_SOLID ,0, COLOR_WHITE));
	}
	else
	{
		SelectObject(lpdis->hDC, CreatePen(PS_SOLID ,0, COLOR_VERYDARKGRAY));
	}
	MoveToEx(lpdis->hDC,lpdis->rcItem.left + w, lpdis->rcItem.top, NULL);
	LineTo(lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top);
	LineTo(lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top + h);
	if(lpdis->itemState & ODS_SELECTED)
	{
		SelectObject(lpdis->hDC, CreatePen(PS_SOLID ,0, COLOR_VERYDARKGRAY));
	}
	else
	{
		SelectObject(lpdis->hDC, CreatePen(PS_SOLID ,0, COLOR_WHITE));
	}
	LineTo(lpdis->hDC, lpdis->rcItem.left + w, lpdis->rcItem.top + h);
	LineTo(lpdis->hDC, lpdis->rcItem.left + w, lpdis->rcItem.top);

	DeleteDC(hdcMem);

	return true;
}

/**
 * Creates the actual window (control) of the button...
 */
bool CPushButton::createWindow()
{
	wcscpy(szClassName, L"BUTTON");
	createControl();
	return true;
}

/**
 * Sets the color of the button
 */
void CPushButton::setColor(COLORREF _color)
{
	color = _color;
}
