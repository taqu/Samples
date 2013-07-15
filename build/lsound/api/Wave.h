#ifndef INC_LSOUND_WAVE_H__
#define INC_LSOUND_WAVE_H__
/**
@file Wave.h
@author t-sakai
@date 2012/08/02 create

*/
#include "../Type.h"

namespace lsound
{
    class StreamBase
    {
    public:
        virtual bool open(const Char* filepath) =0;
        virtual bool openFromMemory(const u8* data, u32 size) =0;

        virtual void close() =0;
        virtual bool reset() =0;
        virtual bool read(u8* buffer, u32 size, u32& readSize) =0;

        u32 size() const{ return size_;}

        virtual void getWaveFormat(WaveFormat& format) =0;
    protected:
        StreamBase()
            :size_(0)
        {}

        u32 size_;
    };

    class WaveStream
    {
    public:
        WaveStream()
            :impl_(NULL)
        {
        }

        ~WaveStream()
        {
            LIME_DELETE(impl_);
        }

        static bool open(const Char* filepath, WaveStream& stream);
        static bool openFromMemory(const u8* data, u32 size, WaveStream& stream);

        bool open(const Char* filepath)
        {
            return impl_->open(filepath);
        }

        bool openFromMemory(const u8* data, u32 size)
        {
            return impl_->openFromMemory(data, size);
        }

        bool valid() const{ return (NULL != impl_);}

        void close()
        {
            return impl_->close();
        }

        bool reset()
        {
            return impl_->reset();
        }

        bool read(u8* buffer, u32 size, u32& readSize)
        {
            return impl_->read(buffer, size, readSize);
        }

        u32 size() const
        {
            return impl_->size();
        }

        void getWaveFormat(WaveFormat& format)
        {
            impl_->getWaveFormat(format);
        }

        void swap(WaveStream& rhs)
        {
            lcore::swap(impl_, rhs.impl_);
        }

    private:
        WaveStream(StreamBase* impl)
            :impl_(impl)
        {}

        WaveStream(const WaveStream&);
        WaveStream& operator=(const WaveStream&);

        StreamBase* impl_;
    };

}
#endif //INC_LSOUND_WAVE_H__
