#ifndef INC_LGRAPHICS_SURFACEREF_H__
#define INC_LGRAPHICS_SURFACEREF_H__
/**
@file SurfaceRef.h
@author t-sakai
@date 2010/01/08 create
@date 2010/12/12 add gles2
*/

#if defined(LIME_DX9)
#include "dx9/SurfaceRef.h"

#elif defined(LIME_GLES2)
#include "gles2/SurfaceRef.h"

#endif

#endif //INC_LGRAPHICS_SURFACEREF_H__
