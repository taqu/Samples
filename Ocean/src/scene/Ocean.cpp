/**
@file Ocean.cpp
@author t-sakai
@date 2013/07/04 create
*/
#include "Ocean.h"

#include <lcore/random/Random.h>
#include <lmath/lmathutil.h>

namespace scene
{
namespace
{
    static const f32 HalfSqrt2 = 0.7071068f;
    static const f32 Gravity = 981.0f; //cm/s^2
    static const f32 InvGravity = 1.0f/Gravity;

    /**
    @param vx ... ”g‚Ì•ûŒü
    @param vy ... ”g‚Ì•ûŒü
    @param wx ... •—‚Ì•ûŒü
    @param wy ... •—‚Ì•ûŒü
    @param windVelocity
    @param amplitude
    @param windDependency
    */
    f32 PhillipsSpectrum(f32 vx, f32 vy, f32 wx, f32 wy, f32 windVelocity, f32 amplitude, f32 windDependency)
    {
        f32 ksqr = vx*vx + vy*vy;
        if(lmath::isZeroPositive(ksqr)){
            return 0.0f;
        }

        f32 l = windVelocity*windVelocity*InvGravity;

        f32 w = l*0.001f;

        f32 kcos = vx*wx + vy*wy;
        f32 phillips = amplitude * lmath::exp(-1.0f/(l*l*ksqr))/(ksqr*ksqr*ksqr)*kcos*kcos;
        if(kcos<0.0f){
            phillips *= windDependency;
        }

        return phillips * lmath::exp(-ksqr*w*w);
    }

    void normalize(f32& x, f32& y)
    {
        f32 il = 1.0f/lmath::sqrt(x*x + y*y);
        x *= il;
        y *= il;
    }
}

    Ocean::Ocean()
#if 0
        :heightMap_(NULL)
        ,angularFrequencies_(NULL)
        ,displacement_(NULL)
#endif
    {
        diffuse_.set(0.0f, 0.01f, 0.05f, 1.0f);
        specular_.set(1.0f, 1.0f, 1.0f, 100.0f);
        transparent_.set(1.0f, 1.0f, 1.0f, 1.33f);
    }

    Ocean::~Ocean()
    {
        release();
    }

    void Ocean::initialize(const Parameter& param)
    {
        release();

        param_ = param;
        param_.gridLength_ = Resolution/param_.size_;
        normalize(param_.windDirectionX_, param_.windDirectionY_);

        fft_.initialize();
#if 0
        s32 size = calcHeightMapSize();
        heightMap_ = LIME_NEW V[size];
        angularFrequencies_ = LIME_NEW f32[size];
        differential_ = LIME_NEW V[param_.resolution_*param_.resolution_*3];
#endif
        lcore::memset(differentialX_, 0, sizeof(V)*Resolution*Resolution);
        lcore::memset(differentialY_, 0, sizeof(V)*Resolution*Resolution);
        lcore::memset(differentialZ_, 0, sizeof(V)*Resolution*Resolution);
        lcore::memset(displacement_, 0, sizeof(Displacement)*Resolution*Resolution);

        f32 wx = param_.windDirectionX_;
        f32 wy = param_.windDirectionY_;
        normalize(wx, wy);
        f32 amplitude = param_.amplitude_ * 1e-7f;
        f32 velocity = param_.windSpeed_;
        f32 windDependency = param_.windDependency_;

        lcore::RandomXorshift random;
        random.srand(lcore::getDefaultSeed());

        f32 kx, ky; //wave vector
        f32 invSize = 1.0f/param_.size_;

        //s32 reso = param_.resolution_;
        s32 reso = Resolution;
        for(s32 i=0; i<=reso; ++i){
            ky = (-0.5f*reso + i)*(PI2*invSize);
            for(s32 j=0; j<=reso; ++j){
                kx = (-0.5f*reso + j)*(PI2*invSize);

                f32 phillips = (lmath::isZero(kx)&&lmath::isZero(ky))
                    ? 0.0f
                    : lmath::sqrt(PhillipsSpectrum(kx, ky, wx, wy, velocity, amplitude, windDependency));

                s32 index = calcHeightMapIndex(j, i);
                heightMap_[index].x_ = phillips * calcGaussian(random) * HalfSqrt2;
                heightMap_[index].y_ = phillips * calcGaussian(random) * HalfSqrt2;

                angularFrequencies_[index] = lmath::sqrt(Gravity * lmath::sqrt(kx*kx + ky*ky));
            }
        }
    }

    void Ocean::release()
    {
#if 0
        LIME_DELETE_ARRAY(displacement_);
        LIME_DELETE_ARRAY(angularFrequencies_);
        LIME_DELETE_ARRAY(heightMap_);
#endif
    }

    void Ocean::update(f32 time)
    {
        time *= param_.timeScale_;

        s32 reso = Resolution;
        for(s32 i=0; i<reso; ++i){
            for(s32 j=0; j<reso; ++j){
                update(j, i, time);
            }
        }

        fft_.inv2D(differentialX_);
        fft_.inv2D(differentialY_);
        fft_.inv2D(differentialZ_);


        for(s32 i=0; i<reso; ++i){
            for(s32 j=0; j<reso; ++j){
                s32 index = i*reso + j;
                f32 sign = ((i+j)&0x01U)? -param_.choppyScale_ : param_.choppyScale_;
                displacement_[index].x_ = lcore::toBinary16Float(differentialX_[index].x_ * sign);
                displacement_[index].y_ = lcore::toBinary16Float(differentialY_[index].x_ * sign);
                displacement_[index].z_ = lcore::toBinary16Float(differentialZ_[index].x_ * sign);
            }
        }
    }

    void Ocean::update(s32 x, s32 y, f32 time)
    {
        s32 inIndex = calcHeightMapIndex(x, y);
        //s32 inMinus = calcHeightMapIndex(param_.resolution_-x, param_.resolution_-y);
        s32 inMinus = calcHeightMapIndex(Resolution-x, Resolution-y);
        //s32 outIndex = y*param_.resolution_ + x;
        s32 outIndex = y*Resolution + x;

        const V& h0_k = heightMap_[inIndex];
        const V& h0_mk = heightMap_[inMinus];

        f32 sn, cs;
        lmath::sincos(sn, cs, time*angularFrequencies_[inIndex]);

        //dz
        V dz;
        dz.x_ = (h0_k.x_ + h0_mk.x_) * cs - (h0_k.y_ + h0_mk.y_) * sn;
        dz.y_ = (h0_k.x_ - h0_mk.x_) * sn + (h0_k.y_ - h0_mk.y_) * cs;

        //dx, dy
        //f32 half = param_.resolution_ >> 1;
        f32 half = Resolution>>1;
        f32 kx = x - half;
        f32 ky = y - half;
        f32 sqrK = kx*kx + ky*ky;
        f32 rsqrK = (lmath::isZeroPositive(sqrK))? 0.0f : 1.0f/lmath::sqrt(sqrK);

        kx *= rsqrK;
        ky *= rsqrK;

        V dx;
        dx.x_ = dz.y_ * kx;
        dx.y_ = -dz.x_ * kx;

        V dy;
        dy.x_ = dz.y_ * ky;
        dy.y_ = -dz.x_ * ky;

        differentialZ_[outIndex] = dz;
        differentialX_[outIndex] = dx;
        differentialY_[outIndex] = dy;
    }

    s32 Ocean::calcHeightMapSize() const
    {
        //return (param_.resolution_ + 4) * (param_.resolution_ + 1);
        return MapSize;
    }

    s32 Ocean::calcHeightMapIndex(s32 x, s32 y) const
    {
        //return y*(param_.resolution_+4) + x;
        return y*(Resolution+1) + x;
    }

    f32 Ocean::calcGaussian(lcore::RandomXorshift& random) const
    {
        f32 x0 = random.frand();
        f32 x1 = random.frand();

        x0 = lcore::maximum(1e-6f, x0);
        return lmath::sqrt(-2.0f*lmath::log(x0)) * lmath::cosf(PI2*x1);
    }

namespace
{
    class Perlin
    {
    public:
        static const s32 NumSamples = 0x80;
        void initialize();

        f32 operator()(f32 x, f32 y) const;
    private:
        u32 srand(u32 v, u32 i) const
        {
            return (1812433253 * (v^(v >> 30)) + i); 
        }

        f32 Perlin::frand(u32 x) const;

        f32 Perlin::frand(u32 x, u32 y) const
        {
            return frand(x + 57*y);
        }

        f32 rand_[NumSamples];
    };

    void Perlin::initialize()
    {
        lcore::RandomWELL well;
        well.srand(lcore::getDefaultSeed());
        for(s32 i=0; i<NumSamples; ++i){
            rand_[i] = well.frand();
        }
    }

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
    }

    f32 Perlin::frand(u32 x) const
    {
        u32 y = srand(x, 1);
        u32 z = srand(y, 2);
        u32 w = srand(z, 3);

        u32 t = x^(x<<11);
        w = (w^(w>>19)) ^ (t^(t>>8));
        return rand_[w%NumSamples];
    }
}

    void Ocean::generateBumpNoise2D(Bump* noise, s32 size)
    {
        Perlin perlinX;
        perlinX.initialize();

        Perlin perlinY;
        perlinY.initialize();

        for(s32 i=0; i<size; ++i){
            for(s32 j=0; j<size; ++j){
                f32 x = 2.0f * perlinX(j, i) - 1.0f;
                f32 y = 2.0f * perlinY(j, i) - 1.0f;
                noise[size*i + j].x_ = lcore::toBinary16Float(x);
                noise[size*i + j].y_ = lcore::toBinary16Float(y);
            }
        }
    }
}
