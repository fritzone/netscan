#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include "Control.h"

#include <vector>

using namespace std;


class COwnerDrawControl;
class CTextField;

/**
 * Represents a container class. A Container is not a window class, it is more a
 * Logical class which groups controls
 */
class CContainer: public CControl
{
public:

	/**
	 * Creats a new container
	 *
	 * \param _x the X position of the container
	 * \param _y the Y position of the container
	 * \param _w the Width of the container
	 * \param _h the Height of the container
	 * \param _id the ID
	 * \return 
	 */
	CContainer(int _x, int _y, int _w, int _h, DWORD _id);

	/**
	 * Destroys a container
	 */
	virtual ~CContainer(void);

    /**
     * Adds a new control to this container
	 * \param _control the control to be added
     */
   	virtual void addControl(CControl* _control);

	/**
     * Adds a new text control to this container
	 * \param _control the text field to be added
     */
   	virtual void addControl(CTextField* _control);

	/**
     * Adds a new owner draw control to this container
	 * \param _control the control to be added
     */
   	virtual void addControl(COwnerDrawControl* _control);

	/**
	 * Removes a control from this container
	 */
	void removeControl(CControl* _control);

	/**
 	 * Removes a text field from this container
	 */
	void removeControl(CTextField* _textField);

	/**
	 * Checks whether the contaienr has this controls
	 */
	bool hasControl(CControl* _control);

	/**
	 * Hides the container and the controls
	 */
	void hide();

	/**
	 * Shows the contaienrs and the controls
	 */
	void show();

	/**
	 * Repaints the container and the controls
	 */
	void repaint();

	CControl* getControl(HWND hwnd);

public:

    // contains a list of all the controls of this window
    vector <CControl*> controls;

	// contains a list of all the controls of this window
    vector <COwnerDrawControl*> ownerDrawControls;

};

#endif
