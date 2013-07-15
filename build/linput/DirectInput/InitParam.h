#ifndef INC_INITPARAM_H__
#define INC_INITPARAM_H__
/**
@file InitParam.h
@author t-sakai
@date 2010/07/23 create
*/
#include "linputcore.h"

#if defined(LIME_DINPUT)
struct HWND__;
#endif

namespace linput
{
#if defined(LIME_DINPUT)
    struct InitParam
    {
        HWND__* hWnd_;
    };
#endif
}
#endif //INC_INITPARAM_H__
