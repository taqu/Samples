#ifndef INC_RANDOM_H__
#define INC_RANDOM_H__
/**
@file Random.h
@author t-sakai
@date 2011/08/20 create
*/
#include "../lcore.h"

namespace lcore
{

    u32 getStaticSeed();
    u32 getDefaultSeed();

    //---------------------------------------------
    //---
    //--- RandomXorshift
    //---
    //---------------------------------------------
    class RandomXorshift
    {
    public:
        RandomXorshift();
        ~RandomXorshift();

        /**
        @brief 擬似乱数生成器初期化
        @param seed
        */
        void srand(u32 seed);

        /**
        @brief 0 - 0xFFFFFFFFUの乱数生成
        */
        u32 rand();

        /**
        @brief 0 - 1の乱数生成
        */
        f32 frand();

        /**
        @brief 0.0 - 0.999999881の乱数生成
        */
        f32 frand2();

        /**
        @brief 0 - 1の乱数生成
        */
        f64 drand();

    private:
        u32 rand(u32 v, u32 i);

        u32 x_;
        u32 y_;
        u32 z_;
        u32 w_;
    };

    //---------------------------------------------
    //---
    //--- RandomWELL
    //---
    //---------------------------------------------
    class RandomWELL
    {
    public:
        RandomWELL();
        ~RandomWELL();

        /**
        @brief 擬似乱数生成器初期化
        @param seed
        */
        void srand(u32 seed);

        /**
        @brief 0 - 0xFFFFFFFFUの乱数生成
        */
        u32 rand();

        /**
        @brief 0 - 1の乱数生成
        */
        f32 frand();

        /**
        @brief 0.0 - 0.999999881の乱数生成
        */
        f32 frand2();

        /**
        @brief 0 - 1の乱数生成
        */
        f64 drand();

    private:
        static const u32 N = 16;

        u32 state_[N];
        u32 index_;
    };

namespace random
{
    /**
    @brief [vmin, vmax]
    */
    template<class T, class U>
    U range(T& random, U vmin, U vmax)
    {
        LASSERT(vmin<=vmax);

        //return static_cast<U>(random.frand2()*(vmax-vmin)) + vmin;

        u32 i = random.rand() % (vmax - vmin + 1);
        return vmin + static_cast<U>(i);
    }

    template<class T>
    f32 range(T& random, f32 vmin, f32 vmax)
    {
        LASSERT(vmin<=vmax);
        return (vmax-vmin)*random.frand() + vmin;
    }
}
}

#endif //INC_RANDOM_H__
