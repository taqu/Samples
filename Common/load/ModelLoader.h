#ifndef INC_RENDER_MODELLOADER_H__
#define INC_RENDER_MODELLOADER_H__
/**
@file ModelLoader.h
@author t-sakai
@date 2011/10/29 create
*/
#include "load/load.h"
#include <lcore/liostream.h>
#include <lframework/anim/Skeleton.h>

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
    class Texture;

    class ModelLoader
    {
    public:
        ModelLoader()
            :directoryPathLength_(0)
            ,textureAddress_(lgraphics::TexAddress_Clamp)
            ,convertRefractiveIndex_(true)
        {
            directoryPath_[0] = '\0';
        }

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
        bool load(render::AnimObject& obj);

        lanim::Skeleton::pointer getSkeleton(){ return skeleton_;}

        void getTextureNameTable(u32 numTextures, load::Texture* textures);

        void setTextureAddress(lgraphics::TextureAddress texAddress)
        {
            textureAddress_ = texAddress;
        }

        void setConvertRefractiveIndex(bool enable)
        {
            convertRefractiveIndex_ = enable;
        }

        static bool save(render::Object& obj, load::Texture* textures, lanim::Skeleton::pointer& skeleton, const Char* filepath);
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
        @brief �X�P���g�����[�h
        */
        lanim::Skeleton* loadSkeleton(u32 numJoints);

        /**
        @brief �e�N�X�`�����[�h
        @return ����
        @param �o�́B�e�N�X�`��
        */
        bool load(lgraphics::Texture2DRef& texture, const load::Texture& loadTexture);

        /**
        @brief ���_���C�A�E�g�쐬
        @param header ... ���_�t���O
        @param inputLayout ... �o�́B���_���C�A�E�g
        */
        void createInputLayout(u32 vflag, lgraphics::InputLayoutRef& inputLayout);

        lcore::ifstream is_; /// �t�@�C���X�g���[��

        Header header_; /// �w�b�_
        u32 directoryPathLength_;
        Char directoryPath_[MaxPathSize+MaxFileNameSize];

        lanim::Skeleton::pointer skeleton_;

        lgraphics::TextureAddress textureAddress_;

        bool convertRefractiveIndex_;
    };
}
#endif //INC_RENDER_MODELLOADER_H__
