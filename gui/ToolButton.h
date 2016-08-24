#ifndef _TOOLBUTTON_H_
#define _TOOLBUTTON_H_

#include "Control.h"

class CToolBar;

/**
 * This class represents a tool button. Tool buttons are placed on tool bars...
 */
class CToolButton : public CControl
{
public:

	friend class CToolBar;

	/**
	 * Creates a Tool button.
	 * @param _imageIndex is the index of the image that was loaded to the toolbar.
	 * @param _text is the text of the button... Not really used
	 * @param _id is the unique ID of the toolbutton
	 * @param _style is the extended style of the button (Check like, Group like)
	 */
	CToolButton(int _imageIndex, LPCWSTR _text, DWORD _id, LPCWSTR _tooltip, BYTE _style=0);

	/**
	 * Destroys a button
	 */
	virtual ~CToolButton(void);

	/**
	 * Presses this toolbutton
	 */
	void press();

	/**
	 * Checks this toolbutton
	 */
	void check();

	/**
	 * Unchecks this button
	 */
	void uncheck();

	/**
	 * Returns the state of this toolbutton
	 */
	DWORD getState() const;

	/**
     * This method is the one responsible for actually creating the window. Done like this because
     * every CControl derived class can have its own way to specify the flags for CreateWindow
	 * such as different class names, etc...
     */
	virtual bool createWindow();

private:

	// The Win32 API structure
	TBBUTTON button;

	// the image of the button
	int imageIndex;

	// the tooltip of the button
	wstring tooltip;

	// the parent toolbar
	CToolBar* parent;
};

#endif
