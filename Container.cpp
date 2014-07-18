#include "Container.h"

#include "Debugger.h"

#include "Control.h"
#include "OwnerDrawControl.h"
#include "TabControl.h"
#include "TextField.h"
#include "Static.h"

/**
 * Constructor
 */
CContainer::CContainer(int _x, int _y, int _w, int _h, DWORD _id):CControl(NULL, _x, _y, _w, _h, L"", _id)
{
	wcscpy(szClassName, L"BASIC_CONTAINER");
}

/**
 * Destructor
 */
CContainer::~CContainer(void)
{
}

/**
 * Adds a text field to this container
 */
void CContainer::addControl(CTextField* _control)
{
	controls.push_back(_control);
	controls.push_back(_control->staticText);
}

/**
 * Adds a control to this container
 */
void CContainer::addControl(CControl* _control)
{
	controls.push_back(_control);
}

/**
 * Adds an owner darwn control to this container
 */
void CContainer::addControl(COwnerDrawControl* _control)
{
	controls.push_back(_control);
	ownerDrawControls.push_back(_control);
}

/**
 * Hides the container
 */
void CContainer::hide()
{
size_t ctrlSize = controls.size();
	for(size_t i=0; i<ctrlSize; i++)
	{
		controls[i]->hide();
	}
	CControl::hide();
}

/**
 * Shows the container
 */
void CContainer::show()
{
size_t ctrlSize = controls.size();
	for(size_t i=0; i<ctrlSize; i++)
	{
		if(dynamic_cast<CTabControl*>(controls[i]))
		{
			dynamic_cast<CTabControl*>(controls[i])->show();
		}
		else
		{
		CControl* ctrlI = controls[i];
			ctrlI->show();
		}
	}
	CControl::show();
}

/**
 * Removes the specified control
 */
void CContainer::removeControl(CControl *_control)
{
size_t i=0;
	while(i<controls.size())
	{
	CControl* ctrlI = controls[i];
		if(controls[i] == _control)
		{
			controls.erase(controls.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

/**
 * Removes the specified control
 */
void CContainer::removeControl(CTextField* _textField)
{
size_t i=0;
	while(i<controls.size())
	{
		if(controls[i] == _textField)
		{
			if(dynamic_cast<CContainer*>(_textField->parent))
			{
				dynamic_cast<CContainer*>(_textField->parent)->removeControl(_textField->staticText);
			}
			controls.erase(controls.begin() + i);
			return;
		}
		i++;
	}
}

/**
 * Checks if the container hs the given control or not
 */
bool CContainer::hasControl(CControl *_control)
{
size_t ctrlSize = controls.size();
	for(size_t i=0; i<ctrlSize; i++)
	{
		if(controls[i] == _control)
		{
			return true;
		}
	}
	return false;
}

/**
 * Repaints the container
 */
void CContainer::repaint()
{
size_t ctrlSize = controls.size();
	for(size_t i=0; i<ctrlSize; i++)
	{
		controls[i]->repaint();
	}
	CControl::repaint();
}

/**
 * Returns the control with the given handle
 */
CControl* CContainer::getControl(HWND hwnd)
{
size_t ctrlSize = controls.size();
	for(size_t i=0; i<ctrlSize; i++)
	{
		if(controls[i]->getHandle() == hwnd)
		{
			return controls[i];
		}
	}
	return NULL;
}