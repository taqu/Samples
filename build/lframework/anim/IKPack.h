#ifndef INC_IKPACK_H__
#define INC_IKPACK_H__
/**
@file IKPack.h
@author t-sakai
@date 2010/12/30 create
*/
#include <lcore/smart_ptr/intrusive_ptr.h>
#include "lanim.h"

namespace lanim
{
    struct IKEntry
    {
        IKEntry()
            :children_(NULL)
        {}

        ~IKEntry()
        {
            LIME_DELETE_ARRAY(children_);
        }

        u16 targetJoint_;
        u16 effectorJoint_;
        u16 chainLength_;
        u16 numIterations_;
        f32 limitAngle_;
        u8* children_;
    };

    class IKPack
    {
    public:
        typedef smart_ptr::intrusive_ptr<IKPack> pointer;

        IKPack();
        explicit IKPack(u32 numIKs);
        ~IKPack();

        inline u32 getNumIKs() const;
        inline IKEntry& get(u32 index);
        inline const IKEntry& get(u32 index) const;

        void swap(IKPack& rhs);
    private:
        IKPack(const IKPack&);
        IKPack& operator=(const IKPack&);

        friend inline void intrusive_ptr_addref(IKPack* ptr);
        friend inline void intrusive_ptr_release(IKPack* ptr);

        /// 参照カウント
        void addRef()
        {
            ++refCount_;
        }

        /// 参照カウント開放
        void release()
        {
            if(--refCount_ == 0){
                LIME_DELETE_NONULL this;
            }
        }

        s32 refCount_;

        u32 numIKs_;
        IKEntry* iks_;
    };

    inline u32 IKPack::getNumIKs() const
    {
        return numIKs_;
    }

    inline IKEntry& IKPack::get(u32 index)
    {
        LASSERT(0<=index && index<numIKs_);
        return iks_[index];
    }

    inline const IKEntry& IKPack::get(u32 index) const
    {
        LASSERT(0<=index && index<numIKs_);
        return iks_[index];
    }

    inline void intrusive_ptr_addref(IKPack* ptr)
    {
        ptr->addRef();
    }

    inline void intrusive_ptr_release(IKPack* ptr)
    {
        ptr->release();
    }
}
#endif //INC_IKPACK_H__
