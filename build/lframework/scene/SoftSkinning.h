#ifndef INC_LSCENE_SOFTSKINNING_H__
#define INC_LSCENE_SOFTSKINNING_H__
/**
@file SoftSkinning.h
@author t-sakai
@date 2010/07/29 create
*/
#include "../lgraphicscore.h"
#include "../api/VertexBufferRef.h"

namespace lmath
{
    class Matrix43;
}

namespace lscene
{
    class VertexDeclarationRef;

    class SoftSkinning
    {
    public:
        SoftSkinning();
        ~SoftSkinning();
#if defined(LIME_GLES1)
        void initialize(VertexDeclarationRef& decl, VertexBufferUPRef& src);
#endif

#if defined(LIME_DX9)
        void initialize(VertexDeclarationRef& decl, VertexBufferRef& src);
#endif

        void update(lmath::Matrix43* matrices, VertexBufferRef& target);
    private:
        struct UpdatePack
        {
            u8* dst_;
            const u8* src_;
            u16 offsetPosition_;
            u16 offsetNormal_;
            u16 offsetIndices_;
            u16 offsetWeights_;
            u32 num_;
            u32 stride_;
            lmath::Matrix43 *matrices_;
        };

        typedef void(*UpdateFunc)(UpdatePack pack);


        static void UpdatePosition(UpdatePack pack);
        static void UpdateNormal(UpdatePack pack);
        static void UpdatePosNormal(UpdatePack pack);
        static void UpdateDummy(UpdatePack pack);

        SoftSkinning(const SoftSkinning&);
        SoftSkinning& operator=(const SoftSkinning&);

        void clear();

        void createDecl(VertexDeclarationRef& decl);

        UpdateFunc func_;
        UpdatePack updatePack_;

        VertexBufferUPRef src_;
#if defined(LIME_GLES1)
        VertexBufferUPRef dst_;
#endif
    };
}

#endif //INC_LSCENE_SOFTSKINNING_H__
