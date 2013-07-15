#ifndef INC_STDAFX_H__
#define INC_STDAFX_H__
/**
@file stdafx.h
@author t-sakai
@date 2011/06/06 create
*/
#include <lcore/lcore.h>
#include <lmath/lmath.h>
#include <lgraphics/lgraphics.h>

namespace lcore
{
    class ifstream;
}

namespace lgraphics
{
    class Texture2DRef;
}

namespace fractal
{
    using lcore::s8;
    using lcore::s16;
    using lcore::s32;

    using lcore::u8;
    using lcore::u16;
    using lcore::u32;

    using lcore::f32;
    using lcore::f64;

    using lcore::Char;

    void* allocate(u32 size);
    void deallocate(void*& ptr, u32 size);


#define FRACTAL_ALLOC(size) fractal::allocate((size))
#define FRACTAL_DEALLOC(ptr, size) fractal::deallocate((void*&)(ptr), (size))

    template<class T>
    T* allocate()
    {
        void* ptr = allocate(sizeof(T));
        return LIME_PLACEMENT_NEW(ptr) T();
    }

    template<class T>
    void deallocate(T*& ptr)
    {
        if(NULL != ptr){
            ptr->~T();
            deallocate((void*&)ptr, sizeof(T));
        }
    }

    template<class T>
    T* construct(u8* buffer, u32 offset, u32 num)
    {
        LASSERT(buffer != NULL);

        u8* ret = buffer + offset;
        u8* t = ret;
        for(u32 i=0; i<num; ++i){
            LIME_PLACEMENT_NEW(t) T();
            t += sizeof(T);
        }
        return reinterpret_cast<T*>(ret);
    }

    template<class T>
    void destruct(T* t, u32 num)
    {
        LASSERT(t != NULL);
        for(u32 i=0; i<num; ++i, ++t){
            t->~T();
        }
    }

    enum Layer
    {
        Layer_0 =0,
        Layer_1,
        Layer_2,
        Layer_3,
        Layer_Player,
        Layer_5,
        Layer_6,
        Layer_7,
        Layer_8,
        Layer_9,
        Layer_Num,
    };

    f32 getLayerY(Layer layer);

    bool loadDDS(lgraphics::Texture2DRef& texture, lcore::ifstream& is, s32 offset);
}
#endif //INC_STDAFX_H__
