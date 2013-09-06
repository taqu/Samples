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
        s16 geometry_; /// �W�I���g���C���f�b�N�X
        s16 material_; /// �}�e���A���C���f�b�N�X
        u32 indexOffset_;
        u32 numIndices_; /// �C���f�b�N�X��
        lmath::Sphere sphere_; /// ���E��
    };
}
#endif //INC_LOAD_MESH_H__
