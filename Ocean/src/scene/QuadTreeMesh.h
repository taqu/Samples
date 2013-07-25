#ifndef INC_SCENE_QUADTREEMESH_H__
#define INC_SCENE_QUADTREEMESH_H__
/**
@file Ocean.h
@author t-sakai
@date 2013/07/05 create
*/
#include "scene.h"
#include <lcore/Vector.h>

#include <lmath/lmath.h>
#include <lgraphics/api/GraphicsDeviceRef.h>
#include <lgraphics/api/InputLayoutRef.h>
#include <lgraphics/api/VertexBufferRef.h>
#include <lgraphics/api/IndexBufferRef.h>
#include <lframework/scene/Frustum.h>

namespace lscene
{
    class Camera;
}

namespace scene
{
    class QuadTreeMesh
    {
    public:
        static const u32 NumMaxLODLevels = 6;

        struct Vertex
        {
            f32 x_;
            f32 y_;
        };

        struct Quad
        {
            u32 numInnerVertices_;
            u32 numInnerFaces_;
            u32 innerStartIndex_;

            u32 numBoundaryVertices_;
            u32 numBoundaryFaces_;
            u32 boundaryStartIndex_;
        };

        struct QuadNode
        {
            QuadNode()
            {}

            QuadNode(f32 left, f32 bottom, f32 length, s32 lod)
                :bottomLeft_(left, bottom)
                ,length_(length)
                ,lod_(lod)
            {
                children_[0] = children_[1] = children_[2] = children_[3] = -1;
            }

            bool isLeaf() const
            {
                return (children_[0]<0) && (children_[1]<0) && (children_[2]<0) && (children_[3]<0);
            }

            lmath::Vector2 bottomLeft_;
            f32 length_;
            s32 lod_;
            s32 children_[4];
        };

        typedef lcore::vector_arena<QuadNode> NodeVector;

        QuadTreeMesh();
        ~QuadTreeMesh();

        s32 getResolution() const{ return resolution_;}

        void updateViewport(f32 screenArea, f32 maxGridCoverage, const lscene::Camera& camera);

        void create(s32 resolution, f32 patchSize);

        void calcVisibleNode(const lscene::Camera& camera);

        const Quad& selectMeshPattern(const QuadNode& node);

        const NodeVector& getVisibleNodes() const{ return nodes_;}

        void attach();

    private:
        struct Rect
        {
            s32 left_;
            s32 right_;
            s32 top_;
            s32 bottom_;
        };

        u16 createIndex(u32 x, u32 y, u32 offsetx, u32 offsety) const;
        u16 createIndex(u32 x, u32 y) const;
        u32 createInnerMesh(const Rect& rect, u16* indices);
        u32 createBondaryMesh(s32 leftDegree, s32 rightDegree, s32 bottomDegree, s32 topDegree, s32 resolution, u16* indices);

        bool checkVisibility(QuadNode& node, const lscene::Camera& camera);

        f32 estimateGridCoverage(const QuadNode& node, const lscene::Camera& camera);

        s32 traverse(QuadNode& node, const lscene::Camera& camera);
        s32 searchLeaf(f32 x, f32 y);

        s32 getType(s32 adjacent, const QuadNode& node);

        s32 resolution_;
        f32 patchSize_;
        s32 numLODs_;

        u32 numVertices_;
        u32 numIndices_;

        Quad meshPatterns_[NumMaxLODLevels][3][3][3][3];

        lgraphics::InputLayoutRef inputLayout_;
        lgraphics::VertexBufferRef vb_;
        lgraphics::IndexBufferRef ib_;

        NodeVector nodes_;
        lscene::Frustum frustum_;
        lmath::Vector4 eyePosition_;

        f32 maxGridCoverage_;
        f32 screenArea_;
    };
}
#endif //INC_SCENE_QUADTREEMESH_H__
