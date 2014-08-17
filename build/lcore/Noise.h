#ifndef INC_LCORE_NOISE_H__
#define INC_LCORE_NOISE_H__
/**
@file Noise.h
@author t-sakai
@date 2014/05/08 create
*/
#include "lcore.h"

namespace lcore
{
    //------------------------------------------------
    //---
    //--- Perlin
    //---
    //------------------------------------------------
    class Perlin
    {
    public:
        static const s32 PermutationTableSize = 256*2;
        static const s32 NumGrad3 = 12;
        static const s32 NumGrad4 = 32;

        Perlin();
        ~Perlin();

        void random(u32 seed);

        f32 operator()(f32 x, f32 y) const;
        f32 operator()(f32 x, f32 y, f32 z) const;
        f32 operator()(f32 x, f32 y, f32 z, f32 w) const;

        static f32 getMin(){ return -1.0f;}
        static f32 getMax(){ return 1.0f;}

    private:
        static s8 grad3_[NumGrad3][3];
        static s8 grad4_[NumGrad4][4];
        static u8 simplex_[64][4];

        inline static s32 fastFloor(f32 x);
        inline static f32 dot2(const s8 grad[2], f32 x, f32 y);
        inline static f32 dot3(const s8 grad[3], f32 x, f32 y, f32 z);
        inline static f32 dot4(const s8 grad[4], f32 x, f32 y, f32 z, f32 w);
        inline static f32 fade(f32 x);

        u8 permutation_[PermutationTableSize];
    };

    //------------------------------------------------
    //---
    //--- Fractal
    //---
    //------------------------------------------------
    class Fractal
    {
    public:
        struct Param
        {
            Param()
                :octaves_(5)
                ,frequency_(0.2f)
                ,persistence_(0.51f)
                ,lacunarity_(2.0f)
                ,offset_(0.0f)
            {}

            Param(
                s32 octaves,
                f32 frequency,
                f32 persistence,
                f32 lacunarity,
                f32 offset)
                :octaves_(octaves)
                ,frequency_(frequency)
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

        struct RidgedParam
        {
            static const s32 MaxOctaves = 16;

            RidgedParam()
                :octaves_(5)
                ,frequency_(0.2f)
                ,gain_(2.0f)
                ,lacunarity_(2.0f)
                ,offset_(1.0f)
            {
                calcSpectralWeights();
            }

            RidgedParam(
                s32 octaves,
                f32 frequency,
                f32 gain,
                f32 lacunarity,
                f32 offset)
                :octaves_(octaves)
                ,frequency_(frequency)
                ,gain_(gain)
                ,lacunarity_(lacunarity)
                ,offset_(offset)
            {
                calcSpectralWeights();
            }

            void calcSpectralWeights();

            s32 octaves_;
            f32 frequency_;
            f32 gain_;
            f32 lacunarity_;
            f32 offset_;

            f32 spectralWeights_[MaxOctaves];
        };

        static f32 fbm(const Perlin& noise, const Param& param, f32 x, f32 y);
        static f32 fbm(const Perlin& noise, const Param& param, f32 x, f32 y, f32 z);
        static f32 fbmConst(f32 noiseValue, const Param& param);

        static f32 turbulence(const Perlin& noise, const Param& param, f32 x, f32 y);
        static f32 turbulence(const Perlin& noise, const Param& param, f32 x, f32 y, f32 z);
        static f32 turbulenceConst(f32 noiseValue, const Param& param);

        static f32 multiFractal(const Perlin& noise, const Param& param, f32 x, f32 y);
        static f32 multiFractal(const Perlin& noise, const Param& param, f32 x, f32 y, f32 z);
        static f32 multiFractalConst(f32 noiseValue, const Param& param);

        static f32 ridgedMultiFractal(const Perlin& noise, const RidgedParam& param, f32 x, f32 y);
        static f32 ridgedMultiFractal(const Perlin& noise, const RidgedParam& param, f32 x, f32 y, f32 z);
        static f32 ridgedMultiFractalConst(f32 noiseValue, const RidgedParam& param);

        static inline f32 normalize(f32 val, f32 minVal, f32 invExtent);

        static f32 linearWrapAround(f32 x);

    };

    inline f32 Fractal::normalize(f32 val, f32 minVal, f32 invExtent)
    {
        return (val-minVal)*invExtent;
    }
}

#endif //INC_LCORE_NOISE_H__
