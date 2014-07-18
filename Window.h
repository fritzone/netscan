#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "Debugger.h"

#include "Container.h"
#include "Control.h"

#include <vector>
#include <map>

using namespace std;

class CMouseCursor;
class CToolBar;

// message handlers should return this value if they don't want to handle the message
const DWORD NOT_HANDLED_MESSAGE = -1;

/*
 * This defines the messaging structures used in the program.
 * For each command handler (Im' talking about Buttons ID's)
 * the programmer must declare it as: ON_COMMAND(BUTTON_ID, handler)
 * in the class CDerivedWindow, where handler must be:
 * static void handler(CDerivedWindow* instance);
 */
#define BEGIN_COMMAND_MAP(CLASS) public:\
	typedef void (*CMDHANDLER) (CLASS*);	\
	map<DWORD, CMDHANDLER> cmdMap; \
	/** \
     * This is the method that maps the commands to the handlers. \
     */ \
	virtual void mapCommands() { 

/**
 * This macro assigns to the "press" event of the BUTTON variable (which must
 * be of type CButton*) the given handler. This should be handled in the WM_COMMAND message
 */
#define ON_COMMAND(BUTTON, PHANDLER) \
		cmdMap.insert(std::pair<DWORD,CMDHANDLER>(BUTTON,(CMDHANDLER)PHANDLER));\
		LOG(string("Button: "#BUTTON)+string(" will be handled by "#PHANDLER));

/**
 * This macro assigns to the "press" event of the BUTTON variable (which must
 * be of type CButton*) the given handler
 */
#define ON_BUTTON(BUTTON, PHANDLER) \
		cmdMap.insert(std::pair<DWORD,CMDHANDLER>(BUTTON->getId(),(CMDHANDLER)PHANDLER));\
		LOG(string("Button: "#BUTTON)+string(" will be handled by "#PHANDLER));

/**
 * This finalizes the command handler macro definitions. Also declares the method which will
 * execute the commandhandlers in case of the command
 */
#define END_COMMAND_MAP(CLASS) \
	} \
	bool handleCommand(DWORD id, CWindow* instance) \
	{\
		if(cmdMap.find(id) != cmdMap.end())\
		{\
			CMDHANDLER cmdHandler = cmdMap[id];\
			cmdHandler(dynamic_cast<CLASS*>(instance));\
			return true;\
		}\
		else \
		{\
			return false;\
		}\
	}

/**
 * This defines the messaging structures used in the program.
 * For each message handler (Im' talking about windows messages (WM...))
 * the programmer must declare it as: ON_MESSAGE(WM_MESSAGE, handler)
 * in the class CDerivedWindow, where handler must be:
 * static void handler(CDerivedWindow* instance, LPARAM lParam, WPARAM wParam);
 */
#define BEGIN_MESSAGE_MAP(CLASS) public:\
	typedef LRESULT (*MSGHANDLER) (CLASS*, WPARAM, LPARAM);	\
	map<DWORD, MSGHANDLER> msgMap; \
	virtual void mapMessages() { \
		wstring className = wstring(L#CLASS);
	
/**
 * This macro assigns to a message the method that will handle it.
 */
#define ON_MESSAGE(message, PHANDLER) \
		msgMap.insert(std::pair<DWORD, MSGHANDLER>(message,(MSGHANDLER)PHANDLER));

/**
 * This method declares the method that will handle the message, ie. delegate it to the assigned handler
 */
#define END_MESSAGE_MAP(CLASS) \
	} \
	LRESULT handleMessage(DWORD message, CWindow* instance, WPARAM wParam, LPARAM lParam) \
	{\
		if(msgMap.find(message) != msgMap.end())\
		{\
		MSGHANDLER msgHandler = (MSGHANDLER) msgMap[message];\
			return msgHandler(dynamic_cast<CLASS*>(instance), wParam, lParam);\
		}\
		else\
		{\
			return NOT_HANDLED_MESSAGE;\
		}\
	}\


/**
 * This class represents a window that can be shown on the screen.
 */
class CWindow : public CContainer
{
public:
    /**
     * Creates a new window object. Basically initializes some basic things to default values, such
     * as coodinate, size, etc...
     *
     * @param _x - the x position of the window
     * @param _y - the y position of the window
     * @param _w - the width of the window
     * @param _h - the height of the window
     * @param _id - the identifier of the window
     */
	CWindow(int _x, int _y, int _w, int _h, DWORD _id, HINSTANCE _hInstance);

    /**
     * Destroys an existing window
     */
	virtual ~CWindow();

    /**
	 * Creates the content of the window, ie. all the buttons, controls, etc...
	 * The programmer responsible for creating the window should override this method
	 * to create the actual content of the window
	 */
	virtual bool createContent() = 0;

    /**
     * This method is the one responsible for actually creating the window. The one who creates
	 * the window is responsible to call this method after new CMyWindow(hInstance).
	 * Creates the window, including the content.
	 * This includes: Registering a window class from the szClassName member,
	 * creating the windows control and creating  the content.
     */
	bool createWindow();

    /**
     * This is the Window procedure. Since static members cannot be made virtual too...
     * It's up to the coder to be aware of this issue
     */
    static LRESULT CALLBACK basicWindowProcedure (HWND, UINT, WPARAM, LPARAM);

	/**
	 * Returns the top coordinate of the window
	 */
	int getTop();

	/**
	 * Returns the left coordinate of the main window
	 */
	int getLeft();

	/**
	 * Implement a dummy draw, because this is not requried for the windows
	 */
	virtual void draw(LPDRAWITEMSTRUCT dis)
	{
	}

	/**
	 * Creates the control
	 */
	virtual bool createControl();

	/**
	 * Shows the window as a modal dialog. Implementation is responsible for turning the canHide value
	 * to true when the window can be removed, and to hide the window
	 */
	int showModal();

	/**
	 * Brings the window to front
	 */
	void bringToFront();

	/**
	 * Shows the given message
	 */
	void ShowMessage(LPCWSTR);

	/**
	 * Docks this window into the new parent
	 */
	void dock(CWindow* _newParent, bool _alwaysVisible = false);

	/**
	 * Shows the _win (which is a docked window in this one) and hides the other docked windows.
	 */
	void popupDockedWindow(CWindow* _win);

	/**
	 * Handles the message
	 */
	virtual LRESULT handleMessage(DWORD message, CWindow* instance, WPARAM wParam, LPARAM lParam) = 0;

	/**
	 * Handles the command
	 */
	virtual bool handleCommand(DWORD id, CWindow* instance) = 0;

	/**
	 * Repaints the window
	 */
	virtual void repaint() = 0;

	/**
	 * Creates the window as a child of _parentWindow
	 */
	bool createAsChild(CWindow* _parentWindow);

	/**
	 * Sets the cursor to the parameter
	 */
	void setCursor(CMouseCursor* _newCursor);

	/**
	 * Prepares an Open/Save dialog's structure
	 */
	static void prepareOSDialog(OPENFILENAME&, bool open, LPCWSTR filter);

	/**
	 * Message mapping method, implemented by the BEGIN_MESSAGE_MAP macro in the derived classes
	 */
	virtual void mapMessages() = 0;

	/**
	 * Command mapping method, implemented by the BEGIN_COMMAND_MAPPING macro in the derived class.
	 */
	virtual void mapCommands() = 0;

	/**
	 * Handles the extensions operations for the given ID. This MUST be overridden in derived classes
	 */
	virtual bool handleExtensionCommand(DWORD id)
	{
		return false;
	}

	/**
	 * Checks if the window has the id. MUST be overridden in the CExtendableWindow class, it's not required in the others
	 */
	virtual bool hasIDRegd(DWORD id)
	{
		return false;
	}

	CToolBar* getToolbar() const
	{
		return mainToolbar;
	}

protected:

	// the vetor of docked windows
	vector <CWindow*> dockedWindows;

	// the basic instance of the window (the Window class)
	CWindow* baseInstance;

	// this windowis the latest popup window that was popped up using the popupDockedWindow method
	CWindow* lastPopupWindow;

	// true if we can hide the window in a showModal loop
	bool canHide;

	// in case this window is behaving like a dialog this is the button that was pressed
	int pressedButton;

	static bool finishing;

	CToolBar* mainToolbar;

private:

	// this map holds actually the instances of ALL windows that were created in the system...
	// very useful when handling the window proc...
	static map<HWND, CWindow*> windowInstances;

	/**
	 * Handles the WM_NOTIFY messages arrived to the window
	 */
	static LRESULT handleNotifyMessages(HWND, UINT, WPARAM, LPARAM);
};

#endif // _WINDOW_H_
