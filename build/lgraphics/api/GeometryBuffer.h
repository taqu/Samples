#ifndef INC_LGRAPHICS_GEOMETRYBUFFER_H__
#define INC_LGRAPHICS_GEOMETRYBUFFER_H__
/**
@file GeometryBuffer.h
@author t-sakai
@date 2010/01/08 create
@date 2010/07/10 add gles1 implementation
@date 2010/12/12 add gles2
*/

#if defined(LIME_DX9)
#include "dx9/GeometryBuffer.h"

#elif defined(LIME_DX11)
#include "dx11/GeometryBuffer.h"

#elif defined(LIME_GLES1)
#include "gles1/GeometryBuffer.h"

#elif defined(LIME_GLES2)
#include "gles2/GeometryBuffer.h"
#endif

#endif //INC_LGRAPHICS_GEOMETRYBUFFER_H__
