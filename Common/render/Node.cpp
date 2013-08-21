/**
@file Node.cpp
@author t-sakai
@date 2012/02/28 create
*/
#include "Node.h"
#include "load/load.h"

namespace render
{
    //--------------------------------------------------
    NodeBase::NodeBase()
        :rotationOrder_(0)
        ,numSkinningMatrices_(0)
        ,meshStartIndex_(0)
        ,numMeshes_(0)
        ,meshes_(NULL)
        ,skinningMatrices_(NULL)
    {
    }

    //--------------------------------------------------
    // スワップ
    void NodeBase::swap(NodeBase& rhs)
    {
        lcore::swap(world_, rhs.world_);
        lcore::swap(rotationOrder_, rhs.rotationOrder_);
        lcore::swap(numSkinningMatrices_, rhs.numSkinningMatrices_);
        lcore::swap(meshStartIndex_, rhs.meshStartIndex_);
        lcore::swap(numMeshes_, rhs.numMeshes_);
        lcore::swap(meshes_, rhs.meshes_);
        lcore::swap(skinningMatrices_, rhs.skinningMatrices_);
    }


    //--------------------------------------------------
    Node::Node()
        :index_(0)
        ,parentIndex_(load::InvalidNode)
        ,childStartIndex_(0)
        ,numChildren_(0)
    {
    }

    Node::~Node()
    {
    }

    //--------------------------------------------------
    void Node::clearTransform()
    {
        translation_.zero();
        rotation_.zero();
        scale_.one();
    }

    //--------------------------------------------------
    // スワップ
    void Node::swap(Node& rhs)
    {
        NodeBase::swap(rhs);

        lcore::swap(index_, rhs.index_);
        lcore::swap(parentIndex_, rhs.parentIndex_);
        lcore::swap(childStartIndex_, rhs.childStartIndex_);
        lcore::swap(numChildren_, rhs.numChildren_);

        translation_.swap(rhs.translation_);
        rotation_.swap(rhs.rotation_);
        scale_.swap(rhs.scale_);
    }

    //--------------------------------------------------
    void Node::calcLocalMatrix(lmath::Matrix44& local)
    {
        local.identity();
        local.setScale(scale_.x_, scale_.y_, scale_.z_);
        f32 x = -rotation_.x_;
        f32 y = -rotation_.y_;
        f32 z = -rotation_.z_;

        switch(rotationOrder_)
        {
        case load::RotationOrder_EulerXYZ:
            local.rotateX(x);
            local.rotateY(y);
            local.rotateZ(z);
            break;
        case load::RotationOrder_EulerXZY:
            local.rotateX(x);
            local.rotateZ(z);
            local.rotateY(y);
            break;
        case load::RotationOrder_EulerYZX:
            local.rotateY(y);
            local.rotateZ(z);
            local.rotateX(x);
            break;
        case load::RotationOrder_EulerYXZ:
            local.rotateY(y);
            local.rotateX(x);
            local.rotateZ(z);
            break;
        case load::RotationOrder_EulerZXY:
            local.rotateZ(z);
            local.rotateX(x);
            local.rotateY(y);
            break;
        case load::RotationOrder_EulerZYX:
            local.rotateZ(z);
            local.rotateY(y);
            local.rotateX(x);
            break;
        case load::RotationOrder_SphericXYZ:
            LASSERT(false);
            break;
        };

        local.translate(translation_.x_, translation_.y_, translation_.z_);
    }
}
