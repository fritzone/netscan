#ifndef _TOOLTIP_H_
#define _TOOLTIP_H_

#include "Control.h"

/**
 * This class represents a simple tooltip
 */
class CToolTip : public CControl
{
public:

    /**
     * Creates a new tooltip object.
     * @param _hInstance - the instance of the program (TODO: consider to remove this)
     * @param _hwnd - the handle of the window to which this tooltip is attached
     * @param _text - the text which will be shown
     */
    CToolTip(HINSTANCE _hInstance, HWND _hwnd, LPCTSTR _text);

    /**
     * Destroys a tooltip object
     */
    virtual ~CToolTip(void);

    /**
     * Draws the tooltip. Needed only for the virtuality of the CControl...
     */
    void draw(LPDRAWITEMSTRUCT){}

    /**
     * This method is the one responsible for actually creating the window. Done like this because
     * every CControl derived class can have its own way to specify the flags for CreateWindow
	 * such as different class names, etc...
     */
	virtual bool createWindow() ;

private:

    // the handle of the tooltip
    HWND handle;
};

#endif
