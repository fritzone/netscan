#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include "wind_upg.h"

class CWindow;

/**
 * Class responsible for creating the main menu of the application
 */
class CMainMenu
{
public:

	/**
	 * Creates a main menu for this
	 */
	CMainMenu(CWindow* _parent);

	/**
	 * Destructor
	 */
	virtual ~CMainMenu();

	/**
	 * Returns the handle of the menu
	 */
	HMENU getHandle();

private:

	// the handle of the menu
	HMENU handle;

	// the file menu
	HMENU fileMenu;

	// the help menu
	HMENU helpMenu;

	// the edit menu
	HMENU editMenu;

	HMENU viewMenu;

	HMENU createMenu;

	// the window menu
	HMENU windowMenu;

	// the "Projected" menu from the View menu
	HMENU projectedMenu;

	// the menu is in this window
	CWindow* parent;
};

#endif 
