#ifndef INC_LMATH_QBVH_H__
#define INC_LMATH_QBVH_H__
/**
@file QBVH.h
@author t-sakai
@date 2013/05/09 create
*/
#include "../lmath.h"
#include <lcore/Vector.h>

namespace lcore
{
    class istream;
}

namespace lmath
{
    class Ray;
    class Sphere;

    struct HitRecord;

    class QBVHConstructor
    {
    public:
        static const f32 BoundingExpansion;// = F32_EPSILON*2.0f;

        static const u32 MinLeafFaces = 4;
        static const u32 MaxLeafFaces = 0xFFU;
        static const u32 MaxFaces = (((u32)-1) >> 9) - 1;

        struct FaceSortFunc;

        struct Face
        {
            lmath::Vector3 normal_;
            u16 v0_;
            u16 v1_;
            u16 v2_;
            u16 reserved_;
        };

        struct Vertex
        {
            lmath::Vector3 position_;
        };

        struct Node
        {
            static const u32 EmptyMask = (u32)-1;
            static const u32 LeafMask = ~(EmptyMask>>1);

            static bool isLeaf(u32 child)
            {
                return (LeafMask&child) != 0;
            }

            static bool isEmpty(u32 child)
            {
                return EmptyMask == child;
            }

            static u32 getLeaf(u32 faceIndex, u32 numFaces)
            {
                return LeafMask|((faceIndex&0xFFFFFFU)<<8)|(numFaces&0xFFU);
            }

            static u32 getFaceIndex(u32 child)
            {
                return ((~LeafMask)&child) >> 8;
            }

            static u32 getFaceNum(u32 child)
            {
                return (~LeafMask)&child&0xFFU;
            }
            f32 bbox_[2][3][4];
            u32 children_[4];
            s32 axis0_;
            s32 axis1_;
            s32 axis2_;
            s32 reserved_;
        };

        QBVHConstructor();
        ~QBVHConstructor();

        void construct(u32 numVertices, Vertex* vertices, u32 numFaces, Face* faces, const lmath::Vector3& bmin, const lmath::Vector3& bmax);
        void save(const Char* filepath);

        void setBoundingExpantion(f32 expansion){ boundingExpansion_ = expansion;}

    private:
        friend struct FaceSortFunc;
        friend class QBVH;

        u32 recursiveConstruct(u32 begin, u32 numFaces, const lmath::Vector3& bmin, const lmath::Vector3& bmax);

        void sort(u32 numFaces, Face* faces, s32 axis);
        void calcBBox(lmath::Vector3& bmin, lmath::Vector3& bmax, const Face& face);

        void getBBox(lmath::Vector3& bmin, lmath::Vector3& bmax, const Face* faces, u32 numFaces);

        f32 boundingExpansion_;
        lcore::vector_arena<Node> nodes_;
        lmath::Vector3 bmin_;
        lmath::Vector3 bmax_;

        u32 numVertices_;
        Vertex* vertices_;

        u32 numFaces_;
        Face* faces_;

    };


    class QBVH
    {
    public:
        static const u32 MinLeafFaces = 4;
        static const u32 MaxLeafFaces = 0xFFU;
        static const u32 MaxFaces = (((u32)-1) >> 9) - 1;

        static const u32 TestStackSize = 64;

        struct Face
        {
            lmath::Vector3 normal_;
            u16 v0_;
            u16 v1_;
            u16 v2_;
            u16 reserved_;
        };

        struct Vertex
        {
            lmath::Vector3 position_;
        };

        struct Node
        {
            static const u32 EmptyMask = (u32)-1;
            static const u32 LeafMask = ~(EmptyMask>>1);

            static bool isLeaf(u32 child)
            {
                return (LeafMask&child) != 0;
            }

            static bool isEmpty(u32 child)
            {
                return EmptyMask == child;
            }

            static u32 getLeaf(u32 faceIndex, u32 numFaces)
            {
                return LeafMask|((faceIndex&0xFFFFFFU)<<8)|(numFaces&0xFFU);
            }

            static u32 getFaceIndex(u32 child)
            {
                return ((~LeafMask)&child) >> 8;
            }

            static u32 getFaceNum(u32 child)
            {
                return (~LeafMask)&child&0xFFU;
            }

            lmath::lm128 bbox_[2][3];
            u32 children_[4];
            s32 axis0_;
            s32 axis1_;
            s32 axis2_;
            s32 reserved_;
        };

        struct HitRecord
        {
            f32 t_;
            const Face* face_;
        };

        QBVH();
        ~QBVH();

        void translateToLocal(lmath::Ray& ray);

        const lmath::Vector3& getPosition() const{ return position_;}
        void setPosition(f32 x, f32 y, f32 z){ position_.set(x, y, z);}

        bool load(lcore::istream& in, s32 offset);
        void copyFrom(QBVHConstructor& constructor);

        bool test(HitRecord& hitRecord, const lmath::Ray& localRay);
        bool test(HitRecord& hitRecord, const lmath::Vector3& bmin, const lmath::Vector3& bmax);
        //bool test(HitRecord& hitRecord, const lmath::Sphere& sphere);

        void getBBox(lmath::Vector3& bmin, lmath::Vector3& bmax)
        {
            bmin = bmin_;
            bmax = bmax_;

            bmin += position_;
            bmax += position_;
        }
    private:
        bool innerTest(HitRecord& hitRecord, const lmath::Ray& ray, f32 tmin, f32 tmax);
        bool innerTest(HitRecord& hitRecord, lmath::lm128 bbox[2][3]);

        lmath::Vector3 position_;
        s32 raySign_[3];
        u32 numTestFaces_;

        u32 numNodes_;
        Node* nodes_;

        lmath::Vector3 bmin_;
        lmath::Vector3 bmax_;

        u32 numVertices_;
        Vertex* vertices_;

        u32 numFaces_;
        Face* faces_;
    };
}
#endif //INC_LMATH_QBVH_H__
