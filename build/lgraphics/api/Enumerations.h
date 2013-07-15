#ifndef INC_LGRAPHICS_ENUMERATIONS_H__
#define INC_LGRAPHICS_ENUMERATIONS_H__
/**
@file Enumerations.h
@author t-sakai
@date 2010/04/02 create
@date 2010/06/06 add gles1 implementation
@date 2010/12/12 add gles2
*/
#if defined(LIME_DX9)
#include "dx9/Enumerations.h"
#elif defined(LIME_DX11)
#include "dx11/Enumerations.h"
#elif defined(LIME_GLES1)
#include "gles1/Enumerations.h"
#elif defined(LIME_GLES2)
#include "gles2/Enumerations.h"
#endif

#endif //INC_LGRAPHICS_ENUMERATIONS_H__
