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
#include <lgraphics/io11/IOPNG.h>
#include <lgraphics/io11/IOBMP.h>

#include "load/load_geometry.h"
#include "load/load_material.h"
#include "load/load_mesh.h"
#include "load/load_node.h"
#include "load/load_joint.h"
#include "load/load_texture.h"

#include "render/Object.h"
#include "render/AnimObject.h"
#include <lcore/liostream.h>

#include "System.h"

namespace load
{
    //---------------------------------------------
    // �t�@�C���I�[�v��
    bool ModelLoader::open(const Char* filepath)
    {
        LASSERT(filepath != NULL);
        if(is_.open(filepath, lcore::ios::binary)){
            s32 len = lcore::strlen(filepath);
            if(MaxPathLength<len){
                len = MaxPathLength;
            }
            directoryPathLength_ = lcore::extractDirectoryPath(directoryPath_, filepath, len);
            return load(header_);
        }
        return false;
    }

    void ModelLoader::close()
    {
        is_.close();
    }

    //---------------------------------------------
    // �I�u�W�F�N�g���[�h
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
    // �I�u�W�F�N�g���[�h
    bool ModelLoader::load(render::AnimObject& obj)
    {
        u32 numGeometries = header_.elems_[Elem_Geometry].number_;
        u32 numMeshes = header_.elems_[Elem_Mesh].number_;
        u32 numMaterials = header_.elems_[Elem_Material].number_;
        u32 numNodes = header_.elems_[Elem_Node].number_;
        u32 numJoints = header_.elems_[Elem_Joint].number_;
        u32 numTextures = header_.elems_[Elem_Texture].number_;

        bool ret = obj.create(
            numGeometries,
            numMeshes,
            numMaterials,
            numNodes,
            numTextures,
            numJoints);

        if(!ret){
            return false;
        }

        ret = loadInternal(obj);

        //�m�[�h�ɍs��Z�b�g
        if(ret){
            for(u32 i=0; i<obj.getNumNodes(); ++i){
                obj.getNode(i).numSkinningMatrices_ = numJoints;
                obj.getNode(i).skinningMatrices_ = obj.getSkinningMatrices();
                obj.getNode(i).dualQuaternions_ = obj.getDualQuaternions();
            }
        }

        return ret;
    }

    //---------------------------------------------
    // �I�u�W�F�N�g���[�h
    bool ModelLoader::loadInternal(render::Object& obj)
    {
        u32 numGeometries = header_.elems_[Elem_Geometry].number_;
        u32 numMeshes = header_.elems_[Elem_Mesh].number_;
        u32 numMaterials = header_.elems_[Elem_Material].number_;
        u32 numNodes = header_.elems_[Elem_Node].number_;
        u32 numJoints = header_.elems_[Elem_Joint].number_;
        u32 numTextures = header_.elems_[Elem_Texture].number_;


        {//�W�I���g�����[�h
            for(u32 i=0; i<numGeometries; ++i){
                if(false == load(obj.getGeometry(i))){
                    return false;
                }
            }
        }

        {//�}�e���A�����[�h
            for(u32 i=0; i<numMaterials; ++i){
                if(false == load(obj.getMaterial(i))){
                    return false;
                }
            }
        }


        {//���b�V�����[�h
            for(u32 i=0; i<numMeshes; ++i){
                if(false == load(obj, obj.getMesh(i))){
                    return false;
                }
            }
        }

        {//�m�[�h���[�h
            for(u32 i=0; i<numNodes; ++i){
                if(false == load(obj, obj.getNode(i))){
                    return false;
                }
            }
        }

        {//�X�P���g�����[�h
            skeleton_ = loadSkeleton(numJoints);
        }

        {//�e�N�X�`�����[�h
            load::Texture loadTexture;
            lgraphics::Texture2DRef texture;
            for(u32 i=0; i<numTextures; ++i){
                lcore::io::read(is_, &loadTexture, sizeof(load::Texture));

                if(false == load(texture, loadTexture)){
                    continue;
                }
                obj.getTexture(i) = texture;

            }

            //�}�e���A���ɎQ�ƃZ�b�g
            for(u32 i=0; i<numMaterials; ++i){
                render::Material& material = obj.getMaterial(i);

                for(u32 j=0; j<render::Material::Tex_Num; ++j){

                    if(0<=material.textureIDs_[j] && material.textureIDs_[j]<numTextures){
                        s16 id = material.textureIDs_[j];
                        if(obj.getTexture(id).valid()){
                            material.textures_[j] = &(obj.getTexture(id));
                        }else{
                            material.textures_[j] = NULL;
                        }
                    }
                }
            }
        }

        {//���E��
            lmath::Sphere sphere;
            sphere.zero();
            for(u32 i=0; i<numMeshes; ++i){
                sphere.combine(sphere, obj.getMesh(i).getSphere());
            }
            obj.setSphere(sphere);
        }
        return true;
    }

    //---------------------------------------------
    // �w�b�_���[�h
    bool ModelLoader::load(Header& header)
    {
        return (LOAD_TRUE == lcore::io::read(is_, header));
    }

    //---------------------------------------------
    // �W�I���g�����[�h
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

        //�C���f�b�N�X�o�b�t�@�쐬
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
    // ���b�V�����[�h
    bool ModelLoader::load(render::Object& obj, render::Mesh& mesh)
    {
        load::Mesh tmp;
        if( LOAD_TRUE != lcore::io::read(is_, tmp) ){
            return false;
        }
        render::Material* material = &obj.getMaterial(tmp.material_);

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
    // �}�e���A�����[�h
    bool ModelLoader::load(render::Material& material)
    {
        load::Material tmp;
        if(LOAD_TRUE != lcore::io::read(is_, tmp)){
            return false;
        }

        material.flags_ = tmp.flags_;
        material.diffuse_ = tmp.diffuse_;
        material.specular_ = tmp.specular_;
        material.shadow_ = tmp.shadow_;
        material.textureIDs_[0] = tmp.texColor_;
        material.textureIDs_[1] = tmp.texNormal_;

        if((material.flags_ & Material::Flag_RefractiveIndex) && convertRefractiveIndex_){
            material.shadow_.w_ = lcore::calcFresnelTerm(material.shadow_.w_);
        }
        return true;
    }

    //---------------------------------------------
    // �m�[�h���[�h
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
        //node.rotation_ = tmp.rotation_;
        node.scale_ = tmp.scale_;
        node.type_ = tmp.type_;
        node.rotationOrder_ = tmp.rotationOrder_;
        node.meshStartIndex_ = tmp.meshStartIndex_;
        node.numMeshes_ = tmp.numMeshes_;
        node.numSkinningMatrices_ = 0;

        node.calcRotation(tmp.rotation_);
        node.world_.identity();

        if(node.meshStartIndex_ == load::InvalidNode){
            node.meshes_ = NULL;
        }else{
            node.meshes_ = &obj.getMesh(node.meshStartIndex_);
        }

        return true;
    }

    //---------------------------------------------
    lanim::Skeleton* ModelLoader::loadSkeleton(u32 numJoints)
    {
        if(numJoints<=0){
            return NULL;
        }

        lanim::Skeleton* skeleton = LIME_NEW lanim::Skeleton(numJoints);
        load::Joint tjoint;
        lanim::Name name;
        for(u32 i=0; i<numJoints; ++i){
            if(LOAD_TRUE != lcore::io::read(is_, tjoint)){
                LIME_DELETE(skeleton);
                return NULL;
            }
            lanim::Joint& joint = skeleton->getJoint(i);
            joint.setParentIndex(tjoint.parent_);
            joint.setSubjectTo(tjoint.subjectTo_);
            joint.setType(tjoint.type_);
            joint.setFlag(tjoint.flag_);
            joint.setPosition(tjoint.position_);
            skeleton->setJointName(i, tjoint.name_, load::MaxNameLength);
        }
        return skeleton;
    }

    //---------------------------------------------
    // �e�N�X�`�����[�h
    bool ModelLoader::load(lgraphics::Texture2DRef& texture, const load::Texture& loadTexture)
    {
        u32 len = lcore::strlen(loadTexture.name_);
        const Char* ext = lcore::rFindChr(loadTexture.name_, '.', len);
        if(NULL == ext){
            return false;
        }

        s32 type = 0;
        if(0==lcore::strncmp(ext, ".dds", 4)){
            type = 0;
        }else if(0==lcore::strncmp(ext, ".png", 4)){
            type = 1;
        }else if(0==lcore::strncmp(ext, ".bmp", 4)){
            type = 2;
        }

        lcore::strncpy(directoryPath_+directoryPathLength_, MaxFileNameSize, loadTexture.name_, len);

        lcore::ifstream texfile(directoryPath_, lcore::ios::binary);
        if(!texfile.is_open()){
            return false;
        }

        u32 width = 0;
        u32 height = 0;
        u32 rowBytes = 0;
        lgraphics::DataFormat format;

        switch(type)
        {
        case 0:
            {
                u32 size = texfile.getSize(0);
                u8* buffer = LIME_NEW u8[size];
                lcore::io::read(texfile, buffer, size);
                bool ret = lgraphics::io::IODDS::read(texture, buffer, size, lgraphics::Usage_Immutable, lgraphics::TexFilter_MinMagMipLinear, textureAddress_);
                LIME_DELETE_ARRAY(buffer);
                return ret;
            }
            break;

        case 1:
            if(false == lgraphics::io::IOPNG::read(texfile, NULL, width, height, rowBytes, format, lgraphics::io::IOPNG::Swap_RGB)){
                return false;
            }
            break;

        case 2:
            if(false == lgraphics::io::IOBMP::read(texfile, NULL, width, height, format)){
                return false;
            }
            rowBytes = 4*width;
            break;
        };

        u32 size = rowBytes * height;
        u8* buffer = LIME_NEW u8[size];
        bool ret = false;
        switch(type)
        {
        case 1:
            ret = lgraphics::io::IOPNG::read(texfile, buffer, width, height, rowBytes, format, lgraphics::io::IOPNG::Swap_BGR);
            break;

        case 2:
            ret = lgraphics::io::IOBMP::read(texfile, buffer, width, height, format);
            break;
        };

        if(ret){
            lgraphics::SubResourceData initData;
            initData.mem_ = buffer;
            initData.pitch_ = rowBytes;
            initData.slicePitch_ = 0;

            lgraphics::ResourceViewDesc viewDesc;
            viewDesc.dimension_ = lgraphics::ViewSRVDimension_Texture2D;
            viewDesc.format_ = format;
            viewDesc.tex2D_.mostDetailedMip_ = 0;
            viewDesc.tex2D_.mipLevels_ = 1;

            texture = lgraphics::Texture::create2D(
                width,
                height,
                1,
                1,
                format,
                lgraphics::Usage_Immutable,
                lgraphics::BindFlag_ShaderResource,
                lgraphics::CPUAccessFlag_None,
                lgraphics::ResourceMisc_None,
                lgraphics::TexFilter_MinMagLinearMipPoint,
                textureAddress_,
                lgraphics::Cmp_Never,
                0.0f,
                &initData,
                &viewDesc);
            ret = texture.valid();
        }

        LIME_DELETE_ARRAY(buffer);
        return ret;
    }

    //---------------------------------------------
    // ���_���C�A�E�g�쐬
    void ModelLoader::createInputLayout(u32 vflag, lgraphics::InputLayoutRef& inputLayout)
    {
        render::Layout layoutType = render::Layout_P;
        switch(vflag)
        {
        case (VElem_Position):
            layoutType = render::Layout_P;
            break;

        case (VElem_Position|VElem_Normal):
            layoutType = render::Layout_PN;
            break;

        case (VElem_Position|VElem_Texcoord):
            layoutType = render::Layout_PU;
            break;

        case (VElem_Position|VElem_Color):
            layoutType = render::Layout_PC;
            break;

        case (VElem_Position|VElem_Normal|VElem_Tangent|VElem_Binormal):
            layoutType = render::Layout_PNTB;
            break;

        case (VElem_Position|VElem_Normal|VElem_Texcoord):
            layoutType = render::Layout_PNU;
            break;

        case (VElem_Position|VElem_Normal|VElem_Color|VElem_Texcoord):
            layoutType = render::Layout_PNCU;
            break;

        case (VElem_Position|VElem_Normal|VElem_Tangent|VElem_Binormal|VElem_Texcoord):
            layoutType = render::Layout_PNTBU;
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
    void ModelLoader::getTextureNameTable(u32 numTextures, load::Texture* textures)
    {
        if(!is_.is_open()){
            return;
        }
        is_.seekg(0, lcore::ios::beg);
        load(header_);

        is_.seekg(header_.elems_[Elem_Texture].offset_, lcore::ios::beg);
        LASSERT(numTextures == header_.elems_[Elem_Texture].number_);

        for(u32 i=0; i<numTextures; ++i){
            lcore::io::read(is_, &textures[i], sizeof(load::Texture));
        }
    }

    //
    bool ModelLoader::save(render::Object& obj, load::Texture* textures, lanim::Skeleton::pointer& skeleton, const Char* filepath)
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
            loadMaterials[i].texColor_ = material.textureIDs_[0];
            loadMaterials[i].texNormal_ = material.textureIDs_[1];

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

            loadNodes[i].type_ = node.type_;
            loadNodes[i].rotationOrder_ = node.rotationOrder_;
            loadNodes[i].meshStartIndex_ = node.meshStartIndex_;
            loadNodes[i].numMeshes_ = node.numMeshes_;

            offset += sizeof(load::Node);
        }

        load::Joint* loadJoints = NULL;
        if(NULL == skeleton){
            loadHeader.elems_[load::Elem_Joint].number_ = 0;
            loadHeader.elems_[load::Elem_Joint].offset_ = offset;
        }else{
            loadJoints = LIME_NEW load::Joint[skeleton->getNumJoints()];
            loadHeader.elems_[load::Elem_Joint].number_ = skeleton->getNumJoints();
            loadHeader.elems_[load::Elem_Joint].offset_ = offset;

            for(s32 i=0; i<skeleton->getNumJoints(); ++i){
                lanim::Joint& joint = skeleton->getJoint(i);
                loadJoints[i].parent_ = joint.getParentIndex();
                loadJoints[i].subjectTo_ = joint.getSubjectTo();
                loadJoints[i].type_ = joint.getType();
                loadJoints[i].flag_ = joint.getFlag();
                loadJoints[i].position_ = joint.getPosition();
                lcore::memcpy(loadJoints[i].name_, skeleton->getJointName(i).c_str(), load::MaxNameSize);
                offset += sizeof(load::Joint);
            }
        }
        loadHeader.elems_[load::Elem_Texture].number_ = obj.getNumTextures();
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
        if(NULL != loadJoints){
            lcore::io::write(os, loadJoints, sizeof(load::Joint)*skeleton->getNumJoints());
        }

        lcore::io::write(os, textures, sizeof(load::Texture)*obj.getNumTextures());

        os.close();

        LIME_DELETE_ARRAY(loadJoints);
        LIME_DELETE_ARRAY(loadNodes);
        LIME_DELETE_ARRAY(loadMeshes);
        LIME_DELETE_ARRAY(loadMaterials);
        LIME_DELETE_ARRAY(loadGeometries);
        return true;
    }
}
