#ifndef INC_LCORE_CLIBRARY_H__
#define INC_LCORE_CLIBRARY_H__
/**
@file CLibrary.h
@author t-sakai
@date 2009/02/16 create
*/
#define NOMINMAX
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

namespace lcore
{
#if defined(ANDROID)
    template<class T>
    inline T absolute(T val)
    {
        return abs(val);
    }

    template<>
    inline s32 absolute<s32>(s32 val)
    {
        return abs(val);
    }

    template<>
    inline u8 absolute<u8>(u8 val)
    {
        return val;
    }

    template<>
    inline u16 absolute<u16>(u16 val)
    {
        return val;
    }

    template<>
    inline u32 absolute<u32>(u32 val)
    {
        return val;
    }

    template<>
    inline f32 absolute<f32>(f32 val)
    {
#if 1
        *((u32*)&val) &= 0x7FFFFFFFU;
        return val;
#else
        return fabs(val);
#endif
    }

    template<>
    inline f64 absolute<f64>(f64 val)
    {
        return fabs(val);
    }

#else
    template<class T>
    inline T absolute(T val)
    {
        return abs(val);
    }


    template<>
    inline u8 absolute<u8>(u8 val)
    {
        return val;
    }

    template<>
    inline u16 absolute<u16>(u16 val)
    {
        return val;
    }

    template<>
    inline u32 absolute<u32>(u32 val)
    {
        return val;
    }

    template<>
    inline f32 absolute<f32>(f32 val)
    {
#if 1
        *((u32*)&val) &= 0x7FFFFFFFU;
        return val;
#else
        return fabs(val);
#endif
    }

    template<>
    inline f64 absolute<f64>(f64 val)
    {
        return fabs(val);
    }
#endif

    inline bool isNan(double f)
    {
#if defined(_WIN32)
        return (0 != ::_isnan(f));
#else
        return (0 != isnan(f));
#endif
    }

    inline bool isSpace(Char c)
    {
        return (isspace(c) != 0);
    }

    inline bool isNullChar(Char c)
    {
        return (c == '\0');
    }

    inline int isEOL(const Char* str)
    {
        if(0x0D == *str){
            return (0x0A == *(str+1))? 2 : 1;
        }else if(0x0A == *str){
            return 1;
        }
        return 0;
    }

    inline int strncmp(const Char* str1, const Char* str2, int maxCount)
    {
        return ::strncmp(str1, str2, maxCount);
    }

    inline char* strncpy(Char* dst, size_t dstSize, const Char* src, size_t count)
    {
#if defined(_WIN32) || defined(_WIN64)
        strncpy_s(dst, dstSize, src, count);
        return dst;
#else
        return ::strncpy(dst, src, count);
#endif
    }

    inline size_t strlen(const Char* str)
    {
        return ::strlen(str);
    }

    inline const Char* strstr(const Char* str, const Char* key)
    {
        return ::strstr(str, key);
    }


    //-----------------------------------------------------
    //
    inline int memcmp(const void* buff0, const void* buff1, unsigned int size)
    {
        return ::memcmp(buff0, buff1, size);
    }

    //-----------------------------------------------------
    //
    inline void* memset(void* dst, int val, unsigned int size)
    {
        return ::memset(dst, val, size);
    }

    //-----------------------------------------------------
    //
    inline void* memcpy(void* dst, const void* src, size_t size)
    {
        return ::memcpy(dst, src, size);
    }
}
#endif //INC_LCORE_CLIBRARY_H__
