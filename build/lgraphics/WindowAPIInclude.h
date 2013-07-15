#ifndef INC_WINDOWAPIINCLUDE_H__
#define INC_WINDOWAPIINCLUDE_H__
/**
@file WindowAPIInclude.h
@author t-sakai
@date 2009/04/26 create
*/
#if defined(__CYGWIN__) || defined(__linux__)
#define LGRAPHICS_USEX__ (1)
#endif

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct WindowHandle
{
    HWND hWnd_;
};

#elif defined(LGRAPHICS_USEX__)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

typedef int(*WNDPROC)(XEvent& event);
struct WindowHandle
{
    Display* display_;
    ::Window window_;
};
#endif

#endif //INC_WINDOWAPIINCLUDE_H__
