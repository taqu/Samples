/**
@file DebugSkeleton.cpp
@author t-sakai
@date 2013/07/31 create
*/
#include "DebugSkeleton.h"

#include <lgraphics/api/InputLayoutRef.h>
#include <lgraphics/api/VertexBufferRef.h>
#include <lgraphics/api/IndexBufferRef.h>

#include "System.h"

#include "render/AnimObject.h"
#include "load/load_geometry.h"

namespace anim
{
namespace
{
    void createBone(load::VertexPNUBone* vertices, u16* indices, u16 index, u16 bone, const lmath::Vector3& startPos, const lmath::Vector3& endPos)
    {
        lmath::Vector3 positions[6];
        lmath::Vector3 normals[6];

        f32 l = startPos.distance(endPos);
        f32 w = l * 0.1f;
        f32 ml = 0.3f * l;

        positions[0].set(0.0f, 0.0f, 0.0f);
        positions[1].set(-w, w, ml);
        positions[2].set(w, w, ml);
        positions[3].set(-w, -w, ml);
        positions[4].set(w, -w, ml);
        positions[5].set(0.0f, 0.0f, l);

        lcore::memset(normals, 0, sizeof(lmath::Vector3)*6);
        lcore::memset(vertices, 0, sizeof(load::VertexPNUBone)*6);

        //インデックスセット
        indices[0] = index + 0;
        indices[1] = index + 1;
        indices[2] = index + 2;

        indices[3] = index + 0;
        indices[4] = index + 3;
        indices[5] = index + 1;

        indices[6] = index + 0;
        indices[7] = index + 2;
        indices[8] = index + 4;

        indices[9] = index + 0;
        indices[10] = index + 4;
        indices[11] = index + 3;


        indices[12] = index + 5;
        indices[13] = index + 2;
        indices[14] = index + 1;

        indices[15] = index + 5;
        indices[16] = index + 1;
        indices[17] = index + 3;

        indices[18] = index + 5;
        indices[19] = index + 4;
        indices[20] = index + 2;

        indices[21] = index + 5;
        indices[22] = index + 3;
        indices[23] = index + 4;

        lmath::Matrix44 localMatrix;
        localMatrix.identity();

        //法線計算
        if(!lmath::isZeroPositive(l)){
            for(u32 i=0; i<8; ++i){
                u32 vindex = 3*i;

                u32 v0 = indices[vindex + 0] - index;
                u32 v1 = indices[vindex + 1] - index;
                u32 v2 = indices[vindex + 2] - index;

                lmath::Vector3 d0, d1;
                d0.sub(positions[v1], positions[v0]);
                d1.sub(positions[v2], positions[v0]);

                d0.normalize();
                d1.normalize();

                lmath::Vector3 n;
                n.cross(d0, d1);
                n.normalize();

                normals[v0] += n;
                normals[v1] += n;
                normals[v2] += n;
            }

            for(u32 i=0; i<6; ++i){
                normals[i].normalize();
            }

            lmath::Vector3 up(0.0f, 1.0f, 0.0f);
            lmath::Vector3 d;
            d.sub(endPos, startPos);
            if(lmath::isEqual(0.0f, d.x_) && lmath::isEqual(0.0f, d.z_)){
                up.set(0.0f, 0.0f, 1.0f);
            }

            localMatrix.lookAt(startPos, endPos, up);
            localMatrix.invert();
        }

        for(u32 i=0; i<6; ++i){
            lmath::Vector4 v;
            v.set(positions[i].x_, positions[i].y_, positions[i].z_, 1.0f);
            v.mul(localMatrix, v);

            vertices[i].x_ = v.x_;
            vertices[i].y_ = v.y_;
            vertices[i].z_ = v.z_;

            v.set(normals[i].x_, normals[i].y_, normals[i].z_, 0.0f);
            v.mul(localMatrix, v);
            vertices[i].nx_ = v.x_;
            vertices[i].ny_ = v.y_;
            vertices[i].nz_ = v.z_;

            vertices[i].u_ = 0.0f;
            vertices[i].v_ = 0.0f;

            vertices[i].bone_[0] = bone;
            vertices[i].bone_[1] = bone;
            vertices[i].weight_[0] = lcore::toBinary16Float(1.0f);
            vertices[i].weight_[1] = lcore::toBinary16Float(0.0f);
        }
    }
}

    void DebugSkeleton::create(render::AnimObject& animObject, const lanim::Skeleton::pointer& skeleton)
    {
        LASSERT(NULL != skeleton);

        s32 numRoots = 0;
        for(s32 i=0; i<skeleton->getNumJoints(); ++i){
            if(lanim::InvalidJointIndex == skeleton->getJoint(i).getParentIndex()){
                ++numRoots;
            }
        }


        animObject.create(
            1,
            1,
            1,
            1,
            0,
            skeleton->getNumJoints());

        u32 numBones = skeleton->getNumJoints() - numRoots;
        u32 numVertices = 6 * numBones;
        u32 numIndices = 8*3 * numBones;

        load::VertexPNUBone* vertices = LIME_NEW load::VertexPNUBone[numVertices];
        u16* indices = LIME_NEW u16[numIndices];


        lmath::Sphere sphere;
        sphere.zero();

        lmath::Vector3 startPos;
        lmath::Vector3 endPos;
        u32 index = 0;
        u32 ioffset = 0;
        for(s32 i=numRoots; i<skeleton->getNumJoints(); ++i){
            const lanim::Joint& joint = skeleton->getJoint(i);

            //親のオフセット計算
            calcOffset(startPos, joint.getParentIndex(), skeleton);

            //ジョイントのオフセット計算
            calcOffset(endPos, i, skeleton);

            createBone(vertices + index, indices + ioffset, index, joint.getParentIndex(), startPos, endPos);

            index += 6;
            ioffset += 24;
        }

        lgraphics::InputLayoutRef& inputLayout = fractal::System::getInputLayoutFactory().get(render::Layout_PNUBone);

        lgraphics::VertexBufferRef vb = lgraphics::VertexBuffer::create(
            numVertices * sizeof(load::VertexPNUBone),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            0,
            vertices);

        lgraphics::IndexBufferRef ib = lgraphics::IndexBuffer::create(
            numIndices * sizeof(u16),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            0,
            indices);

        render::Geometry& geometry = animObject.getGeometry(0);
        geometry.create(
            load::VElem_Position | load::VElem_Normal | load::VElem_Bone | load::VElem_BoneWeight,
            sizeof(load::VertexPNUBone),
            numVertices,
            NULL,
            numIndices,
            NULL,
            inputLayout,
            vb,
            ib);

        LIME_DELETE_ARRAY(indices);
        LIME_DELETE_ARRAY(vertices);

        render::Material& material = animObject.getMaterial(0);

        render::Mesh& mesh = animObject.getMesh(0);
        mesh.create(
            lgraphics::Primitive_TriangleList,
            0,
            numIndices,
            &geometry,
            &material,
            sphere);

        render::Node& node = animObject.getNode(0);
        node.clearTransform();

        node.rotationOrder_ = load::RotationOrder_EulerZYX;
        node.numSkinningMatrices_ = numBones;
        node.meshStartIndex_ = 0;
        node.numMeshes_ = 1;
        node.meshes_ = &mesh;
        node.skinningMatrices_ = animObject.getSkinningMatrices();

    }

    void DebugSkeleton::calcOffset(lmath::Vector3& offset, u8 jointIndex, const lanim::Skeleton::pointer& skeleton)
    {
        //offset.zero();

        //while(lanim::InvalidJointIndex != skeleton->getJoint(jointIndex).getParentIndex()){
        //    offset += skeleton->getJoint(jointIndex).getPosition();
        //    jointIndex = skeleton->getJoint(jointIndex).getParentIndex();
        //}
        offset = skeleton->getJoint(jointIndex).getPosition();
    }
}
