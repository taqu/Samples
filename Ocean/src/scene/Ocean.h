#ifndef INC_SCENE_OCEAN_H__
#define INC_SCENE_OCEAN_H__
/**
@file Ocean.h
@author t-sakai
@date 2013/07/04 create
*/
#include "scene.h"
#include <lmath/lmath.h>
#include "FFT.h"

namespace lcore
{
    class RandomXorshift;
}

namespace scene
{
    class Ocean
    {
    public:
        static const s32 Resolution = 64;
        static const s32 MapSize = (Resolution+1)*(Resolution+1);
        typedef Complex V;

        struct Displacement
        {
            u16 x_;
            u16 y_;
            u16 z_;
            u16 w_;
        };

        struct Parameter
        {
            Parameter()
                :size_(1000.0f)
                ,timeScale_(1.0f)
                ,amplitude_(150.0f)
                ,windDirectionX_(1.0f)
                ,windDirectionY_(0.0f)
                ,windSpeed_(100.0f)
                ,windDependency_(0.8f)
                ,choppyScale_(0.4f)
                ,noiseSize_(1.0f)
                ,noiseSpeed_(0.06f)
            {}

            //s32 resolution_;
            f32 size_;

            f32 timeScale_;
            f32 amplitude_;

            f32 windDirectionX_;
            f32 windDirectionY_;

            f32 windSpeed_;
            f32 windDependency_;
            f32 choppyScale_;

            f32 noiseSize_;
            f32 noiseSpeed_;

            f32 gridLength_;
        };

        Ocean();
        ~Ocean();

        void initialize(const Parameter& param);
        void release();

        void update(f32 time);

        const Displacement* getDisplacement() const{ return displacement_;}

        const lmath::Vector4& getDiffuse() const{ return diffuse_;}
        const lmath::Vector4& getSpecular() const{ return specular_;}
        const lmath::Vector4& getTransparent() const{ return transparent_;}

        const Parameter& getParameter() const{ return param_;}

        struct Bump
        {
            u16 x_;
            u16 y_;
        };

        static void generateBumpNoise2D(Bump* noise, s32 size);
    private:
        Ocean(const Ocean&);
        Ocean& operator=(const Ocean&);

        void update(s32 x, s32 y, f32 time);

        s32 calcHeightMapSize() const;
        s32 calcHeightMapIndex(s32 x, s32 y) const;
        f32 calcGaussian(lcore::RandomXorshift& random) const;

        Parameter param_;
        lmath::Vector4 diffuse_;
        lmath::Vector4 specular_;
        lmath::Vector4 transparent_;

        FFT<Resolution> fft_;

#if 0
        V* heightMap_;
        f32* angularFrequencies_;

        V* differential_;
#else
        V heightMap_[MapSize];
        f32 angularFrequencies_[MapSize];

        V differentialX_[Resolution*Resolution];
        V differentialY_[Resolution*Resolution];
        V differentialZ_[Resolution*Resolution];

        Displacement displacement_[Resolution*Resolution];
#endif
    };
}
#endif //INC_SCENE_OCEAN_H__
