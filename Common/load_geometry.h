#ifndef INC_LOAD_GEOMETRY_H__
#define INC_LOAD_GEOMETRY_H__
/**
@file load_geometry.h
@author t-sakai
@date 2012/03/31 create
*/
#include "load.h"
#include <vector>
#include <string>
#include <lmath/geometry/Sphere.h>

namespace load
{
    class Geometry
    {
    public:
        inline Geometry();

        u32 vflag_; /// 頂点宣言フラグ
        u32 vsize_; /// 頂点サイズ
        u32 numVertices_; /// 頂点数
        u32 numIndices_; /// インデックス数
    };

    inline Geometry::Geometry()
        :vflag_(0)
        ,vsize_(0)
        ,numVertices_(0)
        ,numIndices_(0)
    {
    }

    class GeometryCVT : public Geometry
    {
    public:
        typedef std::vector<f32> VertexVector;
        typedef std::vector<u16> IndexVector;

        VertexVector vertices_;
        IndexVector indices_;
        //std::string name_;
    };

}
#endif //INC_LOAD_GEOMETRY_H__
