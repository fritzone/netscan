// just some generalities

#include "MainWindow.h"
#include "AboutWindow.h"

// headers of GUI elements

// headers of different kind of controls
#include "TextField.h"
#include "PushButton.h"
#include "Static.h"
#include "ImageControl.h"
#include "ListBox.h"
#include "MainMenu.h"
#include "ImageList.h"
#include "TabControl.h"
#include "TabPage.h"
#include "ToolBar.h"
#include "ToolButton.h"
#include "TreeControl.h"
#include "PopupMenu.h"
#include "IPControl.h"
#include "StatusBar.h"
#include "TreeItem.h"
#include "CheckBox.h"
#include "Computer.h"
#include "manufacturers.h"

#include <Ntsecapi.h>
#include <lm.h>
#include <iostream>
#include <vector>
#include <string>
#include <strstream>
#include <iosfwd>
#include <SvcGuid.h>
#include <NspAPI.h>
#include <time.h>
#include <Sddl.h>
#include <AclAPI.h>
// #include <netmon.h>
#include "netsgui/resource.h"

HANDLE CMainWindow::computerThreadMutex;
vector<HANDLE> CMainWindow::threads;
int CMainWindow::threadsRunning = 0;

long hexToInt(wstring _s)
{
	long tmp;
	wscanf(_s.c_str(), L"%X", &tmp);

	return tmp;
}


/********************************************************************************************************
 *                                         Constructor, destructor                                      *
 ********************************************************************************************************/

CMainWindow* CMainWindow::instance = NULL;

/**
 * Creates a new instance, based on the passed-in handle
 */
CMainWindow::CMainWindow(HINSTANCE _hInst) : CStandaloneWindow(0, 0, 640, 480, 0, _hInst), 
mainTab(0), statusBar(0), ipRangesGo(0), ipRangesOptions(0), ipRangesTree(0)
{
	// Settings that affect how the window will look

    progInstance = _hInst;
	wcscpy(szClassName, L"netscanner");
	wcscpy(text, L"Net Scan 0.2");
	style = WS_OVERLAPPEDWINDOW  | WS_EX_APPWINDOW;
	styleEx = 0;

    instance = this;
}

/**
 * Destroys the object, frees the resources
 */
CMainWindow::~CMainWindow()
{

}

/********************************************************************************************************
 *                        Creation of the window, and its content                                       *
 ********************************************************************************************************/

/**
 * Creates the main toolbar of the window
 * @param hwndParent is the handle to the toolbar's parent window. 
 */
bool CMainWindow::createToolBar(HWND hWndParent)
{
	mainToolbar			= new CToolBar(this, MAKEINTRESOURCE(IDB_BITMAP1), 22);
	btnNewMap			= new CToolButton(0, 0, BTN_NEW, L"New scan");
	
	mainToolbar->addButton(btnNewMap);
	mainToolbar->addSeparator();

	mainToolbar->autoSize();

	return true;
} 

/**
 * Creates all the objects that are on the main window
 */
bool CMainWindow::createContent()
{
int statSize[] = {120, 240, -1};

	createToolBar(hwnd);

	imgTree = new CImageList(16,16);
	
	imgTree->addImage(L"icn_zero.bmp");				// 0
	imgTree->addImage(L"icn_gray.bmp");				// 1
	imgTree->addImage(L"icn_red.bmp");				// 2
	imgTree->addImage(L"icn_green.bmp");			// 3
	imgTree->addImage(L"compic.bmp");				// 4
	imgTree->addImage(L"NT.bmp");					// 5
	imgTree->addImage(L"server.bmp");				// 6
	imgTree->addImage(L"netcon.bmp");				// 7
	imgTree->addImage(L"netcard.bmp");				// 8
	imgTree->addImage(L"transport.bmp");			// 9
	imgTree->addImage(L"plaintext.bmp");			// 10
	imgTree->addImage(L"settings.bmp");				// 11
	imgTree->addImage(L"netcard2.bmp");				// 12
	imgTree->addImage(L"shares.bmp");				// 13
	imgTree->addImage(L"the-share-icon.bmp");		// 14
	imgTree->addImage(L"users.bmp");				// 15
	imgTree->addImage(L"user.bmp");					// 16
	imgTree->addImage(L"lock.bmp");					// 17
	imgTree->addImage(L"usergrps.bmp");				// 18

	mainMenu = new CMainMenu(this);

	SetMenu(hwnd, mainMenu->getHandle());
	DrawMenuBar(hwnd);

	mainTab = new CTabControl(this, 10, 60, w, h, TAB_MAIN);
	mainTab->setButtonStyle();
	ipRangeTabPage = new CTabPage(L"IP Range Scan");
	domainsTabPage = new CTabPage(L"Domain Scan");	

	mainTab->addPage(ipRangeTabPage);
	mainTab->addPage(domainsTabPage);

	ipFrom = new CIPControl(this, 10, 65, 140, 20, 888, L"From", 35);
	ipRangeTabPage->addControl(ipFrom);

	ipTo = new CIPControl(this, 190, 65, 140, 20, 889, L"To", 20);
	ipRangeTabPage->addControl(ipTo);

	ipRangesGo = new CPushButton(this, 640 - 75, 65, 65, 20, IPRANGES_GO, L"Go", L"Start an IP Range Scan");
	ipRangesOptions = new CPushButton(this, 640 - 65 - 75, 65, 65, 20, IPRANGES_GO, L"Options", L"Options... ie. what to retrieve");

	ipRangesTree = new CTreeControl(this, 5, 90, 320, 400, TREE_IPLIST);
	ipRangesTree->setImages(imgTree);
	ipRangesRestricted = new CCheckBox(this, 360, 65, 130, 20, 999, L"Restricted range", L"Check this if you don't want the ip \"roll over\"");

	statusBar = new CStatusBar(this, STB_MAIN, 3, statSize);
	return true;
}


/********************************************************************************************************
 *                                Command handling methods                                              *
 ********************************************************************************************************/

/**
 * Called when the user chooses about
 */
void CMainWindow::onAbout(CMainWindow* instance)
{
	instance->disable();
	instance->aboutWindow->showModal();
	instance->enable();
	instance->bringToFront();
}

/**
 * Prepares the IP address based on the four ints, and puts them into addr
 */
void prepareIp(unsigned wchar_t ip1, unsigned wchar_t ip2, unsigned wchar_t ip3, unsigned wchar_t ip4, wchar_t* addr)
{
	wsprintf(addr, L"%d.%d.%d.%d", ip1, ip2, ip3, ip4);
}


/**
 * Puts in value the cnt.th byte from the IP. Start from 0.
 */
void populateIPParts(const wchar_t* ip, int cnt, wchar_t* value)
{
	int dotC = 0;
	bool found = false;
	unsigned int i=0;
	unsigned int vc = 0;
	do
	{
		if(ip[i] == '.')
		{
			dotC ++;
		}
		if(dotC == cnt)
		{
			found = true;
			if(cnt)
			{
				i++;
			}
		}
		while(found && ip[i]!='.' && i<wcslen(ip))
		{
			value[vc++] = ip[i++];
		}
		i++;
	}
	while(!found && i<wcslen(ip) && vc < 254);   // 254 - to not to run into an overflow ... although it might happen
	value[vc] = 0;
}

/**
 * Populates the four uchars as the four numbers from the ip
 * first tells if this is the first IP we are working on, or not (why: To handle * ? + wildchars)
 */
void getIpPartsAsNumbers(const wchar_t* ip, wchar_t* ip1, wchar_t* ip2, wchar_t* ip3, wchar_t* ip4)
{
	populateIPParts(ip, 0, ip1);
	populateIPParts(ip, 1, ip2);
	populateIPParts(ip, 2, ip3);
	populateIPParts(ip, 3, ip4);
}

void CMainWindow::onGo(CMainWindow * instance)
{
	instance->mainToolbar->autoSize();
	// here build the list of IPs
	wchar_t cip1_1[256],cip1_2[256],cip1_3[256],cip1_4[256],cip2_1[256],cip2_2[256],cip2_3[256],cip2_4[256];
	wstring startIp = instance->ipFrom->getText();
	wstring endIp = instance->ipTo->getText();
	if(endIp == L"0.0.0.0")
	{
		instance->ipTo->setText(instance->ipFrom->getText().c_str());
		endIp = startIp;
	}
	getIpPartsAsNumbers(startIp.c_str(), cip1_1, cip1_2, cip1_3, cip1_4);
	getIpPartsAsNumbers(endIp.c_str(), cip2_1, cip2_2, cip2_3, cip2_4);
	wchar_t full_ip[32];
	vector<wstring> allIps;
	allIps.clear();
	bool restricted = instance->ipRangesRestricted->isChecked();

	int ipp11 = _wtoi(cip1_1),
		ipp12 = _wtoi(cip2_1),
		ipp21 = _wtoi(cip1_2), 
		ipp22 = _wtoi(cip2_2),
		ipp31 = _wtoi(cip1_3),
		ipp32 = _wtoi(cip2_3), 
		ipp41 = _wtoi(cip1_4),
		ipp42 = _wtoi(cip2_4);

	for(int ip1_ctr = ipp11; ip1_ctr <= ipp12; ip1_ctr ++)
	{
		int ip2_start = restricted?ipp21:0;
		if(ip1_ctr == ipp11) ip2_start = ipp21;
		int ip2_end = restricted?ipp22:255;
		if(ip1_ctr == ipp12) ip2_end = ipp22;
		for(int ip2_ctr = ip2_start ; ip2_ctr <= ip2_end; ip2_ctr ++)
		{
			int ip3_start = restricted?ipp31:0;
			if(ip2_ctr == ipp21) ip3_start = ipp31;
			int ip3_end = restricted?ipp32:255;
			if(ip2_ctr == ipp22) ip3_end = ipp32;
			for(int ip3_ctr = ip3_start; ip3_ctr <= ip3_end; ip3_ctr ++)
			{
				int ip4_start = restricted?ipp41:0;
				if(ip3_ctr == ipp31) ip4_start = ipp41;
				int ip4_end = restricted?ipp42:255;
				if(ip3_ctr == ipp32) ip4_end = ipp42;
				for(int ip4_ctr = ip4_start; ip4_ctr <= ip4_end ; ip4_ctr ++)
				{
					wcscpy(full_ip, L"");
					prepareIp(ip1_ctr, ip2_ctr, ip3_ctr, ip4_ctr, full_ip);
					allIps.push_back(wstring(full_ip));
				}
			}
		}
	}

	instance->computers.clear();

	computerThreadMutex = CreateMutex(0, 0, L"ComputerThreadMutex");

	for(int i=0; i<allIps.size(); i++)
	{
		CComputer* newComputer = new CComputer(allIps[i]);
		instance->computers.push_back(newComputer);
	}
	// and here create the threads and populate them with the required ranges to work on

	int compsPerThread = instance->computers.size() / MAX_THREADS + 1;
	if(compsPerThread == 0) compsPerThread = 1;
	int threadCo = min(instance->computers.size(), MAX_THREADS-((compsPerThread-1)*2) + 1);
	int startRange = 0;
	int endRange = startRange + compsPerThread;


	for(int i=0; i<threadCo; i++)
	{
		CMainWindow::CVectorRange *vri = new CMainWindow::CVectorRange(
			startRange > instance->computers.size()?instance->computers.size():startRange,
			endRange > instance->computers.size()?instance->computers.size():endRange,
			instance->computers, instance, i);
		startRange += compsPerThread + 1;
		endRange = startRange + compsPerThread;

		DWORD tid = 0;
		HANDLE newHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CMainWindow::ComputerThread, vri, CREATE_SUSPENDED, &tid);
		if(newHandle != NULL)
		{
			threads.push_back(newHandle);
			threadsRunning ++;
		}
		
	}

	for(int i=0; i<threadCo; i++)
	{
		ResumeThread(threads[i]);
		Sleep(20);
	}
}

/**
* Prints the platform ID
*/
wstring CMainWindow::getPlatform(DWORD platformId)
{
	wstring g =L"";

	switch(platformId)
	{
	case PLATFORM_ID_DOS:
		g +=  L"DOS";
		break;
	case PLATFORM_ID_OS2: 
		g +=  L"OS2";
		break;
	case PLATFORM_ID_NT:
		g +=  L"NT";
		break;
	case PLATFORM_ID_OSF:
		g +=  L"OSF";
		break;
	case PLATFORM_ID_VMS:
		g +=  L"VMS";
		break;
	}
	

	return g;
}


/**
* Formats the type and the description into one convenient entry
*/
wstring CMainWindow::formatTypeAndDescription(wstring type, wstring description)
{
	wstring result = description + L" (";
	result += type;
	result += L"),";
	return result;
}

/**
* Prints the type of the computer
*/
wstring CMainWindow::getComputerType(DWORD type)
{
	wstring g = L"";
	if (type &  SV_TYPE_WORKSTATION)		g +=   formatTypeAndDescription(L"SV_TYPE_WORKSTATION", L"LAN Manager workstation");
	if (type &  SV_TYPE_SERVER)				g +=   formatTypeAndDescription(L"SV_TYPE_SERVER", L"LAN Manager server");
	if (type &  SV_TYPE_SQLSERVER)			g +=   formatTypeAndDescription(L"SV_TYPE_SQLSERVER", L"Microsoft SQL Server");
	if (type &  SV_TYPE_DOMAIN_CTRL)		g +=   formatTypeAndDescription(L"SV_TYPE_DOMAIN_CTRL", L"Primary domain controller");
	if (type &  SV_TYPE_DOMAIN_BAKCTRL)		g +=   formatTypeAndDescription(L"SV_TYPE_DOMAIN_BAKCTRL", L"Backup domain controller");
	if (type &  SV_TYPE_TIME_SOURCE)		g +=   formatTypeAndDescription(L"SV_TYPE_TIME_SOURCE", L"Timesource service");
	if (type &  SV_TYPE_AFP)				g +=   formatTypeAndDescription(L"SV_TYPE_AFP", L"Apple File Protocol");
	if (type &  SV_TYPE_NOVELL)				g +=   formatTypeAndDescription(L"SV_TYPE_NOVELL", L"Novell");
	if (type &  SV_TYPE_DOMAIN_MEMBER)		g +=   formatTypeAndDescription(L"SV_TYPE_DOMAIN_MEMBER", L"LAN Manager 2.x domain member");
	if (type &  SV_TYPE_LOCAL_LIST_ONLY)	g +=   formatTypeAndDescription(L"SV_TYPE_LOCAL_LIST_ONLY", L"Servers maintained by the browser");
	if (type &  SV_TYPE_PRINTQ_SERVER)		g +=   formatTypeAndDescription(L"SV_TYPE_PRINTQ_SERVER", L"Server sharing print queue");
	if (type &  SV_TYPE_DIALIN_SERVER)		g +=   formatTypeAndDescription(L"SV_TYPE_DIALIN_SERVER", L"Server running dial-in service");
	if (type &  SV_TYPE_XENIX_SERVER)		g +=   formatTypeAndDescription(L"SV_TYPE_XENIX_SERVER", L"Xenix/Unix server");
	if (type &  SV_TYPE_SERVER_MFPN)		g +=   formatTypeAndDescription(L"SV_TYPE_SERVER_MFPN", L"Microsoft File and Print for NetWare");
	if (type &  SV_TYPE_NT)					g +=   formatTypeAndDescription(L"SV_TYPE_NT", L"Windows (Server 2003/XP/2000/NT)");
	if (type &  SV_TYPE_WFW)				g +=   formatTypeAndDescription(L"SV_TYPE_WFW", L"Windows for Workgroups");
	if (type &  SV_TYPE_SERVER_NT)			g +=   formatTypeAndDescription(L"SV_TYPE_SERVER_NT", L"Windows Server (2003/2000/NT)");
	if (type &  SV_TYPE_POTENTIAL_BROWSER)	g +=   formatTypeAndDescription(L"SV_TYPE_POTENTIAL_BROWSER", L"Browser service");
	if (type &  SV_TYPE_BACKUP_BROWSER)		g +=   formatTypeAndDescription(L"SV_TYPE_BACKUP_BROWSER", L"Browser service (backup)");
	if (type &  SV_TYPE_MASTER_BROWSER)		g +=   formatTypeAndDescription(L"SV_TYPE_MASTER_BROWSER", L"Browser service (master)");
	if (type &  SV_TYPE_DOMAIN_MASTER)		g +=   formatTypeAndDescription(L"SV_TYPE_DOMAIN_MASTER", L"Domain master browser");
	if (type &  SV_TYPE_DOMAIN_ENUM)		g +=   formatTypeAndDescription(L"SV_TYPE_DOMAIN_ENUM", L"Primary domain");
	if (type &  SV_TYPE_WINDOWS)			g +=   formatTypeAndDescription(L"SV_TYPE_WINDOWS", L"Windows (Me/98/95)");
	if (type &  SV_TYPE_TERMINALSERVER)		g +=   formatTypeAndDescription(L"SV_TYPE_TERMINALSERVER", L"Terminal Server");
	if (type &  SV_TYPE_CLUSTER_NT)			g +=   formatTypeAndDescription(L"SV_TYPE_CLUSTER_NT", L"Server clusters available in the domain");
	if (type &  SV_TYPE_CLUSTER_VS_NT)		g +=   formatTypeAndDescription(L"SV_TYPE_CLUSTER_VS_NT", L"Cluster virtual servers available in the domain");

	return g;
}


/**
* Prints the version
*/
wstring CMainWindow::getVersion(DWORD major, DWORD minor, DWORD type)
{
	bool workst = type & SV_TYPE_WORKSTATION;
	wstring g = L"";
	if(major == 4)
	{
		g += L"Microsoft Windows NT 4.0 (4.x)";
	}
	else 
	if(major == 5)
	{
		if(minor == 0)
		{
			g += L"Microsoft Windows 2000 (5.x)";
		}
		else 
		if(minor == 1)
		{
			g += L"Microsoft Windows XP (5.1)";
		}
		else 
		if(minor == 2)
		{
			g += L"Microsoft Windows Server 2003 (5.2)";
		}
	}
	else 
	if(major == 6)
	{
		if(minor == 0)
		{
			g += L"Microsoft Windows Vista/Server 2008 (6.0)";
		}
		else
		{
			if(workst)
			{
				g += L"Microsoft Windows 7(6.1)";
			}
			else
			{
				g += L"Microsoft Windows Server 2008 R2 (6.1)";			
			}
		}
	}

	return g;
}

wstring nrAsString(DWORD nr)
{
	wchar_t s[128];
	wsprintf(s, L"%d", nr);
	return wstring(s);
}

DWORD CMainWindow::ComputerThread(LPVOID computer)
{
	CMainWindow::CVectorRange* range = (CMainWindow::CVectorRange*)computer;
	if(range->start == range->end) return 0;

	DWORD dwWaitResult; 
	bool nameFound = true;
	CTreeControl* tree = range->theWindow->ipRangesTree;

	// Request ownership of mutex for the first run: Getting the name of the hosts in this thread
	dwWaitResult = WaitForSingleObject(CMainWindow::computerThreadMutex, INFINITE);
	switch (dwWaitResult) 
	{
		// The thread got mutex ownership.
	case WAIT_OBJECT_0: 
		try 
		{
			for(int i=range->start; i<=range->end; i++)
			{
				if(i < range->theWindow->computers.size())
				{
					range->theWindow->setThreadStatus(range->tidc, range->theWindow->computers[i]->getIp(), L"Getting name");
					CTreeItem* newItem = 0;
					newItem = new CTreeItem(range->theWindow->computers[i]->getIp(), 1, 0, range->theWindow->computers[i]);
					tree->addItem(newItem);
					range->theWindow->computers[i]->treeItem = newItem;

					char ipIn[256];
					wcstombs(ipIn, range->theWindow->computers[i]->getIp().c_str(), 255);

					// Resolves the hosts' name
					struct sockaddr_in saGNI;
					saGNI.sin_addr.s_addr = inet_addr(ipIn);
					struct hostent *he = gethostbyaddr((const char*)&(saGNI.sin_addr.s_addr), 4, AF_INET);

					if(he)
					{
						wchar_t whostName[256];
						mbstowcs(whostName, he->h_name, 255);
						newItem->setImage(3);
						newItem->setSelectedImage(3);
						newItem->setText((LPWSTR)(wstring(range->theWindow->computers[i]->getIp() + L" (" + whostName + L")").c_str()));
						range->theWindow->computers[i]->name = wstring(whostName);
						newItem->check();
					}
					else
					{
						continue;
					}
				}
			}
		} 
		catch(...){}

		// Release ownership of the mutex object.
		if (! ReleaseMutex(CMainWindow::computerThreadMutex)) 
		{ 
			// Deal with error.
		} 

		break; 

		// Cannot get mutex ownership due to time-out.
	case WAIT_TIMEOUT: 
		// Got ownership of the abandoned mutex object.
	case WAIT_ABANDONED: 
		threadsRunning --;
		ReleaseMutex(CMainWindow::computerThreadMutex);
		return 0;
	}

	// Request Second ownership of the mutex for the Server information

	dwWaitResult = WaitForSingleObject(CMainWindow::computerThreadMutex, INFINITE);
	switch (dwWaitResult) 
	{
		// The thread got mutex ownership.
	case WAIT_OBJECT_0: 
		try 
		{
			for(int i=range->start; i<=range->end; i++)
			{
				if(i < range->theWindow->computers.size())
				{
					wstring name = range->theWindow->computers[i]->name;	
					range->theWindow->setThreadStatus(range->tidc, range->theWindow->computers[i]->getIp(), L"Fetching the server status");

					if(name.length() > 0)
					{
						LPSERVER_INFO_102 buf = NULL;
						NET_API_STATUS nStatus;
						DWORD level = 102;
						nStatus = NetServerGetInfo((LPWSTR)name.c_str(), level, (LPBYTE*)&buf);
						if(nStatus == NERR_Success)
						{
							CTreeItem* newItem = range->theWindow->computers[i]->treeItem;
							CTreeItem* serverRoot = new CTreeItem(L"Server (102): " + range->theWindow->getVersion(buf->sv102_version_major, buf->sv102_version_minor, buf->sv102_type), 6, newItem, 0);
							tree->addItem(serverRoot);

							wstring platformType = range->theWindow->getPlatform(buf->sv102_platform_id);


							platformType = wstring(L"Platform: ") + platformType;
							CTreeItem* platfItem = new CTreeItem(platformType, -1, serverRoot, 0) ;
							tree->addItem(platfItem);
							if(platformType == L"Platform: NT")
							{
								platfItem->setImage(5);
								platfItem->setSelectedImage(5);
							}


							CTreeItem* sv102NameItem = new CTreeItem(wstring(L"Name: ")+(LPWSTR)buf->sv102_name, 4, serverRoot, 0);
							tree->addItem(sv102NameItem);

							CTreeItem* sv102Type = new CTreeItem(wstring(L"Type"), 11, serverRoot, 0);
							tree->addItem(sv102Type);

							wstring t = range->theWindow->getComputerType(buf->sv102_type);
							int j=0;
							while (j<t.length())
							{
								wstring seq = L"";
								while(t.at(j) != L',')
								{
									seq += t.at(j++);
								}
								j++;	// skip the ,
								CTreeItem* tType = new CTreeItem(seq, 10, sv102Type, 0);
								tree->addItem(tType);
							}

							CTreeItem* sv102Comment = new CTreeItem(wstring(L"Comment:") + (LPWSTR)buf->sv102_comment, 10, serverRoot, 0);
							tree->addItem(sv102Comment);

							CTreeItem* sv102Users = new CTreeItem(wstring(L"Max Users:") + nrAsString(buf->sv102_users), 10, serverRoot, 0);
							tree->addItem(sv102Users);

							CTreeItem* sv102DiscoTime = new CTreeItem(wstring(L"Disconnect time:") + nrAsString(buf->sv102_disc), 10, serverRoot, 0);
							tree->addItem(sv102DiscoTime);

							CTreeItem* sv102Hidden = new CTreeItem(wstring(L"Hidden:") + (buf->sv102_hidden==SV_VISIBLE?L"No":L"Yes"), 10, serverRoot, 0);
							tree->addItem(sv102Hidden);

							CTreeItem* sv102UserPath = new CTreeItem(wstring(L"User Path:") + (LPWSTR)buf->sv102_userpath, 10, serverRoot, 0);
							tree->addItem(sv102UserPath);

							newItem->setImage(3);
							newItem->setSelectedImage(3);

						}	
						else
						{
							// 102 not supported, testing for 101
							LPSERVER_INFO_101 buf = NULL;
							level = 101;
							nStatus = NetServerGetInfo((LPWSTR)name.c_str(), level, (LPBYTE*)&buf);
							if(nStatus == NERR_Success)
							{
								CTreeItem* newItem = range->theWindow->computers[i]->treeItem;
								CTreeItem* serverRoot = new CTreeItem(L"Server (101): " + range->theWindow->getVersion(buf->sv101_version_major, buf->sv101_version_minor, buf->sv101_type), 6, newItem, 0);
								tree->addItem(serverRoot);

								wstring platformType = range->theWindow->getPlatform(buf->sv101_platform_id);


								platformType = wstring(L"Platform: ") + platformType;
								CTreeItem* platfItem = new CTreeItem(platformType, -1, serverRoot, 0) ;
								tree->addItem(platfItem);
								if(platformType == L"Platform: NT")
								{
									platfItem->setImage(5);
									platfItem->setSelectedImage(5);
								}


								CTreeItem* sv101NameItem = new CTreeItem(wstring(L"Name: ")+(LPWSTR)buf->sv101_name, 4, serverRoot, 0);
								tree->addItem(sv101NameItem);

								CTreeItem* sv101Type = new CTreeItem(wstring(L"Type"), 11, serverRoot, 0);
								tree->addItem(sv101Type);

								wstring t = range->theWindow->getComputerType(buf->sv101_type);
								int j=0;
								while (j<t.length())
								{
									wstring seq = L"";
									while(t.at(j) != L',')
									{
										seq += t.at(j++);
									}
									j++;	// skip the ,
									CTreeItem* tType = new CTreeItem(seq, 10, sv101Type, 0);
									tree->addItem(tType);
								}

								CTreeItem* sv101Comment = new CTreeItem(wstring(L"Comment:") + (LPWSTR)buf->sv101_comment, 10, serverRoot, 0);
								tree->addItem(sv101Comment);

								newItem->setImage(3);
								newItem->setSelectedImage(3);

							}
							else	// Only 100 is available
							{
								LPSERVER_INFO_100 buf = NULL;
								level = 100;
								nStatus = NetServerGetInfo((LPWSTR)name.c_str(), level, (LPBYTE*)&buf);
								if(nStatus == NERR_Success)
								{
									CTreeItem* newItem = range->theWindow->computers[i]->treeItem;
									CTreeItem* serverRoot = new CTreeItem(L"Server (100)" , 6, newItem, 0);
									tree->addItem(serverRoot);

									wstring platformType = range->theWindow->getPlatform(buf->sv100_platform_id);

									platformType = wstring(L"Platform: ") + platformType;
									CTreeItem* platfItem = new CTreeItem(platformType, 11, serverRoot, 0) ;
									tree->addItem(platfItem);
									if(platformType == L"Platform: NT")
									{
										platfItem->setImage(5);
										platfItem->setSelectedImage(5);
									}

									CTreeItem* sv100NameItem = new CTreeItem(wstring(L"Name: ")+(LPWSTR)buf->sv100_name, 4, serverRoot, 0);
									tree->addItem(sv100NameItem);

									newItem->setImage(3);
									newItem->setSelectedImage(3);
								}
							}
						}

						if(buf != NULL)
						{
							NetApiBufferFree(buf);
						}

					}
				}
			}

		} 
		catch(...){}

		// Release ownership of the mutex object.
		if (! ReleaseMutex(CMainWindow::computerThreadMutex)) 
		{ 
			// Deal with error.
		} 

		break; 

		// Cannot get mutex ownership due to time-out.
	case WAIT_TIMEOUT: 
		// Got ownership of the abandoned mutex object.
	case WAIT_ABANDONED: 
		threadsRunning --;
		ReleaseMutex(CMainWindow::computerThreadMutex);
		return 0;
	}

	// Request Third ownership of the mutex for the join information
	dwWaitResult = WaitForSingleObject(CMainWindow::computerThreadMutex, INFINITE);
	switch (dwWaitResult) 
	{
		// The thread got mutex ownership.
	case WAIT_OBJECT_0: 
		try 
		{
			for(int i=range->start; i<=range->end; i++)
			{
				if(i < range->theWindow->computers.size())
				{
					range->theWindow->setThreadStatus(range->tidc, range->theWindow->computers[i]->getIp(), L"Fetching the joined domain");

					wstring name = range->theWindow->computers[i]->name;
					if(name.length() > 0)
					{
						LPWSTR joinedTo = NULL;

						NETSETUP_JOIN_STATUS ens;
						NetGetJoinInformation(name.c_str(), &joinedTo, &ens);

						if(joinedTo != NULL)
						{
							CTreeItem* newItem = range->theWindow->computers[i]->treeItem;
							CTreeItem* joinedToItem = new CTreeItem(L"Joined to" , 7, newItem, 0);
							tree->addItem(joinedToItem);

							if(ens == NetSetupDomainName)
							{
								CTreeItem* domainItem = new CTreeItem(wstring(L"Domain: ") + joinedTo, 10, joinedToItem , 0);
								tree->addItem(domainItem);
							}
							else
							if(ens == NetSetupWorkgroupName)
							{
								CTreeItem* domainItem = new CTreeItem(wstring(L"Workgroup: ") + joinedTo, 10, joinedToItem , 0);
								tree->addItem(domainItem);
							}
							else
							if(ens == NetSetupUnjoined)
							{
								CTreeItem* domainItem = new CTreeItem(wstring(L"Not joined") + joinedTo, 10, joinedToItem , 0);
								tree->addItem(domainItem);
							}
							else
							{
								CTreeItem* domainItem = new CTreeItem(wstring(L"Can't really decide:") + joinedTo, 10, joinedToItem , 0);
								tree->addItem(domainItem);
							}
						}
					}
				}
			}
		} 
		catch(...){}

		// Release ownership of the mutex object.
		if (! ReleaseMutex(CMainWindow::computerThreadMutex)) 
		{ 
			// Deal with error.
		} 

		break; 

		// Cannot get mutex ownership due to time-out.
	case WAIT_TIMEOUT: 
		// Got ownership of the abandoned mutex object.
	case WAIT_ABANDONED: 
		threadsRunning --;
		ReleaseMutex(CMainWindow::computerThreadMutex);
		return 0;
	}

	// Request Fourth ownership of the mutex for the transport enumeration
	dwWaitResult = WaitForSingleObject(CMainWindow::computerThreadMutex, INFINITE);
	switch (dwWaitResult) 
	{
		// The thread got mutex ownership.
	case WAIT_OBJECT_0: 
		try 
		{
			for(size_t j=range->start; j<=range->end; j++)
			{
				if(j < range->theWindow->computers.size())
				{
					wstring name = range->theWindow->computers[j]->name;

					if(name.length() > 0)
					{

						range->theWindow->setThreadStatus(range->tidc, range->theWindow->computers[j]->getIp(), L"Fetching the transports");

						LPSERVER_TRANSPORT_INFO_1 pBuf = NULL;
						LPSERVER_TRANSPORT_INFO_1 pTmpBuf;
						DWORD dwLevel = 1;
						DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
						DWORD dwEntriesRead = 0;
						DWORD dwTotalEntries = 0;
						DWORD dwResumeHandle = 0;
						DWORD dwTotalCount = 0;
						NET_API_STATUS nStatus;
						DWORD i;
						bool first = true;
						CTreeItem* newItem = range->theWindow->computers[j]->treeItem;
						CTreeItem* transportItem = NULL;

						do // begin do
						{
							nStatus = NetServerTransportEnum((LPWSTR)name.c_str(), dwLevel, (LPBYTE *) &pBuf, dwPrefMaxLen, 
								&dwEntriesRead, &dwTotalEntries, &dwResumeHandle);
							if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
							{
								if ((pTmpBuf = pBuf) != NULL)
								{
									for (i = 0; i < dwEntriesRead; i++)
									{
										if (pTmpBuf == NULL)
										{
											break;	// leave this machine
										}
										else
										{
											if(first)
											{
												transportItem = new CTreeItem(L"Transports" , 8, newItem, 0);
												tree->addItem(transportItem);
												first = false;
											}

											CTreeItem* ctransportItem = new CTreeItem((LPWSTR)pTmpBuf->svti1_transportname , 9, transportItem, 0);
											tree->addItem(ctransportItem);

											// MAC Address
											wstring wmac((LPWSTR)pTmpBuf->svti1_networkaddress);
											wstring wmac2(L"");
											for(int k=0; k<wmac.length(); k++)
											{
												wmac2 += wmac[k];
												if(k%2==1) wmac2 += L":";
											}
											CTreeItem* transportNetworkAddressItem = new CTreeItem(wmac2, 12, ctransportItem, 0);
											tree->addItem(transportNetworkAddressItem );

											LPBYTE na = pTmpBuf->svti1_transportaddress;
											wchar_t wna[256];
											mbstowcs(wna, (char*)na, 256);

											CTreeItem* transportAddressItem = new CTreeItem((LPWSTR)wna, 10, ctransportItem, 0);
											tree->addItem(transportAddressItem );
											
											range->theWindow->analyzeMacAddress((LPWSTR)pTmpBuf->svti1_networkaddress, ctransportItem);

											pTmpBuf++;
											dwTotalCount++;
										}
									}
								}
							}
							else
							{
								if (pBuf != NULL)
								{
									NetApiBufferFree(pBuf);
									pBuf = NULL;
								}
								break;	// get out of the Do, to fetch the next computer
							}
							if (pBuf != NULL)
							{
								NetApiBufferFree(pBuf);
								pBuf = NULL;
							}
						}
						while (nStatus == ERROR_MORE_DATA); // end do

						if (pBuf != NULL)
						{
							NetApiBufferFree(pBuf);
						}
					}
				}
			}
		} 
		catch(...){}

		// Release ownership of the mutex object.
		if (! ReleaseMutex(CMainWindow::computerThreadMutex)) 
		{ 
			// Deal with error.
		} 

		break; 

		// Cannot get mutex ownership due to time-out.
	case WAIT_TIMEOUT: 
		// Got ownership of the abandoned mutex object.
	case WAIT_ABANDONED: 
		threadsRunning --;
		ReleaseMutex(CMainWindow::computerThreadMutex);
		return 0;
	}

	// Request Fifth ownership of the mutex for the shares
	dwWaitResult = WaitForSingleObject(CMainWindow::computerThreadMutex, INFINITE);
	switch (dwWaitResult) 
	{
		// The thread got mutex ownership.
	case WAIT_OBJECT_0: 
		try 
		{
			for(int j=range->start; j<=range->end;j++)
			{
				if(j < range->theWindow->computers.size())
				{
					wstring name = range->theWindow->computers[j]->name;
					if(name.length() > 0)
					{
						PSHARE_INFO_1 BufPtr,p;
						NET_API_STATUS res;
						DWORD er=0,tr=0,resume=0, i;
						CTreeItem* newItem = range->theWindow->computers[j]->treeItem;
						CTreeItem* sharesItem = NULL;
						bool first = true;

						do // begin do
						{
							res = NetShareEnum((LPWSTR)name.c_str(), 1, (LPBYTE *) &BufPtr, -1, &er, &tr, &resume);
							if(res == ERROR_SUCCESS || res == ERROR_MORE_DATA)
							{
								p = BufPtr;
								for(i=1; i<=er; i++)
								{

									PSHARE_INFO_501 infBuffer;
									NET_API_STATUS nst2 = NetShareGetInfo((LPWSTR)name.c_str(), p->shi1_netname, 501, (LPBYTE*)&infBuffer );

									if(first)
									{
										sharesItem = new CTreeItem(L"Shares" , 13, newItem, 0);
										tree->addItem(sharesItem);
										first = false;
									}
									CTreeItem* shareItem = new CTreeItem((LPWSTR)p->shi1_netname, 14, sharesItem, 0);
									tree->addItem(shareItem);
									p++;
								}
								NetApiBufferFree(BufPtr);
							}
						}
						while (res == ERROR_MORE_DATA); // end do

					}
				}
			}
		} 
		catch(...){}

		// Release ownership of the mutex object.
		if (! ReleaseMutex(CMainWindow::computerThreadMutex)) 
		{ 
			// Deal with error.
		} 

		break; 

		// Cannot get mutex ownership due to time-out.
	case WAIT_TIMEOUT: 
		// Got ownership of the abandoned mutex object.
	case WAIT_ABANDONED: 
		threadsRunning --;
		ReleaseMutex(CMainWindow::computerThreadMutex);
		return 0;
	}


	// Request Sixth ownership of the mutex for the user infos
	dwWaitResult = WaitForSingleObject(CMainWindow::computerThreadMutex, INFINITE);
	switch (dwWaitResult) 
	{
		// The thread got mutex ownership.
	case WAIT_OBJECT_0: 
		try 
		{
			for(int j=range->start; j<=range->end; j++)
			{
				if(j < range->theWindow->computers.size())
				{
					range->theWindow->setThreadStatus(range->tidc, range->theWindow->computers[j]->getIp(), L"Fetching the User Info");

					wstring name = range->theWindow->computers[j]->name;
					if(name.length() > 0)
					{
						LPUSER_INFO_10 pBuf = NULL;
						LPUSER_INFO_10 pTmpBuf;
						DWORD dwLevel = 10;
						DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
						DWORD dwEntriesRead = 0;
						DWORD dwTotalEntries = 0;
						DWORD dwResumeHandle = 0;
						DWORD i;
						DWORD dwTotalCount = 0;
						NET_API_STATUS nStatus;
						LPTSTR pszServerName = NULL;

						pszServerName = (LPTSTR)name.c_str();
						CTreeItem* newItem = range->theWindow->computers[j]->treeItem;
						CTreeItem* usersItem = NULL;
						bool first = true;

						do 
						{
							nStatus = NetUserEnum((LPWSTR)name.c_str(), dwLevel, 0, (LPBYTE*)&pBuf, dwPrefMaxLen, &dwEntriesRead, &dwTotalEntries, &dwResumeHandle);
							if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
							{
								if ((pTmpBuf = pBuf) != NULL)
								{
									for (i = 0; (i < dwEntriesRead); i++)
									{
										if (pTmpBuf == NULL)
										{
											break;
										}
										if(first)
										{
											usersItem = new CTreeItem(L"Users" , 15, newItem, 0);
											tree->addItem(usersItem);
											first = false;
										}

										CTreeItem* userItem = new CTreeItem((LPWSTR)pTmpBuf->usri10_name, 16, usersItem, 0);
										tree->addItem(userItem);
										wstring wfname(L"Full name: "); wfname += (LPWSTR)pTmpBuf->usri10_full_name;

										CTreeItem* userFullnameItem = new CTreeItem(wfname, 4, userItem, 0);
										tree->addItem(userFullnameItem);

										wstring wusercmt(L"Comments:"); wusercmt += (LPWSTR)pTmpBuf->usri10_comment;
										CTreeItem* userCommentItem = new CTreeItem(wusercmt, 10, userItem, 0);
										tree->addItem(userCommentItem);

										// now try to get more info from this user
										LPUSER_INFO_4 pBuf;
										DWORD dwLevel4 = 4;
										NET_API_STATUS nStatus2 = NetUserGetInfo((LPWSTR)name.c_str(), pTmpBuf->usri10_name, dwLevel4, (LPBYTE *)&pBuf);
										if(nStatus2 == NERR_Success)
										{
											range->theWindow->showUserInfo(pBuf, userItem, name);
											//showUserGroup(name, pBuf->usri4_name);
											//showUserLocalGroup(name, pBuf->usri4_name);
										}

										pTmpBuf++;
										dwTotalCount++;
									}
								}
							}
							else
							{
								break;
							}
							if (pBuf != NULL)
							{
								NetApiBufferFree(pBuf);
								pBuf = NULL;
							}
						}
						while (nStatus == ERROR_MORE_DATA); // end do
						if (pBuf != NULL) NetApiBufferFree(pBuf);
					}
				}
			}
		} 
		catch(...){}

		// Release ownership of the mutex object.
		if (! ReleaseMutex(CMainWindow::computerThreadMutex)) 
		{ 
			// Deal with error.
		} 

		break; 

		// Cannot get mutex ownership due to time-out.
	case WAIT_TIMEOUT: 
		// Got ownership of the abandoned mutex object.
	case WAIT_ABANDONED: 
		threadsRunning --;
		ReleaseMutex(CMainWindow::computerThreadMutex);
		return 0;
	}


	// Request Seventh ownership of the mutex for the files
	dwWaitResult = WaitForSingleObject(CMainWindow::computerThreadMutex, INFINITE);
	switch (dwWaitResult) 
	{
		// The thread got mutex ownership.
	case WAIT_OBJECT_0: 
		try 
		{
			for(int j=range->start; j<=range->end;j++)
			{
				if(j < range->theWindow->computers.size())
				{
					wstring name = range->theWindow->computers[j]->name;
					if(name.length() > 0)
					{
						LPFILE_INFO_3 BufPtr,p;
						NET_API_STATUS res;
						DWORD er=0,tr=0,resume=0, i;
						CTreeItem* newItem = range->theWindow->computers[j]->treeItem;
						CTreeItem* sharesItem = NULL;
						bool first = true;

						do // begin do
						{
							res = NetFileEnum((LPWSTR)name.c_str(), NULL, NULL, 3, (LPBYTE *) &BufPtr, MAX_PREFERRED_LENGTH, &er, &tr, &resume);
							if(res == ERROR_SUCCESS || res == ERROR_MORE_DATA)
							{
								p = BufPtr;
								for(i=1; i<=er; i++)
								{

									if(first)
									{
										sharesItem = new CTreeItem(L"Files" , 0, newItem, 0);
										tree->addItem(sharesItem);
										first = false;
									}
									CTreeItem* shareItem = new CTreeItem((LPWSTR)p->fi3_pathname, 10, sharesItem, 0);
									tree->addItem(shareItem);
									p++;
								}
								NetApiBufferFree(BufPtr);
							}
						}
						while (res == ERROR_MORE_DATA); // end do

					}
				}
			}
		} 
		catch(...){}

		// Release ownership of the mutex object.
		if (! ReleaseMutex(CMainWindow::computerThreadMutex)) 
		{ 
			// Deal with error.
		} 

		break; 

		// Cannot get mutex ownership due to time-out.
	case WAIT_TIMEOUT: 
		// Got ownership of the abandoned mutex object.
	case WAIT_ABANDONED: 
		threadsRunning --;
		ReleaseMutex(CMainWindow::computerThreadMutex);
		return 0;
	}


	threadsRunning --;	
	ReleaseMutex(CMainWindow::computerThreadMutex);

	range->theWindow->setThreadStatus(-1, L"", L"");

	return 1; 

}

/********************************************************************************************************
 *                                         Message handlers                                             *
 ********************************************************************************************************/


/**
 * Message handler for WM_DESTROY
 */
LRESULT CMainWindow::onDestroy(CMainWindow* instance, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage (0);
	return 0;
}

/**
* Message handler for WM_DESTROY
*/
LRESULT CMainWindow::onSize(CMainWindow* instance, WPARAM wParam, LPARAM lParam)
{
	RECT rt;
	GetClientRect(instance->hwnd, &rt);


	if(instance->mainTab)
	{
	RECT rtr = rt;
		rtr.left = 0;
		rtr.top = 32;
		rtr.bottom = rt.bottom;		
		rtr.right = rt.right;
		instance->mainTab->move(rtr);
	}

	if(instance->statusBar)
	{
		instance->statusBar->autoSize();
	}

	if(instance->ipRangesGo)
	{
		RECT rtt = rt;
		rtt.left = rtt.right - instance->ipRangesGo->width() - 4;
		rtt.top = instance->ipRangesGo->top();
		rtt.right = rtt.left + instance->ipRangesGo->width();
		rtt.bottom = rtt.top + instance->ipRangesGo->height();
		instance->ipRangesGo->move(rtt);
	}
	if(instance->ipRangesOptions)
	{
		RECT rtt = rt;
		rtt.left = rtt.right - instance->ipRangesOptions->width() - instance->ipRangesGo->width() - 5;
		rtt.top = instance->ipRangesOptions->top();
		rtt.right = rtt.left + instance->ipRangesOptions->width();
		rtt.bottom = rtt.top + instance->ipRangesOptions->height();
		instance->ipRangesOptions->move(rtt);
	}

	if(instance->ipRangesTree)
	{
		RECT rtt = rt;
		rtt.left = instance->ipRangesTree->left();
		rtt.top = instance->ipRangesTree->top();
		rtt.right = ((rt.right - rt.left) / 3) + instance->ipRangesTree->left();
		rtt.bottom = rt.bottom - 25;
		instance->ipRangesTree->move(rtt);
	}

	if (instance->mainToolbar)
	{
		instance->mainToolbar->autoSize();
	}

	instance->repaint();
	
	
    return 0;
}



/********************************************************************************************************
 *               Other functions, useful when dealing with different issues.                            *
 ********************************************************************************************************/

void CMainWindow::setThreadStatus(int i, wstring ip, wstring opr)
{
	if(i == -1)
	{
		this->statusBar->setText(L"Done", 2);
		return;
	}

	wchar_t s[256];
	wsprintf(s, L"%d", i);
	wstring t(L"Thread ");
	t += s;
	t += L" checking IP: ";
	t += ip;
	t += L". ";
	t += opr;
	this->statusBar->setText((LPWSTR)t.c_str(), 2);
}

wstring CMainWindow::getUserPrivileges(DWORD priv)
{
	switch(priv)
	{
	case USER_PRIV_GUEST:
		return wstring(L"Guest");
	case USER_PRIV_USER:
		return wstring(L"User");
	case USER_PRIV_ADMIN:
		return wstring(L"Administrator");
	default:
		return wstring(L"Unidentifiable");
	}
}

void CMainWindow::userFlagMagic(DWORD flag, CTreeItem* parent)
{
	{
		wstring s(L"Account is disabled:");
		if(flag & UF_ACCOUNTDISABLE) s += L"Yes"; else s += L"No";
		CTreeItem* item = new CTreeItem(s, 10, parent, 0);
		parent->getTree()->addItem(item);
	}
	

	{
		wstring s(L"Account is locked out:");
		if(flag & UF_LOCKOUT)  s += L"Yes"; else s += L"No";
		CTreeItem* item = new CTreeItem(s, 10, parent, 0);
		parent->getTree()->addItem(item);
	}

	{
		wstring s(L"Password not required: ");
		if(flag & UF_PASSWD_NOTREQD) s += L"True"; else s += L"False";
		CTreeItem* item = new CTreeItem(s, 10, parent, 0);
		parent->getTree()->addItem(item);
	}

	{
		wstring s(L"Password can't be changed: ");
		if(flag & UF_PASSWD_CANT_CHANGE) s += L"True"; else s += L"False";
		CTreeItem* item = new CTreeItem(s, 10, parent, 0);
		parent->getTree()->addItem(item);
	}

	{
		wstring s(L"Password never expires: ");
		if(flag & UF_DONT_EXPIRE_PASSWD) s += L"True"; else s += L"False";
		CTreeItem* item = new CTreeItem(s, 10, parent, 0);
		parent->getTree()->addItem(item);
	}
}

BOOL CMainWindow::GetTextualSid(
				   PSID pSid,            // binary SID
				   LPTSTR TextualSid,    // buffer for Textual representation of SID
				   DWORD lpdwBufferLen // required/provided TextualSid buffersize
				   )
{
	PSID_IDENTIFIER_AUTHORITY psia;
	DWORD dwSubAuthorities;
	DWORD dwSidRev=SID_REVISION;
	DWORD dwCounter;
	DWORD dwSidSize;

	// Validate the binary SID.

	if(!IsValidSid(pSid)) return FALSE;

	// Get the identifier authority value from the SID.

	psia = GetSidIdentifierAuthority(pSid);

	// Get the number of subauthorities in the SID.

	dwSubAuthorities = *GetSidSubAuthorityCount(pSid);

	// Compute the buffer length.
	// S-SID_REVISION- + IdentifierAuthority- + subauthorities- + NULL

	dwSidSize=(15 + 12 + (12 * dwSubAuthorities) + 1) * sizeof(TCHAR);

	// Check input buffer length.
	// If too small, indicate the proper size and set last error.

	if (lpdwBufferLen < dwSidSize)
	{
		return FALSE;
	}

	// Add 'S' prefix and revision number to the string.

	dwSidSize=wsprintf(TextualSid, TEXT("S-%lu-"), dwSidRev );

	// Add SID identifier authority to the string.

	if ( (psia->Value[0] != 0) || (psia->Value[1] != 0) )
	{
		dwSidSize+=wsprintf(TextualSid + lstrlen(TextualSid),
			TEXT("0x%02hx%02hx%02hx%02hx%02hx%02hx"),
			(USHORT)psia->Value[0],
			(USHORT)psia->Value[1],
			(USHORT)psia->Value[2],
			(USHORT)psia->Value[3],
			(USHORT)psia->Value[4],
			(USHORT)psia->Value[5]);
	}
	else
	{
		dwSidSize+=wsprintf(TextualSid + lstrlen(TextualSid),
			TEXT("%lu"),
			(ULONG)(psia->Value[5]      )   +
			(ULONG)(psia->Value[4] <<  8)   +
			(ULONG)(psia->Value[3] << 16)   +
			(ULONG)(psia->Value[2] << 24)   );
	}

	// Add SID subauthorities to the string.
	//
	for (dwCounter=0 ; dwCounter < dwSubAuthorities ; dwCounter++)
	{
		dwSidSize+=wsprintf(TextualSid + dwSidSize, TEXT("-%lu"),
			*GetSidSubAuthority(pSid, dwCounter) );
	}

	return TRUE;
}


bool CMainWindow::showUserInfo(LPUSER_INFO_4 inf, CTreeItem* userTreeItem, wstring compname)
{
	wstring spriv = getUserPrivileges(inf->usri4_priv);
	spriv = wstring(L"Privileges: ") + spriv;
	CTreeItem* privilegesItem = new CTreeItem(spriv, 10, userTreeItem, 0);
	userTreeItem->getTree()->addItem(privilegesItem);

	if(inf->usri4_home_dir != NULL) 
	{
		wstring s(L"Homedir: "); s += (LPWSTR)inf->usri4_home_dir;
		CTreeItem* item = new CTreeItem(s, 10, userTreeItem, 0);
		userTreeItem->getTree()->addItem(item);
	}

	if(inf->usri4_parms != NULL)
	{
		wstring s(L"Params: "); s += (LPWSTR)inf->usri4_parms;
		CTreeItem* item = new CTreeItem(s, 10, userTreeItem, 0);
		userTreeItem->getTree()->addItem(item);

	}
	if(inf->usri4_profile != NULL)
	{
		wstring s(L"Profile: "); s += (LPWSTR)inf->usri4_profile;
		CTreeItem* item = new CTreeItem(s, 10, userTreeItem, 0);
		userTreeItem->getTree()->addItem(item);
	}

	{
		wstring passwExp = L"No";
		if(inf->usri4_password_expired != 0) passwExp = L"Yes";
		wstring s(L"Password expired: "); s += passwExp;
		CTreeItem* item = new CTreeItem(s, 10, userTreeItem, 0);
		userTreeItem->getTree()->addItem(item);
	}

	if(inf->usri4_workstations != NULL) 
	{
		wstring s(L"Allowed workstations: "); s += (LPWSTR)inf->usri4_workstations;
		CTreeItem* item = new CTreeItem(s, 10, userTreeItem, 0);
		userTreeItem->getTree()->addItem(item);
	}

	wstring s(L"Last logon: "); s += dwordAsDate(inf->usri4_last_logon);
	CTreeItem* item = new CTreeItem(s, 10, userTreeItem, 0);
	userTreeItem->getTree()->addItem(item);

	if(inf->usri4_script_path != 0)
	{
		wstring s(L"Script path: "); s += inf->usri4_script_path;
		CTreeItem* item = new CTreeItem(s, 10, userTreeItem, 0);
		userTreeItem->getTree()->addItem(item);
	}

	if(inf->usri4_logon_server != 0)
	{
		wstring s(L"Logon server: "); s += inf->usri4_logon_server;
		CTreeItem* item = new CTreeItem(s, 10, userTreeItem, 0);
		userTreeItem->getTree()->addItem(item);
	}

	CTreeItem* lockitem = new CTreeItem(L"Locks", 17, userTreeItem, 0);
	userTreeItem->getTree()->addItem(lockitem);
	userFlagMagic(inf->usri4_flags, lockitem);

wchar_t textsid[1024];
	if(GetTextualSid(inf->usri4_user_sid, textsid, 1024))
	{
		wstring s(L"SID:"); s += textsid;
		CTreeItem* item = new CTreeItem(s, 10, userTreeItem, 0);
		userTreeItem->getTree()->addItem(item);
	}

	{
		// fetching the groups of the user
		CTreeItem* grpsitem = new CTreeItem(L"Groups", 18, userTreeItem, 0);
		userTreeItem->getTree()->addItem(grpsitem);

		LPGROUP_USERS_INFO_0 pBuf = NULL;
		DWORD dwLevel = 0;
		DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
		DWORD dwEntriesRead = 0;
		DWORD dwTotalEntries = 0;
		NET_API_STATUS nStatus;

		nStatus = NetUserGetGroups(compname.c_str(), inf->usri4_name, dwLevel, (LPBYTE*)&pBuf, dwPrefMaxLen, &dwEntriesRead, &dwTotalEntries);
		if (nStatus == NERR_Success)
		{

			LPGROUP_USERS_INFO_0 pTmpBuf;
			DWORD i;
			DWORD dwTotalCount = 0;

			if ((pTmpBuf = pBuf) != NULL)
			{
				for (i = 0; i < dwEntriesRead; i++)
				{
					if (pTmpBuf == NULL)
					{
						break;
					}
					CTreeItem* grpitem = new CTreeItem(pTmpBuf->grui0_name, 10, grpsitem, 0);
					userTreeItem->getTree()->addItem(grpitem);
					pTmpBuf++;
					dwTotalCount++;
				}
			}
		}
		if (pBuf != NULL)
			NetApiBufferFree(pBuf);

	}


	{
		CTreeItem* grpsitem = new CTreeItem(L"Local Groups", 18, userTreeItem, 0);
		userTreeItem->getTree()->addItem(grpsitem);

		LPLOCALGROUP_USERS_INFO_0 pBuf = NULL;
		DWORD dwLevel = 0;
		DWORD dwFlags = LG_INCLUDE_INDIRECT ;
		DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
		DWORD dwEntriesRead = 0;
		DWORD dwTotalEntries = 0;
		NET_API_STATUS nStatus;
		nStatus = NetUserGetLocalGroups(compname.c_str(), inf->usri4_name, dwLevel, dwFlags, (LPBYTE *) &pBuf, dwPrefMaxLen, &dwEntriesRead, &dwTotalEntries);
		if (nStatus == NERR_Success)
		{
			LPLOCALGROUP_USERS_INFO_0 pTmpBuf;
			DWORD i;
			DWORD dwTotalCount = 0;

			if ((pTmpBuf = pBuf) != NULL)
			{
				for (i = 0; i < dwEntriesRead; i++)
				{

					if (pTmpBuf == NULL)
					{
						fprintf(stderr, "An access violation has occurred\n");
						break;
					}

					CTreeItem* grpitem = new CTreeItem(pTmpBuf->lgrui0_name, 10, grpsitem, 0);
					userTreeItem->getTree()->addItem(grpitem);
					pTmpBuf++;
					dwTotalCount++;
				}
			}
		}
		if (pBuf != NULL)
			NetApiBufferFree(pBuf);

	}

	return true;
}

/**
 * Prints the DWORD value as a date
 */
wstring CMainWindow::dwordAsDate(DWORD value)
{
	if(value == (DWORD)-1) return L"Never";

	time_t tt = value;
	tm tva;
	localtime_s(&tva, &tt);
 char strTime[256];
 strftime(strTime, 255, "%#c", &tva);
//	char *strTime = ctime((time_t*)&value);
	if(strTime[strlen(strTime) - 1] == '\n')
	{
		strTime[strlen(strTime) - 1] = 0;
	}
wchar_t s[256];
	mbstowcs(s, strTime, 255);
	return wstring(s);
}


/**
 * Analyzes the given mac address, tries to determine its properties and puts the result under the given tree item
 */
void CMainWindow::analyzeMacAddress(LPWSTR ws, CTreeItem* goTo)
{

	wstring g(ws);

	wchar_t byte1[3], byte2[3], byte3[3], byte4[3], byte5[3], byte6[3];
	byte1[2] = byte2[2] = byte3[2] = byte4[2] = byte5[2] = byte6[2] = 0;

	byte1[0] = g[0]; byte1[1] = g[1];
	byte2[0] = g[2]; byte2[1] = g[3];
	byte3[0] = g[4]; byte3[1] = g[5];
	byte4[0] = g[6]; byte4[1] = g[7];
	byte5[0] = g[8]; byte5[1] = g[9];
	byte6[0] = g[10]; byte6[1] = g[11];


	byte1[0] = towupper(byte1[0]); byte1[1] = towupper(byte1[1]);
	byte2[0] = towupper(byte2[0]); byte2[1] = towupper(byte2[1]);
	byte3[0] = towupper(byte3[0]); byte3[1] = towupper(byte3[1]);

	wstring firstThree = wstring(byte1);
	firstThree += byte2;
	firstThree += byte3;

	bool found = false;

	for(size_t i=0; i<sizeof(manList)/sizeof(manList[0]); i++)
	{
		if(firstThree == manList[i].code)
		{
			wstring g (manList[i].manufacturer);
			CTreeItem* nameItem = new CTreeItem(g, 4, goTo, 0);
			ipRangesTree->addItem(nameItem);
			found = true;
			break;
		}
	}
}
