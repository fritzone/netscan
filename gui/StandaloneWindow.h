#ifndef _STANDALONE_WINDOW_H_
#define _STANDALONE_WINDOW_H_

#include "ApplicationWindow.h"

/**
 * Class representing a single application window without assigned object (such as point)
 */
class CStandaloneWindow : public CApplicationWindow
{
public:

	/**
	 * Creates a new window
	 */
	CStandaloneWindow(int _x, int _y, int _w, int _h, DWORD _id, HINSTANCE _hInstance);

	/**
	 * Destructor
	 */
	virtual ~CStandaloneWindow(void);

	/**
	 * Standalone windows don't have to update their content
	 */
	virtual void updateContent() {};

	/**
	 * Standalone windows don't have this 
	 */
	virtual void setObject(CParameterObject*){};

};

#endif
