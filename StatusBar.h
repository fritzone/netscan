#ifndef _STATUSBAR_H_
#define _STATUSBAR_H_

#include "Control.h"

class CWindow;

/**
 * Class representing a Status bar control
 */
class CStatusBar:public CControl
{
public:
	/**
	 * Creates a new Status bar
	 * @param _parent : is the parent window
	 * @param _id : is the ID of the status bar
	 * @param _parts : is the number of parts
	 * @param _partPos : is a vector of ints containing the start position of each part
	 */
	CStatusBar(CWindow* _parent, int _id, int _parts, int* _partPos);

	virtual bool createWindow() {return true;}

	void autoSize();

	void setText(LPCWSTR text, int idx);

};

#endif
