#ifndef INC_LGRAPHICS_LEGL_H__
#define INC_LGRAPHICS_LEGL_H__
/**
@file legl.h
@author t-sakai
@date 2010/12/03 create

*/
#include <EGL/egl.h>
//#include <EGL/eglext.h>
#include "../../lgraphicscore.h"

#ifdef __cplusplus
extern "C"
{
#endif

    bool initializeEGL();
    void terminateEGL();

    void dumpEGLExtensions(const char* filepath);

#if !defined(LIME_EGL_API)
#define LIME_EGL_API extern
#endif

#if !defined(LIME_EGL_APIENTRY)
#define LIME_EGL_APIENTRY EGLAPIENTRY
#endif

#if !defined(LIME_EGL_FUNC_NULL)
#define LIME_EGL_FUNC_NULL
#endif

#if !defined(LIME_EGL_FUNC)
#define LIME_EGL_FUNC(ret, name, args) LIME_EGL_API ret (LIME_EGL_APIENTRY *fptr_##name) args LIME_EGL_FUNC_NULL;
#endif

#if !defined(LIME_EGL_FUNCPTR)
#define LIME_EGL_FUNCPTR(name) fptr_##name
#endif

#if defined(LIME_INIT_EGL)

LIME_EGL_FUNC(EGLint, eglGetError, (void));
LIME_EGL_FUNC(EGLDisplay, eglGetDisplay, (EGLNativeDisplayType display_id));
LIME_EGL_FUNC(EGLBoolean, eglInitialize, (EGLDisplay dpy, EGLint *major, EGLint *minor));
LIME_EGL_FUNC(EGLBoolean, eglTerminate, (EGLDisplay dpy));
LIME_EGL_FUNC(const char*, eglQueryString, (EGLDisplay dpy, EGLint name));
LIME_EGL_FUNC(EGLBoolean, eglDestroySurface, (EGLDisplay dpy, EGLSurface surface));
LIME_EGL_FUNC(EGLBoolean, eglBindAPI, (EGLenum api));
LIME_EGL_FUNC(EGLenum, eglQueryAPI, (void));
LIME_EGL_FUNC(EGLBoolean, eglWaitClient, (void));
LIME_EGL_FUNC(EGLBoolean, eglReleaseThread, (void));
LIME_EGL_FUNC(EGLBoolean, eglBindTexImage, (EGLDisplay dpy, EGLSurface surface, EGLint buffer));
LIME_EGL_FUNC(EGLBoolean, eglReleaseTexImage, (EGLDisplay dpy, EGLSurface surface, EGLint buffer));
LIME_EGL_FUNC(EGLBoolean, eglSwapInterval, (EGLDisplay dpy, EGLint interval));
LIME_EGL_FUNC(EGLBoolean, eglDestroyContext, (EGLDisplay dpy, EGLContext ctx));
LIME_EGL_FUNC(EGLContext, eglGetCurrentContext, (void));
LIME_EGL_FUNC(EGLSurface, eglGetCurrentSurface, (EGLint readdraw));
LIME_EGL_FUNC(EGLDisplay, eglGetCurrentDisplay, (void));
LIME_EGL_FUNC(EGLBoolean, eglWaitGL, (void));
LIME_EGL_FUNC(EGLBoolean, eglWaitNative, (EGLint engine));
LIME_EGL_FUNC(EGLBoolean, eglSwapBuffers, (EGLDisplay dpy, EGLSurface surface));

// egl1.4ヘッダには含まれない
LIME_EGL_FUNC(EGLSurface, eglCreateWindowSurface, (EGLDisplay dpy, EGLConfig config, NativeWindowType window, const EGLint *attrib_list));
LIME_EGL_FUNC(EGLContext, eglCreateContext, (EGLDisplay dpy, EGLConfig config, EGLContext share_list, const EGLint *attrib_list));
LIME_EGL_FUNC(EGLBoolean, eglMakeCurrent, (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx));
LIME_EGL_FUNC(EGLBoolean, eglChooseConfig, (EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config));
LIME_EGL_FUNC(void*, eglGetProcAddress, (const char *procname));

/**************************************************************/
LIME_EGL_FUNC(EGLBoolean, eglLockSurfaceKHR, (EGLDisplay display, EGLSurface surface, const EGLint *attrib_list));
LIME_EGL_FUNC(EGLBoolean, eglUnlockSurfaceKHR, (EGLDisplay display, EGLSurface surface));

LIME_EGL_FUNC(EGLImageKHR, eglCreateImageKHR, (EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint *attrib_list));
LIME_EGL_FUNC(EGLBoolean, eglDestroyImageKHR, (EGLDisplay dpy, EGLImageKHR image));

LIME_EGL_FUNC(EGLSyncKHR, eglCreateSyncKHR, (EGLDisplay dpy, EGLenum type, const EGLint *attrib_list));
LIME_EGL_FUNC(EGLBoolean, eglDestroySyncKHR, (EGLDisplay dpy, EGLSyncKHR sync));
LIME_EGL_FUNC(EGLint, eglClientWaitSyncKHR, (EGLDisplay dpy, EGLSyncKHR sync, EGLint flags, EGLTimeKHR timeout));
LIME_EGL_FUNC(EGLBoolean, eglSignalSyncKHR, (EGLDisplay dpy, EGLSyncKHR sync, EGLenum mode));
LIME_EGL_FUNC(EGLBoolean, eglGetSyncAttribKHR, (EGLDisplay dpy, EGLSyncKHR sync, EGLint attribute, EGLint *value));

LIME_EGL_FUNC(EGLSurface, eglCreatePixmapSurfaceHI, (EGLDisplay dpy, EGLConfig config, struct EGLClientPixmapHI* pixmap));

LIME_EGL_FUNC(EGLImageKHR, eglCreateDRMImageMESA, (EGLDisplay dpy, const EGLint *attrib_list));
LIME_EGL_FUNC(EGLBoolean, eglExportDRMImageMESA, (EGLDisplay dpy, EGLImageKHR image, EGLint *name, EGLint *handle, EGLint *stride));


#if !defined(LIME_EGL_NO_FUNCDEF)

/**************************************************************/
#define eglGetError	LIME_EGL_FUNCPTR(eglGetError)
#define eglGetDisplay	LIME_EGL_FUNCPTR(eglGetDisplay)
#define eglInitialize	LIME_EGL_FUNCPTR(eglInitialize)
#define eglTerminate	LIME_EGL_FUNCPTR(eglTerminate)
#define eglQueryString	LIME_EGL_FUNCPTR(eglQueryString)
#define eglDestroySurface	LIME_EGL_FUNCPTR(eglDestroySurface)
#define eglBindAPI	LIME_EGL_FUNCPTR(eglBindAPI)
#define eglQueryAPI	LIME_EGL_FUNCPTR(eglQueryAPI)
#define eglWaitClient	LIME_EGL_FUNCPTR(eglWaitClient)
#define eglReleaseThread	LIME_EGL_FUNCPTR(eglReleaseThread)
#define eglBindTexImage	LIME_EGL_FUNCPTR(eglBindTexImage)
#define eglReleaseTexImage	LIME_EGL_FUNCPTR(eglReleaseTexImage)
#define eglSwapInterval	LIME_EGL_FUNCPTR(eglSwapInterval)
#define eglDestroyContext	LIME_EGL_FUNCPTR(eglDestroyContext)
#define eglGetCurrentContext	LIME_EGL_FUNCPTR(eglGetCurrentContext)
#define eglGetCurrentSurface	LIME_EGL_FUNCPTR(eglGetCurrentSurface)
#define eglGetCurrentDisplay	LIME_EGL_FUNCPTR(eglGetCurrentDisplay)
#define eglWaitGL	LIME_EGL_FUNCPTR(eglWaitGL)
#define eglWaitNative	LIME_EGL_FUNCPTR(eglWaitNative)
#define eglSwapBuffers	LIME_EGL_FUNCPTR(eglSwapBuffers)

// egl1.4ヘッダには含まれない
#define eglCreateWindowSurface	LIME_EGL_FUNCPTR(eglCreateWindowSurface)
#define eglCreateContext	LIME_EGL_FUNCPTR(eglCreateContext)
#define eglMakeCurrent	LIME_EGL_FUNCPTR(eglMakeCurrent)
#define eglChooseConfig	LIME_EGL_FUNCPTR(eglChooseConfig)
#define eglGetProcAddress LIME_EGL_FUNCPTR(eglGetProcAddress);

/**************************************************************/
#define eglLockSurfaceKHR	LIME_EGL_FUNCPTR(eglLockSurfaceKHR)
#define eglUnlockSurfaceKHR	LIME_EGL_FUNCPTR(eglUnlockSurfaceKHR)

#define eglCreateImageKHR	LIME_EGL_FUNCPTR(eglCreateImageKHR)
#define eglDestroyImageKHR	LIME_EGL_FUNCPTR(eglDestroyImageKHR)

#define eglCreateSyncKHR	LIME_EGL_FUNCPTR(eglCreateSyncKHR)
#define eglDestroySyncKHR	LIME_EGL_FUNCPTR(eglDestroySyncKHR)
#define eglClientWaitSyncKHR	LIME_EGL_FUNCPTR(eglClientWaitSyncKHR)
#define eglSignalSyncKHR	LIME_EGL_FUNCPTR(eglSignalSyncKHR)
#define eglGetSyncAttribKHR	LIME_EGL_FUNCPTR(eglGetSyncAttribKHR)

#define eglCreatePixmapSurfaceHI	LIME_EGL_FUNCPTR(eglCreatePixmapSurfaceHI)

#define eglCreateDRMImageMESA	LIME_EGL_FUNCPTR(eglCreateDRMImageMESA)
#define eglExportDRMImageMESA	LIME_EGL_FUNCPTR(eglExportDRMImageMESA)

#endif //LIME_EGL_NO_FUNCDEF

#endif //LIME_INIT_EGL

#ifdef __cplusplus
}
#endif

#endif //INC_LGRAPHICS_LEGL_H__
