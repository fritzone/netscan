#include "TreeControl.h"
#include "Window.h"
#include "ImageList.h"
#include "wind_upg.h"
#include <commctrl.h>
#include "drawing_tools.h"
#include "leaker.h"

/**
 * Creates a new tree control
 */
CTreeControl::CTreeControl(CContainer* _parent, int _x, int _y, int _w, int _h, DWORD _id):CControl(_parent, _x, _y, _w, _h, L"", _id)
{
	hwnd = CreateWindowEx(WS_EX_CONTROLPARENT,
            WC_TREEVIEW,
            L"Tree View",
			WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_EDITLABELS | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_TRACKSELECT | TVS_SHOWSELALWAYS | WS_TABSTOP, 
            _x, 
            _y, 
            _w, 
            _h,
            mainHandle, 
            (HMENU)_id, 
            (HINSTANCE) GetWindowLong(_parent->getHandle(), GWL_HINSTANCE), 
            NULL); 

	//SendMessage(hwnd, WM_SETFONT, (WPARAM)sysFont, TRUE);
DWORD currentStyle = GetWindowLong(hwnd, GWL_STYLE);
	SetWindowLong(hwnd, GWL_STYLE | currentStyle, TVS_CHECKBOXES);
	TreeView_SetLineColor(hwnd,RGB(0,0,0));

	onStartEditItemsLabel = NULL;
	onFinishEditItemsLabel = NULL;
	items.clear();
	allControls.insert(std::pair<HWND,CControl*>(hwnd,this));
}

/** 
 * Destructor
 */
CTreeControl::~CTreeControl(void)
{
}

/**
 * Sets the images of this tree control
 */
void CTreeControl::setImages(CImageList* imgList)
{
	imgs = imgList;
	TreeView_SetImageList(hwnd, imgs->imageList, TVSIL_NORMAL); 
}

/**
 * Adds a new tree item
 */
void CTreeControl::addItem(CTreeItem* item)
{
TVITEM tvi; 
TVINSERTSTRUCT tvins; 
	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM; 
	tvi.state = 1;
	tvi.pszText = (LPWSTR)item->text.c_str();
	tvi.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]); 
	tvi.iImage = item->imageIndex;
	tvi.iSelectedImage = item->imageIndex;
	tvi.lParam = (LPARAM)item->param;
	
	tvins.item = tvi;
	tvins.hParent = (item->parent == NULL? TVI_ROOT : item->parent->handle);
	tvins.hInsertAfter = (HTREEITEM)TVI_LAST;
	
	item->handle = (HTREEITEM)SendMessage(hwnd, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins); 

	item->tree = this;

	items.push_back(item);
}

/** 
 * Does nothing
 */
bool CTreeControl::createWindow()
{
	return true;
}

/**
 * Returns the checked items of the control
 */
vector<CTreeItem*> CTreeControl::getCheckedItems()
{
vector<CTreeItem*> res;
	for(size_t i=0; i<items.size(); i++)
	{
		if(items[i]->isChecked())
		{
			res.push_back(items[i]);
		}
	}
	return res;
}

/**
 * Makes sure the item is visible
 */
void CTreeControl::ensureVisible(CTreeItem* _item)
{
	TreeView_EnsureVisible(hwnd, _item->handle);
}

/**
 * Removes the item from the tree
 */
void CTreeControl::removeItem(CTreeItem* item)
{
	TreeView_DeleteItem(hwnd, item->handle);
}

/**
 * Deletes the items (all of them)
 */
void CTreeControl::deleteAllItems()
{
	TreeView_DeleteAllItems(hwnd);
}

/**
 * Returns the item that cen be found at the given position
 */
CTreeItem* CTreeControl::itemAt(int x, int y)
{
TVHITTESTINFO ht;
	ht.pt.x = x;
	ht.pt.y = y;
	ht.flags = TVHT_ONITEM | TVHT_ONITEMLABEL;
HTREEITEM got = TreeView_HitTest(hwnd, &ht);
	if(got == NULL)
	{
		return NULL;
	}

	for(size_t i=0; i<items.size(); i++)
	{
		if(got == items[i]->handle)
		{
			return items[i];
		}
	}

	return NULL;
}

/**
 * Returns the item if the two coordinate items are on the checkbox of it
 */
CTreeItem* CTreeControl::clickOnCheckBox(int x, int y)
{
TVHITTESTINFO ht;
	ht.pt.x = x;
	ht.pt.y = y;
	ht.flags = TVHT_ONITEMSTATEICON;
HTREEITEM got = TreeView_HitTest(hwnd, &ht);
	if(got == NULL || ( (ht.flags & TVHT_ONITEMSTATEICON) != TVHT_ONITEMSTATEICON))
	{
		return NULL;
	}

	for(size_t i=0; i<items.size(); i++)
	{
		if(got == items[i]->handle)
		{
			return items[i];
		}
	}

	return NULL;
}


/**
 * Deletes the given item. Also removes the item from the items vector...
 */
void CTreeControl::deleteItem(CTreeItem* _item)
{
	LOG_S("Deleting item: ", _item->getText());
	TreeView_DeleteItem(hwnd, _item->handle);
size_t i=0;
CTreeItem* actItem = _item;
	while(i<items.size())
	{
		if(items[i] == _item)
		{
			items.erase(items.begin() + i);
			return; 
		}
		i++;
	}
}

/**
 * Activates the callback for selchanging
 */
LPARAM CTreeControl::selChanging(LPNMTREEVIEW tv)
{
	if(onSelChanging)
	{
		onSelChanging(this, getItem(tv->itemNew.hItem));
	}
	return 1;
}

/**
 * Activates the callback for selchanged
 */
LPARAM CTreeControl::selChanged(LPNMTREEVIEW tv)
{
	if(onSelChanged)
	{
		onSelChanged(this, getItem(tv->itemNew.hItem));
	}
	return 1;
}

/**
 * Activates the callback for selchanged
 */
LPARAM CTreeControl::endLabelEdit(LPNMTREEVIEW tv)
{
	if(onFinishEditItemsLabel)
	{
	LPNMTVDISPINFO tvdi = (LPNMTVDISPINFO)tv;
		onFinishEditItemsLabel(this, getItem(tvdi->item.hItem));
	}
	return 1;
}

/**
 * Activates the callback for selchanged
 */
LPARAM CTreeControl::startLabelEdit(LPNMTREEVIEW tv)
{
	if(onStartEditItemsLabel)
	{
	LPNMTVDISPINFO tvdi = (LPNMTVDISPINFO)tv;
		onStartEditItemsLabel(this, getItem(tvdi->item.hItem));
	}
	return 1;
}


CTreeItem *CTreeControl::getItem(HTREEITEM ht)
{
size_t l = this->items.size();
	for(size_t i=0; i<l; i++)
	{
	CTreeItem* itemI = this->items[i];
		if(itemI->handle == ht)
		{
			return itemI;
		}
	}
	return NULL;
}

CTreeItem* CTreeControl::getSelectedItem()
{
HTREEITEM sel = TreeView_GetSelection(hwnd);
	if(sel)
	{
		CTreeItem* item = getItem(sel);
		return item;
	}

	return NULL;
}
