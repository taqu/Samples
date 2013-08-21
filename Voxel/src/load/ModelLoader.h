#ifndef INC_RENDER_MODELLOADER_H__
#define INC_RENDER_MODELLOADER_H__
/**
@file ModelLoader.h
@author t-sakai
@date 2011/10/29 create
*/
#include "load/load.h"
#include <lcore/liostream.h>

namespace lgraphics
{
    class Texture2DRef;
    class InputLayoutRef;
}

namespace render
{
    class Object;
    class AnimObject;
    class Geometry;
    class Mesh;
    class Material;
    class Node;
}

namespace load
{
    class ModelLoader
    {
    public:
        ModelLoader()
        {}

        ~ModelLoader()
        {}

        /**
        @brief �t�@�C���I�[�v��
        @return ����
        @param filepath
        */
        bool open(const Char* filepath);

        void close();

        /**
        @brief �I�u�W�F�N�g���[�h
        @return ����
        @param �o�́B�I�u�W�F�N�g
        */
        bool load(render::Object& obj);

        /**
        @brief �I�u�W�F�N�g���[�h
        @return ����
        @param �o�́B�I�u�W�F�N�g
        */
        bool load(render::AnimObject& obj, u32 numSkinningMatrices);

        static bool save(render::Object& obj, const Char* filepath);
    private:
        ModelLoader(const ModelLoader&);
        ModelLoader& operator=(const ModelLoader&);

        /**
        @brief �w�b�_���[�h
        @return ����
        @param �o�́B�w�b�_
        */
        bool load(Header& header);

        /**
        @brief �I�u�W�F�N�g���[�h
        @return ����
        @param �o�́B�I�u�W�F�N�g
        */
        bool loadInternal(render::Object& obj);

        /**
        @brief �W�I���g�����[�h
        @return ����
        @param �o�́B�W�I���g��
        */
        bool load(render::Geometry& geometry);

        /**
        @brief ���b�V�����[�h
        @return ����
        @param �o�́B���b�V��
        */
        bool load(render::Object& obj, render::Mesh& mesh);

        /**
        @brief �}�e���A�����[�h
        @return ����
        @param �o�́B�}�e���A��
        */
        bool load(render::Material& material);

        /**
        @brief �m�[�h���[�h
        @return ����
        @param �o�́B�m�[�h
        */
        bool load(render::Object& obj, render::Node& node);

        /**
        @brief �e�N�X�`�����[�h
        @return ����
        @param �o�́B�e�N�X�`��
        */
        bool load(lgraphics::Texture2DRef& texture);

        /**
        @brief ���_���C�A�E�g�쐬
        @param header ... ���_�t���O
        @param inputLayout ... �o�́B���_���C�A�E�g
        */
        void createInputLayout(u32 vflag, lgraphics::InputLayoutRef& inputLayout);

        lcore::ifstream is_; /// �t�@�C���X�g���[��

        Header header_; /// �w�b�_
    };
}
#endif //INC_RENDER_MODELLOADER_H__
