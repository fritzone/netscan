#include "ToolBar.h"

#include "leaker.h"

#define MARRAYSIZE(S) (sizeof(S)/sizeof(S[0]))

/**
 * Creates a toolbar by specifying the images to load, and the number of buttons.
 * These buttons must be added later by the addButton method.
 * @param _hwnd - the handle of the parent window
 * @param _images - the name of the bitmap file which contains the images
 * @param _nButtons - the number of buttons to add
 */
CToolBar::CToolBar(CControl* _parent, LPCWSTR _images, int _nButtons) : CControl(_parent, 0,0,0,0, L"", -1)
{
	hwnd = CreateWindowEx(
		0, 
		TOOLBARCLASSNAME, 
		NULL, 
		WS_CHILD | CCS_ADJUSTABLE | TBSTYLE_TOOLTIPS | CCS_TOP, 
		0, 0, 0, 0, 
		_parent->getHandle(), 
		(HMENU) 1, 
		(HINSTANCE)GetWindowLong(_parent->getHandle(), GWL_HINSTANCE), 
		NULL); 

	// Send the TB_BUTTONSTRUCTSIZE message, which is required for 
	// backward compatibility. 
	SendMessage(hwnd, TB_BUTTONSTRUCTSIZE, (WPARAM) sizeof(TBBUTTON), 0); 
	SendMessage(hwnd, TB_AUTOSIZE, 0, 0); 

	ShowWindow(hwnd, SW_SHOW); 

	
	HBITMAP bmp = (HBITMAP)LoadImage((HINSTANCE)GetWindowLong(_parent->getHandle(), 
		GWL_HINSTANCE), _images, IMAGE_BITMAP, 0, 0, LR_LOADTRANSPARENT | LR_LOADMAP3DCOLORS);
	if(bmp == NULL)
	{
		MessageBox(_parent->getHandle(), L"Toolbar creation: Cannot load bitmap", _images, MB_OK);
		return;
	}

	TBADDBITMAP addBmp;
	addBmp.hInst = NULL;
	addBmp.nID = (UINT_PTR)bmp;

	if(SendMessage(hwnd, TB_ADDBITMAP, _nButtons, (LPARAM)&addBmp) == -1)
	{
		MessageBox(_parent->getHandle(), L"Toolbar creation: Something went wrong in TB_ADDBITMAP", L"Error", MB_OK);
	}

    allControls.insert(std::pair<HWND,CControl*>(hwnd,this));

}

/**
 * Destructor
 */
CToolBar::~CToolBar()
{
}

/**
 * Adds the specified toolbar button to the toolbar
 */
void CToolBar::addButton(CToolButton* _button)
{
	SendMessage(hwnd, TB_ADDBUTTONS, 1, (LPARAM)&_button->button);
	buttons.push_back(_button);
	_button->parent = this;
}

/**
 * Adds a separator to the toolbar, so that groupin buttons is possible
 */
void CToolBar::addSeparator()
{
TBBUTTON btn;
	btn.idCommand = 0;
	btn.iString = 0;
	btn.fsState = TBSTATE_ENABLED ;
	btn.fsStyle = BTNS_SEP ;
	btn.dwData = 0;
	btn.iBitmap = 0;

	SendMessage(hwnd, TB_ADDBUTTONS, 1, (LPARAM)&btn);
}

/**
 * Returns the tooltip of the button
 */
const wchar_t* CToolBar::getTooltip(int _id)
{
	for(size_t i=0; i<buttons.size(); i++)
	{
		if(buttons[i]->getId() == _id)
		{
			return buttons[i]->tooltip.c_str();
		}
	}
	return L"...";
}

void CToolBar::autoSize()
{
    SendMessage(hwnd, TB_AUTOSIZE, 0, 0);
}