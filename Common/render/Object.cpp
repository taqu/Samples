/**
@file Object.cpp
@author t-sakai
@date 2012/01/03 create
*/
#include "Object.h"
#include "../stdafx.h"

namespace render
{
    //-----------------------------------------------------
    Object::Object()
        :numGeometries_(0)
        ,geometries_(NULL)
        ,numMeshes_(0)
        ,meshes_(NULL)
        ,numMaterials_(0)
        ,materials_(NULL)
        ,numNodes_(0)
        ,nodes_(NULL)
        ,numTextures_(0)
        ,textures_(NULL)
    {
        position_.identity();
        rotation_.identity();
        sphere_.zero();
    }

    //-----------------------------------------------------
    Object::~Object()
    {
        release();
    }


    //-----------------------------------------------------
    // バッファ作成
    bool Object::create(u32 numGeometries, u32 numMeshes, u32 numMaterials, u32 numNodes, u32 numTextures)
    {
        release();

        numGeometries_ = numGeometries;
        numMeshes_ = numMeshes;
        numMaterials_ = numMaterials;
        numNodes_ = numNodes;
        numTextures_ = numTextures;

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

        return true;
    }

    //-----------------------------------------------------
    bool Object::clone(Object& dst)
    {
        if(false == dst.create(numGeometries_, numMeshes_, numMaterials_, numNodes_, numTextures_)){
            return false;
        }

        cloneElements(dst);

        return true;
    }

    //-----------------------------------------------------
    void Object::swap(Object& rhs)
    {
        position_.swap(rhs.position_);
        rotation_.swap(rhs.rotation_);
        sphere_.swap(rhs.sphere_);

        lcore::swap(numGeometries_, rhs.numGeometries_);
        lcore::swap(geometries_, rhs.geometries_);
        lcore::swap(numMeshes_, rhs.numMeshes_);
        lcore::swap(meshes_, rhs.meshes_);
        lcore::swap(numMaterials_, rhs.numMaterials_);
        lcore::swap(materials_, rhs.materials_);
        lcore::swap(numNodes_, rhs.numNodes_);
        lcore::swap(nodes_, rhs.nodes_);
        lcore::swap(numTextures_, rhs.numTextures_);
        lcore::swap(textures_, rhs.textures_);
    }

    //-----------------------------------------------------
    void Object::release()
    {
        for(u32 i=0; i<numGeometries_; ++i){
            geometries_[i].~Geometry();
        }

        for(u32 i=0; i<numMeshes_; ++i){
            meshes_[i].~Mesh();
        }

        for(u32 i=0; i<numMaterials_; ++i){
            materials_[i].~Material();
        }

        for(u32 i=0; i<numNodes_; ++i){
            nodes_[i].~Node();
        }

        for(u32 i=0; i<numTextures_; ++i){
            textures_[i].~Texture2DRef();
        }

        LIME_FREE(geometries_);

        numGeometries_ = 0;
        geometries_ = NULL;

        numMeshes_ = 0;
        meshes_ = NULL;

        numMaterials_ = 0;
        materials_ = NULL;

        numNodes_ = 0;
        nodes_ = NULL;

        numTextures_ = 0;
        textures_ = NULL;
    }

    u32 Object::calcBufferSize() const
    {
        u32 size = numGeometries_ * sizeof(Geometry)
            + numMeshes_ * sizeof(Mesh)
            + numMaterials_ * sizeof(Material)
            + numNodes_ * sizeof(Node)
            + numTextures_ * sizeof(lgraphics::Texture2DRef);
        return size;
    }

    void Object::cloneElements(Object& dst)
    {
        //Geometryクローン
        for(u32 i=0; i<numGeometries_; ++i){
            geometries_[i].clone(dst.geometries_[i]);
        }

        //Meshクローン
        for(u32 i=0; i<numMeshes_; ++i){
            u32 geomIndex = (meshes_[i].getGeometry() - geometries_)/sizeof(Geometry);
            u32 materialIndex = (meshes_[i].getMaterial() - materials_)/sizeof(Material);

            dst.meshes_[i].create(
                meshes_[i].getType(),
                meshes_[i].getIndexOffset(),
                meshes_[i].getNumIndices(),
                &dst.geometries_[geomIndex],
                &dst.materials_[materialIndex],
                meshes_[i].getSphere());
        }

        //Textureクローン
        for(u32 i=0; i<numTextures_; ++i){
            dst.textures_[i] = textures_[i];
        }

        //Materialクローン
        for(u32 i=0; i<numMaterials_; ++i){
            dst.materials_[i].flags_ = materials_[i].flags_;
            dst.materials_[i].diffuse_ = materials_[i].diffuse_;
            dst.materials_[i].specular_ = materials_[i].specular_;

            for(u32 j=0; j<Material::Tex_Num; ++j){
                if(NULL != materials_[i].textures_[j]){
                    u32 texIndex = (materials_[i].textures_[j] - textures_)/sizeof(lgraphics::Texture2DRef);
                    dst.materials_[i].textures_[j] = &dst.textures_[texIndex];
                }
            }
        }

        //Nodeクローン
        for(u32 i=0; i<numNodes_; ++i){
            dst.nodes_[i] = nodes_[i];
            dst.nodes_[i].meshes_ = dst.meshes_ + dst.nodes_[i].meshStartIndex_;
        }

        dst.position_ = position_;
        dst.rotation_ = rotation_;
        dst.sphere_ = sphere_;
    }
}
