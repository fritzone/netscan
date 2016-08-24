#ifndef _PUSHBUTTON_H_
#define _PUSHBUTTON_H_

#include "OwnerDrawControl.h"

class CToolTip;
class CWindow;

class CPushButton : public COwnerDrawControl
{
public:

	// the types of the Buttons

	// simple pushbutton with normal text
	static const int BTN_TXT = 1;

	// simple push button with image instead of text
	static const int BTN_BMP = 2;

	// simple pushbutton with a given color
	static const int BTN_COLOR = 4;

public:
	/**
	 * Creates a new Button
	 * @param _mainw is the main window on whcih this button resided
	 * @param (_x,_y) - the coordinates where this button is
	 * @param (_w,_h) - the width and height of the button
	 * @param _id  - the id of the button
	 * @param _text - if the style is BTN_TXT the text that will be drawn on the button, else the name of the bitmap
	 * @param _Style - the style of the button, can be: BTN_TXT or BTN_BMP
	 */
	CPushButton(CWindow* _parent, int _x, int _y, int _w, int _h, DWORD _id, const wchar_t* _text, const wchar_t* _tooltip = NULL, int _style = BTN_TXT);

	/**
	 * Destructor
	 */
	virtual ~CPushButton();

	/**
     * This method is the one responsible for actually creating the window. Done like this because
     * every CControl derived class can have its own way to specify the flags for CreateWindow
	 * such as different class names, etc...
     */
	virtual bool createWindow();

	/**
	 * Will draw the bitmap on the given device context
	 */
	virtual bool draw(LPDRAWITEMSTRUCT);

	/**
	 * Sets the color of the button if style == BTN_COLOR
	 */
	void setColor(COLORREF);
    
private:
    // if the buttn is having a tooltip, define it here
    CToolTip* toolTip;

	// the color if the button is BTN_COLOR
	COLORREF color;
};

#endif
