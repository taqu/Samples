#ifndef INC_LGRAPHICS_IOVERTEXBUFFERUP_H__
#define INC_LGRAPHICS_IOVERTEXBUFFERUP_H__
/**
@file IOVertexBufferUP.h
@author t-sakai
@date 2010/07/29 create
*/
#include <lcore/liostream.h>

namespace lgraphics
{
    class VertexBufferUPRef;

namespace io
{
    class IOVertexBufferUP
    {
    public:
        IOVertexBufferUP(){}
        ~IOVertexBufferUP(){}

        static bool read(lcore::istream& is, VertexBufferUPRef& buffer);
        static bool write(lcore::ostream& os, VertexBufferUPRef& buffer);
    };
}
}
#endif //INC_LGRAPHICS_IOVERTEXBUFFERUP_H__
