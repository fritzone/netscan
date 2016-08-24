#include "ImageControl.h"
#include "Window.h"

#pragma warning(disable : 4996)

#include "leaker.h"

/**
 * Creates a new control
 */
CImageControl::CImageControl(CWindow* _parent, int _x, int _y, int _w, int _h):CControl(_parent, _x, _y, _w, _h, L"", 0)
{
	style = SS_BITMAP;
	hImage = NULL;
	createWindow();
}

/**
 * Destructor
 */
CImageControl::~CImageControl(void)
{
	DeleteObject(hImage);
}

/**
 * Sets the image
 */
void CImageControl::setImage(const wchar_t* _image)
{
	if(hImage!= NULL)
	{
		DeleteObject(hImage);
	}
	hImage = LoadImage(NULL, _image, IMAGE_BITMAP, w, h, LR_LOADFROMFILE);
	SendMessage(hwnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);
}

/**
 * Creates the window
 */
bool CImageControl::createWindow()
{
	wcscpy(szClassName, L"STATIC");
	createControl();
	return true;
}
