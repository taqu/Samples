#include "stdafx.h"

#include <lcore/liostream.h>
#include <lgraphics/io11/IODDS.h>
#include <lgraphics/api/TextureRef.h>

#include "System.h"

namespace fractal
{
    void* allocate(u32 size)
    {
        return System::getAllocator().allocate(size);
    }

    void deallocate(void*& ptr, u32 size)
    {
        System::getAllocator().deallocate(ptr, size);
        ptr = NULL;
    }

    f32 getLayerY(Layer layer)
    {
        return layer * 1.0f;
    }

    bool loadDDS(lgraphics::Texture2DRef& texture, lcore::ifstream& is, s32 offset)
    {
        fractal::HeapAllocator& allocator = fractal::System::getAllocator();
        lcore::s8* data = reinterpret_cast<lcore::s8*>( allocator.getTemporaryBuffer() );

        u32 size = is.getSize(offset);
        is.read(data, size);

        lgraphics::io::IODDS::read(data, size, texture);

        allocator.releaseTemporaryBuffer(data);
        return true;
    }
}
