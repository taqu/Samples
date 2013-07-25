#ifndef INC_SCENE_FFT_H__
#define INC_SCENE_FFT_H__
/**
@file FFT.h
@author t-sakai
@date 2013/07/04 create
*/
#include <lmath/lmath.h>
#include "scene.h"

namespace scene
{
    struct Complex
    {
        f32 x_;
        f32 y_;
    };

    /**
    @param N ... 4à»è„ÇÃ2ÇÃó›èÊ
    */
    template<int N>
    class FFT
    {
    public: 
        FFT(){}
        ~FFT(){}

        void initialize()
        {
            initSinTable();
            initBitTable();
        }

        void run(Complex* comp);
        void inv(Complex* comp);

        void run2D(Complex* comp);
        void inv2D(Complex* comp);
    private:
        void initSinTable();
        void initBitTable();

        void runV(Complex* comp);
        void invV(Complex* comp);

        void bitSwap(Complex* comp);
        void bitSwapV(Complex* comp);

        f32 sinTable_[N];
        s32 bitTable_[N];
    };

    template<int N>
    void FFT<N>::initSinTable()
    {
        s32 n2 = N/2;
        s32 n4 = N/4;
        s32 n8 = N/8;

        f32 t = lmath::sinf(PI/N);
        f32 dc = 2.0f*t*t;
        f32 ds = lmath::sqrt(dc*(2.0f-dc));
        t = 2.0f*dc;
        f32 c = sinTable_[n4] = 1.0f;
        f32 s = sinTable_[0] = 0.0f;

        for(s32 i=1; i<n8; ++i){
            c -= dc;
            dc += t*c;
            s += ds;
            ds -= t*s;
            sinTable_[i] = s;
            sinTable_[n4-i] = c;
        }

        if(0!=n8){
            sinTable_[n8] = lmath::sqrt(0.5f);
        }
        for(s32 i=0; i<n4; ++i){
            sinTable_[n2-i] = sinTable_[i];
        }
        for(s32 i=0; i<n2+n4; ++i){
            sinTable_[n2+i] = -sinTable_[i];
        }
    }

    template<int N>
    void FFT<N>::initBitTable()
    {
        s32 n2 = N/2;
        s32 i=0;
        s32 j=0;
        for(;;){
            bitTable_[i] = j;
            if(N<=++i){
                break;
            }
            s32 k=n2;
            while(k<=j){
                j -= k;
                k /= 2;
            }
            j += k;
        }
    }

    template<int N>
    void FFT<N>::run(Complex* comp)
    {
        bitSwap(comp);

        s32 n4 = N/4;
        s32 k2;
        for(s32 k=1; k<N; k=k2){
            s32 h=0;
            k2 = k+k;
            s32 d= N/k2;

            for(s32 j=0; j<k; ++j){
                f32 c = sinTable_[h+n4];
                f32 s = sinTable_[h];
                for(s32 i=j; i<N; i+=k2){
                    s32 ik = i+k;

                    f32 dx = s*comp[ik].y_ + c*comp[ik].x_;
                    f32 dy = c*comp[ik].y_ - s*comp[ik].x_;
                    comp[ik].x_ = comp[i].x_ - dx;
                    comp[i].x_ += dx;

                    comp[ik].y_ = comp[i].y_ - dy;
                    comp[i].y_ += dy;
                } //for(s32 i=j;
                h += d;
            } //for(s32 j=0;
        } //for(s32 k=1;

        const f32 invN = 1.0f/N;
        for(s32 i=0; i<N; ++i){
            comp[i].x_ *= invN;
            comp[i].y_ *= invN;
        }
    }

    template<int N>
    void FFT<N>::inv(Complex* comp)
    {
        bitSwap(comp);

        s32 n4 = N/4;
        s32 k2;
        for(s32 k=1; k<N; k=k2){
            s32 h=0;
            k2 = k+k;
            s32 d= N/k2;

            for(s32 j=0; j<k; ++j){
                f32 c = sinTable_[h+n4];
                f32 s = -sinTable_[h];
                for(s32 i=j; i<N; i+=k2){
                    s32 ik = i+k;
                    f32 dx = s*comp[ik].y_ + c*comp[ik].x_;
                    f32 dy = c*comp[ik].y_ - s*comp[ik].x_;
                    comp[ik].x_ = comp[i].x_ - dx;
                    comp[i].x_ += dx;

                    comp[ik].y_ = comp[i].y_ - dy;
                    comp[i].y_ += dy;
                } //for(s32 i=j;
                h += d;
            } //for(s32 j=0;
        } //for(s32 k=1;
    }

    template<int N>
    void FFT<N>::run2D(Complex* comp)
    {
        //â°
        for(s32 i=0; i<N; ++i){
            s32 iN = i*N;
            run(comp + iN);
        }

        //èc
        for(s32 i=0; i<N; ++i){
            runV(comp + i);
        }
    }

    template<int N>
    void FFT<N>::inv2D(Complex* comp)
    {
        //èc
        for(s32 i=0; i<N; ++i){
            invV(comp + i);
        }

        //â°
        for(s32 i=0; i<N; ++i){
            s32 iN = i*N;
            inv(comp + iN);
        }
    }

    template<int N>
    void FFT<N>::runV(Complex* comp)
    {
        bitSwapV(comp);

        s32 n4 = N/4;
        s32 k2;
        for(s32 k=1; k<N; k=k2){
            s32 h=0;
            k2 = k+k;
            s32 d= N/k2;

            for(s32 j=0; j<k; ++j){
                f32 c = sinTable_[h+n4];
                f32 s = sinTable_[h];
                for(s32 i=j; i<N; i+=k2){
                    s32 ik = (i+k)*N;
                    s32 iN = i*N;

                    f32 dx = s*comp[ik].y_ + c*comp[ik].x_;
                    f32 dy = c*comp[ik].y_ - s*comp[ik].x_;

                    comp[ik].x_ = comp[iN].x_ - dx;
                    comp[iN].x_ += dx;

                    comp[ik].y_ = comp[iN].y_ - dy;
                    comp[iN].y_ += dy;

                } //for(s32 i=j;
                h += d;
            } //for(s32 j=0;
        } //for(s32 k=1;

        const f32 invN = 1.0f/N;
        for(s32 i=0; i<N; ++i){
            comp[i*N].x_ *= invN;
            comp[i*N].y_ *= invN;
        }
    }

    template<int N>
    void FFT<N>::invV(Complex* comp)
    {
        bitSwapV(comp);

        s32 n4 = N/4;
        s32 k2;
        for(s32 k=1; k<N; k=k2){
            s32 h=0;
            k2 = k+k;
            s32 d= N/k2;

            for(s32 j=0; j<k; ++j){
                f32 c = sinTable_[h+n4];
                f32 s = -sinTable_[h];
                for(s32 i=j; i<N; i+=k2){
                    s32 ik = (i+k)*N;
                    s32 iN = i*N;

                    f32 dx = s*comp[ik].y_ + c*comp[ik].x_;
                    f32 dy = c*comp[ik].y_ - s*comp[ik].x_;

                    comp[ik].x_ = comp[iN].x_ - dx;
                    comp[iN].x_ += dx;

                    comp[ik].y_ = comp[iN].y_ - dy;
                    comp[iN].y_ += dy;
                } //for(s32 i=j;
                h += d;
            } //for(s32 j=0;
        } //for(s32 k=1;
    }

    template<int N>
    void FFT<N>::bitSwap(Complex* comp)
    {
        for(s32 i=0; i<N; ++i){
            s32 j=bitTable_[i];
            if(i<j){
                lcore::swap(comp[i], comp[j]);
            }
        }
    }

    template<int N>
    void FFT<N>::bitSwapV(Complex* comp)
    {
        for(s32 i=0; i<N; ++i){
            s32 j=bitTable_[i];
            if(i<j){
                lcore::swap(comp[i*N], comp[j*N]);
            }
        }
    }
}
#endif //INC_SCENE_FFT_H__