#include "ApplicationWindow.h"

#include "leaker.h"

/**
 * Creates the object
 */
CApplicationWindow::CApplicationWindow(int _x, int _y, int _w, int _h, DWORD _id, HINSTANCE _hInstance):
	CWindow(_x, _y, _w, _h, _id, _hInstance)
{
}

/**
 * Sets the functional implementation
 */
void CApplicationWindow::setFunctionalImplementation(CFunctionalImplementation* _functImpl)
{
	LOG("Setting the functional implementation");
	functionalImplementation = _functImpl;
}
