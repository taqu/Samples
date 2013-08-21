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
        @brief ファイルオープン
        @return 成否
        @param filepath
        */
        bool open(const Char* filepath);

        void close();

        /**
        @brief オブジェクトロード
        @return 成否
        @param 出力。オブジェクト
        */
        bool load(render::Object& obj);

        /**
        @brief オブジェクトロード
        @return 成否
        @param 出力。オブジェクト
        */
        bool load(render::AnimObject& obj, u32 numSkinningMatrices);

        static bool save(render::Object& obj, const Char* filepath);
    private:
        ModelLoader(const ModelLoader&);
        ModelLoader& operator=(const ModelLoader&);

        /**
        @brief ヘッダロード
        @return 成否
        @param 出力。ヘッダ
        */
        bool load(Header& header);

        /**
        @brief オブジェクトロード
        @return 成否
        @param 出力。オブジェクト
        */
        bool loadInternal(render::Object& obj);

        /**
        @brief ジオメトリロード
        @return 成否
        @param 出力。ジオメトリ
        */
        bool load(render::Geometry& geometry);

        /**
        @brief メッシュロード
        @return 成否
        @param 出力。メッシュ
        */
        bool load(render::Object& obj, render::Mesh& mesh);

        /**
        @brief マテリアルロード
        @return 成否
        @param 出力。マテリアル
        */
        bool load(render::Material& material);

        /**
        @brief ノードロード
        @return 成否
        @param 出力。ノード
        */
        bool load(render::Object& obj, render::Node& node);

        /**
        @brief テクスチャロード
        @return 成否
        @param 出力。テクスチャ
        */
        bool load(lgraphics::Texture2DRef& texture);

        /**
        @brief 頂点レイアウト作成
        @param header ... 頂点フラグ
        @param inputLayout ... 出力。頂点レイアウト
        */
        void createInputLayout(u32 vflag, lgraphics::InputLayoutRef& inputLayout);

        lcore::ifstream is_; /// ファイルストリーム

        Header header_; /// ヘッダ
    };
}
#endif //INC_RENDER_MODELLOADER_H__
