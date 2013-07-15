#ifndef INC_LGRAPHICS_H__
#define INC_LGRAPHICS_H__
/**
@file lgraphics.h
@author t-sakai
@date 2009/01/18 create
*/
#include "lgraphicscore.h"
#include "api/GraphicsDeviceRef.h"

namespace lgraphics
{
    struct InitParam;

    class Graphics
    {
        Graphics()
        {}

        ~Graphics()
        {}
    public:
        
        static bool initialize(const InitParam& initParam);

        static void terminate();

        static GraphicsDeviceRef& getDevice()
        {
            return device_;
        }
    private:
        static GraphicsDeviceRef device_;
    };
}

#endif //INC_LGRAPHICS_H__
