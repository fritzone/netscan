#ifndef _ABOUTWINDOW_H_
#define _ABOUTWINDOW_H_

#include "Window.h"
#include "control_ids.h"

class CPushButton;
class CStatic;
class CMainWindow;

/**
 * This window has a short about message... Nothing spectacular
 */
class CAboutWindow : public CWindow
{
public:

	/**
	 * Creates a new object
	 */
	CAboutWindow(HINSTANCE);

	/**
	 * Destructor
	 */
	virtual ~CAboutWindow();

	/**
	 * On Btn Ok messageHandler
	 */
	static void onBtnOk(CAboutWindow*);

	/**
	 * Creates the content of the window
	 */
	virtual bool createContent();

	/**
	 * Overridden function, but does nothing
	 */
	virtual void repaint(){}

public:

	/*
	 * Message handling
	 */
	BEGIN_COMMAND_MAP(CAboutWindow)
		ON_COMMAND(BTN_OK_ABT, onBtnOk)
	END_COMMAND_MAP(CAboutWindow)

	BEGIN_MESSAGE_MAP(CAboutWindow)
	END_MESSAGE_MAP(CAboutWindow)

public:

	// the instance of the main window
	CMainWindow* mainWindow;

private:

	// the button which will close the window
	CPushButton* btnOk;

	// the about text...
	CStatic* aboutText1;
	CStatic* aboutText2;

	// the instance of this window
	static CAboutWindow* instance;
};

#endif
