#include "AboutWindow.h"
#include "PushButton.h"
#include "Static.h"

#include "leaker.h"

CAboutWindow* CAboutWindow::instance = NULL;

/**
 * Creates a new window
 */
CAboutWindow::CAboutWindow(HINSTANCE hInst):CWindow(0,
														  0,
														  224, 
														  244, 
														  0, 
														  hInst)
{
	progInstance = hInst;
	instance = this;
	wcscpy(szClassName, L"aboutWindow");
	style = WS_DLGFRAME;
	styleEx = 0;
	mainWindow = NULL;

	createWindow();
}

/**
 * Destructor
 */
CAboutWindow::~CAboutWindow()
{
	delete btnOk;
	delete aboutText1;
	delete aboutText2;
}

/**
 * Creates the content of the window
 */
bool CAboutWindow::createContent()
{
	btnOk = new CPushButton(this, 5, 187, 75, 25, BTN_OK_ABT, L"Ok", L"Done...");
	aboutText1 = new CStatic(this, 5,5, 200, 14, L"NetScan 0.2 (c) fritzone 2010-2014");

	return true;
}

/**
 * When the ok was pressed closes the window
 */
void CAboutWindow::onBtnOk(CAboutWindow* instance)
{
// TODO: implement a normal modal dialog stuff
	instance->canHide = true;	
	instance->hide();
}
