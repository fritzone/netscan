#pragma once

#include "Control.h"

/**
 * The owner draw control is a control which is responsible fro drawing its surface.
 */
class COwnerDrawControl:public CControl
{
public:

	/**
	 * Creates a new Owner Draw control
	 */
	COwnerDrawControl(CControl* _parent, int _x, int _y, int _w, int _h, wstring _text, DWORD _id):CControl(_parent, _x, _y, _w, _h, _text, _id)
	{
		wcscpy(szClassName, L"BASIC_OWNERDRAWCONTROL");
	}

	/**
	 * Destroys the control
	 */
	virtual ~COwnerDrawControl(void)
	{
	}

	/**
	 * Draws the given control. Each control should have its own style to draw.
	 */
	virtual bool draw(LPDRAWITEMSTRUCT) = 0;
};
