#include "Menu.h"

#include "leaker.h"

/**
 * Creates a new, empty menu
 */
CMenu::CMenu()
{
	menuHandle = CreatePopupMenu();
	items.clear();
}
