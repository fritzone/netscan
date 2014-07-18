#include "ImageList.h"
#include "MainWindow.h"

#include "leaker.h"

/**
 * Creates a new control
 */
CImageList::CImageList(int _width, int _height)
{
	cx = _width;
	cy = _height;

	imageList = ImageList_Create(cx, cy, ILC_COLOR24, 10, 10);
}

/**
 * Adds the given image to the list
 */
bool CImageList::addImage(LPCWSTR image)
{
	HANDLE hbmp = LoadImage(NULL, image, IMAGE_BITMAP, cx, cy, LR_LOADFROMFILE | LR_LOADTRANSPARENT| LR_LOADMAP3DCOLORS );
int newIndex = ImageList_Add(imageList, (HBITMAP)hbmp, NULL);
	if(newIndex == -1)
	{
		MessageBox(NULL, L"Image load failure", L"Error", MB_OK);
		return false;
	}

	return true;
}

/**
* Adds the given image to the list
*/
bool CImageList::addResourceImage(DWORD id)
{
HANDLE hbmp = LoadImage((HINSTANCE)GetWindowLong(CMainWindow::getInstance()->getHandle(), GWL_HINSTANCE), MAKEINTRESOURCE(id), IMAGE_BITMAP, cx, cy, LR_LOADTRANSPARENT| LR_LOADMAP3DCOLORS );
int newIndex = ImageList_Add(imageList, (HBITMAP)hbmp, NULL);
	if(newIndex == -1)
	{
		MessageBox(NULL, L"Resource Image load failure", L"Error", MB_OK);
		return false;
	}

	return true;
}
