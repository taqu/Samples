/**
@file Noise.cpp
@author t-sakai
@date 2013/04/15 create

*/
#include "Noise.h"
#include <lmath/lmath.h>
#include <lmath/lmathutil.h>
#include <lgraphics/lgraphics.h>

namespace fractal
{
    void Perlin::initialize(u32 seed)
    {
        lcore::RandomWELL well;
        well.srand(seed);
        for(u32 i=0; i<NumSamples; ++i){
            rand_[i] = well.frand();
        }
    }

    u32 Perlin::srand(u32 v, u32 i) const
    {
        return (1812433253 * (v^(v >> 30)) + i); 
    }

    f32 Perlin::frand(u32 x) const
    {
#if 1
        u32 y = srand(x, 1);
        u32 z = srand(y, 2);
        u32 w = srand(z, 3);

        u32 t = x^(x<<11);
        w = (w^(w>>19)) ^ (t^(t>>8));

        //x = (x<<13) ^ x;
        //u32 w = x * (x * x * 15731 + 789221) + 1376312589;

        return rand_[w%NumSamples];
#else
        x = (x<<13) ^ x;
        return ( 1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7FFFFFFF) / 1073741824.0);
#endif
    }

    f32 Perlin::frand(u32 x, u32 y) const
    {
        return frand(x + 57*y);
    }

    f32 Perlin::frand(u32 x, u32 y, u32 z) const
    {
        return frand(x + 57*y + 131*z);
    }

    //f32 Perlin::smoothedRand(u32 x) const
    //{
    //    return frand(x) * 0.25f + frand(x+1) * 0.5f + frand(x+2) * 0.25f;
    //}

    f32 Perlin::operator()(f32 x) const
    {
        u32 ix = (u32)x;
        f32 fracx = x - ix;

        f32 p0 = frand(ix-1);
        f32 p1 = frand(ix);
        f32 p2 = frand(ix+1);
        f32 p3 = frand(ix+2);

        f32 ret;
        lmath::catmullRomCurve(ret, fracx, p0, p1, p2, p3);
        return ret;
#if 0
        f32 v0 = smoothedRand(ix);
        f32 v1 = smoothedRand(ix+1);

        return lcore::lerp(v0, v1, fracx);
#endif
    }

    //f32 Perlin::smoothedRand(u32 x, u32 y) const
    //{
    //    u32 x0 = x-1;
    //    u32 x1 = x;
    //    u32 x2 = x+1;

    //    u32 y0 = y-1;
    //    u32 y1 = y;
    //    u32 y2 = y+1;

    //    f32 corners = (frand(x0, y0) + frand(x2, y0) + frand(x0, y2) + frand(x2, y2)) * 1.0f/16.0f;
    //    f32 sides = (frand(x1, y0) + frand(x0, y1) + frand(x2, y1) + frand(x1, y2)) * 1.0f/8.0f;
    //    f32 center = frand(x1, y1) * 1.0f/4.0f;
    //    return (corners + sides + center);
    //}

    f32 Perlin::operator()(f32 x, f32 y) const
    {
        u32 ix = (u32)x;
        f32 fracx = x - ix;
        u32 iy = (u32)y;
        f32 fracy = y - iy;

        
        f32 p0 = frand(ix-1, iy-1);
        f32 p1 = frand(ix, iy-1);
        f32 p2 = frand(ix+1, iy-1);
        f32 p3 = frand(ix+2, iy-1);

        lmath::Vector4 v0(p0, p1, p2, p3);

        p0 = frand(ix-1, iy);
        p1 = frand(ix, iy);
        p2 = frand(ix+1, iy);
        p3 = frand(ix+2, iy);
        lmath::Vector4 v1(p0, p1, p2, p3);

        p0 = frand(ix-1, iy+1);
        p1 = frand(ix, iy+1);
        p2 = frand(ix+1, iy+1);
        p3 = frand(ix+2, iy+1);
        lmath::Vector4 v2(p0, p1, p2, p3);

        p0 = frand(ix-1, iy+2);
        p1 = frand(ix, iy+2);
        p2 = frand(ix+1, iy+2);
        p3 = frand(ix+2, iy+2);
        lmath::Vector4 v3(p0, p1, p2, p3);

        lmath::Vector4 dst;
        lmath::catmullRomCurve(dst, fracy, v0, v1, v2, v3);
        f32 ret;
        lmath::catmullRomCurve(ret, fracx, dst.x_, dst.y_, dst.z_, dst.w_);
        return ret;
#if 0
        f32 v0 = smoothedRand(ix, iy);
        f32 v1 = smoothedRand(ix+1, iy);
        f32 v2 = smoothedRand(ix, iy+1);
        f32 v3 = smoothedRand(ix+1, iy+1);

        f32 f0 = lcore::lerp(v0, v1, fracx);
        f32 f1 = lcore::lerp(v2, v3, fracx);
        return lcore::lerp(f0, f1, fracy);
#endif
    }

    //f32 Perlin::smoothedRand(u32 x, u32 y, u32 z) const
    //{
    //    u32 x0 = x-1;
    //    u32 x1 = x;
    //    u32 x2 = x+1;

    //    u32 y0 = y-1;
    //    u32 y1 = y;
    //    u32 y2 = y+1;

    //    u32 z0 = z-1;
    //    u32 z1 = z;
    //    u32 z2 = z+1;

    //    f32 corners = frand(x0, y0, z0) + frand(x2, y0, z0) + frand(x0, y2, z0) + frand(x2, y2, z0)
    //        + frand(x0, y0, z2) + frand(x2, y0, z2) + frand(x0, y2, z2) + frand(x2, y2, z2);

    //    f32 sides0 = frand(x0, y0, z1) + frand(x2, y0, z1) + frand(x0, y2, z1) + frand(x2, y2, z1)
    //        + frand(x1, y0, z0) + frand(x1, y0, z2) + frand(x1, y2, z0) + frand(x1, y2, z2)
    //        + frand(x0, y1, z0) + frand(x2, y1, z0) + frand(x0, y1, z2) + frand(x2, y1, z2);

    //    f32 sides1 = frand(x0, y1, z1) + frand(x2, y1, z1) + frand(x1, y1, z0) + frand(x1, y1, z2) + frand(x1, y0, z1) + frand(x1, y2, z1);

    //    f32 center = frand(x1, y1, z1);
    //    return (8.0f/64.0f)*center + (4.0f/64.0f)*sides1 + (2.0f/64.0f)*sides0 + (1.0f/64.0f)*corners;
    //}

    f32 Perlin::operator()(f32 x, f32 y, f32 z) const
    {
        u32 ix = (u32)x;
        f32 fracx = x - ix;
        u32 iy = (u32)y;
        f32 fracy = y - iy;
        u32 iz = (u32)z;
        f32 fracz = z - iz;

        f32 p0, p1, p2, p3;
        lmath::Vector4 v0, v1, v2, v3;
        lmath::Vector4 d[4];

        for(s32 i=-1; i<3; ++i){
            u32 tz = iz+i;
            p0 = frand(ix-1, iy-1, tz);
            p1 = frand(ix, iy-1, tz);
            p2 = frand(ix+1, iy-1, tz);
            p3 = frand(ix+2, iy-1, tz);
            v0.set(p0, p1, p2, p3);

            p0 = frand(ix-1, iy, tz);
            p1 = frand(ix, iy, tz);
            p2 = frand(ix+1, iy, tz);
            p3 = frand(ix+2, iy, tz);
            v1.set(p0, p1, p2, p3);

            p0 = frand(ix-1, iy+1, tz);
            p1 = frand(ix, iy+1, tz);
            p2 = frand(ix+1, iy+1, tz);
            p3 = frand(ix+2, iy+1, tz);
            v2.set(p0, p1, p2, p3);

            p0 = frand(ix-1, iy+2, tz);
            p1 = frand(ix, iy+2, tz);
            p2 = frand(ix+1, iy+2, tz);
            p3 = frand(ix+2, iy+2, tz);
            v3.set(p0, p1, p2, p3);
            lmath::catmullRomCurve(d[i+1], fracy, v0, v1, v2, v3);
        }



        lmath::Vector4 dst;
        lmath::catmullRomCurve(dst, fracz, d[0], d[1], d[2], d[3]);
        f32 ret;
        lmath::catmullRomCurve(ret, fracx, dst.x_, dst.y_, dst.z_, dst.w_);
        return ret;
#if 0
        f32 v0 = smoothedRand(ix, iy, iz);
        f32 v1 = smoothedRand(ix+1, iy, iz);
        f32 v2 = smoothedRand(ix, iy+1, iz);
        f32 v3 = smoothedRand(ix+1, iy+1, iz);

        f32 v4 = smoothedRand(ix, iy, iz+1);
        f32 v5 = smoothedRand(ix+1, iy, iz+1);
        f32 v6 = smoothedRand(ix, iy+1, iz+1);
        f32 v7 = smoothedRand(ix+1, iy+1, iz+1);

        f32 f0 = lcore::lerp(v0, v1, fracx);
        f32 f1 = lcore::lerp(v2, v3, fracx);
        f32 f2 = lcore::lerp(v4, v5, fracx);
        f32 f3 = lcore::lerp(v6, v7, fracx);


        f32 r0 = lcore::lerp(f0, f1, fracy);
        f32 r1 = lcore::lerp(f2, f3, fracy);

        return lcore::lerp(r0, r1, fracz);
#endif
    }

    void Perlin::normalize2(f32 v[2]) const
    {
        f32 inv = 1.0f/lmath::sqrt(v[0] * v[0] + v[1] * v[1]);

        v[0] *= inv;
        v[1] *= inv;
    }

    //------------------------------------------
    //---
    //--- Fractal
    //---
    //------------------------------------------
    f32 fractal2(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y)
    {
        f32 total = 0.0f;

        f32 amplitude = 1.0f;
        f32 frequency = param.frequency_;
        for(s32 i=0; i<param.octaves_; ++i){
            total += noise(x*frequency, y*frequency) * amplitude;

            frequency *= param.lacunarity_;
            amplitude *= param.persistence_;
        }

        return total;
    }

    f32 fractal3(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y, f32 z)
    {
        f32 total = 0.0f;

        f32 amplitude = 1.0f;
        f32 frequency = param.frequency_;
        for(s32 i=0; i<param.octaves_; ++i){
            total += noise(x*frequency, y*frequency, z*frequency) * amplitude;

            frequency *= param.lacunarity_;
            amplitude *= param.persistence_;
        }

        return total;
    }

    f32 turbulence2(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y)
    {
        f32 total = 0.0f;

        f32 amplitude = 1.0f;
        f32 frequency = param.frequency_;
        for(s32 i=0; i<param.octaves_; ++i){
            f32 v = 2.0f*noise(x*frequency, y*frequency) - 1.0f;
            total += lcore::absolute(v) * amplitude;

            frequency *= param.lacunarity_;
            amplitude *= param.persistence_;
        }

        return total;
    }

    f32 turbulence3(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y, f32 z)
    {
        f32 total = 0.0f;

        f32 amplitude = 1.0f;
        f32 frequency = param.frequency_;
        for(s32 i=0; i<param.octaves_; ++i){
            f32 v = 2.0f*noise(x*frequency, y*frequency, z*frequency) - 1.0f;
            total += lcore::absolute(v) * amplitude;

            frequency *= param.lacunarity_;
            amplitude *= param.persistence_;
        }

        return total;
    }

    f32 multiFractal2(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y)
    {
        f32 total;
        f32 increment;
        f32 amplitude = 1.0f;
        f32 frequency = param.frequency_;

        total = param.offset_ + noise(x*frequency, y*frequency);

        for(s32 i=1; i<param.octaves_; ++i){
            frequency *= param.lacunarity_;
            amplitude *= param.persistence_;

            increment = (param.offset_ + noise(x*frequency, y*frequency)) * amplitude;
            increment = lcore::minimum(1.0f, increment) * total;
            total += increment;
        }

        return total;
    }

    f32 multiFractal3(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y, f32 z)
    {
        f32 total;
        f32 increment;
        f32 amplitude = 1.0f;
        f32 frequency = param.frequency_;

        total = param.offset_ + noise(x*frequency, y*frequency, z*frequency);

        for(s32 i=1; i<param.octaves_; ++i){
            frequency *= param.lacunarity_;
            amplitude *= param.persistence_;

            increment = (param.offset_ + noise(x*frequency, y*frequency, z*frequency)) * amplitude;
            increment = lcore::minimum(1.0f, increment) * total;
            total += increment;
        }

        return total;
    }

    f32 hybridMultiFractal2(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y)
    {
        f32 total;
        f32 weight;
        f32 signal;
        f32 amplitude = 1.0f;
        f32 frequency = param.frequency_;

        total = param.offset_ + noise(x*frequency, y*frequency);
        weight = total;

        for(s32 i=1; i<param.octaves_; ++i){
            frequency *= param.lacunarity_;
            amplitude *= param.persistence_;

            if(1.0f<weight){
                weight = 1.0f;
            }

            signal = (param.offset_ + noise(x*frequency, y*frequency)) * amplitude;
            weight *= signal;

            total += weight;
        }

        return total;
    }

    f32 hybridMultiFractal3(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y, f32 z)
    {
        f32 total;
        f32 weight;
        f32 signal;
        f32 amplitude = 1.0f;
        f32 frequency = param.frequency_;

        total = param.offset_ + noise(x*frequency, y*frequency, z*frequency);
        weight = total;

        for(s32 i=1; i<param.octaves_; ++i){
            frequency *= param.lacunarity_;
            amplitude *= param.persistence_;

            if(1.0f<weight){
                weight = 1.0f;
            }

            signal = (param.offset_ + noise(x*frequency, y*frequency, z*frequency)) * amplitude;
            weight *= signal;

            total += weight;
        }

        return total;
    }

    f32 ridgedMultiFractal2(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y)
    {
        f32 total;
        f32 weight;
        f32 signal;
        f32 amplitude = 1.0f;
        f32 frequency = param.frequency_;

        signal = param.offset_ - lcore::absolute(2.0f*noise(x*frequency, y*frequency) - 1.0f);
        signal *= signal;
        total = signal;

        for(s32 i=1; i<param.octaves_; ++i){
            frequency *= param.lacunarity_;
            amplitude *= param.persistence_;
            weight = signal * 2.0f;

            if(1.0f<weight){
                weight = 1.0f;
            }

            signal = param.offset_ - lcore::absolute(2.0f*noise(x*frequency, y*frequency) - 1.0f);
            signal *= signal;
            signal *= weight;

            total += signal * amplitude;
        }

        return total;
    }

    f32 ridgedMultiFractal3(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y, f32 z)
    {
        f32 total;
        f32 weight;
        f32 signal;
        f32 amplitude = 1.0f;
        f32 frequency = param.frequency_;

        signal = param.offset_ - lcore::absolute(2.0f*noise(x*frequency, y*frequency, z*frequency) - 1.0f);
        signal *= signal;
        total = signal;

        for(s32 i=1; i<param.octaves_; ++i){
            frequency *= param.lacunarity_;
            amplitude *= param.persistence_;
            weight = signal * 2.0f;

            if(1.0f<weight){
                weight = 1.0f;
            }

            signal = param.offset_ - lcore::absolute(2.0f*noise(x*frequency, y*frequency, z*frequency) - 1.0f);
            signal *= signal;
            signal *= weight;

            total += signal * amplitude;
        }

        return total;
    }

    FractalFuc2::FractalFuc2(const FractalParam* param, const NoiseBase* noise, FuncType2D func2)
        :param_(param)
        ,noise_(noise)
        ,func2_(func2)
    {
        LASSERT(NULL != param_);
        LASSERT(NULL != noise_);
        LASSERT(NULL != func2_);
    }

    FractalFuc3::FractalFuc3(const FractalParam* param, const NoiseBase* noise, FuncType3D func3)
        :param_(param)
        ,noise_(noise)
        ,func3_(func3)
    {
        LASSERT(NULL != param_);
        LASSERT(NULL != noise_);
        LASSERT(NULL != func3_);
    }
}
