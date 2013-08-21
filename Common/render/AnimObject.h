#ifndef INC_RENDER_ANIMOBJECT_H__
#define INC_RENDER_ANIMOBJECT_H__
/**
@file AnimObject.h
@author t-sakai
@date 2013/07/31 create
*/
#include "Object.h"

namespace lmath
{
    class Matrix34;
}

namespace render
{
    class AnimObject : public Object
    {
    public:
        AnimObject();
        virtual ~AnimObject();

        /**
        @brief バッファ作成
        @return 成否
        @param numMeshes ... メッシュ数
        @param numMaterials ... マテリアル数
        */
        bool create(
            u32 numGeometries,
            u32 numMeshes,
            u32 numMaterials,
            u32 numNodes,
            u32 numTextures,
            u32 numMatrices);

        bool clone(AnimObject& dst);

        const lmath::Matrix34* getSkinningMatrices() const{ return skinningMatrices_;}
        lmath::Matrix34* getSkinningMatrices(){ return skinningMatrices_;}

        /// スワップ
        void swap(AnimObject& rhs);
    private:
        virtual void release();

        u32 calcBufferSize() const;

        void cloneElements(AnimObject& dst);

        u32 numMatrices_;
        lmath::Matrix34* skinningMatrices_;
    };
}
#endif //INC_RENDER_ANIMOBJECT_H__
