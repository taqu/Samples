#ifndef INC_LOAD_NODE_H__
#define INC_LOAD_NODE_H__
/**
@file load_node.h
@author t-sakai
@date 2012/03/18 create
*/
#include "load.h"
#include <lmath/Vector3.h>
#include <lmath/Vector4.h>
#include <lmath/Matrix34.h>

namespace load
{
    class Node
    {
    public:
        Node()
            :index_(0)
            ,parentIndex_(0)
            ,childStartIndex_(0)
            ,numChildren_(0)
            ,meshStartIndex_(0)
            ,numMeshes_(0)
        {
            scale_.set(0.0f, 0.0f, 0.0f);
            translation_ = rotation_ = scale_;
        }

        ~Node()
        {}

        u8 index_;
        u8 parentIndex_;
        u8 childStartIndex_;
        u8 numChildren_;

        lmath::Vector3 translation_;
        lmath::Vector3 rotation_;
        lmath::Vector3 scale_;

        u8 rotationOrder_;
        u8 reserved_;
        u8 meshStartIndex_;
        u8 numMeshes_;
    };
}
#endif //INC_LOAD_NODE_H__
