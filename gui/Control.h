#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "wind_upg.h"

typedef unsigned long DWORD ;

#include <string>
#include <map>

using namespace std;

class CContainer;

/**
 * The Control class is the base class for all the controls found in the interface.
 */
class CControl
{
public:

	/**
	 * Creates a new control without any coordinates, just a simple id. Menus use this construct
	 */
	CControl(DWORD _id);

	/**
	 * Creates a new control
	 * @param id - the ID of the control
	 */
	CControl(CControl* _parent, int _x, int _y, int _w, int _h, wstring _text, DWORD _id);

	/**
	 * Virtual destructor
	 */ 
	virtual ~CControl()
	{
	}

	/**
	 * Hides the control
	 */
	virtual void hide();

	/**
	 * Shows the controls
	 */
	virtual void show();

	/**
	 * Returns the ID of the control
	 */
	DWORD getId() const;

	/**
	 * Returns the handle of the control
	 */
	HWND getHandle() const;

	/**
	 * Brings the control to the front
	 */
	void bringToFront();

	/**
	 * Sends this back to the bottom...
	 */
	void sendToBack();

	/**
	 * Repaints the control
	 */
	void repaint();

	/**
	 * Checks if the control is visible or not
	 */
	bool isVisible();

	/**
	 * Sets the text of this text field
	 */
	void setText(LPCWSTR);

	/**
	 * Returns the text of this edit
	 */
	wstring getText();

	/**
	 * Disables the control
	 */
	void disable();

	/**
	 * Enables the control
	 */
	void enable();

	/**
	 * Sets the focus to this window
	 */
	void setFocus();

	/**
	 * Updates the window
	 */
	void update();

	/**
	 * Creates the class for this object
	 */
	bool createWindowClass(WNDPROC _windowProcedure);

	/**
	 * Creates the control (ie. Calls CreateWindowEx, it assumes the user has filled up all the 
	 * properties he needs and created the class in case of a normal window)
	 */
	bool createControl();

    /**
     * This method is the one responsible for actually creating the window. Done like this because
     * every CControl derived class can have its own way to specify the flags for CreateWindow
	 * such as different class names, etc...
     */
	virtual bool createWindow() = 0;

	/**
	 * Returns a fancy control for the given handle
	 */
	template<class T>
	static T* getControlForHandle(HWND hwndFrom)
	{
		if(allControls.find(hwndFrom) != allControls.end())
		{
			return (T*)allControls[hwndFrom];
		}
		return NULL;
	};

	void move(RECT& rc);

	int width() const {return w;}
	int height() const {return h;}
	int left() const {return x;}
	int top() const {return y;}

public:
	// the handle of the control
	HWND hwnd;

	// the tooltips handle
	HWND hwndTT;

	// the handle of the main window
	HWND mainHandle;
	CControl* parent;

	HINSTANCE progInstance;

	// the ID of the control
	DWORD id;

	// the handle of the bitmap attached to this control when selected
	HBITMAP bmpSel;

	// the handle of the bitmap attached to this control when not selected
	HBITMAP bmpNoSel;

	// x position
	int x;

	// y position
	int y;

	// width
	int w;

	// height
	int h;

	// the text "printed" on this control
	wchar_t text[256];

	// whether the control is visible or not
	bool visible;

	// the window class of this control
	WNDCLASSEX windowClass;

	// the style of this control, used by CreateWindow
	int style;

	// the Extended style used by CreateWindowEx
	int styleEx;

	// the class name. Each derived window must initialize this
	wchar_t szClassName[256];

	// holds a map of control handles and controls from all the application
	static map<HWND,CControl*> allControls;
};

#endif
