#ifndef _STATIC_H_
#define _STATIC_H_

#include "Control.h"

class CWindow;

/**
 * This class represents a static object.
 */
class CStatic : public CControl
{
public:
	/**
	 * Creates a new static object
	 */
	CStatic(CWindow* _mainw, int _x, int _y, int _w, int _h, wchar_t* _text);

	/**
	 * Destroys a static object
	 */
	virtual ~CStatic();

    /**
     * This method is the one responsible for actually creating the window. Done like this because
     * every CControl derived class can have its own way to specify the flags for CreateWindow
	 * such as different class names, etc...
     */
	virtual bool createWindow();
};

#endif 
