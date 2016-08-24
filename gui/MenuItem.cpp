#include "MenuItem.h"

#include "leaker.h"

/**
 * Cretes a new menu item
 */
CMenuItem::CMenuItem(const wchar_t* _label, int _id):CControl(_id)
{
	label = _label;
}
