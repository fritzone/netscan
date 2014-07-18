#ifndef _IPCONTROL_H_
#define _IPCONTROL_H_

#include "Control.h"

#include <string>

class CWindow;
class CStatic;

using namespace std;

/**
 * This class represents an editable text field. The text fields can have a label which is a separate
 * static control (CStatic class)
 */
class CIPControl: public CControl
{
public:

	/**
	 * Creates a new text field
	 * @param _mainw is the window on which this ip control will be
	 * @param _x,_y are the positions where this ip control will be
	 * @param _w,_h are the dimensions of the ip control
	 * @param _ID ist he ID of this ip control
	 * @param _statText id the text that will be shown on the static before the text field. if NULL static won't be created
	 * @param _stw is the width of the static control. If 0, no static will be created 
	 */
	CIPControl(CWindow* _mainw, int _x, int _y, int _w, int _h, DWORD _ID, const wchar_t* _statText = NULL, int _stw = 0);

	/**
	 * Destructor
	 */
	virtual ~CIPControl();

	/**
	 * Returns the value of this text field as a number
	 */
	long double asNumber();

	/**
	 * Sets the text of the edit to the given long double value
	 */
	void operator = (long double _v);

	/**
	* Sets the text of the edit to the given long double value
	*/
	void operator = (int _v);

	/**
	 * Sets the text ofthe edit to the given string
	 */
	void operator = (wstring _s);

	/**
	 * Hides the control
	 */
	void hide();

	/**
	 * Shows the controls
	 */
	void show();

	/**
	 * Sets the text of the label of the edit
	 */
	void setLabel(wstring);

	/**
     * This method is the one responsible for actually creating the window. Done like this because
     * every CWindow derived class can have its own way to specify the flags for CreateWindow
     */
	virtual bool createWindow();

public:

	// the handle of the static before the text field
	CStatic* staticText;
};


#endif
