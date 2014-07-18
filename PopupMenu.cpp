#include "PopupMenu.h"
#include "Window.h"
#include "MenuItem.h"

#include "leaker.h"

/**
 * Creates a new popup menu
 */
CPopupMenu::CPopupMenu(CWindow* _parent):CMenu()
{
	parent = _parent;
}

/**
 * Destructor
 */
CPopupMenu::~CPopupMenu()
{
size_t itemsSize = items.size();
	for(size_t i=0; i<itemsSize; i++)
	{
		delete items[i];
	}
	items.clear();
}

/**
 * Adds the item to the menu
 */
void CPopupMenu::addItem(const wchar_t* _label, int _id)
{
CMenuItem* newItem = new CMenuItem(_label, _id);
	items.push_back(newItem);

	AppendMenu(menuHandle, MF_STRING, _id, _label);
}

/**
 * Adds a separator
 */
void CPopupMenu::addSeparator()
{
	AppendMenu(menuHandle, MF_SEPARATOR, 0, 0);
}

/**
 * Pops the menu up at...
 */
void CPopupMenu::popAt(int x, int y)
{
	TrackPopupMenu(menuHandle, TPM_LEFTALIGN, x, y, 0, parent->getHandle(), NULL);
}

/**
 * Sets a new label at the given position
 */
void CPopupMenu::setNewLabel(int pos, const wchar_t* text)
{
	ModifyMenu(menuHandle, pos, MF_BYPOSITION | MF_STRING, items[pos]->getId(), text);
	items[pos]->label = text;
}

/**
 * Disables the entry at...
 */
void CPopupMenu::disableEntry(int pos)
{
	ModifyMenu(menuHandle, pos, MF_BYPOSITION | MF_GRAYED, items[pos]->getId(), items[pos]->label.c_str());
}

/**
 * Sets the default label to be...
 */
void CPopupMenu::setAsDefault(int pos)
{
	ModifyMenu(menuHandle, pos, MF_BYPOSITION | MF_DISABLED | MF_STRING, items[pos]->getId(), items[pos]->label.c_str());
	SetMenuDefaultItem(menuHandle, pos, TRUE);
}
