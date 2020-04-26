// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#include <iostream>
#include <Windows.h>
#include <sys/types.h>
#include <Winsock2.h>
#include <vector>
#include "ImageServerLib.h"
#include <windows.h>
#include <sstream>
#include <shellapi.h>
#include <filesystem>
#include <conio.h>
#include <string>
#include <locale>
#include <shlobj_core.h>

#pragma warning( disable : 4996)
#pragma comment(lib, "Ws2_32.lib")

#endif //PCH_H
