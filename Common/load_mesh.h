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
        s32 geometry_; /// ジオメトリインデックス
        s32 material_; /// マテリアルインデックス
        u16 indexOffset_;
        u16 numIndices_; /// インデックス数
        lmath::Sphere sphere_; /// 境界球
    };
}
#endif //INC_LOAD_MESH_H__
