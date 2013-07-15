#ifndef INC_LGRAPHICS_INDEXBUFFERREF_H__
#define INC_LGRAPHICS_INDEXBUFFERREF_H__
/**
@file IndexBufferRef.h
@author t-sakai
@date 2010/01/08 create
@date 2010/06/08 add gles1 implementation
@date 2010/12/12 add gles2
*/

#if defined(LIME_DX9)
#include "dx9/IndexBufferRef.h"

#elif defined(LIME_DX11)
#include "dx11/IndexBufferRef.h"

#elif defined(LIME_GLES1)
#include "gles1/IndexBufferRef.h"

#elif defined(LIME_GLES2)
#include "gles2/IndexBufferRef.h"
#endif

#endif //INC_LGRAPHICS_INDEXBUFFERREF_H__
