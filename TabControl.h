#ifndef _TABCONTROL_H_
#define _TABCONTROL_H_

#include "Container.h"

#include <vector>

using namespace std;

class CWindow;
class CTabPage;

/**
 * This class is a TabControl (just the way it's defined in the MSDN).
 * This is NOT a property sheet, it has its own way of handling the 
 * adding of controls, and displaying them in the correct tab.
 */
class CTabControl : public CContainer
{
public:

	// This is a callback that can be used in different cases, such as TCN_SELCHANING handling
	typedef bool (*TABCONTROL_CALLBACK)(CTabControl*);

	/**
	 * Creates a new tab control, at the given position and size, with the given ID.
	 */
	CTabControl(CContainer* _parent, int _x, int _y, int _w, int _h, int _id);

	/**
     * This method is the one responsible for actually creating the window. Done like this because
     * every CControl derived class can have its own way to specify the flags for CreateWindow
	 * such as different class names, etc...
     */
	virtual bool createWindow();

	/**
	 * Adds a tab page. USer firstly must create the tab pages, and after this they must populate them
	 * after this they should add the tab page to the tab control
	 */
	void addPage(CTabPage*);

	/**
	 * Returns the tab page with the given index
	 */
	CTabPage* getPage(int _idx) const
	{
		return pages[_idx];
	}

	/**
	 * Does nothing special, but calls the 'onSelChanging' Callback method (if any)
	 */
	LPARAM selChanging();

	/**
	 * Does nothing special, but calls the 'onSelChanged' Callback method (if any)
	 */
	LPARAM selChanged();

	/**
	 * Hides the given page
	 */
	void hidePage(int);

	/**
	 * Shows the given page
	 */
	void showPage(int);

	/**
	 * Selects the specified page
	 */
	void selectPage(int);

	/**
	 * Returns the index of the page that was selected before the actual page
	 */
	int getOldPage();

	/**
	 * Return the index of the current page
	 */
	int getCurrentPage();

	/**
	 * Shows the tab control
	 */
	void show();

	/**
	 * Hides the tab control
	 */
	void hide();

	/**
	 * Repaints the tab control
	 */
	void repaint();

	/**
	 * Handles the owner drawn items placed in this tab control
	 */
	bool handleOwnerDraws(LPDRAWITEMSTRUCT lpdis);

	void setButtonStyle();

public:

	// called when the selection is about to be changed. This is a callback supplied by the parent of the tab control
	TABCONTROL_CALLBACK onSelChanging;
	
	// called when the selection has been changed. This is a callback supplied by the parent of the tab control
	TABCONTROL_CALLBACK onSelChanged;

private:

	/**
	 * Hides all the pages of the tab control
	 */
	void hideAllPages();

private:

	// the pages of the tab control
	vector<CTabPage*> pages;

	// the old page
	int oldPage;

	// the current page
	CTabPage* currentPage;
};

#endif
