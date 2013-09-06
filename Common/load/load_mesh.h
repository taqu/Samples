#ifndef INC_LOAD_MESH_H__
#define INC_LOAD_MESH_H__
/**
@file load_mesh.h
@author t-sakai
@date 2012/03/12 create
*/
#include "load.h"
#include <lmath/geometry/Sphere.h>

namespace load
{
    class Mesh
    {
    public:
        //Char name_[MaxNameSize];
        s16 geometry_; /// ジオメトリインデックス
        s16 material_; /// マテリアルインデックス
        u32 indexOffset_;
        u32 numIndices_; /// インデックス数
        lmath::Sphere sphere_; /// 境界球
    };
}
#endif //INC_LOAD_MESH_H__
