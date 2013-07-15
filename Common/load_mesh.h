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
        s32 geometry_; /// �W�I���g���C���f�b�N�X
        s32 material_; /// �}�e���A���C���f�b�N�X
        u16 indexOffset_;
        u16 numIndices_; /// �C���f�b�N�X��
        lmath::Sphere sphere_; /// ���E��
    };
}
#endif //INC_LOAD_MESH_H__
