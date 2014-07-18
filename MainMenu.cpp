#include "MainMenu.h"
#include "MainWindow.h"

#include "leaker.h"

/**
 * Creates the main menu of the application
 */
CMainMenu::CMainMenu(CWindow* _parent)
{
	parent = _parent;

	handle = CreateMenu();

	fileMenu = CreatePopupMenu();
	helpMenu = CreatePopupMenu();

	AppendMenu(fileMenu, MF_STRING, MENU_FILE_NEW, L"&New\tCtrl+N");
	AppendMenu(fileMenu, MF_STRING, MENU_FILE_OPEN, L"&Open\tCtrl+O");
	AppendMenu(fileMenu, MF_STRING, MENU_FILE_SAVE, L"&Save\tCtrl+S");
	AppendMenu(fileMenu, MF_STRING, MENU_FILE_SAVEAS, L"Save &as...");
	AppendMenu(fileMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(fileMenu, MF_STRING, MENU_FILE_EXPORT, L"&Export...");
	AppendMenu(fileMenu, MF_STRING, MENU_FILE_IMPORT, L"&Import...");
	AppendMenu(fileMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(fileMenu, MF_STRING, MENU_FILE_EXIT, L"E&xit\tAlt+F4");

	AppendMenu(helpMenu, MF_STRING, MENU_HELP_ABOUT, L"&About...");

	AppendMenu(handle, MF_POPUP | MF_STRING, (UINT_PTR)fileMenu, L"&File");
	AppendMenu(handle, MF_POPUP | MF_STRING, (UINT_PTR)helpMenu, L"&Help");
}

/**
 * Destructor
 */
CMainMenu::~CMainMenu()
{
	DestroyMenu(helpMenu);
	DestroyMenu(fileMenu);
	DestroyMenu(handle);
}

/**
 * Returns the handle of the menu
 */
HMENU CMainMenu::getHandle()
{
	return handle;
}
