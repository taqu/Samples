#ifndef INC_LGRAPHICSAPIINCLUDE_H__
#define INC_LGRAPHICSAPIINCLUDE_H__
/**
@file lgraphicsAPIInclude.h
@author t-sakai
@date 2009/04/26 create
*/
#include "lgraphicscore.h"

//----------------------------------------------------------
#if defined(LIME_INCLUDE_DX9)
#include <d3d9.h>
#include <d3dx9.h>
#include <dxgi.h>
#endif

//----------------------------------------------------------
#if defined(LIME_INCLUDE_DX11)
#include <dxgi.h>
#include <d3d11.h>
#endif

//----------------------------------------------------------
#if defined(LIME_INCLUDE_GLES1)

#if defined(LIME_INITGL)
#include "api/gles1/lgl.h"
#include "api/gles1/lglext.h"
#else
#include <GLES/gl.h>
#endif

#if defined(ANDROID)
#else
#include <GLES/egl.h>
#endif

#endif

//----------------------------------------------------------
#if defined(LIME_INCLUDE_GLES2)

#if defined(LIME_INCLUDE_EGL)
#include "api/gles2/legl.h"
#endif

#include "api/gles2/lgl2.h"
#include "api/gles2/lgl2ext.h"

#endif //LIME_GLES2


namespace lgraphics
{
#if defined(LIME_GL)
    typedef s32 HANDLE;
#elif defined(LIME_DX9)
    typedef D3DXHANDLE HANDLE;
#endif
}
#endif //INC_LGRAPHICSAPIINCLUDE_H__
