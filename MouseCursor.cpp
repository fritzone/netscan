#include "MouseCursor.h"

#include "leaker.h"

/**
 * Constructor
 */
CMouseCursor::CMouseCursor(const wchar_t* _fileName)
{
	handle = (HCURSOR)LoadImage(NULL, _fileName, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if(handle == NULL)
	{
		return;
	}
}

/**
 * Destructor
 */
CMouseCursor::~CMouseCursor()
{
	DestroyCursor(handle);
}

/**
 * Returns the handle
 */
HCURSOR CMouseCursor::getHandle() const
{
	return handle;
}
