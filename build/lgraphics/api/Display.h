#ifndef INC_LGRAPHICS_DISPLAY_H__
#define INC_LGRAPHICS_DISPLAY_H__
/**
@file Display.h
@author t-sakai
@date 2013/05/16 create
*/

#if defined(LIME_DX9)

#elif defined(LIME_DX11)
#include "dx11/Display.h"

#elif defined(LIME_GLES1)

#elif defined(LIME_GLES2)
#endif

#endif //INC_LGRAPHICS_DISPLAY_H__