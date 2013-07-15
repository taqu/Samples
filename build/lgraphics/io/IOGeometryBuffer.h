#ifndef INC_LGRAPHICS_IOGEOMETRYBUFFER_H__
#define INC_LGRAPHICS_IOGEOMETRYBUFFER_H__
/**
@file IOGeometryBuffer.h
@author t-sakai
@date 2010/05/05 create
*/
#include <lcore/liostream.h>
#include "../api/GeometryBuffer.h"
namespace lgraphics
{
namespace io
{
    class IOGeometryBuffer
    {
    public:
        IOGeometryBuffer(){}
        ~IOGeometryBuffer(){}

        static bool read(lcore::istream& is, GeometryBuffer::pointer& buffer);
        static bool write(lcore::ostream& os, GeometryBuffer::pointer& buffer);
    };
}
}
#endif //INC_LGRAPHICS_IOGEOMETRYBUFFER_H__
