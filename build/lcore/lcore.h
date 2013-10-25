#ifndef INC_LCORE_H__
#define INC_LCORE_H__
/**
@file lcore.h
@author t-sakai
@date 2009/01/17 create
*/
//#define NOMINMAX
#include <assert.h>
#include <float.h>

//-------------------
#if defined(_WIN32)

#if defined(_DEBUG)
#include <malloc.h>
#include <new>

#define LIME_NEW new(__FILE__,__LINE__)
//#define LIME_NEW new
#define LIME_RAW_NEW new

#else //_DEBUG
#include <malloc.h>
#include <new>

#define LIME_NEW new
#define LIME_RAW_NEW new

#endif

//#if !defined(WIN32_LEAN_AND_MEAN)
//#define WIN32_LEAN_AND_MEAN
//#endif //WIN32_LEAN_AND_MEAN
//#include <windows.h>

#else //_WIN32
#include <malloc.h>
#include <new>
#define LIME_NEW new
#define LIME_RAW_NEW new
#endif

//-------------------
#if defined(ANDROID) || defined(__linux__)
#include <stdint.h>
#endif //ANDROID __linux__

//-------------------
#if defined(ANDROID)
#include <android/log.h>
#endif //ANDROID


#include "LangSpec.h"


// メモリ確保・開放
//-------------------
void* lcore_malloc(std::size_t size);
void* lcore_malloc(std::size_t size, std::size_t alignment);

void lcore_free(void* ptr);
void lcore_free(void* ptr, std::size_t alignment);

void* lcore_malloc(std::size_t size, const char* file, int line);
void* lcore_malloc(std::size_t size, std::size_t alignment, const char* file, int line);

//-------------------
inline void* operator new(std::size_t size)
{
    return lcore_malloc(size);
}

inline void operator delete(void* ptr)
{
    lcore_free(ptr);
}

inline void* operator new[](std::size_t size)
{
    return lcore_malloc(size);
}

inline void operator delete[](void* ptr)
{
    lcore_free(ptr);
}

//-------------------
inline void* operator new(std::size_t size, const char* file, int line)
{
    return lcore_malloc(size, file, line);
}

inline void operator delete(void* ptr, const char* /*file*/, int /*line*/)
{
    lcore_free(ptr);
}

inline void* operator new[](std::size_t size, const char* file, int line)
{
    return lcore_malloc(size, file, line);
}

inline void operator delete[](void* ptr, const char* /*file*/, int /*line*/)
{
    lcore_free(ptr);
}


/// 16バイトアライメント変数指定
#define LIME_ALIGN16 _declspec(align(16))

#if defined(_DEBUG)

#define LIME_PLACEMENT_NEW(ptr) new(ptr)
#define LIME_DELETE(p) { delete p; (p)=NULL;}
#define LIME_DELETE_NONULL(p) delete p
//#define LIME_OPERATOR_NEW ::operator new
//#define LIME_OPERATOR_DELETE ::operator delete

#define LIME_DELETE_ARRAY(p) {delete[] (p); (p) = NULL;}

#define LIME_MALLOC(size) (lcore_malloc(size, __FILE__, __LINE__))
#define LIME_FREE(mem) {lcore_free(mem); mem = NULL;}

/// アライメント指定malloc
#define LIME_ALIGNED_MALLOC(size, align) (lcore_malloc(size, align, __FILE__, __LINE__))
/// アライメント指定free
#define LIME_ALIGNED_FREE(mem, align) (lcore_free(mem, align))

#else //defined(_DEBUG)

#define LIME_PLACEMENT_NEW(ptr) new(ptr)
#define LIME_DELETE(p) { delete p; (p)=NULL;}
#define LIME_DELETE_NONULL(p) delete p
//#define LIME_OPERATOR_NEW ::operator new
//#define LIME_OPERATOR_DELETE ::operator delete

#define LIME_DELETE_ARRAY(p) {delete[] (p); (p) = NULL;}

#define LIME_MALLOC(size) (lcore_malloc(size))
#define LIME_FREE(mem) {lcore_free(mem); mem = NULL;}

/// アライメント指定malloc
#define LIME_ALIGNED_MALLOC(size, align) (lcore_malloc(size, align))
/// アライメント指定free
#define LIME_ALIGNED_FREE(mem, align) (lcore_free(mem, align))
#endif


// 例外
//-------------------
#define LIME_THROW0 throw()


// Assertion
//-------------------
#if defined(_DEBUG)

#if defined(ANDROID)
#define LASSERT(expression) {if((expression)==false){__android_log_assert("assert", "lime", "%s (%d)", __FILE__, __LINE__);}}while(0)

#elif defined(__linux__)
#define LASSERT(expression) ( assert(expression) )

#else
#define LASSERT(expression) ( assert(expression) )
#endif

#else
#define LASSERT(expression)
#endif


//ユーティリティ
#define LCORE_BIT(n) (0x00000001U<<n)

namespace lcore
{


#if defined(_WIN32) || defined(_WIN64)
    typedef char Char;
    typedef __int8 s8;
    typedef __int16 s16;
    typedef __int32 s32;
    typedef __int64 s64;

    typedef unsigned __int8 u8;
    typedef unsigned __int16 u16;
    typedef unsigned __int32 u32;
    typedef unsigned __int64 u64;

    typedef float f32;
    typedef double f64;

    typedef ptrdiff_t  ptrdiff_t;
    typedef size_t lsize_t;

    typedef void* LHMODULE;

#elif defined(ANDROID) || defined(__linux__)
    typedef char Char;
    typedef int8_t s8;
    typedef int16_t s16;
    typedef int32_t s32;
    typedef int64_t s64;

    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;

    typedef float f32;
    typedef double f64;

    typedef ptrdiff_t  ptrdiff_t;
    typedef size_t lsize_t;

    typedef void* LHMODULE;

#else
    typedef char Char;
    typedef char s8;
    typedef short s16;
    typedef long s32;
    typedef long long s64;

    typedef unsigned char u8;
    typedef unsigned short u16;
    typedef unsigned long u32;
    typedef unsigned long long u64;

    typedef float f32;
    typedef double f64;

    typedef ptrdiff_t  ptrdiff_t;
    typedef size_t lsize_t;

    typedef void* LHMODULE;
#endif

#if defined(ANDROID) || defined(__linux__)
    typedef clock_t ClockType;
#else
    typedef u64 ClockType;
#endif


    //---------------------------------------------------------
    //---
    //--- numeric_limits
    //---
    //---------------------------------------------------------
    template<typename T>
    class numeric_limits
    {
    public:
        static T epsilon() LIME_THROW0
        {
            return std::numeric_limits<T>::epsilon();
        }

        static T minimum() LIME_THROW0
        {
            return (std::numeric_limits<T>::min)();
        }

        static T maximum() LIME_THROW0
        {
            return (std::numeric_limits<T>::max)();
        }

        static T inifinity() LIME_THROW0
        {
            return std::numeric_limits<T>::infinity();
        }
    };

    //---------------------------------------------------------
    //--- Utility
    //---------------------------------------------------------
    template<class T>
    inline void swap(T& l, T& r)
    {
        T tmp(l);
        l = r;
        r = tmp;
    }

    template<class T>
    inline T lerp(const T& v0, const T& v1, f32 ratio)
    {
        return v0 + ratio*(v1 - v0);
        //return (1.0f-ratio)*v0 + ratio*v1;
    }

    template<class T>
    inline const T& maximum(const T& left, const T& right)
    {
        
        return (left<right)? right : left;
    }

    template<class T>
    inline const T& minimum(const T& left, const T& right)
    {
        return (right<left)? right : left;
    }

    template<class T>
    inline T clamp(T val, T low, T high)
    {
        if (val <= low) return low;
        else if (val >= high) return high;
        else return val;
    }

    inline f32 clamp01(f32 v)
    {
        s32* t = (s32*)&v;
        s32 s = (*t) >> 31;
        s = ~s;
        *t &= s;

        v -= 1.0f;
        s = (*t) >> 31;
        *t &= s;
        v += 1.0f;
        return v;
    }


    bool isLittleEndian();

    struct U32F32Union
    {
        union
        {
            u32 u_;
            f32 f_;
        };
    };

    u16 toBinary16Float(f32 f);

    f32 fromBinary16Float(u16 s);

    inline u32 getARGB(u8 a, u8 r, u8 g, u8 b)
    {
        return (a << 24) | (r << 16) | (g << 8) | b;
    }

    inline u32 getABGR(u8 a, u8 r, u8 g, u8 b)
    {
        return (a << 24) | r | (g << 8) | (b << 16);
    }

    inline u32 getRGBA(u8 a, u8 r, u8 g, u8 b)
    {
        return (r << 24) | (g << 16) | (b << 8) | a;
    }


    inline u8 getAFromARGB(u32 argb)
    {
        return static_cast<u8>((argb>>24) & 0xFFU);
    }

    inline u8 getRFromARGB(u32 argb)
    {
        return static_cast<u8>((argb>>16) & 0xFFU);
    }

    inline u8 getGFromARGB(u32 argb)
    {
        return static_cast<u8>((argb>>8) & 0xFFU);
    }

    inline u8 getBFromARGB(u32 argb)
    {
        return static_cast<u8>((argb) & 0xFFU);
    }


    inline u8 getAFromABGR(u32 abgr)
    {
        return static_cast<u8>((abgr>>24) & 0xFFU);
    }

    inline u8 getRFromABGR(u32 abgr)
    {
        return static_cast<u8>((abgr) & 0xFFU);
    }

    inline u8 getGFromABGR(u32 abgr)
    {
        return static_cast<u8>((abgr>>8) & 0xFFU);
    }

    inline u8 getBFromABGR(u32 abgr)
    {
        return static_cast<u8>((abgr>>16) & 0xFFU);
    }


    inline u8 getRFromRGBA(u32 rgba)
    {
        return static_cast<u8>((rgba>>24) & 0xFFU);
    }

    inline u8 getGFromRGBA(u32 rgba)
    {
        return static_cast<u8>((rgba>>16) & 0xFFU);
    }

    inline u8 getBFromRGBA(u32 rgba)
    {
        return static_cast<u8>((rgba>>8) & 0xFFU);
    }

    inline u8 getAFromRGBA(u32 rgba)
    {
        return static_cast<u8>((rgba) & 0xFFU);
    }

    enum RefractiveIndex
    {
        RefractiveIndex_Vacuum =0,

        //気体
        RefractiveIndex_Air,
        
        //液体
        RefractiveIndex_Water,
        RefractiveIndex_Sea,

        //個体
        RefractiveIndex_SodiumChloride, //塩化ナトリウム
        RefractiveIndex_Carbon, //炭素
        RefractiveIndex_Silicon, //ケイ素

        //樹脂
        RefractiveIndex_FluorocarbonPolymers, //フッ素樹脂
        RefractiveIndex_SiliconPolymers, //シリコン樹脂
        RefractiveIndex_AcrylicResin, //アクリル樹脂
        RefractiveIndex_Polyethylene, //ポリエチレン
        RefractiveIndex_Polycarbonate, //ポリカーボネート
        RefractiveIndex_Asphalt, //アスファルト

        //ガラス
        RefractiveIndex_Fluorite, //フローライト
        RefractiveIndex_SodaLimeGlass, //ソーダ石灰ガラス
        RefractiveIndex_LeadGlass, //鉛ガラス、クリスタルガラス

        //結晶
        RefractiveIndex_Ice, //氷
        RefractiveIndex_RockCrystal, //水晶
        RefractiveIndex_Peridot, //ペリドット
        RefractiveIndex_Diamond, //ダイヤモンド

        //その他
        RefractiveIndex_Perl, //パール

        RefractiveIndex_Num,
    };

    f32 getRefractiveIndex(RefractiveIndex index);

    /**
    @brief 真空に対するフレネル反射係数の実部
    @param refract ... 出射側媒質の屈折率
    */
    f32 calcFresnelTerm(f32 refract);

    /**
    @brief フレネル反射係数の実部
    @param refract0 ... 入射側媒質の屈折率
    @param refract1 ... 出射側媒質の屈折率
    */
    f32 calcFresnelTerm(f32 refract0, f32 refract1);

#define LIME_MAKE_FOURCC(c0, c1, c2, c3)\
    ( (lcore::u32)(c0) | ((lcore::u32)(c1) << 8) | ((lcore::u32)(c2) << 16) | ((lcore::u32)(c3) << 24) )

    class LeakCheck
    {
    public:
        LeakCheck();
        ~LeakCheck();

    private:
        void* operator new(size_t);
        void* operator new[](size_t);
    };

    class System
    {
    public:
        System();
        ~System();

    private:
        LeakCheck leakCheck_;
    };


//#define LIME_ENABLE_LOG (1)
    void Log(const Char* format, ...);

    class MemorySpace
    {
    public:
        enum Locked
        {
            Locked_Disable = 0,
            Locked_Enable,
        };
        MemorySpace();
        ~MemorySpace();

        bool create(u32 capacity, Locked locked=Locked_Disable);
        void destroy();

        bool valid() const;
        void* allocate(u32 size);
        void deallocate(void* mem);
    private:
        void* mspace_;
    };
}

#endif //INC_LCORE_H__
