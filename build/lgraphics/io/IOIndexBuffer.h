#ifndef INC_LGRAPHICS_IOINDEXBUFFER_H__
#define INC_LGRAPHICS_IOINDEXBUFFER_H__
/**
@file IOIndexBuffer.h
@author t-sakai
@date 2010/05/06 create
*/
#include <lcore/liostream.h>

namespace lgraphics
{
    class IndexBufferRef;

namespace io
{
    class IOIndexBuffer
    {
    public:
        IOIndexBuffer(){}
        ~IOIndexBuffer(){}

        static bool read(lcore::istream& is, IndexBufferRef& buffer);
        static bool write(lcore::ostream& os, IndexBufferRef& buffer);
    };
}
}
#endif //INC_LGRAPHICS_IOINDEXBUFFER_H__
