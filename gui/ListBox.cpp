#include "ListBox.h"
#include "Window.h"
#include "drawing_tools.h"
#include "leaker.h"

/**
 * Creates a new List box
 */
CListBox::CListBox(CWindow* _parent, int _x, int _y, int _w, int _h, DWORD _id,  bool _multiSelect, int _style, LISTITEMDRAW _drawMethod, int _itemHeight ):COwnerDrawControl(_parent, _x, _y, _w, _h, L"", _id)
{
	wcscpy(szClassName, L"LISTBOX");
	hwnd = CreateWindowEx(
		WS_EX_STATICEDGE,
		szClassName,										// predefined class
		L"",													// text
		WS_CHILD |LBS_HASSTRINGS | WS_VSCROLL | LBS_NOTIFY 
			| (_multiSelect			? LBS_EXTENDEDSEL		:0)
			| (_style == LST_BMPS	? LBS_OWNERDRAWFIXED	:0) ,	// styles
		_x,															// starting x position
		_y,															// starting y position
		_w,															// width
		_h,															// height
		mainHandle,													// parent window
		(HMENU)_id,													// No menu
		(HINSTANCE) GetWindowLong(mainHandle, GWL_HINSTANCE),
		NULL);														// pointer not needed

    SendMessage(hwnd, WM_SETFONT, (WPARAM)sysFont, TRUE);

	itemHeight = _itemHeight;
	if(itemHeight != -1)
	{
		SendMessage(hwnd, LB_SETITEMHEIGHT, 0, itemHeight);
	}

	drawer = _drawMethod;
}

/**
 * Destroys an existing listbox
 */
CListBox::~CListBox()
{

}

/**
 * Adds the string to the listbox
 */
void CListBox::addString(const wchar_t* _text)
{
int idx = SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)_text);
	if(drawer)
	{
		setItemData(idx, (void*)_text);
	}
}

/**
 * Adds the string to the listbox
 */
void CListBox::addString(const wchar_t* _text, void* _data)
{
int idx = SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)_text);
	setItemData(idx, _data);
}

/**
 * Returns the number of items
 */
int CListBox::getItemCount()
{
	return SendMessage(hwnd, LB_GETCOUNT, 0, 0);
}

/**
 * Returns the item with the index _indx
 */
wstring CListBox::getItem(int _idx)
{
wchar_t s[256];
	SendMessage(hwnd, LB_GETTEXT, _idx, (LPARAM)s);
	return wstring(s);
}

/**
 * Empties the listbox
 */
void CListBox::clear()
{
	while(getItemCount() > 0)
	{
		SendMessage(hwnd, LB_DELETESTRING, 0, 0);
	}
}

/**
 * returns the index of the selected element
 */
int CListBox::getSelectedIndex()
{
	return SendMessage(hwnd, LB_GETCURSEL, 0, 0);

}

/**
 * Checks if the item s is in the list box
 */
bool CListBox::hasItem(std::wstring s)
{
	for(int i=0;i<getItemCount(); i++)
	{
		if(getItem(i) == s)
		{
			return true;
		}
	}
	return false;
}

/**
 * Returns the selected indexes of this (multiselect) listbox
 */
vector <int> CListBox::getSelectedIndexes()
{
int count = getItemCount();
vector <int> result;
	result.clear();
int i;
	for(i=0;i<count;i++)
	{
	int isel = SendMessage(hwnd, LB_GETSEL, (WPARAM) i, 0);
		if(isel > 0)
		{
			result.push_back(i);
		}
	}

	return result;
}

/**
 * Returns the selected item
 */
wstring CListBox::getSelectedItem()
{
	if(getItemCount() == 0)
	{
		return L"";
	}
	return getItem(getSelectedIndex());
}

/**
 * Creates the window
 */
bool CListBox::createWindow()
{
	return true;
}

/**
 * Adds the string to the items of this listbox
 */
void CListBox::addString(std::wstring s)
{
	addString(s.c_str());
}

/**
 * Draws the listbox if owner draw. Calls the drawer pointer to function
 */
bool CListBox::draw(LPDRAWITEMSTRUCT dis)
{
	if(drawer)
	{
		return drawer(getItem(dis->itemID), dis, this);
	}
	return false;
}

/**
 * Returns the item data associated with this item in the listbox
 */
void* CListBox::getItemData(int _idx)
{
	return (LPVOID) SendMessage(hwnd, LB_GETITEMDATA, _idx, 0);
}

/**
 * Sets the item data at the specified index
 */
void CListBox::setItemData(int _idx, void* _data)
{
	SendMessage(hwnd, LB_SETITEMDATA, _idx, (LPARAM)_data);
}

/**
 * Selects the element with the given index
 */
void CListBox::select(int _idx)
{
	SendMessage(hwnd, LB_SETCURSEL, _idx, 0);
}

/**
 * Selects the given element
 */
void CListBox::select(std::wstring _elem)
{
	for(int i=0; i<getItemCount(); i++)
	{
	wstring itI = getItem(i);
		if(itI == _elem)
		{
			select(i);
			return;
		}
	}
}
