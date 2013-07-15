#ifndef INC_LANIM_ANIMATIONSYSTEM_H__
#define INC_LANIM_ANIMATIONSYSTEM_H__
/**
@file AnimationSystem.h
@author t-sakai
@date 2010/11/18 create

*/
#include "lanim.h"
#include <lcore/Vector.h>
#include "Skeleton.h"
#include "AnimationClip.h"
#include <lcore/HashMap.h>

namespace lanim
{
    class AnimationControler;

    /// 初期化パラメータ
    struct InitParam
    {
        u32 ctrlVectorIncSize_; /// アニメーション配列の拡張単位サイズ
        u32 maxNumSkeleton_;
        u32 maxNumClip_;
    };

    /**
    @brief アニメーション更新システム
    */
    class AnimationSystem
    {
    public:
        AnimationSystem();
        ~AnimationSystem();

        /// 初期化
        void initialize(const InitParam& param);

        ///
        void terminate();

        /// 更新
        void update();

        /**
        @brief アニメーション登録
        @param controler ... 

        性善説でポインタ渡す
        */
        void add(AnimationControler* controler);

        /**
        @brief アニメーション登録削除
        @param controler ... 

        アニメーションの破棄はしない
        */
        void remove(AnimationControler* controler);

        /**
        @brief スケルトンをDBに登録
        @return 同名のスケルトンがすでに登録されている場合失敗する
        */
        inline bool addSkeleton(Skeleton::pointer skeleton);

        /**
        @brief スケルトンをDBから削除
        @param name
        */
        inline void removeSkeleton(const Name& name);

        /**
        @brief スケルトンを検索
        @param name
        */
        inline Skeleton::pointer* findSkeleton(const Name& name);

        /**
        @brief クリップをDBに登録
        @return 同名のクリップがすでに登録されている場合失敗する
        */
        inline bool addClip(AnimationClip::pointer clip);

        /**
        @brief クリップをDBから削除
        @param name
        */
        inline void removeClip(const Name& name);

        /**
        @brief クリップを検索
        @param name
        */
        inline AnimationClip::pointer* findClip(const Name& name);
    private:
        template<class T>
        class HashMapDB
        {
        public:
            typedef lcore::HashMapCharArray<T> hash_map;
            typedef typename hash_map::size_type size_type;

            inline HashMapDB();
            inline ~HashMapDB();

            void resize(u32 size);

            inline bool add(T& value);

            inline void remove(const Name& name);
            void remove(const char* name);
            void remove(const char* name, u32 length);

            inline T* find(const Name& name);
            inline T* find(const char* name);
            inline T* find(const char* name, u32 length);
        private:
            hash_map map_;
        };

        typedef lcore::vector_arena<AnimationControler*> AnimCtrlVector;
        typedef HashMapDB<Skeleton::pointer> SkeletonDB;
        typedef HashMapDB<AnimationClip::pointer> ClipDB;

        AnimCtrlVector controlers_; /// コントローラ配列
        SkeletonDB skeletonDB_;
        ClipDB clipDB_;
    };

    template<class T>
    inline AnimationSystem::HashMapDB<T>::HashMapDB()
    {
    }

    template<class T>
    inline AnimationSystem::HashMapDB<T>::~HashMapDB()
    {
    }

    template<class T>
    void AnimationSystem::HashMapDB<T>::resize(u32 size)
    {
        hash_map map(size);
        map_.swap(map);
    }

    template<class T>
    inline bool AnimationSystem::HashMapDB<T>::add(T& value)
    {
        const Name& name = value->getName();
        return map_.insert(name.c_str(), name.size(), value);
    }

    template<class T>
    inline void AnimationSystem::HashMapDB<T>::remove(const Name& name)
    {
        remove(name.c_str(), name.size());
    }

    template<class T>
    void AnimationSystem::HashMapDB<T>::remove(const char* name)
    {
        size_type pos = map_.find(name);
        if(map_.isEnd(pos)){
            return;
        }
        map_.erase(pos);
    }

    template<class T>
    void AnimationSystem::HashMapDB<T>::remove(const char* name, u32 length)
    {
        size_type pos = map_.find(name, length);
        if(map_.isEnd(pos)){
            return;
        }
        map_.erase(pos);
    }

    template<class T>
    inline T* AnimationSystem::HashMapDB<T>::find(const Name& name)
    {
        return find(name.c_str(), name.size());
    }

    template<class T>
    inline T* AnimationSystem::HashMapDB<T>::find(const char* name)
    {
        size_type pos = map_.find(name);
        return (map_.isEnd(pos))? NULL : &(map_.getValue(pos));
    }

    template<class T>
    inline T* AnimationSystem::HashMapDB<T>::find(const char* name, u32 length)
    {
        size_type pos = map_.find(name, length);
        return (map_.isEnd(pos))? NULL : &(map_.getValue(pos));
    }



    // スケルトンをDBに登録
    inline bool AnimationSystem::addSkeleton(Skeleton::pointer skeleton)
    {
        return skeletonDB_.add(skeleton);
    }

    // スケルトンをDBから削除
    inline void AnimationSystem::removeSkeleton(const Name& name)
    {
        skeletonDB_.remove(name);
    }

    // スケルトンを検索
    inline Skeleton::pointer* AnimationSystem::findSkeleton(const Name& name)
    {
        return skeletonDB_.find(name);
    }

    // クリップをDBに登録
    inline bool AnimationSystem::addClip(AnimationClip::pointer clip)
    {
        return clipDB_.add(clip);
    }

    // クリップをDBから削除
    inline void AnimationSystem::removeClip(const Name& name)
    {
        clipDB_.remove(name);
    }

    // クリップを検索
    inline AnimationClip::pointer* AnimationSystem::findClip(const Name& name)
    {
        return clipDB_.find(name);
    }
}
#endif // INC_LANIM_ANIMATIONSYSTEM_H__
