#ifndef INC_LCORE_SORT_H__
#define INC_LCORE_SORT_H__
/**
@file Sort.h
@author t-sakai
@date 2011/12/31 create
*/
#include "lcore.h"

namespace lcore
{
    template<class T>
    struct SortCompFuncType
    {
        typedef bool(*SortCmp)(const T& lhs, const T& rhs);
    };

    template<class T>
    bool less(const T& lhs, const T& rhs)
    {
        return (lhs<rhs);
    }

    //------------------------------------------------
    //---
    //--- insertionsort
    //---
    //------------------------------------------------
    /**

    Uはbool operator(const T& a, const T& b) const{ return a<b;}が必要
    */
    template<class T, class U>
    void insertionsort(s32 n, T* v, U func)
    {
        for(s32 i=1; i<n; ++i){
            for(s32 j=i; 0<j; --j){
                s32 k = j-1;
                if(func(v[j], v[k])){
                    lcore::swap(v[j], v[k]);
                }else{
                    break;
                }
            }
        }
    }

    template<class T>
    void insertionsort(s32 n, T* v)
    {
        insertionsort(n, v, less<T>);
    }

    //------------------------------------------------
    //---
    //--- heapsort
    //---
    //------------------------------------------------
    /**

    Uはbool operator(const T& a, const T& b) const{ return a<b;}が必要
    */
    template<class T, class U>
    void heapsort(s32 n, T* v, U func)
    {
        LASSERT(0<=n);

        --v;
        s32 i, j;
        T x;
        for(s32 k=n>>1; k>=1; --k){
            i=k;
            x = v[k];
            while((j=i<<1)<=n){
                if(j<n && func(v[j], v[j+1])){
                    ++j;
                }

                if(!func(x, v[j])){
                    break;
                }
                v[i] = v[j];
                i = j;
            }
            v[i] = x;
        }

        while(n>1){
            x = v[n];
            v[n] = v[1];
            --n;
            i = 1;
            while((j=i<<1)<=n){
                if(j<n && func(v[j], v[j+1])){
                    ++j;
                }

                if(!func(x, v[j])){
                    break;
                }
                v[i] = v[j];
                i = j;
            }
            v[i] = x;
        }
    }

    template<class T>
    void heapsort(s32 n, T* v)
    {
        heapsort(n, v, less<T>);
    }

    //------------------------------------------------
    //---
    //--- quicksort
    //---
    //------------------------------------------------
    /**

    Uはbool operator(const T& a, const T& b) const{ return a<b;}が必要
    */
    template<class T, class U>
    void quicksort(s32 n, T* v, U func)
    {
        static const s32 SwitchN = 47;
        if(n<SwitchN){
            insertionsort(n, v, func);
            return;
        }

        s32 i0 = 0;
        s32 i1 = n-1;

        T pivot = v[ (i0+i1)>>1 ];

        for(;;){
            while(func(v[i0], pivot)){
                ++i0;
            }

            while(func(pivot, v[i1])){
                --i1;
            }

            if(i1<=i0){
                break;
            }
            lcore::swap(v[i0], v[i1]);
            ++i0;
            --i1;
        }

        if(1<i0){
            quicksort(i0, v, func);
        }

        ++i1;
        n = n-i1;
        if(1<n){
            quicksort(n, v+i1, func);
        }
    }

    template<class T>
    void quicksort(s32 n, T* v)
    {
        quicksort(n, v, less<T>);
    }

    //------------------------------------------------
    //---
    //--- introsort
    //---
    //------------------------------------------------
    /**

    Uはbool operator(const T& a, const T& b) const{ return a<b;}が必要
    */
    template<class T, class U>
    void introsort(s32 n, T* v, s32 depth, U func)
    {
        static const s32 SwitchN = 47;
        if(n<SwitchN){
            insertionsort(n, v, func);
            return;
        }
        if(depth<=0){
            heapsort(n, v, func);
            return;
        }

        s32 i0 = 0;
        s32 i1 = n-1;

        T pivot = v[ (i0+i1)>>1 ];

        for(;;){
            while(func(v[i0], pivot)){
                ++i0;
            }

            while(func(pivot, v[i1])){
                --i1;
            }

            if(i1<=i0){
                break;
            }
            lcore::swap(v[i0], v[i1]);
            ++i0;
            --i1;
        }

        --depth;
        if(1<i0){
            introsort(i0, v, depth, func);
        }

        ++i1;
        n = n-i1;
        if(1<n){
            introsort(n, v+i1, depth, func);
        }
    }

    template<class T, class U>
    void introsort(s32 n, T* v, U func)
    {
        s32 depth = 0;
        s32 t = n;
        while(1<t){
            ++depth;
            t >>= 1;
        }
        introsort(n, v, depth, func);
    }

    template<class T>
    void introsort(s32 n, T* v)
    {
        introsort(n, v, less<T>);
    }
}
#endif //INC_LCORE_SORT_H__
