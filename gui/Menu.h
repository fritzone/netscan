#ifndef _MENU_H_
#define _MENU_H_

#include "wind_upg.h"

#include <vector>

using namespace std;

class CMenuItem;

/**
 * This class represents a menu 
 */
class CMenu
{
public:

	/**
	 * Creates a new menu
	 */
	CMenu();

protected:

	// the items of the menu
	vector<CMenuItem*> items;
	
	// the handle of the menu
	HMENU menuHandle;
};

#endif
