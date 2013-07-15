#ifndef INC_LGRAPHICS_DX11_DISPLAY_H__
#define INC_LGRAPHICS_DX11_DISPLAY_H__
/**
@file Display.h
@author t-sakai
@date 2013/05/16 create
*/
#include "Enumerations.h"

struct IDXGIFactory1;
struct IDXGIAdapter1;
struct IDXGIOutput;

namespace lgraphics
{
    struct InitParam;

    class Display
    {
    public:
        static const u32 NumMaxAdapters = 4;
        static const u32 NumMaxOutputs = 4;

        Display();
        ~Display();

        bool create();
        void destroy();

        u32 getAdapterCount();

        bool getNearestConfig(InitParam& dst, const InitParam& src);

        //u32 getAdapterModeCount(u32 adapter, BufferFormat format);
        //bool enumAdapterModes(u32 adapter, BufferFormat format, u32 mode, DisplayMode* modes);
    private:
        bool setAdapterOutput();

        IDXGIFactory1* factory_;
        IDXGIAdapter1* adapter_;
        IDXGIOutput* output_;
    };
}

#endif //INC_LGRAPHICS_DX11_DISPLAY_H__
