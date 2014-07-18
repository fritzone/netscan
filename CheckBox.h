#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include "Control.h"

class CToolTip;
class CWindow;

/**
 * This class represents a checkbox
 */
class CCheckBox : public CControl
{
public:
	/**
	 * Creates a new checkbox
     * \param _parent - the window to which this toolbar is attached
     * \param _x - the x positino of the list box
     * \param _y - the y position of the list box
     * \param _w - the width of the list box
     * \param _h - the height of the list box
     * \param _id - the unique ID of the list box
	 * \param _text - the text of the checkbox
	 * \param _tooltip - the text of the tooltip of the checkbox. Leave empty for no tooltip.
 	 */
	CCheckBox(CWindow* _parent, int _x, int _y, int _w, int _h, int _id, wchar_t* _text, LPCWSTR _tooltip);

	/**
	 * Destroys an existing checkbox
	 */
	virtual ~CCheckBox(void);

	/**
	 * Tells if the chckbox is checked or no
	 */
	bool isChecked();

	/**
     * This method is the one responsible for actually creating the window. Done like this because
     * every CControl derived class can have its own way to specify the flags for CreateWindow
	 * such as different class names, etc...
     */
	virtual bool createWindow();

	/**
	 * To assign it to a value
	 */
	void operator = (bool _v);

private:

	// if the buttn is having a tooltip, define it here
	CToolTip* toolTip;

};

#endif
