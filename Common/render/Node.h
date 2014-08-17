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
#include <lmath/Quaternion.h>

namespace lmath
{
    class DualQuaternion;
}

namespace render
{
    class Mesh;

    //--------------------------------------------------
    class NodeBase
    {
    public:
        static const u8 Flag_DualQuaternion = (0x01U<<0);

        NodeBase();

        /// スワップ
        void swap(NodeBase& rhs);

        void setDualQuaternion()
        {
            type_ |= Flag_DualQuaternion;
        }

        bool isDualQuaternion() const
        {
            return 0 != (type_ & Flag_DualQuaternion);
        }

        lmath::Matrix44 world_;

        u8 type_;
        u8 rotationOrder_;
        u8 meshStartIndex_;
        u8 numMeshes_;

        u32 numSkinningMatrices_;
        Mesh* meshes_;
        const lmath::Matrix34* skinningMatrices_;
        const lmath::DualQuaternion* dualQuaternions_;
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

        void calcRotation(const lmath::Vector3& angle);
        void calcLocalMatrix(lmath::Matrix44& local);

        u8 index_;
        u8 parentIndex_;
        u8 childStartIndex_;
        u8 numChildren_;

        lmath::Vector4 translation_;
        lmath::Quaternion rotation_;
        lmath::Vector4 scale_;
    };
}
#endif //INC_RENDER_NODE_H__
