#ifndef INC_LGRAPHICS_IOVERTEXBUFFER_H__
#define INC_LGRAPHICS_IOVERTEXBUFFER_H__
/**
@file IOVertexBuffer.h
@author t-sakai
@date 2010/05/06 create
*/
#include <lcore/liostream.h>

namespace lgraphics
{
    class VertexBufferRef;

namespace io
{
    class IOVertexBuffer
    {
    public:
        IOVertexBuffer(){}
        ~IOVertexBuffer(){}

        static bool read(lcore::istream& is, VertexBufferRef& buffer);
        static bool write(lcore::ostream& os, VertexBufferRef& buffer);
    };
}
}
#endif //INC_LGRAPHICS_IOVERTEXBUFFER_H__
