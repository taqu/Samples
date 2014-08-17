/**
@file Ocean.cpp
@author t-sakai
@date 2013/07/05 create
*/
#include "QuadTreeMesh.h"
#include <lframework/scene/Camera.h>
#include "System.h"

namespace scene
{
namespace
{
    u32 NumQuadNodes[] =
    {
        1,
        5,
        21,
        85,
        341,
        1365,
        5461,
        21845,
        87381,
    };
}

    QuadTreeMesh::QuadTreeMesh()
        :resolution_(0)
        ,numLODs_(0)
        ,numVertices_(0)
        ,numIndices_(0)
    {
    }

    QuadTreeMesh::~QuadTreeMesh()
    {
    }

    void QuadTreeMesh::updateViewport(f32 screenArea, f32 maxGridCoverage, const lscene::Camera& camera)
    {
        screenArea_ = screenArea;
        maxGridCoverage_ = maxGridCoverage;

        frustum_.calcInView(camera, camera.getZNear(), camera.getZFar());
    }

    void QuadTreeMesh::create(s32 resolution, f32 patchSize)
    {
        resolution_ = resolution;
        patchSize_ = patchSize;

        inputLayout_ = fractal::System::getInputLayoutFactory().get(render::Layout_Ocean);

        numVertices_ = (resolution_+1)*(resolution_+1);

        Vertex* vertices = LIME_NEW Vertex[numVertices_];

        for(s32 i=0; i<=resolution_; ++i){
            for(s32 j=0; j<=resolution_; ++j){
                vertices[i*(resolution_+1) + j].x_ = j;
                vertices[i*(resolution_+1) + j].y_ = i;
            }
        }

         vb_ = lgraphics::VertexBuffer::create(
            sizeof(Vertex)*numVertices_,
            lgraphics::Usage_Immutable,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            sizeof(Vertex),
            vertices);
        LIME_DELETE_ARRAY(vertices);

        const u32 NumIndices[] =
        {
            0,
            0,
            4284,
            18828,
            69444,
            254412,
            956916,
            3689820,
            14464836,
        };

        lcore::memset(meshPatterns_, 0, sizeof(meshPatterns_));
        numLODs_ = 0;
        for(s32 i=resolution_; 1<i; i>>=1){
            ++numLODs_;
        }

        u32 indexOffset=0;
        numIndices_ = NumIndices[numLODs_];
        u16* indices = LIME_NEW u16[ numIndices_ ];

        for(s32 level=0; level<=numLODs_-2; ++level){
            s32 leftDegree = resolution;

            for(s32 leftType=0; leftType<3; ++leftType){
                s32 rightDegree = resolution;

                for(s32 rightType=0; rightType<3; ++rightType){
                    s32 bottomDegree = resolution;

                    for(s32 bottomType=0; bottomType<3; ++bottomType){
                        s32 topDegree = resolution;

                        for(s32 topType=0; topType<3; ++topType){
                            Quad& pattern = meshPatterns_[level][leftType][rightType][bottomType][topType];

                            Rect rect;
                            rect.left_ = (leftDegree == resolution)? 0 : 1;
                            rect.right_ = (rightDegree == resolution)? resolution : resolution - 1;
                            rect.bottom_ = (bottomDegree == resolution)? 0 : 1;
                            rect.top_ = (topDegree == resolution)? resolution : resolution - 1;

                            u32 numNewIndices = createInnerMesh(rect, indices + indexOffset);

                            pattern.innerStartIndex_ = indexOffset;
                            pattern.numInnerVertices_ = (resolution+1)*(resolution+1);
                            pattern.numInnerFaces_ = numNewIndices - 2;
                            indexOffset += numNewIndices;

                            s32 lDeg = (leftDegree == resolution)? 0 : leftDegree;
                            s32 rDeg = (rightDegree == resolution)? 0 : rightDegree;
                            s32 bDeg = (bottomDegree == resolution)? 0 : bottomDegree;
                            s32 tDeg = (topDegree == resolution)? 0 : topDegree;

                            numNewIndices = createBondaryMesh(lDeg, rDeg, bDeg, tDeg, resolution, indices + indexOffset);

                            pattern.boundaryStartIndex_ = indexOffset;
                            pattern.numBoundaryVertices_ = (resolution+1)*(resolution+1);
                            pattern.numBoundaryFaces_ = numNewIndices/3;

                            indexOffset += numNewIndices;

                            topDegree /= 2;
                        } //for(u32 topType=0

                        bottomDegree /= 2;
                    } //for(u32 bottomType=0

                    rightDegree /= 2;
                } //for(u32 rightType=0

                leftDegree /= 2;
            } //for(u32 leftType=0

            resolution /= 2;
        } //for(u32 level=0

        ib_ = lgraphics::IndexBuffer::create(
            sizeof(u16)*numIndices_,
            lgraphics::Usage_Immutable,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            0,
            indices);

        LIME_DELETE_ARRAY(indices);
    }

    void QuadTreeMesh::calcVisibleNode(const lscene::Camera& camera)
    {
        nodes_.clear();

        camera.getEyePosition(eyePosition_);

        f32 haflExtent = patchSize_ * (1<<(NumMaxLODLevels-2));

        QuadNode root(-haflExtent, -haflExtent, 2.0f*haflExtent, 0);
        traverse(root, camera);
    }

    const QuadTreeMesh::Quad& QuadTreeMesh::selectMeshPattern(const QuadNode& node)
    {
        f32 halfPatchSize = patchSize_ * 0.5f;
        f32 halfNodeSize = node.length_ * 0.5f;

        f32 x, y;

        x = node.bottomLeft_.x_ - halfPatchSize;
        y = node.bottomLeft_.y_ + halfNodeSize;
        s32 left = searchLeaf(x, y);

        x = node.bottomLeft_.x_ + node.length_ + halfPatchSize;
        y = node.bottomLeft_.y_ + halfNodeSize;
        s32 right = searchLeaf(x, y);

        x = node.bottomLeft_.x_ + halfNodeSize;
        y = node.bottomLeft_.y_ - halfPatchSize;
        s32 bottom = searchLeaf(x, y);

        x = node.bottomLeft_.x_ + halfNodeSize;
        y = node.bottomLeft_.y_ + node.length_ + halfPatchSize;
        s32 top = searchLeaf(x, y);


        s32 leftType = getType(left, node);
        s32 rightType = getType(right, node);
        s32 bottomType = getType(bottom, node);
        s32 topType = getType(top, node);

        return meshPatterns_[node.lod_][leftType][rightType][bottomType][topType];
    }

    s32 QuadTreeMesh::getType(s32 adjacent, const QuadNode& node)
    {
        s32 type = 0;
        if(0<=adjacent && node.length_*0.999f < nodes_[adjacent].length_){
            const QuadNode& adjNode = nodes_[adjacent];
            f32 scale = adjNode.length_/node.length_ * (resolution_>>node.lod_)/(resolution_>>adjNode.lod_);
            if(3.999f<scale){
                type = 2;
            }else if(1.999f<scale){
                type = 1;
            }
        }
        return type;
    }

    u16 QuadTreeMesh::createIndex(u32 x, u32 y, u32 offsetx, u32 offsety) const
    {
        return static_cast<u16>( (y+offsety) * (resolution_ + 1) + x + offsetx );
    }

    u16 QuadTreeMesh::createIndex(u32 x, u32 y) const
    {
        return static_cast<u16>( y * (resolution_ + 1) + x );
    }

    u32 QuadTreeMesh::createInnerMesh(const Rect& rect, u16* indices)
    {
        s32 width = rect.right_ - rect.left_;
        s32 height = rect.top_ - rect.bottom_;

        bool reverse = false;
        u32 count=0;
        for(s32 i=0; i<height; ++i){
            if(reverse){
                indices[count++] = createIndex(width, i, rect.left_, rect.bottom_);
                indices[count++] = createIndex(width, i+1, rect.left_, rect.bottom_);

                for(s32 j=width-1; 0<=j; --j){
                    indices[count++] = createIndex(j, i, rect.left_, rect.bottom_);
                    indices[count++] = createIndex(j, i+1, rect.left_, rect.bottom_);
                }

            }else{

                indices[count++] = createIndex(0, i, rect.left_, rect.bottom_);
                indices[count++] = createIndex(0, i+1, rect.left_, rect.bottom_);

                for(s32 j=0; j<width; ++j){
                    indices[count++] = createIndex(j+1, i, rect.left_, rect.bottom_);
                    indices[count++] = createIndex(j+1, i+1, rect.left_, rect.bottom_);
                }
            }
            reverse = !reverse;
        }
        return count;
    }

    u32 QuadTreeMesh::createBondaryMesh(s32 leftDegree, s32 rightDegree, s32 bottomDegree, s32 topDegree, s32 resolution, u16* indices)
    {
        s32 width = resolution;
        s32 height = resolution;

        u32 count=0;

        // bottom
        if(0<bottomDegree){
            s32 step = width/bottomDegree;
            for(s32 i=0; i<width; i+=step){
                indices[count++] = createIndex(i, 0);
                indices[count++] = createIndex(i+step/2, 1);
                indices[count++] = createIndex(i+step, 0);

                for(s32 j=0; j<step/2; ++j){
                    if(0==i && 0==j && 0<leftDegree){
                        continue;
                    }
                    indices[count++] = createIndex(i, 0);
                    indices[count++] = createIndex(i+j, 1);
                    indices[count++] = createIndex(i+j+1, 1);
                }

                for(s32 j=step/2; j<step; ++j){
                    if((width-step)==i && (step-1)==j && 0<rightDegree){
                        continue;
                    }
                    indices[count++] = createIndex(i+step, 0);
                    indices[count++] = createIndex(i+j, 1);
                    indices[count++] = createIndex(i+j+1, 1);
                }
            } //for(s32 i=0;
        } //if(0<bottomDegree)

        // top
        if(0<topDegree){
            s32 step = width/topDegree;
            for(s32 i=0; i<width; i+=step){
                indices[count++] = createIndex(i, height);
                indices[count++] = createIndex(i+step/2, height-1);
                indices[count++] = createIndex(i+step, height);

                for(s32 j=0; j<step/2; ++j){
                    if(0==i && 0==j && 0<leftDegree){
                        continue;
                    }
                    indices[count++] = createIndex(i, height);
                    indices[count++] = createIndex(i+j, height-1);
                    indices[count++] = createIndex(i+j+1, height-1);
                }

                for(s32 j=step/2; j<step; ++j){
                    if((width-step)==i && (step-1)==j && 0<rightDegree){
                        continue;
                    }
                    indices[count++] = createIndex(i+step, height);
                    indices[count++] = createIndex(i+j, height-1);
                    indices[count++] = createIndex(i+j+1, height-1);
                }
            } //for(s32 i=0;
        } //if(0<topDegree)

        // left
        if(0<leftDegree){
            s32 step = height/leftDegree;
            for(s32 i=0; i<height; i+=step){
                indices[count++] = createIndex(0, i);
                indices[count++] = createIndex(1, i+step/2);
                indices[count++] = createIndex(0, i+step);

                for(s32 j=0; j<step/2; ++j){
                    if(0==i && 0==j && 0<bottomDegree){
                        continue;
                    }
                    indices[count++] = createIndex(0, i);
                    indices[count++] = createIndex(1, i+j);
                    indices[count++] = createIndex(1, i+j+1);
                }

                for(s32 j=step/2; j<step; ++j){
                    if((height-step)==i && (step-1)==j && 0<topDegree){
                        continue;
                    }
                    indices[count++] = createIndex(0, i+step);
                    indices[count++] = createIndex(1, i+j);
                    indices[count++] = createIndex(1, i+j+1);
                }
            } //for(s32 i=0;
        } //if(0<leftDegree)

        // right
        if(0<rightDegree){
            s32 step = height/rightDegree;
            for(s32 i=0; i<height; i+=step){
                indices[count++] = createIndex(width, i);
                indices[count++] = createIndex(width-1, i+step/2);
                indices[count++] = createIndex(width, i+step);

                for(s32 j=0; j<step/2; ++j){
                    if(0==i && 0==j && 0<bottomDegree){
                        continue;
                    }
                    indices[count++] = createIndex(width, i);
                    indices[count++] = createIndex(width-1, i+j);
                    indices[count++] = createIndex(width-1, i+j+1);
                }

                for(s32 j=step/2; j<step; ++j){
                    if((height-step)==i && (step-1)==j && 0<topDegree){
                        continue;
                    }
                    indices[count++] = createIndex(width, i+step);
                    indices[count++] = createIndex(width-1, i+j);
                    indices[count++] = createIndex(width-1, i+j+1);
                }
            } //for(s32 i=0;
        } //if(0<rightDegree)

        return count;
    }

    bool QuadTreeMesh::checkVisibility(QuadNode& node, const lscene::Camera& camera)
    {
        lmath::Vector4 corners[4];
        corners[0].set(node.bottomLeft_.x_, 0.0f, node.bottomLeft_.y_, 1.0f);
        corners[1].set(node.bottomLeft_.x_ + node.length_, 0.0f, node.bottomLeft_.y_, 1.0f);
        corners[2].set(node.bottomLeft_.x_ + node.length_, 0.0f, node.bottomLeft_.y_ + node.length_, 1.0f);
        corners[3].set(node.bottomLeft_.x_, 0.0f, node.bottomLeft_.y_ + node.length_, 1.0f);

        for(s32 i=0; i<4; ++i){
            corners[i].mul(camera.getViewMatrix(), corners[i]);
        }

        //左、右、上、下、近、遠
        for(s32 i=0; i<6; ++i){
            if(0.0f<frustum_.planes_[i].dot(corners[0])
                && 0.0f<frustum_.planes_[i].dot(corners[1])
                && 0.0f<frustum_.planes_[i].dot(corners[2])
                && 0.0f<frustum_.planes_[i].dot(corners[3]))
            {
                return false;
            }
        }
        return true;
    }


    f32 QuadTreeMesh::estimateGridCoverage(const QuadNode& node, const lscene::Camera& camera)
    {
        // Estimate projected area

        // Test 16 points on the quad and find out the biggest one.
        const static f32 sample_pos[16][2] =
        {
            {0, 0},
            {0, 1},
            {1, 0},
            {1, 1},
            {0.5f, 0.333f},
            {0.25f, 0.667f},
            {0.75f, 0.111f},
            {0.125f, 0.444f},
            {0.625f, 0.778f},
            {0.375f, 0.222f},
            {0.875f, 0.556f},
            {0.0625f, 0.889f},
            {0.5625f, 0.037f},
            {0.3125f, 0.37f},
            {0.8125f, 0.704f},
            {0.1875f, 0.148f},
        };

        const lmath::Matrix44& proj = camera.getProjMatrix();
        lmath::Vector3 eye(eyePosition_.x_, eyePosition_.z_, eyePosition_.y_);

        f32 gridSize = node.length_ / resolution_;
        f32 areaWorld = gridSize * gridSize;
        f32 max_area_proj = 0.0f;
        for(int i = 0; i < 16; i++){
            lmath::Vector3 vec(node.bottomLeft_.x_ + node.length_*sample_pos[i][0], node.bottomLeft_.y_ + node.length_*sample_pos[i][1], 0.0f);
            vec -= eye;

            f32 d = vec.length();

            f32 area_proj = areaWorld * proj(0, 0) * proj(1, 1) / (d * d);

            if (max_area_proj < area_proj){
                max_area_proj = area_proj;
            }
        }

        f32 pixel_coverage = max_area_proj * screenArea_ * 0.25f;

        return pixel_coverage;
    }


    s32 QuadTreeMesh::traverse(QuadNode& node, const lscene::Camera& camera)
    {
        if(!checkVisibility(node, camera)){
            return -1;
        }

        f32 half = node.length_ * 0.5f;
        f32 coverage = estimateGridCoverage(node, camera);

        bool visibleChild = true;
        if(maxGridCoverage_<coverage && patchSize_<node.length_){
            QuadNode child0( node.bottomLeft_.x_, node.bottomLeft_.y_, half, 0);
            node.children_[0] = traverse(child0, camera);

            QuadNode child1( node.bottomLeft_.x_ + half, node.bottomLeft_.y_, half, 0);
            node.children_[1] = traverse(child1, camera);

            QuadNode child2( node.bottomLeft_.x_ + half, node.bottomLeft_.y_ + half, half, 0);
            node.children_[2] = traverse(child2, camera);

            QuadNode child3( node.bottomLeft_.x_, node.bottomLeft_.y_ + half, half, 0);
            node.children_[3] = traverse(child3, camera);

            visibleChild = !node.isLeaf();
        }

        if(!visibleChild){
            return -1;
        }

        s32 lod = 0;
        for(;lod<numLODs_-1; ++lod){
            if(maxGridCoverage_<coverage){
                break;
            }
            coverage *= 4;
        }

        node.lod_ = lod;

        s32 pos = nodes_.size();
        nodes_.push_back(node);
        return pos;
    }

    s32 QuadTreeMesh::searchLeaf(f32 x, f32 y)
    {
        s32 index = -1;
        QuadNode* node = &nodes_[nodes_.size()-1];

        while(!node->isLeaf()){
            bool found = false;
            for(s32 i=0; i<4; ++i){
                index = node->children_[i];
                if(index<0){
                    continue;
                }

                QuadNode* child = &nodes_[index];
                if(child->bottomLeft_.x_<=x && x<=(child->bottomLeft_.x_+child->length_)
                    && child->bottomLeft_.y_<=y && y<=(child->bottomLeft_.y_+child->length_))
                {
                    node = child;
                    found = true;
                    break;
                }
            }
            if(!found){
                return -1;
            }
        }
        return index;
    }

    void QuadTreeMesh::attach()
    {
        inputLayout_.attach();
        vb_.attach(0, sizeof(Vertex), 0);
        ib_.attach(lgraphics::Data_R16_UInt, 0);
    }
}
