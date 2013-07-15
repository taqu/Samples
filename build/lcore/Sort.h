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
    void heapsort(u32 n, T* v)
    {
        --v;
        u32 i, j;
        T x;
        for(u32 k=n>>1; k>=1; --k){
            i=k;
            x = v[k];
            while((j=i<<1)<=n){
                if(j<n && v[j] < v[j+1]){
                    ++j;
                }
                if(x>=v[j]){
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
                if(j<n && v[j] < v[j+1]){
                    ++j;
                }
                if(x>=v[j]){
                    break;
                }
                v[i] = v[j];
                i = j;
            }
            v[i] = x;
        }
    }

    /**

    Uはbool operator(const T& a, const T& b) const{ return a<b;}が必要
    */
    template<class T, class U>
    void heapsort(u32 n, T* v, U func)
    {
        --v;
        u32 i, j;
        T x;
        for(u32 k=n>>1; k>=1; --k){
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
}
#endif //INC_LCORE_SORT_H__
