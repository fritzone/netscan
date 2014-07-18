#ifndef _MOUSECURSOR_H_
#define _MOUSECURSOR_H_

#include "wind_upg.h"

/**
 * Holds the data for a mouse Cursor
 */
class CMouseCursor
{
public:
	/**
	 * Constructor
	 */
	CMouseCursor(const wchar_t* _fileName);

	/**
	 * Destructor
	 */
	~CMouseCursor();

	/**
	 * Returns the handle
	 */
	HCURSOR getHandle() const;

private:
	// the cursors handle
	HCURSOR handle;
};

#endif
