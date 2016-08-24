#ifndef _TABPAGE_H_
#define _TABPAGE_H_

#include "Container.h"

class CTextField;

/**
 * The tab page is a page that is present in a tab control. A Tab page can contain controls.
 */
class CTabPage: public CContainer
{
public:

	/**
	 * Creates a new control
	 */
	CTabPage(const wchar_t* _caption);

	/**
     * Adds a new text field to this container, and adds the static of it.
     */
   	virtual void addTextControl(CTextField* _control);

	/**
	 * Crates the window
	 */
	virtual bool createWindow();

	/**
	 * Returns the data of the tab page
	 */
	void* getData() const
	{
		return data;
	}

	/**
	 * Sets the data of this tab page
	 */
	void setData(void* _newData)
	{
		data = _newData;
	}

private:
	/**
	 * The caption of the tab page
	 */
	wstring caption;

	/**
	 * The user level data of the tab page
	 */
	void* data;
};

#endif
