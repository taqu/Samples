#ifndef INC_RENDER_SHADERMANAGER_H__
#define INC_RENDER_SHADERMANAGER_H__
/**
@file ShaderManager.h
@author t-sakai
@date 2011/10/17 create
*/
#include "ShaderType.h"
#include "render/render.h"

namespace render
{
    class ShaderVSBase;
    class ShaderGSBase;
    class ShaderPSBase;

    class ShaderManager
    {
    public:
        static ShaderManager& getInstance()
        {
            LASSERT(NULL != instance_);
            return *instance_;
        }

        static void initialize();
        static void terminate();

        void get(Shader id, ShaderVSBase*& vs, ShaderPSBase*& ps);
        void get(ShaderGS id, ShaderGSBase*& gs);

    private:
        ShaderManager(const ShaderManager&);
        ShaderManager& operator=(const ShaderManager&);

        struct Entry
        {
            ShaderVSBase* vs_;
            ShaderPSBase* ps_;
        };

        ShaderManager();
        ~ShaderManager();

        void initializeImpl();
        void terminateImpl();

        static ShaderManager* instance_;
        Entry entries_[Shader_Num];
        ShaderGSBase* geometryShaders_[ShaderGS_Num];
    };
}
#endif //INC_RENDER_SHADERMANAGER_H__
