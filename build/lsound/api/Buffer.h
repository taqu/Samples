#ifndef INC_LSOUND_BUFFER_H__
#define INC_LSOUND_BUFFER_H__
/**
@file Buffer.h
@author t-sakai
@date 2012/08/03 create

*/
#include <lcore/smart_ptr/intrusive_ptr.h>

#include "../Type.h"
#include <xaudio2.h>

namespace lsound
{
    //----------------------------------------------------
    //---
    //--- Buffer
    //---
    //----------------------------------------------------
    class Buffer
    {
    public:
        typedef smart_ptr::intrusive_ptr<Buffer> pointer;

        Buffer();
        ~Buffer();

        u32 getSize() const{ return desc_.AudioBytes;}
        const u8* getData() { return desc_.pAudioData;}
        const u8* getData() const{ return desc_.pAudioData;}

        /**
        @brief データセット
        @param size ... サイズ
        @param data ... データ
        */
        void setData(u32 size, const u8* data);

        const XAUDIO2_BUFFER& getDesc() const{ return desc_;}
    private:
        Buffer(const Buffer&);
        Buffer& operator=(const Buffer&);

        friend inline void intrusive_ptr_addref(Buffer* ptr);
        friend inline void intrusive_ptr_release(Buffer* ptr);

        void addRef()
        {
            ++refCount_;
        }
        void release()
        {
            if(--refCount_ == 0){
                LIME_DELETE_NONULL this;
            }
        }

        s32 refCount_;
        XAUDIO2_BUFFER desc_;
    };

    inline void intrusive_ptr_addref(Buffer* ptr)
    {
        ptr->addRef();
    }

    inline void intrusive_ptr_release(Buffer* ptr)
    {
        ptr->release();
    }
}
#endif //INC_BUFFER_H__
