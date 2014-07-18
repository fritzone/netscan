#ifndef _MENUITEM_H_
#define _MENUITEM_H_

#include "Control.h"

class CMenu;

/**
 * This class represents a menu item that can be seen in a popup menu
 */
class CMenuItem: public CControl
{
public:
	friend class CMenu;
	friend class CPopupMenu;

	/**
	 * Creates a new menu item with the given label and id
	 */
	CMenuItem(const wchar_t* _label, int _id);

	/**
	 * Does nothing.
	 */
	bool createWindow(){return true;}

private:

	// the label of the menu
	wstring label;

	// the menu of this menuitem
	CMenu* menu;
};

#endif
