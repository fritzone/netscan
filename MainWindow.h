#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "StandaloneWindow.h"
#include "control_ids.h"
#include <LMaccess.h>

#include <vector>

class CAboutWindow;
class CToolBar;
class CToolButton;
class CMainMenu;
class CTabControl;
class CTabPage;
class CStatusBar;
class CIPControl;
class CTreeControl;
class CImageList;
class CPushButton;
class CTreeItem;
class CCheckBox;
class CComputer;

using namespace std;

/**
 */
class CMainWindow: public CStandaloneWindow
{
public:

	/**
	 * Actually this is an index in the vector<CComputer*> vector 
	 */
	struct CVectorRange
	{
		size_t start;
		size_t end;
		vector<CComputer*>& vec;
		CMainWindow* theWindow;
		int tidc;

		CVectorRange(int s, int e, vector<CComputer*> v, CMainWindow* w, int tc) : start(s), end(e), vec(v), theWindow(w), tidc(tc) {}
	};



public:

	/**
	 * Creates a new object
	 */
	CMainWindow(HINSTANCE);

	/**
	 * This method returns the instance of the mainwindow... Actually this does not mean, that this is 
	 * a singleton, but it means, that 'distant' objects need sometime to work with the controls of the main 
	 * window.
	 */
	static CMainWindow* CMainWindow::getInstance()
	{
		return instance;
	}

    /**
     * Destroys the object, frees the resources.
     */
    virtual ~CMainWindow();

	/**
	 * Creates the content of the main window
	 */
	virtual bool createContent();


	/**
	 * Creates the toolbar of this window
	 */
	bool createToolBar(HWND hWndParent);

	/**
	 * Forces the repaint of the window
	 */
	void forceRepaint();

private:

	wstring getPlatform(DWORD platformId);
	wstring formatTypeAndDescription(wstring type, wstring description);
	wstring getComputerType(DWORD type);
	wstring getVersion(DWORD major, DWORD minor, DWORD type);

	void setThreadStatus(int, wstring, wstring);
	void analyzeMacAddress(LPWSTR ws, CTreeItem* goTo);
	wstring getUserPrivileges(DWORD priv);
	bool showUserInfo(LPUSER_INFO_4 inf, CTreeItem* userTreeItem,wstring);
	wstring dwordAsDate(DWORD value);
	void userFlagMagic(DWORD flag, CTreeItem* parent);
	BOOL GetTextualSid(
		PSID pSid,            // binary SID
		LPTSTR TextualSid,    // buffer for Textual representation of SID
		DWORD lpdwBufferLen // required/provided TextualSid buffersize
		);

public:

	/**
	 * Mutex for threads
	 */
	static HANDLE computerThreadMutex;

	/**
	 * The thread which collects an information about a range of computers
	 */
	static DWORD WINAPI ComputerThread(LPVOID computer);

	/**
	 * The vector containing all threads in the app
	 */
	static vector<HANDLE> threads;

	static int threadsRunning;

public:

	// the about window of the application
	CAboutWindow* aboutWindow;

private:

	vector <CComputer*> computers;

	// the toolbar buttons
	CToolButton *btnNewMap;
	
	// the menu of the application
	CMainMenu* mainMenu;

	// the main tab of the application
	CTabControl* mainTab;

	CTabPage* ipRangeTabPage;
	CTabPage* domainsTabPage;

	CStatusBar* statusBar;

	CIPControl* ipLocalHost;
	CIPControl* ipFrom;
	CIPControl* ipTo;

	CTreeControl* ipRangesTree;
	CPushButton* ipRangesGo;
	CPushButton* scanLocalhost;
	CPushButton* ipRangesOptions;

	CCheckBox* ipRangesRestricted = nullptr;

	CImageList* imgTree;

	// the instance of the program
	static CMainWindow* instance;

public:

	/*
	 * Command handlers
	 */


	/**
	 * About window will be shown...
	 */
	static void onAbout(CMainWindow*);

	/**
	 * Go, scan the IPs
	 */
	static void onGo(CMainWindow*);

	/*
	 * Command mapping
	 */
	BEGIN_COMMAND_MAP(CMainWindow)
		ON_COMMAND(MENU_HELP_ABOUT, onAbout)
		ON_COMMAND(IPRANGES_GO, onGo)
	END_COMMAND_MAP(CMainWindow)

	/*
	 * Message handlers
	 */
    static LRESULT onDestroy(CMainWindow *instance, WPARAM wParam, LPARAM lParam);
    static LRESULT onSize(CMainWindow *instance, WPARAM wParam, LPARAM lParam);
	static LRESULT onKeyUp(CMainWindow *instance, WPARAM wParam, LPARAM lParam);
	static LRESULT onDlgCode(CMainWindow *instance, WPARAM wParam, LPARAM lParam);

	/*
	 * Message mapping
	 */
	BEGIN_MESSAGE_MAP(CMainWindow)
		ON_MESSAGE(WM_DESTROY, onDestroy)
        ON_MESSAGE(WM_SIZE, onSize)
		ON_MESSAGE(WM_KEYUP, onKeyUp)
		ON_MESSAGE(WM_GETDLGCODE, onDlgCode)
	END_MESSAGE_MAP(CMainWindow)

	static const int WIDTH = 800;
	static const int HEIGHT = 600;
	static const size_t MAX_THREADS = 50;

};

#endif
