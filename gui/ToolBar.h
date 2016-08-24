#ifndef _TOOLBAR_H_
#define _TOOLBAR_H_

#include "wind_upg.h"

#include "ImageList.h"
#include "ToolButton.h"

#include <vector>

using namespace std;

class CToolBar;
typedef bool (*TOOLBAR_NOTIFY_CALLBACK)(CToolBar*);

/**
 * GUI class, responsible for creating and managin the tooblar controls.	
 */
class CToolBar : public CControl
{
public:

	friend class CToolButton;

	/**
	 * Creates a new toolbar
	 */
	CToolBar(CControl* _parent, LPCWSTR _images, int _nButtons);

	/**
	 * Destroys a toolbar
	 */
	virtual ~CToolBar();

    virtual bool createWindow(){return true;};

	/**
	 * Adds a button.
	 */
	void CToolBar::addButton(CToolButton* _button);

	/**
	 * Adds a separator to the toolbar
	 */
	void addSeparator();

	/**
	 * Returns the tooltip from the given button
	 */
	const wchar_t* getTooltip(int _id);

	/**
	 * Forces the button to have a pressed state
	 */
	void pressButton(CToolButton* _btn);

    void autoSize();

private:

	// the tool buttons contained
	vector<CToolButton*> buttons;
};

#endif
