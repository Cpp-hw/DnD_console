// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// C RunTime Header Files
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <malloc.h>
#include <memory.h>
#include <string>
#include <map>
#include <math.h>
#include <cmath>
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <cstddef>
//#include <stdbool.h>


//#define _WIN32
#ifdef _WIN32
	//define something for Windows
	#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
	//#include <windows.h>
	#include <winsock2.h>
	#include <WS2tcpip.h>
	//#include <Wdbgexts.h>
	#include <windows.h>
	#include "unistd.h"
	#pragma comment(lib, "Ws2_32.lib")
#else
	//define it for a UNIX machine
	#include <arpa/inet.h>
	#include <unistd.h>
#endif





// feature test macro requirements
/*#define _POSIX_C_SOURCE  200809L
#define _GNU_SOURCE
#define _XOPEN_SOURCE 700
#define _XOPEN_SOURCE_EXTENDED*/


#if defined(_MSC_VER) && (_MSC_VER >= 1310) /*Visual Studio: A few warning types are not desired here.*/
#pragma warning( disable : 4244 ) /*implicit conversions: not warned by gcc -Wall -Wextra and requires too much casts*/
#pragma warning( disable : 4996 ) /*VS does not like fopen, but fopen_s is not standard C so unusable here*/
#endif /*_MSC_VER */


using namespace std;
