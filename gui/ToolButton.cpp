#include "ToolButton.h"
#include "ToolBar.h"

#include "leaker.h"

/**
 * Creates a new tool button
 */
CToolButton::CToolButton(int _imageIndex, LPCWSTR _text, DWORD _id, LPCWSTR _tooltip, BYTE _style):CControl(NULL, 0, 0, 0, 0, L"", _id)
{
	imageIndex = _imageIndex;

	button.idCommand = _id;
	button.iString = (UINT_PTR)_text ;
	button.fsState = TBSTATE_ENABLED ;
	button.fsStyle = BTNS_BUTTON | _style;
	button.dwData = 0;
	button.iBitmap = _imageIndex;

	tooltip = _tooltip;
}

/**
 * Destructor
 */
CToolButton::~CToolButton(void)
{
}

/**
 * Presses this button
 */
void CToolButton::press()
{
	SendMessage(parent->getHandle(), TB_PRESSBUTTON, id, TRUE );
}

/**
 * Checks this button
 */
void CToolButton::check()
{
	SendMessage(parent->getHandle(), TB_CHECKBUTTON, id, TRUE );
}

/**
 * Checks this button
 */
void CToolButton::uncheck()
{
	SendMessage(parent->getHandle(), TB_CHECKBUTTON, id, FALSE );
}


/**
 * Returns the state of the button
 */
DWORD CToolButton::getState() const
{
	return (DWORD)SendMessage(parent->getHandle(), TB_GETSTATE, id, 0);
}

/**
 * Does nothing
 */
bool CToolButton::createWindow()
{
	return true;
}
