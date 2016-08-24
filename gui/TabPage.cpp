#include "TabPage.h"
#include "Control.h"
#include "Container.h"
#include "TextField.h"
#include "Static.h"

#include "leaker.h"

/**
 * Creates a new tab page with the given caption
 */
CTabPage::CTabPage(const wchar_t* _caption): CContainer(-1, -1, -1, -1, -1)
{
	setText(_caption);
	caption = _caption;
}

/**
 * Does nothing
 */
bool CTabPage::createWindow()
{
	return true;
}

/**
 * Adds a text control to this container. Also adds the static of it
 */
void CTabPage::addTextControl(CTextField* _control)
{
	addControl(dynamic_cast<CControl*>(_control->staticText));
	addControl(dynamic_cast<CControl*>(_control));
}
