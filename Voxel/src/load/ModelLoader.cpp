/**
@file ModelLoader.cpp
@author t-sakai
@date 2011/10/29 create
*/
#include "stdafx.h"
#include "ModelLoader.h"

#include <lgraphics/api/InputLayoutRef.h>
#include <lgraphics/api/VertexBufferRef.h>
#include <lgraphics/api/IndexBufferRef.h>
#include <lgraphics/io11/IODDS.h>

#include "load/load_geometry.h"
#include "load/load_material.h"
#include "load/load_mesh.h"
#include "load/load_node.h"
#include "load/load_texture.h"

#include "render/Object.h"
#include "render/AnimObject.h"
#include <lcore/liostream.h>

#include "System.h"

namespace load
{
    //---------------------------------------------
    // ファイルオープン
    bool ModelLoader::open(const Char* filepath)
    {
        LASSERT(filepath != NULL);
        if(is_.open(filepath, lcore::ios::binary)){

            return load(header_);
        }
        return false;
    }

    void ModelLoader::close()
    {
        is_.close();
    }

    //---------------------------------------------
    // オブジェクトロード
    bool ModelLoader::load(render::Object& obj)
    {
        u32 numGeometries = header_.elems_[Elem_Geometry].number_;
        u32 numMeshes = header_.elems_[Elem_Mesh].number_;
        u32 numMaterials = header_.elems_[Elem_Material].number_;
        u32 numNodes = header_.elems_[Elem_Node].number_;
        u32 numTextures = header_.elems_[Elem_Texture].number_;

        bool ret = obj.create(
            numGeometries,
            numMeshes,
            numMaterials,
            numNodes,
            numTextures);

        if(!ret){
            return false;
        }

        return loadInternal(obj);
    }

    //---------------------------------------------
    // オブジェクトロード
    bool ModelLoader::load(render::AnimObject& obj, u32 numSkinningMatrices)
    {
        u32 numGeometries = header_.elems_[Elem_Geometry].number_;
        u32 numMeshes = header_.elems_[Elem_Mesh].number_;
        u32 numMaterials = header_.elems_[Elem_Material].number_;
        u32 numNodes = header_.elems_[Elem_Node].number_;
        u32 numTextures = header_.elems_[Elem_Texture].number_;

        bool ret = obj.create(
            numGeometries,
            numMeshes,
            numMaterials,
            numNodes,
            numTextures,
            numSkinningMatrices);

        if(!ret){
            return false;
        }

        ret = loadInternal(obj);

        //ノードに行列セット
        if(ret){
            for(u32 i=0; i<obj.getNumNodes(); ++i){
                obj.getNode(i).numSkinningMatrices_ = numSkinningMatrices;
                obj.getNode(i).skinningMatrices_ = obj.getSkinningMatrices();
            }
        }

        return ret;
    }

    //---------------------------------------------
    // オブジェクトロード
    bool ModelLoader::loadInternal(render::Object& obj)
    {
        u32 numGeometries = header_.elems_[Elem_Geometry].number_;
        u32 numMeshes = header_.elems_[Elem_Mesh].number_;
        u32 numMaterials = header_.elems_[Elem_Material].number_;
        u32 numNodes = header_.elems_[Elem_Node].number_;
        u32 numTextures = header_.elems_[Elem_Texture].number_;


        {//ジオメトリロード
            for(u32 i=0; i<numGeometries; ++i){
                if(false == load(obj.getGeometry(i))){
                    return false;
                }
            }
        }

        {//マテリアルロード
            for(u32 i=0; i<numMaterials; ++i){
                if(false == load(obj.getMaterial(i))){
                    return false;
                }
            }
        }


        {//メッシュロード
            for(u32 i=0; i<numMeshes; ++i){
                if(false == load(obj, obj.getMesh(i))){
                    return false;
                }
            }
        }

        {//ノードロード
            for(u32 i=0; i<numNodes; ++i){
                if(false == load(obj, obj.getNode(i))){
                    return false;
                }
            }
        }

        {//テクスチャロード
            lgraphics::Texture2DRef textures[ load::TexType_Num ];
            u32 count = 0;
            for(u32 i=0; i<load::TexType_Num; ++i){
                if(false == load(textures[i])){
                    return false;
                }
                if(false == textures[i].valid()){
                    continue;
                }

                if(count<numTextures){
                    obj.getTexture(count) = textures[i];
                    ++count;
                }
            }

            for(u32 i=0; i<numMaterials; ++i){
                render::Material& material = obj.getMaterial(i);

                count = 0;
                for(u32 j=0; j<render::Material::Tex_Num; ++j){

                    if(textures[j].valid()){
                        material.textures_[j] = &(obj.getTexture(count));
                        ++count;
                    }else{
                        material.textures_[j] = NULL;
                    }
                }
            }
        }

        {//境界球
            lmath::Sphere sphere;
            sphere.zero();
            for(u32 i=0; i<numMeshes; ++i){
                sphere.combine(sphere, obj.getMesh(i).getSphere());
            }
            obj.setSphere(sphere);
        }

        {
            //lcore::ofstream file("log.txt", lcore::ios::binary);
            //for(u32 i=0; i<obj.getNumNodes(); ++i){
            //    render::Node& node = obj.getNode(i);
            //    node.calcLocalMatrix();
                //file.print("node:%d, parent:%d\n", node.index_, node.parentIndex_);
                //file.print("scale: %f, %f, %f\n", node.scale_.x_, node.scale_.y_, node.scale_.z_);
                //file.print("rotate: %f, %f, %f\n", node.rotation_.x_, node.rotation_.y_, node.rotation_.z_);
                //file.print("translate: %f, %f, %f\n", node.translation_.x_, node.translation_.y_, node.translation_.z_);
            //}
            //file.close();


            //lmath::Matrix44 parentMat;
            //for(;i<obj.getNumNodes(); ++i){
            //    render::Node& node = obj.getNode(i);
            //    render::Node& parent = obj.getNode( node.parentIndex_ );

            //    parentMat = parent.matrix_;
            //    parentMat.invert();
            //    node.matrix_.mul(parentMat, node.matrix_);
            //}
        }

        return true;
    }

    //---------------------------------------------
    // ヘッダロード
    bool ModelLoader::load(Header& header)
    {
        return (LOAD_TRUE == lcore::io::read(is_, header));
    }

    //---------------------------------------------
    // ジオメトリロード
    bool ModelLoader::load(render::Geometry& geometry)
    {
        load::Geometry tmp;
        if( LOAD_TRUE != lcore::io::read(is_, tmp) ){
            return false;
        }

        lgraphics::InputLayoutRef inputLayout;
        createInputLayout(tmp.vflag_, inputLayout);
        if(false == inputLayout.valid()){
            return false;
        }

        u32 size = tmp.vsize_*tmp.numVertices_;
        u8* vertices = reinterpret_cast<u8*>( LIME_MALLOC(size) );
        if( LOAD_TRUE != lcore::io::read(is_, vertices, size) ){
            LIME_FREE(vertices);
            return false;
        }

        lgraphics::VertexBufferRef vb = lgraphics::VertexBuffer::create(
            size,
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            0,
            vertices);

        //f32* vertices = reinterpret_cast<f32*>(data);
        //for(u32 i=0; i<tmp.numVertices_; ++i){
        //    lcore::Log("(%f, %f, %f), (%f, %f, %f)", vertices[0], vertices[1], vertices[2], vertices[3], vertices[4], vertices[5]);
        //    vertices += 6;
        //}


        //インデックスバッファ作成
        size = sizeof(u16) * tmp.numIndices_;
        u16* indices = reinterpret_cast<u16*>( LIME_MALLOC(size) );

        if( LOAD_TRUE != lcore::io::read(is_, indices, size) ){
            LIME_FREE(indices);
            return false;
        }

        lgraphics::IndexBufferRef ib = lgraphics::IndexBuffer::create(
            size,
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            0,
            indices);


        geometry.create(
            tmp.vflag_,
            tmp.vsize_,
            tmp.numVertices_,
            vertices,
            tmp.numIndices_,
            indices,
            inputLayout, vb, ib);
        return true;
    }

    //---------------------------------------------
    // メッシュロード
    bool ModelLoader::load(render::Object& obj, render::Mesh& mesh)
    {
        load::Mesh tmp;
        if( LOAD_TRUE != lcore::io::read(is_, tmp) ){
            return false;
        }

        mesh.create(
            lgraphics::Primitive_TriangleList,
            tmp.indexOffset_,
            tmp.numIndices_,
            &obj.getGeometry(tmp.geometry_),
            &obj.getMaterial(tmp.material_),
            tmp.sphere_);

        return true;
    }

    //---------------------------------------------
    // マテリアルロード
    bool ModelLoader::load(render::Material& material)
    {
        load::Material tmp;
        if(LOAD_TRUE != lcore::io::read(is_, tmp)){
            return false;
        }

        material.flags_ = tmp.flags_;
        material.diffuse_ = tmp.diffuse_;
        material.specular_ = tmp.specular_;
        material.shadow_ = tmp.shadow_; //編集するために屈折をフレネル反射率に変換しない
        material.textureIDs_[0] = tmp.texColor_;
        material.textureIDs_[1] = tmp.texNormal_;

        return true;
    }

    //---------------------------------------------
    // ノードロード
    bool ModelLoader::load(render::Object& obj, render::Node& node)
    {
        load::Node tmp;

        if(LOAD_TRUE != lcore::io::read(is_, tmp)){
            return false;
        }

        node.index_ = tmp.index_;
        node.parentIndex_ = tmp.parentIndex_;
        node.childStartIndex_ = tmp.childStartIndex_;
        node.numChildren_ = tmp.numChildren_;
        node.translation_ = tmp.translation_;
        node.rotation_ = tmp.rotation_;
        node.scale_ = tmp.scale_;
        node.rotationOrder_ = tmp.rotationOrder_;
        node.numSkinningMatrices_ = tmp.reserved_;
        node.meshStartIndex_ = tmp.meshStartIndex_;
        node.numMeshes_ = tmp.numMeshes_;

        node.world_.identity();

        if(node.meshStartIndex_ == load::InvalidNode){
            node.meshes_ = NULL;
        }else{
            node.meshes_ = &obj.getMesh(node.meshStartIndex_);
        }

        return true;
    }

    //---------------------------------------------
    // テクスチャロード
    bool ModelLoader::load(lgraphics::Texture2DRef& texture)
    {
        load::Texture tex;
        lcore::io::read(is_, tex);
        if(tex.type_ == load::TexType_Num){
            return true;
        }

        //TODO:
        //fractal::BufferAllocator& allocator = fractal::System::getAllocator();

        ////インデックスバッファ作成
        //u32 size = sizeof(u16) * tmp.numIndices_;
        //u8* data = allocator.allocate(size);

        //return lgraphics::io::IODDS::read(is_, texture);
        return true;
    }

    //---------------------------------------------
    // 頂点レイアウト作成
    void ModelLoader::createInputLayout(u32 vflag, lgraphics::InputLayoutRef& inputLayout)
    {
        render::Layout layoutType = render::Layout_P;
        switch(vflag)
        {
        case (VElem_Position|VElem_Normal):
            layoutType = render::Layout_PN;
            break;

        case (VElem_Position|VElem_Normal|VElem_Texcoord|VElem_Bone|VElem_BoneWeight):
            layoutType = render::Layout_PNUBone;
            break;

        default:
            LASSERT(false);
            break;
        }

        inputLayout = fractal::System::getInputLayoutFactory().get(layoutType);
    }

    //
    bool ModelLoader::save(render::Object& obj, const Char* filepath)
    {
        lcore::ofstream os(filepath, lcore::ios::binary);
        if(!os.is_open()){
            return false;
        }

        load::Header loadHeader;
        loadHeader.major_ = 1;
        loadHeader.minor_ = 0;
        u32 offset = sizeof(load::Header);

        load::Geometry* loadGeometries = LIME_NEW load::Geometry[obj.getNumGeometries()];
        loadHeader.elems_[load::Elem_Geometry].number_ = obj.getNumGeometries();
        loadHeader.elems_[load::Elem_Geometry].offset_ = offset;
        for(u32 i=0; i<obj.getNumGeometries(); ++i){
            render::Geometry& geometry = obj.getGeometry(i);
            loadGeometries[i].vflag_ = geometry.getVFlag();
            loadGeometries[i].vsize_ = geometry.getVSize();
            loadGeometries[i].numVertices_ = geometry.getNumVertices();
            loadGeometries[i].numIndices_ = geometry.getNumIndices();

            offset += sizeof(load::Geometry);
            offset += geometry.getVSize() * geometry.getNumVertices();
            offset += sizeof(u16) * geometry.getNumIndices();
        }

        load::Material* loadMaterials = LIME_NEW load::Material[obj.getNumMaterials()];
        loadHeader.elems_[load::Elem_Material].number_ = obj.getNumMaterials();
        loadHeader.elems_[load::Elem_Material].offset_ = offset;
        for(u32 i=0; i<obj.getNumMaterials(); ++i){
            render::Material& material = obj.getMaterial(i);
            loadMaterials[i].flags_ = material.flags_;
            loadMaterials[i].diffuse_ = material.diffuse_;
            loadMaterials[i].specular_ = material.specular_;
            loadMaterials[i].shadow_ = material.shadow_;
            loadMaterials[i].texColor_ = 0;
            loadMaterials[i].texNormal_ = 0;

            offset += sizeof(load::Material);
        }

        load::Mesh* loadMeshes = LIME_NEW load::Mesh[obj.getNumMeshes()];
        loadHeader.elems_[load::Elem_Mesh].number_ = obj.getNumMeshes();
        loadHeader.elems_[load::Elem_Mesh].offset_ = offset;
        for(u32 i=0; i<obj.getNumMeshes(); ++i){
            render::Mesh& mesh = obj.getMesh(i);

            loadMeshes[i].geometry_ = (mesh.getGeometry() - &obj.getGeometry(0));
            loadMeshes[i].material_ = (mesh.getMaterial() - &obj.getMaterial(0));
            loadMeshes[i].indexOffset_ = mesh.getIndexOffset();
            loadMeshes[i].numIndices_ = mesh.getNumIndices();
            loadMeshes[i].sphere_ = mesh.getSphere();

            offset += sizeof(load::Mesh);
        }

        load::Node* loadNodes = LIME_NEW load::Node[obj.getNumNodes()];
        loadHeader.elems_[load::Elem_Node].number_ = obj.getNumNodes();
        loadHeader.elems_[load::Elem_Node].offset_ = offset;
        for(u32 i=0; i<obj.getNumNodes(); ++i){
            render::Node& node = obj.getNode(i);
            loadNodes[i].index_ = node.index_;
            loadNodes[i].parentIndex_ = node.parentIndex_;
            loadNodes[i].childStartIndex_ = node.childStartIndex_;
            loadNodes[i].numChildren_ = node.numChildren_;
            loadNodes[i].translation_.set(node.translation_.x_, node.translation_.y_, node.translation_.z_);
            loadNodes[i].rotation_.set(node.rotation_.x_, node.rotation_.y_, node.rotation_.z_);
            loadNodes[i].scale_.set(node.scale_.x_, node.scale_.y_, node.scale_.z_);

            loadNodes[i].rotationOrder_ = node.rotationOrder_;
            loadNodes[i].reserved_ = node.numSkinningMatrices_;
            loadNodes[i].meshStartIndex_ = node.meshStartIndex_;
            loadNodes[i].numMeshes_ = node.numMeshes_;

            offset += sizeof(load::Node);
        }

        loadHeader.elems_[load::Elem_Texture].number_ = 0;
        loadHeader.elems_[load::Elem_Texture].offset_ = offset;


        lcore::io::write(os, loadHeader);
        for(u32 i=0; i<obj.getNumGeometries(); ++i){
            render::Geometry& geometry = obj.getGeometry(i);

            lcore::io::write(os, loadGeometries[i]);
            lcore::io::write(os, geometry.getVertices(), geometry.getVSize()*geometry.getNumVertices());
            lcore::io::write(os, geometry.getIndices(), sizeof(u16)*geometry.getNumIndices());
        }

        lcore::io::write(os, loadMaterials, sizeof(load::Material)*obj.getNumMaterials());
        lcore::io::write(os, loadMeshes, sizeof(load::Mesh)*obj.getNumMeshes());
        lcore::io::write(os, loadNodes, sizeof(load::Node)*obj.getNumNodes());

        os.close();

        LIME_DELETE_ARRAY(loadNodes);
        LIME_DELETE_ARRAY(loadMeshes);
        LIME_DELETE_ARRAY(loadMaterials);
        LIME_DELETE_ARRAY(loadGeometries);
        return true;
    }
}
