#ifndef INC_LGRAPHICS_SHADERREF_H__
#define INC_LGRAPHICS_SHADERREF_H__
/**
@file ShaderRef.h
@author t-sakai
@date 2010/01/08 create
@date 2010/12/12 add gles2
*/

#if defined(LIME_DX9)
#include "dx9/ShaderRef.h"

#elif defined(LIME_DX11)
#include "dx11/ShaderRef.h"

#elif defined(LIME_GLES2)
#include "gles2/ShaderRef.h"

#endif

#endif //INC_LGRAPHICS_SHADERREF_H__
