#ifndef _COMBOBOX_H_
#define _COMBOBOX_H_

#include "Control.h"

#include <string>
#include <vector>

using namespace std;

class CWindow;

/**
 * Combo Box Class
 */
class CComboBox : public CControl
{
public:

	/**
	 * Creates a new Combo Box
	 *
	 * \param _parent the parent window of this control
	 * \param _x the X position
	 * \param _y the Y position
	 * \param _w the Width
	 * \param _h the Height
	 * \param _id the ID of the control
	 * \return 
	 */
	CComboBox(CWindow* _parent, int _x, int _y, int _w, int _h, DWORD _id);

	/**
	 * Actually creates the window
	 */
	bool createWindow() {return true;}

	/**
	 * Adds the given string to the content of the combo box
	 */
	void addString(string);

	/**
	 * Destructor
	 */
	~CComboBox(void);

private:
	vector<string> strings;
};

#endif
