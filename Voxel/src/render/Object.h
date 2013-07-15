#ifndef INC_RENDER_OBJECT_H__
#define INC_RENDER_OBJECT_H__
/**
@file Object.h
@author t-sakai
@date 2012/01/03 create
*/
#include <lcore/Buffer.h>

#include <lmath/Vector4.h>
#include <lmath/Quaternion.h>
#include <lmath/geometry/Sphere.h>
#include <lgraphics/api/TextureRef.h>

#include "render.h"

#include "Geometry.h"
#include "Mesh.h"
#include "Material.h"
#include "Node.h"

namespace render
{
    class Object
    {
    public:
        Object();
        ~Object();

        /**
        @brief �o�b�t�@�쐬
        @return ����
        @param numMeshes ... ���b�V����
        @param numMaterials ... �}�e���A����
        */
        bool create(
            u32 numGeometries,
            u32 numMeshes,
            u32 numMaterials,
            u32 numNodes,
            u32 numTextures);

        bool clone(Object& dst);

        inline const lmath::Vector4& position() const;
        inline const lmath::Quaternion& rotation() const;

        inline lmath::Vector4& position();
        inline lmath::Quaternion& rotation();

        inline void setPosition(lmath::Vector4& position);
        inline void setRotation(lmath::Quaternion& rotation);

        /// ���E���Z�b�g
        inline void setSphere(const lmath::Sphere& s);

        /// ���E���Q�b�g
        inline const lmath::Sphere& getSphere() const;

        /// �W�I���g����
        inline u32 getNumGeometries() const;

        /**
        @brief �W�I���g��
        */
        inline Geometry& getGeometry(u32 index);

        /// ���b�V����
        inline u32 getNumMeshes() const;

        /**
        @brief ���b�V��
        */
        inline Mesh& getMesh(u32 index);

        /// �}�e���A����
        inline u32 getNumMaterials() const;

        /**
        @brief �}�e���A��
        */
        inline Material& getMaterial(u32 index);

        /// �m�[�h��
        inline u32 getNumNodes() const;

        /// �m�[�h
        inline Node& getNode(u32 index);

        /// �e�N�X�`����
        inline u32 getNumTextures() const;

        /// �e�N�X�`��
        inline lgraphics::Texture2DRef& getTexture(u32 index);

        /// �X���b�v
        void swap(Object& rhs);
    private:
        Object(const Object&);
        Object& operator=(const Object&);

        template<class T>
        T* placementArrayConstruct(void* ptr, u32 num)
        {
            T* t = reinterpret_cast<T*>(ptr);
            for(u32 i=0; i<num; ++i){
                LIME_PLACEMENT_NEW(&t[i]) T();
            }
            return t;
        }

        void release();

        u32 calcBufferSize() const;

        lmath::Vector4 position_;
        lmath::Quaternion rotation_;
        lmath::Sphere sphere_;

        u32 numGeometries_;
        Geometry* geometries_;
        u32 numMeshes_; /// ���b�V����
        Mesh* meshes_; /// ���b�V���z��
        u32 numMaterials_; /// �}�e���A����
        Material* materials_; /// �}�e���A���z��
        u32 numNodes_;
        Node* nodes_;
        u32 numTextures_;
        lgraphics::Texture2DRef* textures_;
    };

    //--------------------------------------------------
    inline const lmath::Vector4& Object::position() const
    {
        return position_;
    }

    //--------------------------------------------------
    inline const lmath::Quaternion& Object::rotation() const
    {
        return rotation_;
    }

    //--------------------------------------------------
    inline lmath::Vector4& Object::position()
    {
        return position_;
    }

    //--------------------------------------------------
    inline lmath::Quaternion& Object::rotation()
    {
        return rotation_;
    }

    //--------------------------------------------------
    inline void Object::setPosition(lmath::Vector4& position)
    {
        position_ = position;
    }

    //--------------------------------------------------
    inline void Object::setRotation(lmath::Quaternion& rotation)
    {
        rotation_ = rotation;
    }

    //--------------------------------------------------
    inline void Object::setSphere(const lmath::Sphere& s)
    {
        sphere_ = s;
    }

    //--------------------------------------------------
    inline const lmath::Sphere& Object::getSphere() const
    {
        return sphere_;
    }

    //--------------------------------------------------
    /// �W�I���g����
    inline u32 Object::getNumGeometries() const
    {
        return numGeometries_;
    }

    //--------------------------------------------------
    /**
    @brief �W�I���g��
    */
    inline Geometry& Object::getGeometry(u32 index)
    {
        LASSERT(index<numGeometries_);
        return geometries_[index];
    }

    //--------------------------------------------------
    // ���b�V����
    inline u32 Object::getNumMeshes() const
    {
        return numMeshes_;
    }

    //--------------------------------------------------
    // ���b�V��
    inline Mesh& Object::getMesh(u32 index)
    {
        LASSERT(index<numMeshes_);
        return meshes_[index];
    }

    //--------------------------------------------------
    // �}�e���A����
    inline u32 Object::getNumMaterials() const
    {
        return numMaterials_;
    }

    //--------------------------------------------------
    // �}�e���A��
    inline Material& Object::getMaterial(u32 index)
    {
        LASSERT(index<numMaterials_);
        return materials_[index];
    }

    //--------------------------------------------------
    // �m�[�h��
    inline u32 Object::getNumNodes() const
    {
        return numNodes_;
    }

    //--------------------------------------------------
    // �m�[�h
    inline Node& Object::getNode(u32 index)
    {
        LASSERT(index<numNodes_);
        return nodes_[index];
    }

    //--------------------------------------------------
    // �e�N�X�`����
    inline u32 Object::getNumTextures() const
    {
        return numTextures_;
    }

    //--------------------------------------------------
    // �e�N�X�`��
    inline lgraphics::Texture2DRef& Object::getTexture(u32 index)
    {
        LASSERT(index<numTextures_);
        return textures_[index];
    }
}
#endif //INC_RENDER_OBJECT_H__
