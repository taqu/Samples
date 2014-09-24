/**
@file DebugDraw.cpp
@author t-sakai
@date 2012/05/17 create
*/
#include "DebugDraw.h"

#include <lcore/FixedArray.h>
#include <lmath/lmath.h>
#include <lgraphics/lgraphics.h>

#include "System.h"

#include "render/Shader.h"
#include "render/ShaderManager.h"
#include "Object.h"

namespace render
{
namespace
{
    enum SubSide
    {
        SubSide_PlusX,
        SubSide_MinusX,
        SubSide_PlusY,
        SubSide_MinusY,
        SubSide_PlusZ,
        SubSide_MinusZ,
    };

    void subdivide(DebugDraw::VertexPN* dst, s32 voffset, f32 radius, s32 resolution, SubSide side)
    {
        lmath::Vector3 center;
        s32 axis0, axis1;
        switch(side)
        {
        case SubSide_PlusX:
            center.set(1.0f, 0.0f, 0.0f);
            axis0 = 2;
            axis1 = 1;
            break;
        case SubSide_MinusX:
            center.set(-1.0f, 0.0f, 0.0f);
            axis0 = 1;
            axis1 = 2;
            break;
        case SubSide_PlusY:
            center.set(0.0f, 1.0f, 0.0f);
            axis0 = 0;
            axis1 = 2;
            break;
        case SubSide_MinusY:
            center.set(0.0f, -1.0f, 0.0f);
            axis0 = 2;
            axis1 = 0;
            break;
        case SubSide_PlusZ:
            center.set(0.0f, 0.0f, 1.0f);
            axis0 = 1;
            axis1 = 0;
            break;
        case SubSide_MinusZ:
            center.set(0.0f, 0.0f, -1.0f);
            axis0 = 0;
            axis1 = 1;
            break;
        default:
            LASSERT(false);
            break;
        };

        const f32 size = 2.0f;
        const f32 half = 0.5f*size;

        f32 unit = size/resolution;
        lmath::Vector3 v;
        for(s32 i=0; i<=resolution; ++i){
            for(s32 j=0; j<=resolution; ++j){
                f32 v0 = unit*j - half;
                f32 v1 = half - unit*i;
                v = center;
                v[axis0] = v0;
                v[axis1] = v1;
                lmath::cubeToSphere(v);

                s32 index = (resolution+1)*i + j + voffset;

                v.normalize();
                dst[index].nx_ = lcore::toBinary16Float(v.x_);
                dst[index].ny_ = lcore::toBinary16Float(v.y_);
                dst[index].nz_ = lcore::toBinary16Float(v.z_);

                v *= radius;
                dst[index].x_ = v.x_;
                dst[index].y_ = v.y_;
                dst[index].z_ = v.z_;
            }
        }
    }

    void setTriangleStripIndices(u16* dst, s32 resolution, s32 offset)
    {
        u16 w = static_cast<u16>(resolution+1);
        u16 h = static_cast<u16>(resolution);
        s32 count = 0;
        for(u16 i=0; i<h; ++i){
            if(0<i){
                u16 index = w*i + w + offset;
                dst[count + 0] = dst[count - 1];
                dst[count + 1] = index;
                count += 2;
            }

            for(u16 j=0; j<w; ++j){
                u16 index = w*i + j + offset;
                dst[count++] = index + w;
                dst[count++] = index;
            }
        }
    }
}

    Object* DebugDraw::createPlane(f32 size, s32 resolution, u32 argb, f32 luminosity)
    {
        LASSERT(0.0f<=size);
        LASSERT(1<=resolution);

        f32 half = size * 0.5f;
        f32 unit = size/resolution;
        u32 numVertices = (resolution+1)*(resolution+1);
        u32 numIndices = 2*resolution*(resolution+1) + 2*(resolution-1);

        lcore::FixedArray<VertexPN> vertices(numVertices);
        for(s32 i=0; i<=resolution; ++i){
            for(s32 j=0; j<=resolution; ++j){
                f32 x = unit*j - half;
                f32 z = half - unit*i;
                f32 y = 0.0f;
                u16 nx = lcore::toBinary16Float(0.0f);
                u16 ny = lcore::toBinary16Float(1.0f);
                u16 nz = lcore::toBinary16Float(0.0f);
                s32 index = i*(resolution+1) + j;
                vertices[index].x_ = x;
                vertices[index].y_ = y;
                vertices[index].z_ = z;
                vertices[index].nx_ = nx;
                vertices[index].ny_ = ny;
                vertices[index].nz_ = nz;
            }
        }

        lgraphics::VertexBufferRef vb = lgraphics::VertexBuffer::create(
            numVertices*sizeof(VertexPN),
            lgraphics::Usage_Default,
            lgraphics::BindFlag_VertexBuffer,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            sizeof(VertexPN),
            vertices.begin());

        lcore::FixedArray<u16> indices(numIndices);
        setTriangleStripIndices(indices.begin(), resolution, 0);

        lgraphics::IndexBufferRef ib = lgraphics::IndexBuffer::create(
            sizeof(u16)*numIndices,
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            sizeof(u16),
            indices.begin());

        lmath::Sphere bsphere(0.0f, 0.0f, 0.0f, lmath::sqrt(size*size+size*size));

        return createObject(lgraphics::Primitive_TriangleStrip, numVertices, vb, numIndices, ib, bsphere, argb, luminosity);
    }

    Object* DebugDraw::createBox(f32 size, u32 argb, f32 luminosity)
    {
        LASSERT(0.0f<=size);

        f32 half = size * 0.5f;
        f32 unit = size;
        u32 numVertices = 24;
        u32 numIndices = 6*6;

        lcore::FixedArray<VertexPN> vertices(numVertices);
        u16 nx, ny, nz;
        s32 start;

        //-Z
        start = 0;
        nx = lcore::toBinary16Float(0.0f); ny = lcore::toBinary16Float(0.0f); nz = lcore::toBinary16Float(-1.0f);
        vertices[start + 0].x_ = -half; vertices[start + 0].y_ = -half; vertices[start + 0].z_ = -half;
        vertices[start + 1].x_ =  half; vertices[start + 1].y_ = -half; vertices[start + 1].z_ = -half;
        vertices[start + 2].x_ = -half; vertices[start + 2].y_ =  half; vertices[start + 2].z_ = -half;
        vertices[start + 3].x_ =  half; vertices[start + 3].y_ =  half; vertices[start + 3].z_ = -half;
        vertices[start + 0].nx_ = nx; vertices[start + 0].ny_ = ny; vertices[start + 0].nz_ = nz;
        vertices[start + 1].nx_ = nx; vertices[start + 1].ny_ = ny; vertices[start + 1].nz_ = nz;
        vertices[start + 2].nx_ = nx; vertices[start + 2].ny_ = ny; vertices[start + 2].nz_ = nz;
        vertices[start + 3].nx_ = nx; vertices[start + 3].ny_ = ny; vertices[start + 3].nz_ = nz;

        //-Y
        start = 4;
        nx = lcore::toBinary16Float(0.0f); ny = lcore::toBinary16Float(-1.0f); nz = lcore::toBinary16Float(0.0f);
        vertices[start + 0].x_ = -half; vertices[start + 0].y_ = -half; vertices[start + 0].z_ = -half;
        vertices[start + 1].x_ = -half; vertices[start + 1].y_ = -half; vertices[start + 1].z_ =  half;
        vertices[start + 2].x_ =  half; vertices[start + 2].y_ = -half; vertices[start + 2].z_ = -half;
        vertices[start + 3].x_ =  half; vertices[start + 3].y_ = -half; vertices[start + 3].z_ =  half;
        vertices[start + 0].nx_ = nx; vertices[start + 0].ny_ = ny; vertices[start + 0].nz_ = nz;
        vertices[start + 1].nx_ = nx; vertices[start + 1].ny_ = ny; vertices[start + 1].nz_ = nz;
        vertices[start + 2].nx_ = nx; vertices[start + 2].ny_ = ny; vertices[start + 2].nz_ = nz;
        vertices[start + 3].nx_ = nx; vertices[start + 3].ny_ = ny; vertices[start + 3].nz_ = nz;

        //-X
        start = 8;
        nx = lcore::toBinary16Float(-1.0f); ny = lcore::toBinary16Float(0.0f); nz = lcore::toBinary16Float(0.0f);
        vertices[start + 0].x_ = -half; vertices[start + 0].y_ = -half; vertices[start + 0].z_ = -half;
        vertices[start + 1].x_ = -half; vertices[start + 1].y_ =  half; vertices[start + 1].z_ = -half;
        vertices[start + 2].x_ = -half; vertices[start + 2].y_ = -half; vertices[start + 2].z_ =  half;
        vertices[start + 3].x_ = -half; vertices[start + 3].y_ =  half; vertices[start + 3].z_ =  half;
        vertices[start + 0].nx_ = nx; vertices[start + 0].ny_ = ny; vertices[start + 0].nz_ = nz;
        vertices[start + 1].nx_ = nx; vertices[start + 1].ny_ = ny; vertices[start + 1].nz_ = nz;
        vertices[start + 2].nx_ = nx; vertices[start + 2].ny_ = ny; vertices[start + 2].nz_ = nz;
        vertices[start + 3].nx_ = nx; vertices[start + 3].ny_ = ny; vertices[start + 3].nz_ = nz;

        //+Z
        start = 12;
        nx = lcore::toBinary16Float(0.0f); ny = lcore::toBinary16Float(0.0f); nz = lcore::toBinary16Float(1.0f);
        vertices[start + 0].x_ = -half; vertices[start + 0].y_ = -half; vertices[start + 0].z_ =  half;
        vertices[start + 1].x_ = -half; vertices[start + 1].y_ =  half; vertices[start + 1].z_ =  half;
        vertices[start + 2].x_ =  half; vertices[start + 2].y_ = -half; vertices[start + 2].z_ =  half;
        vertices[start + 3].x_ =  half; vertices[start + 3].y_ =  half; vertices[start + 3].z_ =  half;
        vertices[start + 0].nx_ = nx; vertices[start + 0].ny_ = ny; vertices[start + 0].nz_ = nz;
        vertices[start + 1].nx_ = nx; vertices[start + 1].ny_ = ny; vertices[start + 1].nz_ = nz;
        vertices[start + 2].nx_ = nx; vertices[start + 2].ny_ = ny; vertices[start + 2].nz_ = nz;
        vertices[start + 3].nx_ = nx; vertices[start + 3].ny_ = ny; vertices[start + 3].nz_ = nz;

        //+Y
        start = 16;
        nx = lcore::toBinary16Float(0.0f); ny = lcore::toBinary16Float(1.0f); nz = lcore::toBinary16Float(0.0f);
        vertices[start + 0].x_ = -half; vertices[start + 0].y_ = half; vertices[start + 0].z_ = -half;
        vertices[start + 1].x_ =  half; vertices[start + 1].y_ = half; vertices[start + 1].z_ = -half;
        vertices[start + 2].x_ = -half; vertices[start + 2].y_ = half; vertices[start + 2].z_ =  half;
        vertices[start + 3].x_ =  half; vertices[start + 3].y_ = half; vertices[start + 3].z_ =  half;
        vertices[start + 0].nx_ = nx; vertices[start + 0].ny_ = ny; vertices[start + 0].nz_ = nz;
        vertices[start + 1].nx_ = nx; vertices[start + 1].ny_ = ny; vertices[start + 1].nz_ = nz;
        vertices[start + 2].nx_ = nx; vertices[start + 2].ny_ = ny; vertices[start + 2].nz_ = nz;
        vertices[start + 3].nx_ = nx; vertices[start + 3].ny_ = ny; vertices[start + 3].nz_ = nz;

        //+X
        start = 20;
        nx = lcore::toBinary16Float(1.0f); ny = lcore::toBinary16Float(0.0f); nz = lcore::toBinary16Float(0.0f);
        vertices[start + 0].x_ = half; vertices[start + 0].y_ = -half; vertices[start + 0].z_ = -half;
        vertices[start + 1].x_ = half; vertices[start + 1].y_ = -half; vertices[start + 1].z_ =  half;
        vertices[start + 2].x_ = half; vertices[start + 2].y_ =  half; vertices[start + 2].z_ = -half;
        vertices[start + 3].x_ = half; vertices[start + 3].y_ =  half; vertices[start + 3].z_ =  half;
        vertices[start + 0].nx_ = nx; vertices[start + 0].ny_ = ny; vertices[start + 0].nz_ = nz;
        vertices[start + 1].nx_ = nx; vertices[start + 1].ny_ = ny; vertices[start + 1].nz_ = nz;
        vertices[start + 2].nx_ = nx; vertices[start + 2].ny_ = ny; vertices[start + 2].nz_ = nz;
        vertices[start + 3].nx_ = nx; vertices[start + 3].ny_ = ny; vertices[start + 3].nz_ = nz;

        lgraphics::VertexBufferRef vb = lgraphics::VertexBuffer::create(
            numVertices*sizeof(VertexPN),
            lgraphics::Usage_Default,
            lgraphics::BindFlag_VertexBuffer,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            sizeof(VertexPN),
            vertices.begin());


        lcore::FixedArray<u16> indices(numIndices);
        u16 vstart;
        for(s32 i=0; i<6; ++i){
            start = i*6;
            vstart = i*4;
            indices[start + 0] = vstart + 1;
            indices[start + 1] = vstart + 0;
            indices[start + 2] = vstart + 2;
            indices[start + 3] = vstart + 1;
            indices[start + 4] = vstart + 2;
            indices[start + 5] = vstart + 3;
        }
        lgraphics::IndexBufferRef ib = lgraphics::IndexBuffer::create(
            sizeof(u16)*numIndices,
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            sizeof(u16),
            indices.begin());

        lmath::Sphere bsphere(0.0f, 0.0f, 0.0f, lmath::sqrt(size*size+size*size+size*size));

        return createObject(lgraphics::Primitive_TriangleList, numVertices, vb, numIndices, ib, bsphere, argb, luminosity);
    }

    Object* DebugDraw::createSphere(f32 radius, s32 resolution, u32 argb, f32 luminosity)
    {
        u32 onePlaneVertices = (resolution+1)*(resolution+1);
        u32 numVertices = onePlaneVertices*6;
        u32 onePlaneIndices = 2*resolution*(resolution+1) + 2*(resolution-1);
        u32 numIndices = onePlaneIndices*6 + 10;

        lcore::FixedArray<VertexPN> vertices(numVertices);
        lcore::FixedArray<u16> indices(numIndices);

        s32 vcount = 0;
        s32 icount = 0;

        subdivide(vertices.begin(), vcount, radius, resolution, SubSide_MinusZ);
        setTriangleStripIndices(indices.begin()+icount, resolution, vcount);
        vcount += onePlaneVertices;
        icount += onePlaneIndices;

        indices[icount] = indices[icount-1];
        icount+=2;
        subdivide(vertices.begin(), vcount, radius, resolution, SubSide_PlusZ);
        setTriangleStripIndices(indices.begin()+icount, resolution, vcount);
        indices[icount-1] = indices[icount];
        vcount += onePlaneVertices;
        icount += onePlaneIndices;

        indices[icount] = indices[icount-1];
        icount+=2;
        subdivide(vertices.begin(), vcount, radius, resolution, SubSide_MinusX);
        setTriangleStripIndices(indices.begin()+icount, resolution, vcount);
        indices[icount-1] = indices[icount];
        vcount += onePlaneVertices;
        icount += onePlaneIndices;

        indices[icount] = indices[icount-1];
        icount+=2;
        subdivide(vertices.begin(), vcount, radius, resolution, SubSide_PlusX);
        setTriangleStripIndices(indices.begin()+icount, resolution, vcount);
        indices[icount-1] = indices[icount];
        vcount += onePlaneVertices;
        icount += onePlaneIndices;

        indices[icount] = indices[icount-1];
        icount+=2;
        subdivide(vertices.begin(), vcount, radius, resolution, SubSide_MinusY);
        setTriangleStripIndices(indices.begin()+icount, resolution, vcount);
        indices[icount-1] = indices[icount];
        vcount += onePlaneVertices;
        icount += onePlaneIndices;

        indices[icount] = indices[icount-1];
        icount+=2;
        subdivide(vertices.begin(), vcount, radius, resolution, SubSide_PlusY);
        setTriangleStripIndices(indices.begin()+icount, resolution, vcount);
        indices[icount-1] = indices[icount];
        vcount += onePlaneVertices;
        icount += onePlaneIndices;

        lgraphics::VertexBufferRef vb = lgraphics::VertexBuffer::create(
            sizeof(VertexPN)*numVertices,
            lgraphics::Usage_Default,
            lgraphics::BindFlag_VertexBuffer,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            sizeof(VertexPN),
            vertices.begin());

        lgraphics::IndexBufferRef ib = lgraphics::IndexBuffer::create(
            sizeof(u16)*numIndices,
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            sizeof(u16),
            indices.begin());

        lmath::Sphere bsphere(0.0f, 0.0f, 0.0f, radius);

        return createObject(lgraphics::Primitive_TriangleStrip, numVertices, vb, numIndices, ib, bsphere, argb, luminosity);
    }

    Object* DebugDraw::createObject(
        lgraphics::Primitive type,
        u32 numVertices,
        lgraphics::VertexBufferRef& vb,
        u32 numIndices,
        lgraphics::IndexBufferRef& ib,
        const lmath::Sphere& bsphere,
        u32 argb,
        f32 luminosity)
    {
        Object* object = LIME_NEW Object();
        object->create(
            1,
            1,
            1,
            1,
            0);

        lgraphics::InputLayoutRef inputLayout = fractal::System::getInputLayoutFactory().get(render::Layout_PN);
        Geometry& geometry = object->getGeometry(0);
        geometry.create(
            load::VElem_Position|load::VElem_Normal,
            sizeof(VertexPN),
            numVertices,
            NULL,
            numIndices,
            NULL,
            inputLayout,
            vb,
            ib);

        Mesh& mesh = object->getMesh(0);
        mesh.create(
            type,
            0,
            numIndices,
            &(object->getGeometry(0)),
            &(object->getMaterial(0)),
            bsphere);

        Material& material = object->getMaterial(0);
        material.flags_ = Material::Flag_CastShadow | Material::Flag_RecieveShadow;
        material.diffuse_.set(
            (1.0f/255.0f)*lcore::getRFromARGB(argb)*luminosity,
            (1.0f/255.0f)*lcore::getGFromARGB(argb)*luminosity,
            (1.0f/255.0f)*lcore::getBFromARGB(argb)*luminosity,
            (1.0f/255.0f)*lcore::getAFromARGB(argb));
        material.specular_.set(1.0f, 1.0f, 1.0f, 18.0f);
        f32 refractive = lcore::getRefractiveIndex(lcore::RefractiveIndex_Silicon);
        material.shadow_.set(0.0f, 0.0f, 0.0f, refractive);

        Node& node = object->getNode(0);
        node.numMeshes_ = 1;
        node.meshStartIndex_ = 0;
        node.meshes_ = &mesh;
        node.translation_.zero();
        node.rotation_.identity();
        node.scale_.one();

        return object;
    }

    DebugDraw::DebugDraw()
        :vs_(NULL)
        ,ps_(NULL)
        ,maxLines_(0)
        ,numLines_(0)
        ,lines_(NULL)
    {
    }

    DebugDraw::~DebugDraw()
    {
    }


    void DebugDraw::initialize(u32 maxLines)
    {
        terminate();

        ShaderVSBase* vs = NULL;
        ShaderPSBase* ps = NULL;
        ShaderManager::getInstance().get(render::Shader_DebugVC, vs, ps);
        vs_ = reinterpret_cast<DebugVCVS*>(vs);
        ps_ = reinterpret_cast<DebugVCPS*>(ps);

        inputLayout_ = fractal::System::getInputLayoutFactory().get(render::Layout_PC);

        lgraphics::VertexBufferRef vb = lgraphics::VertexBuffer::create(
            maxLines * 2 * sizeof(Vertex),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            0,
            NULL);

        vbLines_ = vb;

        lines_ = reinterpret_cast<Vertex*>(LIME_MALLOC(sizeof(Vertex)*maxLines*2));
        maxLines_ = maxLines;
    }

    void DebugDraw::terminate()
    {
        LIME_FREE(lines_);
        numLines_ = 0;

        {
            lgraphics::InputLayoutRef inputLayout;
            inputLayout.swap(inputLayout_);
        }

        {
            lgraphics::VertexBufferRef vbLines;
            vbLines.swap(vbLines_);
        }

        ps_ = NULL;
        vs_ = NULL;
    }

    void DebugDraw::add(const lmath::Vector3& v0, const lmath::Vector3& v1, u32 color)
    {
        if(maxLines_<=numLines_){
            return;
        }

        u32 index = 2*numLines_;
        lines_[index].x_ = v0.x_;
        lines_[index].y_ = v0.y_;
        lines_[index].z_ = v0.z_;
        lines_[index].color_ = color;

        ++index;
        lines_[index].x_ = v1.x_;
        lines_[index].y_ = v1.y_;
        lines_[index].z_ = v1.z_;
        lines_[index].color_ = color;

        ++numLines_;
    }

    void DebugDraw::add(const lmath::Vector4& v0, const lmath::Vector4& v1, u32 color)
    {
        if(maxLines_<=numLines_){
            return;
        }

        u32 index = 2*numLines_;
        lines_[index].x_ = v0.x_;
        lines_[index].y_ = v0.y_;
        lines_[index].z_ = v0.z_;
        lines_[index].color_ = color;

        ++index;
        lines_[index].x_ = v1.x_;
        lines_[index].y_ = v1.y_;
        lines_[index].z_ = v1.z_;
        lines_[index].color_ = color;

        ++numLines_;
    }

    void DebugDraw::draw(lgraphics::GraphicsDeviceRef& device)
    {
        vbLines_.updateSubresource(
            0,
            NULL,
            reinterpret_cast<const void*>(lines_),
            0,
            0);

        inputLayout_.attach();
        vs_->attach();
        ps_->attach();

        device.setPrimitiveTopology(lgraphics::Primitive_LineList);
        vbLines_.attach(0, sizeof(Vertex), 0);
        device.draw(numLines_*2, 0);

        numLines_ = 0;
    }
}
