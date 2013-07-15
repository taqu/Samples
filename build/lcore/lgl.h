#ifndef __LGL_H__
#define __LGL_H__
/*!
\file lgl.h
\author t-sakai
\date 2008/07/29 create
*/
#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/wglext.h>

namespace lime
{
    void glInitialize();

    extern PFNGLGENBUFFERSPROC glGenBuffers;
    extern PFNGLBINDBUFFERPROC glBindBuffer;
    extern PFNGLBUFFERDATAPROC glBufferData;
    extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
    extern PFNGLMAPBUFFERPROC glMapBuffer;
    extern PFNGLUNMAPBUFFERPROC glUnmapBuffer;
    extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;

}
#endif /*__LGL_H__*/
