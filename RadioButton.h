#pragma once

#include "Control.h"

#include <vector>

using namespace std;

class CToolTip;
class CWindow;

/**
 * This class represents a classical Radio button.
 */
class CRadioButton:public CControl
{
public:
	/**
	 * Creates a new radio button.
	 * @param _parent: the parent
	 * @param _x, _y, _w, _h: location, size
	 * @param _items: string entries, separated by ;
	 * @param _tooltips: same as above
	 * @param _space: the space (horizontal) between the items. If this value is 
	 */
	CRadioButton(CWindow* _parent, int _x, int _y, int _w, int _h, int _id, wchar_t* _items, wchar_t* _tooltips, int _space);

	/**
	 * Destructor
	 */
	~CRadioButton(void);

	/**
	 * Returns the selected item or -1 if nothing is selected
	 */
	int getSelectedItem();

	/**
	 * Sets the selected item to the one specified
	 */
	void setSelectedItem(size_t _idx);

    /**
     * This method is the one responsible for actually creating the window. Done like this because
     * every CControl derived class can have its own way to specify the flags for CreateWindow
	 * such as different class names, etc...
     */
	virtual bool createWindow();

	/**
	 * Shows the 'buttons' ofthis radio button. Overrides the standard 'show' mechanism
	 * because the radio button has more than one handle to show
	 */
	virtual void show();

	/**
	 * Hides the control.
	 */
	virtual void hide();

private:
	// the handles of the buttons...
	vector<HWND> handles;

	// the tooltips for each entry
	vector<CToolTip*> toolTips;
};
