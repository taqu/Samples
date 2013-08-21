/**
@file AnimObject.cpp
@author t-sakai
@date 2013/07/31 create
*/
#include "AnimObject.h"
#include <lmath/lmath.h>

namespace render
{
    //-----------------------------------------------------
    AnimObject::AnimObject()
        :numMatrices_(0)
        ,skinningMatrices_(NULL)
    {
    }

    //-----------------------------------------------------
    AnimObject::~AnimObject()
    {
        release();
    }

    //-----------------------------------------------------
    // バッファ作成
    bool AnimObject::create(
        u32 numGeometries,
        u32 numMeshes,
        u32 numMaterials,
        u32 numNodes,
        u32 numTextures,
        u32 numMatrices)
    {
        release();

        numGeometries_ = numGeometries;
        numMeshes_ = numMeshes;
        numMaterials_ = numMaterials;
        numNodes_ = numNodes;
        numTextures_ = numTextures;
        numMatrices_ = numMatrices;

        u32 size = calcBufferSize();

        u8* buffer = static_cast<u8*>( LIME_MALLOC(size) );
        if(NULL == buffer){
            return false;
        }

        u32 offset = 0;
        geometries_ = placementArrayConstruct<Geometry>(buffer+offset, numGeometries_);
        offset += numGeometries_ * sizeof(Geometry);

        meshes_ = placementArrayConstruct<Mesh>(buffer+offset, numMeshes_);
        offset += numMeshes_ * sizeof(Mesh);

        materials_ = placementArrayConstruct<Material>(buffer+offset, numMaterials_);
        offset += numMaterials_ * sizeof(Material);

        nodes_ = placementArrayConstruct<Node>(buffer+offset, numNodes_);
        offset += numNodes_ * sizeof(Node);

        textures_ = placementArrayConstruct<lgraphics::Texture2DRef>(buffer+offset, numTextures_);
        offset += numTextures_ * sizeof(lgraphics::Texture2DRef);

        skinningMatrices_ = placementArrayConstruct<lmath::Matrix34>(buffer+offset, numMatrices_);

        //マトリックスクリア
        for(u32 i=0; i<numMatrices_; ++i){
            skinningMatrices_[i].identity();
        }
        return true;
    }

    //-----------------------------------------------------
    bool AnimObject::clone(AnimObject& dst)
    {
        if(false == dst.create(numGeometries_, numMeshes_, numMaterials_, numNodes_, numTextures_, numMatrices_)){
            return false;
        }

        Object::cloneElements(dst);
        cloneElements(dst);

        return true;
    }

    //-----------------------------------------------------
    void AnimObject::swap(AnimObject& rhs)
    {
        Object::swap(rhs);
        lcore::swap(numMatrices_, rhs.numMatrices_);
        lcore::swap(skinningMatrices_, rhs.skinningMatrices_);
    }

    //-----------------------------------------------------
    void AnimObject::release()
    {
        for(u32 i=0; i<numMatrices_; ++i){
            skinningMatrices_[i].~Matrix34();
        }
        skinningMatrices_ = NULL;

        Object::release();
    }

    //-----------------------------------------------------
    u32 AnimObject::calcBufferSize() const
    {
        return Object::calcBufferSize() + numMatrices_ * sizeof(lmath::Matrix34);
    }

    //-----------------------------------------------------
    void AnimObject::cloneElements(AnimObject& dst)
    {
        //SkinningMaticesクローン
        dst.numMatrices_ = numMatrices_;
        for(u32 i=0; i<numMatrices_; ++i){
            dst.skinningMatrices_[i] = skinningMatrices_[i];
        }
    }
}
