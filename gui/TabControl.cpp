#include "wind_upg.h"

#include "TabControl.h"
#include "OwnerDrawControl.h"
#include "TabPage.h"
#include "Window.h"
#include "drawing_tools.h"

#include "leaker.h"

/**
 * Creates a new control in the given window, with the given coordinates
 */
CTabControl::CTabControl(CContainer* _parent, int _x, int _y, int _w, int _h, int _id):CContainer(_x, _y, _w, _h, _id)
{
	mainHandle = _parent->getHandle();
	createWindow();
	_parent->addControl(this);
	SendMessage(getHandle(), WM_SETFONT, (WPARAM)sysFont, TRUE);

	onSelChanging = NULL;
	onSelChanged = NULL;
	oldPage = -1;
	currentPage = NULL;
}

/**
 * Creates the control
 */
bool CTabControl::createWindow()
{
	style = TCS_BUTTONS | TCS_FOCUSNEVER;
	styleEx = TCS_EX_FLATSEPARATORS ;
	wcscpy(szClassName, WC_TABCONTROL);
	return createControl();
}

/**
 * Adds a new page
 */
void CTabControl::addPage(CTabPage* _page)
{
wchar_t s[256];
TCITEM tc;
	tc.mask = TCIF_PARAM | TCIF_TEXT;
	wcsncpy(s,_page->getText().c_str(), 256);
	tc.pszText = s;
	tc.cchTextMax = wcslen(tc.pszText);
	tc.lParam = (LPARAM)_page;
	
	if(TabCtrl_InsertItem(getHandle(), TabCtrl_GetItemCount(getHandle()), &tc) == -1)
	{
		MessageBox(parent->getHandle(), L"Could not add tab page", L"Error", MB_OK);
	}

	pages.push_back(_page);
	_page->hide();
}

/**
 * Hides the given page. Used when changing
 */
void CTabControl::hidePage(int page)
{
	LOG_S("Hiding page:", pages[page]->getText());
	pages[page]->hide();
}

/**
 * Shows the page. USed when changing
 */
void CTabControl::showPage(int page)
{
	pages[page]->show();
}

/**
 * Called by the CWindow class when theselection is about to be changed
 */
LPARAM CTabControl::selChanging()
{
	LOG("Entered");
	if(onSelChanging != NULL)
	{
		onSelChanging(this);
	}
	oldPage = TabCtrl_GetCurSel(getHandle());
	return 1;
}

/**
 * Called when the selection has been changed
 */
LPARAM CTabControl::selChanged()
{
	LOG("Entered");
	if(onSelChanged != NULL)
	{
		onSelChanged(this);
	}

	hidePage(oldPage);
	showPage(TabCtrl_GetCurSel(getHandle()));
	currentPage = pages[TabCtrl_GetCurSel(getHandle())];
	return 1;
}

/**
 * Selects the page
 */
void CTabControl::selectPage(int page)
{
	LOG_S("Selected the page:", pages[page]->getText());
	TabCtrl_SetCurSel(getHandle(), page);	
	currentPage = pages[page];
	hideAllPages();
	currentPage->show();
}

/**
 * Returns the page that was selected before the current
 */
int CTabControl::getOldPage()
{
	return oldPage;
}

/**
 * Returns the current page
 */
int CTabControl::getCurrentPage()
{
	return TabCtrl_GetCurSel(getHandle());
}

/**
 * Shows the tabcontrol.
 */
void CTabControl::show()
{
	LOG("Entering...");
	ShowWindow(hwnd, SW_SHOW);
	hideAllPages();
	if(currentPage)
	{
		LOG("We have a current page");
		currentPage->show();
	}
	LOG("Leaving");
}

/**
 * Hides all the pages of the control
 */
void CTabControl::hideAllPages()
{
	LOG("Hiding all the pages");
size_t size = pages.size();
	for(size_t i=0; i<size; i++)
	{
		LOG_S("All.Hiding page:", pages[i]->getText());
		pages[i]->hide();
	}
	LOG("Leaving");
}

/**
 * Hides this control
 */
void CTabControl::hide()
{
	LOG("Entering");
	ShowWindow(hwnd, SW_HIDE);
	hideAllPages();
	LOG("Leaving");
}

/**
 * Repaints the control
 */
void CTabControl::repaint()
{
	CControl::repaint();
	if(currentPage)
	{
		currentPage->repaint();
	}
}

/**
 * Handles the owner draw items of the tab control
 */
bool CTabControl::handleOwnerDraws(LPDRAWITEMSTRUCT lpdis)
{
	LOG("Entered");
	if(!currentPage)
	{
		return false;
	}
size_t size = currentPage->controls.size();

	for(size_t i=0; i<size; i++)
	{
		LOG_S("Trying:", string(currentPage->controls[i]->szClassName));
		LOG_X("Handle:", currentPage->controls[i]->getHandle());
		if(currentPage->controls[i]->getHandle() == lpdis->hwndItem && currentPage->controls[i]->isVisible())
		{
			LOG("Drawing it");
			if(!dynamic_cast<COwnerDrawControl*>(currentPage->controls[i])->draw(lpdis))
			{
				return false;
			}
		}
	}
	return true;
}

void CTabControl::setButtonStyle()
{
	SetWindowLong(hwnd, GWL_STYLE, TCS_BUTTONS);
}
