#ifndef INC_LGRAPHICSCORE_H__
#define INC_LGRAPHICSCORE_H__
/**
@file lgraphicscore.h
@author t-sakai
@date 2009/01/19 create
*/
#include <lcore/lcore.h>
#include <lcore/String.h>


//-------------------------------------
//--- Features
//#define LIME_USE_DUAL_QUATERNION_FOR_SKINNING (1)

#ifdef _WIN32
struct HWND__;
struct tagRECT;
#else
typedef void* HWND__;
#endif

#define SAFE_RELEASE(p) {if(p!=NULL){(p)->Release();(p)=NULL;}}
#define LSAFE_RELEASE(p) {if(p!=NULL){(p)->release();(p)=NULL;}}
#define LRELEASE(p) {(p)->release();(p)=NULL;}
#define LBUFFER_OFFSET(offset) ((char*)NULL + (offset))

namespace lgraphics
{
    using lcore::s8;
    using lcore::s16;
    using lcore::s32;
    using lcore::s64;
    using lcore::u8;
    using lcore::u16;
    using lcore::u32;
    using lcore::u64;
    using lcore::f32;
    using lcore::f64;

    using lcore::Char;

    static const u32 LIME_MAX_VERTEX_STREAMS = 2; //最大頂点ストリーム数

#if defined(LIME_GLES1) || defined(LIME_GLES2)
#define LIME_GL (1)
#endif

    static const u32 MAX_NAME_BUFFER_SIZE = 48;
    typedef lcore::String<MAX_NAME_BUFFER_SIZE> NameString;

    static const u32 INVALID_SAMPLER_INDEX = 0xFFFFFFFFU;
    static const u32 INVALID_TEXTURE_ID = 0xFFFFFFFFU;
    static const u32 INVALID_TEXTURE_LOCATION = 0xFFFFFFFFU;

    static const u32 MAX_TEXTURES = 3;

    //Bufferの場合はバイト単位、テクスチャの場合はテクセル単位
    struct Box
    {
        Box(){}
        Box(u32 left, u32 top, u32 front, u32 right, u32 bottom, u32 back)
            :left_(left)
            ,top_(top)
            ,front_(front)
            ,right_(right)
            ,bottom_(bottom)
            ,back_(back)
        {}

        u32 left_;
        u32 top_;
        u32 front_;
        u32 right_;
        u32 bottom_;
        u32 back_;
    };

    template<class T>
    class ScopedCOMPtr
    {
    public:
        ScopedCOMPtr()
            :ptr_(NULL)
        {}

        ScopedCOMPtr(T* ptr)
            :ptr_(ptr)
        {}

        ~ScopedCOMPtr()
        {
            if(ptr_){
                ptr_->Release();
            }
        }

        T* get(){ return ptr_;}

        T* ptr_;
    };

//----------------------------------------------------------
//---
//--- プラットフォーム、API依存のdefine
//---
//----------------------------------------------------------

#if defined(LIME_DX9)
//----------------------------------------------------------
#define LIME_INCLUDE_DX9

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

static const lgraphics::u8 LIME_MAX_SKINNING_MATRICES = (64);

#elif defined(LIME_DX11)
//----------------------------------------------------------
#define LIME_INCLUDE_DX11

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

static const lgraphics::u8 LIME_MAX_SKINNING_MATRICES = (255);
static const u32 MaxRenderTargets = 8;

#elif defined(LIME_GLES1)
//----------------------------------------------------------
#define LIME_INCLUDE_GLES1

#define LIME_INITGL

#elif defined(LIME_GLES2)
//----------------------------------------------------------

#define LIME_INCLUDE_GLES2

static const lgraphics::u8 LIME_MAX_SKINNING_MATRICES = (36);
//static const lgraphics::u8 LIME_MAX_SKINNING_MATRICES = (18);

#if defined(ANDROID)

#else // NOT ANDROID

#if defined(LIME_GLES2_ATI)
#define LIME_INIT_GLES2

#define LIME_INCLUDE_EGL

#define LIME_INIT_EGL

#define LIME_GLES2_DLL_NAME "atioglxx.dll"
#else

#define LIME_INCLUDE_EGL

#endif //LIME_GLES2_ATI

#endif // NOT ANDROID

#endif //LIME_GLES2

}
#endif //INC_LGRAPHICSCORE_H__
