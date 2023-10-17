#ifndef WIN32_WINDOW_H_
#define WIN32_WINDOW_H_

#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <stdint.h>

HWND CreateWin32Window();

LRESULT CALLBACK Win32WindowProc(HWND window, UINT message, WPARAM wparam,
                                 LPARAM lparam);

#endif
