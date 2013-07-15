/**
@file Node.cpp
@author t-sakai
@date 2012/02/28 create
*/
#include "Node.h"
#include "load.h"

namespace render
{
    //--------------------------------------------------
    NodeBase::NodeBase()
        :rotationOrder_(0)
        ,reserved_(0)
        ,meshStartIndex_(0)
        ,numMeshes_(0)
        ,meshes_(NULL)
    {
    }

    //--------------------------------------------------
    // スワップ
    void NodeBase::swap(NodeBase& rhs)
    {
        lcore::swap(world_, rhs.world_);
        lcore::swap(rotationOrder_, rhs.rotationOrder_);
        lcore::swap(reserved_, rhs.reserved_);
        lcore::swap(meshStartIndex_, rhs.meshStartIndex_);
        lcore::swap(numMeshes_, rhs.numMeshes_);
        lcore::swap(meshes_, rhs.meshes_);
    }


    //--------------------------------------------------
    Node::Node()
        :index_(0)
        ,parentIndex_(0)
        ,childStartIndex_(0)
        ,numChildren_(0)
    {
    }

    Node::~Node()
    {
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

        lcore::swap(local_, rhs.local_);
    }

    //--------------------------------------------------
    void Node::calcLocalMatrix()
    {
        local_.identity();
        local_.setScale(scale_.x_, scale_.y_, scale_.z_);
        f32 x = -rotation_.x_;
        f32 y = -rotation_.y_;
        f32 z = -rotation_.z_;

        switch(rotationOrder_)
        {
        case load::RotationOrder_EulerXYZ:
            local_.rotateX(x);
            local_.rotateY(y);
            local_.rotateZ(z);
            break;
        case load::RotationOrder_EulerXZY:
            local_.rotateX(x);
            local_.rotateZ(z);
            local_.rotateY(y);
            break;
        case load::RotationOrder_EulerYZX:
            local_.rotateY(y);
            local_.rotateZ(z);
            local_.rotateX(x);
            break;
        case load::RotationOrder_EulerYXZ:
            local_.rotateY(y);
            local_.rotateX(x);
            local_.rotateZ(z);
            break;
        case load::RotationOrder_EulerZXY:
            local_.rotateZ(z);
            local_.rotateX(x);
            local_.rotateY(y);
            break;
        case load::RotationOrder_EulerZYX:
            local_.rotateZ(z);
            local_.rotateY(y);
            local_.rotateX(x);
            break;
        case load::RotationOrder_SphericXYZ:
            LASSERT(false);
            break;
        };

        local_.translate(translation_.x_, translation_.y_, translation_.z_);
    }
}
