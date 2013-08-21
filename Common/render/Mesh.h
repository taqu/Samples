#ifndef INC_RENDER_MESH_H__
#define INC_RENDER_MESH_H__
/**
@file Mesh.h
@author t-sakai
@date 2011/10/16 create
*/
#include <lmath/geometry/Sphere.h>
#include <lgraphics/api/Enumerations.h>

#include "render.h"

namespace render
{
    class Geometry;
    class Material;

    class Mesh
    {
    public:
        Mesh()
            :type_(lgraphics::Primitive_TriangleList)
            ,numPrimitives_(0)
            ,numIndices_(0)
            ,geometry_(NULL)
            ,material_(NULL)
        {}

        ~Mesh()
        {}

        inline void create(
            lgraphics::Primitive type,
            u32 indexOffset,
            u32 numIndices,
            Geometry* geometry,
            Material* material,
            const lmath::Sphere& sphere);

        inline lgraphics::Primitive getType() const;
        inline u32 getNumPrimitives() const;
        inline u32 getIndexOffset() const;
        inline u32 getNumIndices() const;

        inline Geometry* getGeometry();
        inline Material* getMaterial();

        inline void setSphere(const lmath::Sphere& s);
        inline const lmath::Sphere& getSphere() const;

        /// スワップ
        inline void swap(Mesh& rhs);
    private:
        Mesh(const Mesh&);
        Mesh& operator=(const Mesh&);

        u32 calcNumPrimitives(lgraphics::Primitive type, u32 numIndices);

        lgraphics::Primitive type_;
        u32 numPrimitives_;
        u32 indexOffset_;
        u32 numIndices_;

        Geometry* geometry_;
        Material* material_;

        lmath::Sphere sphere_;
    };

    inline void Mesh::create(
        lgraphics::Primitive type,
        u32 indexOffset,
        u32 numIndices,
        Geometry* geometry,
        Material* material,
        const lmath::Sphere& sphere)
    {
        LASSERT(NULL != geometry);
        LASSERT(NULL != material);

        //TODO:プリミティブタイプごとに処理
        type_ = type;
        numPrimitives_ = calcNumPrimitives(type, numIndices);
        indexOffset_ = indexOffset;
        numIndices_ = numIndices;
        geometry_ = geometry;
        material_ = material;
        sphere_ = sphere;
    }

    inline lgraphics::Primitive Mesh::getType() const
    {
        return type_;
    }

    inline u32 Mesh::getNumPrimitives() const
    {
        return numPrimitives_;
    }

    inline u32 Mesh::getIndexOffset() const
    {
        return indexOffset_;
    }

    inline u32 Mesh::getNumIndices() const
    {
        return numIndices_;
    }

    inline Geometry* Mesh::getGeometry()
    {
        return geometry_;
    }

    inline Material* Mesh::getMaterial()
    {
        return material_;
    }

    inline void Mesh::setSphere(const lmath::Sphere& s)
    {
        sphere_ = s;
    }

    inline const lmath::Sphere& Mesh::getSphere() const
    {
        return sphere_;
    }

    // スワップ
    inline void Mesh::swap(Mesh& rhs)
    {
        lcore::swap(type_, rhs.type_);
        lcore::swap(numPrimitives_, rhs.numPrimitives_);
        lcore::swap(indexOffset_, rhs.indexOffset_);
        lcore::swap(numIndices_, rhs.numIndices_);

        lcore::swap(geometry_, rhs.geometry_);
        lcore::swap(material_, rhs.material_);

        sphere_.swap(rhs.sphere_);
    }
}
#endif //INC_RENDER_MESH_H__
