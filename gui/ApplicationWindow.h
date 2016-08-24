#ifndef _APPLICATIONWINDOW_H_
#define _APPLICATIONWINDOW_H_

#include "Window.h"

class CFunctionalImplementation;
class CParameterObject;

/**
 * This class represents a window from the application... Actually it's a way to group
 * some common properties together that all application windows will share... such as 
 * the "Business implementation"
 */
class CApplicationWindow: public CWindow
{
public: 
	/**
	 * Constructor
	 */
	CApplicationWindow(int _x, int _y, int _w, int _h, DWORD _id, HINSTANCE _hInstance);

	/**
	 * Sets the functional implementation
	 */
	void setFunctionalImplementation(CFunctionalImplementation*);

	/**
	 * Returns the functional implementation
	 */
	CFunctionalImplementation* getFunctionalImplementation() const
	{
		return functionalImplementation;
	}

	/**
	 * Updates the content of the edits to reflect the data of the current object. This must be overridden in
	 * All the classes that use this
	 */
	virtual void updateContent() = 0;

	/**
	 * This sets the object of the docked window
	 */
	virtual void setObject(CParameterObject*) = 0;
	
	/**
	 * Windows that must be repainted will override this
	 */
	virtual void repaint()
	{
		InvalidateRect(hwnd, NULL, TRUE);
	}
public:

	/*
	 * Fake Command handling. This way the childs don't need to declare it if they don't want to
	 */
	BEGIN_COMMAND_MAP(CApplicationWindow)
	END_COMMAND_MAP(CApplicationWindow)

	/*
	 * Fake Message handling
	 */
	BEGIN_MESSAGE_MAP(CApplicationWindow)
	END_MESSAGE_MAP(CApplicationWindow)

protected:
	// the functional implementation object
	CFunctionalImplementation* functionalImplementation;
};

#endif
