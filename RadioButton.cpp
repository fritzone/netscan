#include "RadioButton.h"
#include "utils.h"
#include "Window.h"
#include "ToolTip.h"
#include "drawing_tools.h"
#include "leaker.h"

/**
 * Creates a new control
 */
CRadioButton::CRadioButton(CWindow* _parent, int _x, int _y, int _w, int _h, int _id, wchar_t* _items, wchar_t* _tooltips, int _space):CControl(_parent, _x, _y, _w, _h, L"", _id)
{
wstring s = _items;

wstring t(L"");
	if(_tooltips)
	{
		t = _tooltips;
	}

int i=1;

wstring curItem = getStringSep(s, L';', i);
wstring ctoolTip = getStringSep(t, L';', i);

	while(curItem != L"")
	{
		LOG(curItem + " is being Created");
	HWND hwnd = CreateWindowEx(
			0,
			(LPCWSTR)"BUTTON",   		// predefined class
			(LPCWSTR)curItem.c_str(),    //  text
			WS_VISIBLE | WS_CHILD  | BS_AUTORADIOBUTTON | BS_FLAT,  // styles
			_x,         		// starting x position
			_y + _space * (i-1),// starting y position, every items starts at _Space distance
			_w,        			// width
			_h,        			// height
			mainHandle,       	// window
			(HMENU)id,       	// Be aware of this handling...!!!
			(HINSTANCE) GetWindowLong(mainHandle, GWL_HINSTANCE),
			NULL);      		// pointer not needed

		SendMessage(hwnd, WM_SETFONT, (WPARAM)sysFont, TRUE);

		handles.push_back(hwnd);
        // create the tooltip for the button
        if(_tooltips != NULL)
        {
		CToolTip* toolTip = new CToolTip((HINSTANCE)GetWindowLong(mainHandle, GWL_HINSTANCE), hwnd, (LPTSTR)ctoolTip.c_str());
			toolTips.push_back(toolTip);
        }

		i++;
		curItem = getStringSep(s, ';', i);
		ctoolTip = getStringSep(t, ';', i);		
	}
}

/**
 * Destructor
 */
CRadioButton::~CRadioButton(void)
{
size_t size = toolTips.size();
	for(size_t i = 0; i<size; i++)
	{
		delete toolTips[i];
	}
}

/**
 * Returns the index of the selected item
 */
int CRadioButton::getSelectedItem()
{
size_t size = handles.size();
	for(size_t i=0;i<size; i++)
	{
		if(SendMessage(handles[i], BM_GETCHECK, 0, 0) == BST_CHECKED)
		{
			return i;
		}
	}
	return -1;
}

/**
 * Sets the selected item
 */
void CRadioButton::setSelectedItem(size_t _idx)
{
size_t size = handles.size();
	if(_idx > size )
	{
		return ;
	}

	for(size_t i=0; i<size; i++)
	{
		SendMessage(handles[i], BM_SETCHECK, BST_UNCHECKED, 0);
	}

	SendMessage(handles[_idx], BM_SETCHECK, BST_CHECKED, 0);
}

/**
 * Does nothing. The creation of a Radiobutton is too special...
 */
bool CRadioButton::createWindow()
{
	return true;
}

/**
 * Shows the buttons of the radiobutton
 */
void CRadioButton::show()
{
	LOG("Entered");
size_t size = handles.size();
	for(size_t i=0; i<size; i++)
	{
		LOG_X("Showing:", handles[i]);
		BringWindowToTop(handles[i]);
		ShowWindow(handles[i], SW_SHOW);
	}
}

/**
 * Hides the buttons
 */
void CRadioButton::hide()
{
	LOG("Entered");
size_t size = handles.size();
	for(size_t i=0; i<size; i++)
	{
		LOG_X("Hiding:", handles[i]);
		ShowWindow(handles[i], SW_HIDE);
	}
}
