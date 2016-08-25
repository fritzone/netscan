#ifndef _WIND_UPG_H_
#define _WIND_UPG_H_

#define NOMINMAX

// the role of this header file is to include the windows header and to 'upgrade' it
// to XP so that fancy features will be enabled, such as mousewhell
// 
#ifndef WINVER                // Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0502        // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif
                   

#ifndef _WIN32_WINDOWS        // Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0502 // Change this to the appropriate value to target Windows Me or later.
#else
// #error _WIN32_WINDOWS already defined
#endif

#define _WIN32_WINNT 0x0502

#pragma warning(disable : 4996)
#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#pragma warning(disable : 4312)

#include <WinSock2.h>

#include <Windows.h>
#include <ws2tcpip.h>
#include <CommCtrl.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#endif
