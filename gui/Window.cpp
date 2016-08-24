#include "wind_upg.h"
#include "Window.h"
#include "MouseCursor.h"
#include "OwnerDrawControl.h"
#include "TabControl.h"
#include "TreeControl.h"
#include "ToolBar.h"

#include "leaker.h"

map<HWND, CWindow*> CWindow::windowInstances = map<HWND, CWindow*>();
bool CWindow::finishing = false;

/**
 * Creates a new window at the given position
 */
CWindow::CWindow(int _x, int _y, int _w, int _h, DWORD _id, HINSTANCE _hInstance):CContainer(_x, _y, _w, _h, _id)
{
	baseInstance = this;
	progInstance = _hInstance;
	lastPopupWindow = NULL;
	mainToolbar = 0;
}

/**
 * Destroys an existing window
 */
CWindow::~CWindow()
{
	UnregisterClass(szClassName, progInstance);
}

/**
 * Returns the top position of this window
 */
int CWindow::getTop()
{
WINDOWPLACEMENT a;
	a.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwnd, &a);

	return a.rcNormalPosition.top;
}

/**
 * Returns the left position of this window
 */
int CWindow::getLeft()
{
WINDOWPLACEMENT a;
	a.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwnd, &a);
	return a.rcNormalPosition.left;
}



/**
 * Handles the Notify Messages
 */
LRESULT CWindow::handleNotifyMessages(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
UINT code = ((LPNMHDR) lparam)->code;
    switch(code)
    {
    case TCN_SELCHANGE:
        {
		HWND hwndFrom = ((LPNMHDR)lparam)->hwndFrom;
        CTabControl* tabCtrl = getControlForHandle<CTabControl>(hwndFrom);
			if(tabCtrl)
			{
				return tabCtrl->selChanged();
			}
            break;
        }
        

    case TCN_SELCHANGING:
        {
		HWND hwndFrom = ((LPNMHDR)lparam)->hwndFrom;
        CTabControl* tabCtrl = getControlForHandle<CTabControl>(hwndFrom);
			if(tabCtrl)
			{
				return tabCtrl->selChanging();
			}
            break;
        }

	case TVN_ENDLABELEDIT:
		{
		HWND hwndFrom = ((LPNMHDR)lparam)->hwndFrom;
		CTreeControl* tree = getControlForHandle<CTreeControl>(hwndFrom);
			if(tree)
			{
				return tree->endLabelEdit((LPNMTREEVIEW)lparam);
			}
			break;
		}

	case TVN_BEGINLABELEDIT:
		{
		HWND hwndFrom = ((LPNMHDR)lparam)->hwndFrom;
		CTreeControl* tree = getControlForHandle<CTreeControl>(hwndFrom);
			if(tree)
			{
				return tree->startLabelEdit((LPNMTREEVIEW)lparam);
			}
			break;
		}

	case TTN_GETDISPINFO:
		{
			LPTOOLTIPTEXT lpttt; 
			lpttt = (LPTOOLTIPTEXT) lparam; 
			HWND hwndFrom = lpttt->hdr.hwndFrom;	// the tooltips handle
			HWND ttparent = GetParent(hwndFrom);	// the "main" window
			CWindow* ctrl = getControlForHandle<CWindow>(ttparent);

			CToolBar* tb = ctrl->getToolbar();
			if(tb)
			{
			int idButton = lpttt->hdr.idFrom; 
				lpttt->lpszText = (LPWSTR)tb->getTooltip(idButton);
				lpttt->uFlags |= TTF_DI_SETITEM;
				return 0;
			}
		}

	}
	return NOT_HANDLED_MESSAGE;

}

/**
 * This is the basic window procedure, which or handles some basic commands, such as
 * identifying which command handler should be called or delegates the message to one
 * of the message handlers
 */
LRESULT CWindow::basicWindowProcedure (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//LOG_MSG("Got:", msg);
	
	if(finishing)
	{
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	// first step: Find if we have a registered message handler for the given message
	if(windowInstances.find(hwnd) != windowInstances.end())
	{
		if(msg == WM_NOTIFY)
		{
		LRESULT lr = handleNotifyMessages(hwnd, msg, wparam, lparam);
			if(lr != NOT_HANDLED_MESSAGE)
			{
				return lr;
			};
		}

	// lets search for something that will handle this message
		CWindow* win = windowInstances[hwnd];
		if(win)
		{
	LRESULT res = win->handleMessage(msg, win, wparam, lparam);
			if(res != NOT_HANDLED_MESSAGE)
			{
				return res;
			}
		}
	}

	// second: see if we have a registered command handler, and handle if this is a command
	switch(msg)
	{
	case WM_COMMAND:
		{
		DWORD id = LOWORD(wparam);

			if(windowInstances.find(hwnd) != windowInstances.end())
			{
				// check if we have a registered handler for this (with the ON_COMMAND) 
				if(windowInstances[hwnd]->hasIDRegd(id))
				{
					// no, we don't have a registered handler... let's try to handle it
					if(windowInstances[hwnd]->handleExtensionCommand(id))
					{
						return 0;
					}
				}
					// yes, we have a registered handler
				windowInstances[hwnd]->handleCommand(LOWORD(wparam), windowInstances[hwnd]);
				return 0;
			}
			break;
		}

	// third: If we need to draw some items... draw them
	case WM_DRAWITEM:
		if(windowInstances.find(hwnd) != windowInstances.end())
		{
//			LOG_X("Got WM_DRAWITEM for:", ((LPDRAWITEMSTRUCT) lparam)->hwndItem);
			// find the callbacked control or something similar...
		CWindow* theWindow = windowInstances[hwnd];
			for(size_t i=0; i<theWindow->controls.size(); i++)
			{
				if(dynamic_cast<CTabControl*>(theWindow->controls[i])!=NULL)
				{
					dynamic_cast<CTabControl*>(theWindow->controls[i])->handleOwnerDraws((LPDRAWITEMSTRUCT)lparam);
				}
				else
				if(theWindow->controls[i]->getHandle() == ((LPDRAWITEMSTRUCT) lparam)->hwndItem && theWindow->controls[i]->isVisible())
				{
//					LOG_X("Will be drawn:",theWindow->controls[i]->getHandle());
					dynamic_cast<COwnerDrawControl*>(theWindow->controls[i])->draw((LPDRAWITEMSTRUCT) lparam);
				}
			}
			return TRUE;
		}

		break;
	}

	// finally: nothing was found that could handle that message, let the DefWindowProc do it
	//LOG_MSG("Message will be handled by DefWindowProc:", msg);
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

/**
 * Prepares the Open/Save dialog's structure
 */
void CWindow::prepareOSDialog(OPENFILENAME& ofn, bool open, LPCWSTR filter)
{
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetForegroundWindow();

	ofn.nMaxFile = 260;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST;

	if(open)
	{
		ofn.Flags |=  OFN_FILEMUSTEXIST;
	}
}

/**
 * Show the window as a modal dialog. The implementation is responsible for setting the
 * 'canHide' member to true when this window can disappear
 */
int CWindow::showModal()
{
	show();
	ShowWindow(hwnd, SW_SHOW);
	bringToFront();
	canHide = false;
	pressedButton = -1;
MSG messages;
	while (GetMessage (&messages, NULL, 0, 0) && !canHide)
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}

	return pressedButton;
}

/**
 * Brings this window to the front
 */
void CWindow::bringToFront()
{
	SetForegroundWindow(hwnd);
}

/**
 * Shows a messagebox
 */
void CWindow::ShowMessage(LPCWSTR _msg)
{
	MessageBox(hwnd, _msg, L"Message",  MB_OK);
}

/**
 * Docks the window in the new parent
 */
void CWindow::dock(CWindow* _newParent, bool _alwaysVisible)
{
	SetParent(hwnd, _newParent->hwnd);
	if(!_alwaysVisible)
	{
		_newParent->dockedWindows.push_back(this);
	}
	SetWindowPos(hwnd, HWND_NOTOPMOST, x, y, w, h, SWP_NOSIZE);
	//SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_STATICEDGE);
}

/**
 * In case, there are multiple docked windows show the one in the parameter and hides the others
 */
void CWindow::popupDockedWindow(CWindow* _win)
{
	for(size_t i=0; i<dockedWindows.size(); i++)
	{
		if(dockedWindows[i] != _win)
		{
			dockedWindows[i]->hide();
		}
		else
		{
			dockedWindows[i]->show();
		}
	}
	lastPopupWindow = _win;
}

/**
 * Creates the control and inserts is in the windowInstances static member
 */
bool CWindow::createControl()
{
	visible = false;
	CControl::createControl();
	if(hwnd)
	{
		windowInstances.insert(std::pair<HWND, CWindow*>(hwnd, this));
	}

	return true;
}

/**
 * Creates this window as a child of _parentWindow
 */
bool CWindow::createAsChild(CWindow *_parentWindow)
{
	mainHandle = _parentWindow->getHandle();
	return createWindow();
}

/**
 * Creates the window, including the content.
 * This includes: Registering a window class from the szClassName member,
 * creating the windows control and creating  the content.
 */
bool CWindow::createWindow()
{
	if(!createWindowClass(basicWindowProcedure))
	{
		return false;
	}

	mapMessages();
	mapCommands();

	createControl();
	createContent();
    return true;
}

/**
 * Sets the mouse cursor
 */
void CWindow::setCursor(CMouseCursor *_newCursor)
{
	SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)(_newCursor->getHandle()));
	SetCursor(_newCursor->getHandle());
}
