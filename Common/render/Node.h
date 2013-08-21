#ifndef INC_RENDER_NODE_H__
#define INC_RENDER_NODE_H__
/**
@file Node.h
@author t-sakai
@date 2012/02/25 create
*/
#include "render.h"
#include <lmath/Vector4.h>
#include <lmath/Matrix44.h>

namespace render
{
    class Mesh;

    //--------------------------------------------------
    class NodeBase
    {
    public:
        NodeBase();

        /// スワップ
        void swap(NodeBase& rhs);

        lmath::Matrix44 world_;

        u8 rotationOrder_;
        u8 numSkinningMatrices_;
        u8 meshStartIndex_;
        u8 numMeshes_;

        Mesh* meshes_;
        const lmath::Matrix34* skinningMatrices_;
    };

    //--------------------------------------------------
    class Node : public NodeBase
    {
    public:
        Node();
        ~Node();

        void clearTransform();

        /// スワップ
        void swap(Node& rhs);

        void calcLocalMatrix(lmath::Matrix44& local);

        u8 index_;
        u8 parentIndex_;
        u8 childStartIndex_;
        u8 numChildren_;

        lmath::Vector4 translation_;
        lmath::Vector4 rotation_;
        lmath::Vector4 scale_;
    };
}
#endif //INC_RENDER_NODE_H__
