#ifndef INC_LGRAPHICS_IO11_MEMORYSTREAM_H__
#define INC_LGRAPHICS_IO11_MEMORYSTREAM_H__
/**
@file MemoryStream.h
@author t-sakai
@date 2012/07/26 create
*/
#include "../lgraphicscore.h"
namespace lgraphics
{
namespace io
{
    class MemoryStream
    {
    public:
        MemoryStream(const s8* data, u32 size)
            :data_(data)
            ,size_(size)
            ,current_(0)
        {}

        const s8* getTop() const{ return data_ + current_;}
        bool eof() const{ return (current_>=size_);}

        template<class T>
        void read(T& t)
        {
            read(reinterpret_cast<void*>(&t), sizeof(T));
        }

        void read(void* dst, u32 size);
    private:
        u32 getReadable(u32 size) const
        {
            u32 remain = (size_ - current_);
            return (size>remain)? remain : size;
        }

        const s8* data_;
        u32 size_;
        u32 current_;
    };
}
}
#endif //INC_LGRAPHICS_IO11_MEMORYSTREAM_H__
