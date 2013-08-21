/**
@file Mesh.cpp
@author t-sakai
@date 2013/02/26 create

*/
#include "Mesh.h"

namespace render
{
    u32 Mesh::calcNumPrimitives(lgraphics::Primitive type, u32 numIndices)
    {
        u32 numPrimitives = 0;
        switch(type)
        {
        case lgraphics::Primitive_PointList:
            numPrimitives = numIndices;
            break;

        case lgraphics::Primitive_LineList:
            numPrimitives = numIndices/2;
            break;

        case lgraphics::Primitive_LineStrip:
            numPrimitives = (numIndices<1)? 0 : numIndices - 1;
            break;

        case lgraphics::Primitive_TriangleList:
            numPrimitives = numIndices/3;
            break;

        case lgraphics::Primitive_TriangleStrip:
            numPrimitives = (numIndices<2)? 0 : numIndices - 2;
            break;

        default:
            LASSERT(false);
            break;
        }
        return numPrimitives;
    }
}
