#pragma once

#include "Control.h"

class CWindow;

/**
 * Represents a control, which can show an image (only BMP for now)
 */
class CImageControl :public CControl
{
public:

	/**
	 * Creates a new control
	 */
	CImageControl(CWindow* _parent, int _x, int _y, int _w, int _h);

	/**
	 * Destroys a control
	 */
	virtual ~CImageControl(void);

	/**
	 * Sets the image of this control. Only BMP files are supported yet
	 */
	void setImage(const wchar_t* _image);

    /**
     * This method is the one responsible for actually creating the window. Done like this because
     * every CControl derived class can have its own way to specify the flags for CreateWindow
	 * such as different class names, etc...
     */
	virtual bool createWindow();	

private:

	// the handle of the image
	HANDLE hImage;
};
