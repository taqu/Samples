#ifndef INC_TERRAIN_NOISE_H__
#define INC_TERRAIN_NOISE_H__
/**
@file Noise.h
@author t-sakai
@date 2013/04/15 create

*/
#include "stdafx.h"
#include <lcore/random/Random.h>


namespace fractal
{
    //--------------------------------------
    //---
    //---
    //---
    //--------------------------------------
    class NoiseBase
    {
    public:
        virtual ~NoiseBase()
        {}

        virtual f32 operator()(f32 x) const =0;
        virtual f32 operator()(f32 x, f32 y) const =0;
        virtual f32 operator()(f32 x, f32 y, f32 z) const =0;
    protected:
        NoiseBase(const NoiseBase&);
        NoiseBase& operator=(const NoiseBase&);

        NoiseBase()
        {}
    };

    class Perlin : public NoiseBase
    {
    public:
        Perlin()
        {}

        ~Perlin()
        {}

        void initialize(u32 seed);

        virtual f32 operator()(f32 x) const;
        virtual f32 operator()(f32 x, f32 y) const;
        virtual f32 operator()(f32 x, f32 y, f32 z) const;

    private:
        u32 srand(u32 v, u32 i) const;
        f32 frand(u32 x) const;
        f32 frand(u32 x, u32 y) const;
        f32 frand(u32 x, u32 y, u32 z) const;

        static const u32 NumSamples = 0x200U;

        void normalize2(f32 v[2]) const;
        //f32 smoothedRand(u32 x) const;

        //f32 smoothedRand(u32 x, u32 y) const;

        //f32 smoothedRand(u32 x, u32 y, u32 z) const;

        f32 rand_[NumSamples];
    };

    struct FractalParam
    {
        FractalParam()
            :octaves_(4)
            ,frequency_(0.1f)
            ,persistence_(0.51f)
            ,lacunarity_(2.0f)
            ,offset_(0.0f)
        {}

        FractalParam(
            s32 octaves,
            f32 frecuency,
            f32 persistence,
            f32 lacunarity,
            f32 offset)
            :octaves_(octaves)
            ,frequency_(frecuency)
            ,persistence_(persistence)
            ,lacunarity_(lacunarity)
            ,offset_(offset)
        {}

        s32 octaves_;
        f32 frequency_;
        f32 persistence_;
        f32 lacunarity_;
        f32 offset_;
    };

    f32 fractal2(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y);
    f32 fractal3(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y, f32 z);

    f32 turbulence2(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y);
    f32 turbulence3(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y, f32 z);

    f32 multiFractal2(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y);
    f32 multiFractal3(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y, f32 z);

    f32 hybridMultiFractal2(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y);
    f32 hybridMultiFractal3(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y, f32 z);

    f32 ridgedMultiFractal2(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y);
    f32 ridgedMultiFractal3(const FractalParam& param, const NoiseBase& noise, f32 x, f32 y, f32 z);


    class FractalFuc2
    {
    public:
        typedef f32(*FuncType2D)(const FractalParam&, const NoiseBase&, f32, f32);

        FractalFuc2(const FractalParam* param, const NoiseBase* noise, FuncType2D func2);

        f32 operator()(f32 x, f32 y) const
        {
            return func2_(*param_, *noise_, x, y);
        }
    private:
        const FractalParam* param_;
        const NoiseBase* noise_;
        FuncType2D func2_;
    };

    class FractalFuc3
    {
    public:
        typedef f32(*FuncType3D)(const FractalParam&, const NoiseBase&, f32, f32, f32);

        FractalFuc3(const FractalParam* param, const NoiseBase* noise, FuncType3D func3);

        f32 operator()(f32 x, f32 y, f32 z) const
        {
            return func3_(*param_, *noise_, x, y, z);
        }
    private:
        const FractalParam* param_;
        const NoiseBase* noise_;
        FuncType3D func3_;
    };
}
#endif //INC_TERRAIN_NOISE_H__
