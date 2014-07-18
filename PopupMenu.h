#ifndef _POPUPMENU_H_
#define _POPUPMENU_H_

#include "Menu.h"

#include <string>
#include <vector>

using namespace std;

class CWindow;
class CMenuItem;

/**
 * Class responsible for handling the operations with popup menus
 */
class CPopupMenu: public CMenu
{
public:

	/**
	 * Creates a new Popup menu for the given window
	 */
	CPopupMenu(CWindow*);

	/**
	 * Destructor
	 */
	~CPopupMenu();

	/**
	 * Pops up the menu at the given position
	 */
	void popAt(int x, int y);

	/**
	 * Adds an item with the wchar_t* as label and int as ID
	 */
	void addItem(const wchar_t*, int);

	/**
	 * Adds a separator to the menu
	 */
	void addSeparator();

	/**
	 * Sets the label of the item at position int to be the wchar_t*
	 */
	void setNewLabel(int, const wchar_t*);

	/**
	 * Disables the entry at the given position
	 */
	void disableEntry(int);

	/**
	 * Sets the default eentry to be the one at the given position
	 */
	void setAsDefault(int);

private:

	// the parent of the menu
	CWindow *parent;

	// the items of the menu
	vector <CMenuItem*> items;
};

#endif
