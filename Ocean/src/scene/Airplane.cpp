/**
@file Airplane.cpp
@author t-sakai
@date 2013/07/16 create
*/
#include "Airplane.h"
#include "System.h"

namespace scene
{
namespace
{
    static const f32 AirDensity = 0.0023769f;
    static const f32 Gravity = -32.174f;

    static const f32 Thrust = 10.0f;
    static const f32 MinThrust = 2000.0f;
    static const f32 MaxThrust = 5000.0f;

    f32 LiftCoefficient(f32 angle, s32 flap)
    {
        f32 clf0[9] = {-0.54f, -0.2f, 0.2f, 0.57f, 0.92f, 1.21f, 1.43f, 1.4f, 1.0f};
        f32 clfd[9] = {0.0f, 0.45f, 0.85f, 1.02f, 1.39f, 1.65f, 1.75f, 1.38f, 1.17f};
        f32 clfu[9] = {-0.74f, -0.4f, 0.0f, 0.27f, 0.63f, 0.92f, 1.03f, 1.1f, 0.78f};
        f32 a[9] = {-8.0f, -4.0f, 0.0f, 4.0f, 8.0f, 12.0f, 16.0f, 20.0f, 90.0f};//24.0f};

        f32 cl = 0.0f;
        for(s32 i=0; i<8; ++i){
            if(a[i]<=angle && angle<a[i+1]){
                switch(flap)
                {
                case 0:// flaps not deflected
                    cl = clf0[i] - (a[i] - angle) * (clf0[i] - clf0[i+1]) / (a[i] - a[i+1]);
                    break;
                case -1: // flaps down
                    cl = clfd[i] - (a[i] - angle) * (clfd[i] - clfd[i+1]) / (a[i] - a[i+1]);
                    break;
                case 1: // flaps up
                    cl = clfu[i] - (a[i] - angle) * (clfu[i] - clfu[i+1]) / (a[i] - a[i+1]);
                    break;
                }
                break;
            }
        }
        return cl;
    }

    f32 DragCoefficient(f32 angle, s32 flap)
    {
        f32 cdf0[9] = {0.01f, 0.0074f, 0.004f, 0.009f, 0.013f, 0.023f, 0.05f, 0.12f, 0.21f};
        f32 cdfd[9] = {0.0065f, 0.0043f, 0.0055f, 0.0153f, 0.0221f, 0.0391f, 0.1f, 0.195f, 0.3f};
        f32 cdfu[9] = {0.005f, 0.0043f, 0.0055f, 0.02601f, 0.03757f, 0.06647f, 0.13f, 0.18f, 0.25f};
        f32 a[9] = {-8.0f, -4.0f, 0.0f, 4.0f, 8.0f, 12.0f, 16.0f, 20.0f, 90.0f};//24.0f};

        f32 cd = 0.0f;
        for(s32 i=0; i<8; ++i){
            if(a[i]<=angle && angle<a[i+1]){
                switch(flap)
                {
                case 0:// flaps not deflected
                    cd = cdf0[i] - (a[i] - angle) * (cdf0[i] - cdf0[i+1]) / (a[i] - a[i+1]);
                    break;
                case -1: // flaps down
                    cd = cdfd[i] - (a[i] - angle) * (cdfd[i] - cdfd[i+1]) / (a[i] - a[i+1]);
                    break;
                case 1: // flaps up
                    cd = cdfu[i] - (a[i] - angle) * (cdfu[i] - cdfu[i+1]) / (a[i] - a[i+1]);
                    break;
                }
                break;
            }
        }
        return cd;

    }

    f32 RudderLiftCoefficient(f32 angle)
    {
        f32 clf0[7] = {0.16f, 0.456f, 0.736f, 0.968f, 1.144f, 1.12f, 0.8f};
        f32 a[7] = {0.0f, 4.0f, 8.0f, 12.0f, 16.0f, 20.0f, 24.0f};
        f32	aa = lcore::absolute(angle);

        f32 cl = 0.0f;
        for(s32 i=0; i<6; ++i){
            if(a[i]<=aa && aa<a[i+1]){
                cl = clf0[i] - (a[i] - aa) * (clf0[i] - clf0[i+1]) / (a[i] - a[i+1]);
                if(angle<0.0f){
                    cl = -cl;
                }
                break;
            }
        }
        return cl;
    }

    f32 RudderDragCoefficient(f32 angle)
    {
        f32 cdf0[7] = {0.0032f, 0.0072f, 0.0104f, 0.0184f, 0.04f, 0.096f, 0.168f};
        f32 a[7] = {0.0f, 4.0f, 8.0f, 12.0f, 16.0f, 20.0f, 24.0f};
        f32 aa = lcore::absolute(angle);

        f32 cd = 0.75f;

        for(s32 i=0; i<6; ++i){
            if(a[i]<=aa && aa<a[i+1]){
                cd = cdf0[i] - (a[i] - aa) * (cdf0[i] - cdf0[i+1]) / (a[i] - a[i+1]);
                break;
            }
        }
        return cd;
    }


    void getAngularVelocityMatrix(lmath::Matrix44& dst, const lmath::Vector4& angularVelocity)
    {
        dst.set(
            0.0f, -angularVelocity.z_, angularVelocity.y_, 0.0f,
            angularVelocity.z_, 0.0f, -angularVelocity.x_, 0.0f,
            -angularVelocity.y_, angularVelocity.x_, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }
}

    Airplane::Airplane()
    {
    }

    Airplane::~Airplane()
    {
    }

    void Airplane::reset()
    {
        speed_ = 700.0f;
        thrust_ = 3000.0f;
        stalling_ = false;

        position_.zero();
        velocity_.zero();
        velocityBody_.zero();
        force_.zero();

        orientation_.identity();
        angularVelocity_.zero();
        moment_.zero();

        position_.y_ = 2000.0f;
        velocity_.z_ = 700.0f;

        zeroRudder();
        zeroAilerons();
        zeroElevators();
        zeroFlaps();
    }

    void Airplane::initialize()
    {
        reset();

        elements_[0].mass_ = 2.976f;
        elements_[0].offset_.set(3.658f, 0.762f, 4.42f);
        elements_[0].localInertia_.set(51.27f, 117.18f, 67.96f);
        elements_[0].incidence_ = -3.5f;
        elements_[0].dihedral_ = 0.0f;
        elements_[0].area_ = 2.90f;
        elements_[0].flap_ = 0;

        elements_[1].mass_ = 3.316f;
        elements_[1].offset_.set(1.676f, 0.762f, 4.42f);
        elements_[1].localInertia_.set(59.66f, 164.39f, 107.17f);
        elements_[1].incidence_ = -3.5f;
        elements_[1].dihedral_ = -1.0f;
        elements_[1].area_ = 3.384f;
        elements_[1].flap_ = 0;

        elements_[2].mass_ = 3.316f;
        elements_[2].offset_.set(-1.676f, 0.762f, 4.42f);
        elements_[2].localInertia_.set(59.66f, 164.39f, 107.17f);
        elements_[2].incidence_ = -3.5f;
        elements_[2].dihedral_ = 1.0f;
        elements_[2].area_ = 3.384f;
        elements_[2].flap_ = 0;

        elements_[3].mass_ = 2.976f;
        elements_[3].offset_.set(-3.658f, 0.762f, 4.42f);
        elements_[3].localInertia_.set(51.27f, 117.18f, 67.96f);
        elements_[3].incidence_ = -3.5f;
        elements_[3].dihedral_ = 0.0f;
        elements_[3].area_ = 2.90f;
        elements_[3].flap_ = 0;

        elements_[4].mass_ = 1.188f;
        elements_[4].offset_.set(0.762f, 0.914f, 0.924f);
        elements_[4].localInertia_.set(1.88f, 5.888f, 4.087f);
        elements_[4].incidence_ = 0.0f;
        elements_[4].dihedral_ = 0.0f;
        elements_[4].area_ = 1.0f;
        elements_[4].flap_ = 0;

        elements_[5].mass_ = 1.188f;
        elements_[5].offset_.set(-0.762f, 0.914f, 0.924f);
        elements_[5].localInertia_.set(1.88f, 5.888f, 4.087f);
        elements_[5].incidence_ = 0.0f;
        elements_[5].dihedral_ = 0.0f;
        elements_[5].area_ = 1.0f;
        elements_[5].flap_ = 0;

        elements_[6].mass_ = 1.329f;
        elements_[6].offset_.set(0.0f, 1.524f, 0.686f);
        elements_[6].localInertia_.set(9.482f, 3.506f, 6.161f);
        elements_[6].incidence_ = 0.0f;
        elements_[6].dihedral_ = 90.0f;
        elements_[6].area_ = 1.12f;
        elements_[6].flap_ = 0;

        elements_[7].mass_ = 14.42f;
        elements_[7].offset_.set(0.0f, 0.457f, 4.648f);
        elements_[7].localInertia_.set(4208.0f, 4208.0f, 323.7f);
        elements_[7].incidence_ = 0.0f;
        elements_[7].dihedral_ = 0.0f;
        elements_[7].area_ = 7.80f;
        elements_[7].flap_ = 0;

        for(s32 i=0; i<8; ++i){
            recalcNormal(elements_[i]);
        }

        f32 totalMass = 0.0f;
        for(s32 i=0; i<8; ++i){
            totalMass += elements_[i].mass_;
        }

        lmath::Vector3 moment;
        moment.zero();
        for(s32 i=0; i<8; ++i){
            moment.muladd(elements_[i].mass_, elements_[i].offset_, moment);
        }
        lmath::Vector3 centerOfGravity = moment;
        centerOfGravity /= totalMass;


        for(s32 i=0; i<8; ++i){
            elements_[i].cgCoord_.sub(elements_[i].offset_, centerOfGravity);
        }

        f32 Ixx = 0.0f, Iyy = 0.0f, Izz = 0.0f;
        f32 Ixy = 0.0f, Ixz = 0.0f, Iyz = 0.0f;

        for(s32 i=0; i<8; ++i){
            Ixx += elements_[i].localInertia_.x_ + elements_[i].mass_ * (elements_[i].cgCoord_.y_*elements_[i].cgCoord_.y_ + elements_[i].cgCoord_.z_*elements_[i].cgCoord_.z_);
            Iyy += elements_[i].localInertia_.y_ + elements_[i].mass_ * (elements_[i].cgCoord_.z_*elements_[i].cgCoord_.z_ + elements_[i].cgCoord_.x_*elements_[i].cgCoord_.x_);
            Izz += elements_[i].localInertia_.z_ + elements_[i].mass_ * (elements_[i].cgCoord_.x_*elements_[i].cgCoord_.x_ + elements_[i].cgCoord_.y_*elements_[i].cgCoord_.y_);

            Ixy += elements_[i].mass_ * (elements_[i].cgCoord_.x_ * elements_[i].cgCoord_.y_);
            Ixz += elements_[i].mass_ * (elements_[i].cgCoord_.x_ * elements_[i].cgCoord_.z_);
            Iyz += elements_[i].mass_ * (elements_[i].cgCoord_.y_ * elements_[i].cgCoord_.z_);
        }

        totalMass_ = totalMass;

        inertia_.set(
            Ixx, -Ixy, -Ixz, 0.0f,
            -Ixy, Iyy, -Iyz, 0.0f,
            -Ixz, -Iyz, Izz, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);

        invInertia_ = inertia_;
        invInertia_.invert();

        createMesh();
    }

    void Airplane::createMesh()
    {
        material_.diffuse_.set(1.0f, 1.0f, 1.0f, 1.0f);
        material_.specular_.set(1.0f, 1.0f, 1.0f, 8.0f);
        material_.transparent_.set(1.0f, 1.0f, 1.0f, 1.6f);
        material_.flags_ = render::Material::Flag_CastShadow | render::Material::Flag_RecieveShadow;

        lgraphics::InputLayoutRef inputLayout;
        inputLayout = fractal::System::getInputLayoutFactory().get(render::Layout_PNU);

        u32 numVertices = 24;
        Vertex vertices[24];

        static const f32 Unit = 0.5f;
        vertices[0*4 + 0].position_.set(-Unit, -Unit, Unit); vertices[0*4 + 0].normal_.set(0.0f, 0.0f, 1.0f); vertices[0*4 + 0].texcoord_.zero();
        vertices[0*4 + 1].position_.set( Unit, -Unit, Unit); vertices[0*4 + 1].normal_.set(0.0f, 0.0f, 1.0f); vertices[0*4 + 1].texcoord_.zero();
        vertices[0*4 + 2].position_.set(-Unit,  Unit, Unit); vertices[0*4 + 2].normal_.set(0.0f, 0.0f, 1.0f); vertices[0*4 + 2].texcoord_.zero();
        vertices[0*4 + 3].position_.set( Unit,  Unit, Unit); vertices[0*4 + 3].normal_.set(0.0f, 0.0f, 1.0f); vertices[0*4 + 3].texcoord_.zero();

        vertices[1*4 + 0].position_.set(-Unit, -Unit, -Unit); vertices[1*4 + 0].normal_.set(0.0f, 0.0f, -1.0f); vertices[1*4 + 0].texcoord_.zero();
        vertices[1*4 + 1].position_.set(-Unit,  Unit, -Unit); vertices[1*4 + 1].normal_.set(0.0f, 0.0f, -1.0f); vertices[1*4 + 1].texcoord_.zero();
        vertices[1*4 + 2].position_.set( Unit, -Unit, -Unit); vertices[1*4 + 2].normal_.set(0.0f, 0.0f, -1.0f); vertices[1*4 + 2].texcoord_.zero();
        vertices[1*4 + 3].position_.set( Unit,  Unit, -Unit); vertices[1*4 + 3].normal_.set(0.0f, 0.0f, -1.0f); vertices[1*4 + 3].texcoord_.zero();

        vertices[2*4 + 0].position_.set(-Unit, Unit, -Unit); vertices[2*4 + 0].normal_.set(0.0f, 1.0f, 0.0f); vertices[2*4 + 0].texcoord_.zero();
        vertices[2*4 + 1].position_.set(-Unit, Unit, Unit); vertices[2*4 + 1].normal_.set(0.0f, 1.0f, 0.0f); vertices[2*4 + 1].texcoord_.zero();
        vertices[2*4 + 2].position_.set( Unit, Unit, -Unit); vertices[2*4 + 2].normal_.set(0.0f, 1.0f, 0.0f); vertices[2*4 + 2].texcoord_.zero();
        vertices[2*4 + 3].position_.set( Unit, Unit, Unit); vertices[2*4 + 3].normal_.set(0.0f, 1.0f, 0.0f); vertices[2*4 + 3].texcoord_.zero();

        vertices[3*4 + 0].position_.set(-Unit, -Unit, -Unit); vertices[3*4 + 0].normal_.set(0.0f, -1.0f, 0.0f); vertices[3*4 + 0].texcoord_.zero();
        vertices[3*4 + 1].position_.set( Unit, -Unit, -Unit); vertices[3*4 + 1].normal_.set(0.0f, -1.0f, 0.0f); vertices[3*4 + 1].texcoord_.zero();
        vertices[3*4 + 2].position_.set(-Unit, -Unit, Unit); vertices[3*4 + 2].normal_.set(0.0f, -1.0f, 0.0f); vertices[3*4 + 2].texcoord_.zero();
        vertices[3*4 + 3].position_.set( Unit, -Unit, Unit); vertices[3*4 + 3].normal_.set(0.0f, -1.0f, 0.0f); vertices[3*4 + 3].texcoord_.zero();

        vertices[4*4 + 0].position_.set( Unit, -Unit, -Unit); vertices[4*4 + 0].normal_.set(1.0f, 0.0f, 0.0f); vertices[4*4 + 0].texcoord_.zero();
        vertices[4*4 + 1].position_.set( Unit, Unit, -Unit); vertices[4*4 + 1].normal_.set(1.0f, 0.0f, 0.0f); vertices[4*4 + 1].texcoord_.zero();
        vertices[4*4 + 2].position_.set( Unit, -Unit, Unit); vertices[4*4 + 2].normal_.set(1.0f, 0.0f, 0.0f); vertices[4*4 + 2].texcoord_.zero();
        vertices[4*4 + 3].position_.set( Unit, Unit, Unit); vertices[4*4 + 3].normal_.set(1.0f, 0.0f, 0.0f); vertices[4*4 + 3].texcoord_.zero();

        vertices[5*4 + 0].position_.set(-Unit, -Unit, -Unit); vertices[5*4 + 0].normal_.set(-1.0f, 0.0f, 0.0f); vertices[5*4 + 0].texcoord_.zero();
        vertices[5*4 + 1].position_.set(-Unit, -Unit, Unit); vertices[5*4 + 1].normal_.set(-1.0f, 0.0f, 0.0f); vertices[5*4 + 1].texcoord_.zero();
        vertices[5*4 + 2].position_.set(-Unit, Unit, -Unit); vertices[5*4 + 2].normal_.set(-1.0f, 0.0f, 0.0f); vertices[5*4 + 2].texcoord_.zero();
        vertices[5*4 + 3].position_.set(-Unit, Unit, Unit); vertices[5*4 + 3].normal_.set(-1.0f, 0.0f, 0.0f); vertices[5*4 + 3].texcoord_.zero();

        u32 numIndices = 36;
        const u16 indices[] =
        {
            0, 1, 2,
            2, 1, 3,

            4, 5, 6,
            6, 5, 7,

            8, 9, 10,
            10, 9, 11,

            12, 13, 14,
            14, 13, 15,

            16, 17, 18,
            18, 17, 19,

            20, 21, 22,
            22, 21, 23,
        };

        lgraphics::VertexBufferRef vb = lgraphics::VertexBuffer::create(
            sizeof(Vertex)*numVertices,
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            0,
            vertices);

        lgraphics::IndexBufferRef ib = lgraphics::IndexBuffer::create(
            sizeof(u16)*numIndices,
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            0,
            indices);

        geometry_.create(
            load::VElem_Position | load::VElem_Normal | load::VElem_Texcoord,
            sizeof(Vertex),
            numVertices,
            NULL,
            numIndices,
            NULL,
            inputLayout,
            vb,
            ib);

        const lmath::Sphere boxSphere(0.0f, 0.0f, 0.0f, 0.707106781f);
        mesh_.create(
            lgraphics::Primitive_TriangleList,
            0,
            numIndices,
            &geometry_,
            &material_,
            boxSphere);

        for(s32 i=0; i<8; ++i){
            node_[i].meshStartIndex_ = 0;
            node_[i].numMeshes_ = 1;
            node_[i].meshes_ = &mesh_;
            node_[i].world_.identity();
        }
    }

    void Airplane::update(f32 deltaTime)
    {
        calcLoads();

        lmath::Vector4 accel = force_;
        accel /= totalMass_;

        velocity_.muladd(deltaTime, accel, velocity_);
        position_.muladd(deltaTime, velocity_, position_);

        //lmath::Matrix44 matAngularVelocity;
        //getAngularVelocityMatrix(matAngularVelocity, angularVelocity_);

        lmath::Vector4 angular0;
        angular0.mul(inertia_, angularVelocity_);

        angular0.cross3(angularVelocity_, angular0);
        angular0.sub(moment_, angular0);

        //lmath::Vector4 angular1;
        //angular1.mul(inertia_, angularVelocity_);

        //angular1.mul(matAngularVelocity, angular1);
        //angular1.sub(moment_, angular1);

        //lmath::Vector4 d;
        //d.sub(angular0, angular1);

        angular0.mul(invInertia_, angular0);
        angularVelocity_.muladd(deltaTime, angular0, angularVelocity_);


        lmath::Quaternion quatAngularVelocity;
        quatAngularVelocity.set(angularVelocity_);

        lmath::Quaternion orientation = orientation_;
        orientation *= quatAngularVelocity;
        orientation *= 0.5f * deltaTime;


        orientation_ += orientation;
        
        f32 mag = orientation_.length();
        if(0.0001f<mag){
            orientation_.normalize();
        }else{
            orientation_.identity();
        }

        orientation.conjugate(orientation_);

        velocityBody_ = velocity_;
        velocityBody_.rotate(orientation);

        speed_ = velocity_.length();
        orientation_.getEulerAngles(pitch_, yaw_, roll_);

        //lcore::Log("angular velocity: %f, %f, %f", angularVelocity_.x_, angularVelocity_.y_, angularVelocity_.z_);
        //lcore::Log("euler: %f, %f, %f", pitch_, yaw_, roll_);
        //lcore::Log("orientation: %f, %f, %f", orientation_.x_, orientation_.y_, orientation_.z_);
        //lcore::Log("position: %f, %f, %f", position_.x_, position_.y_, position_.z_);
    }

    void Airplane::draw()
    {
        render::Renderer& renderer = fractal::System::getRenderer();

        lmath::Quaternion toWorld;
        toWorld.conjugate(orientation_);
        lmath::Matrix44 mat;
        toWorld.getMatrix(mat);
        mat.setTranslate(position_.x_, position_.y_, position_.z_);

        for(s32 i=0; i<8; ++i){
            node_[i].world_ = mat;
            node_[i].world_.preTranslate( elements_[i].offset_ );
            renderer.add(&node_[i]);
        }
    }

    void Airplane::calcLoads()
    {
        force_.zero();
        moment_.zero();

        lmath::Vector4 tmpForce;
        lmath::Vector4 tmpMoment;
        tmpForce.zero();
        tmpMoment.zero();
        stalling_ = false;

        lmath::Vector4 thrust(0.0f, 0.0f, 1.0f, 0.0f);
        thrust *= thrust_;

        lmath::Vector4 tmpV;
        lmath::Vector4 localVelocity;
        lmath::Vector4 dragVector;

        lmath::Vector4 liftVector;
        lmath::Vector4 result;
        f32 localSpeed;
        for(s32 i=0; i<8; ++i){
            lmath::Vector4 cgCoord(elements_[i].cgCoord_);
            tmpV.zero();
            tmpV.cross3(angularVelocity_, cgCoord);
            localVelocity.add(velocityBody_, tmpV);

            localSpeed = localVelocity.length();

            if(1.0f<localSpeed){
                dragVector = localVelocity;
                dragVector /= -localSpeed;

                lmath::Vector4 normal(elements_[i].normal_);

                liftVector.zero();
                liftVector.cross3(dragVector, normal);
                liftVector.cross3(liftVector, dragVector);

                liftVector.normalize();

                f32 drag = dragVector.dot(normal);
                drag = lcore::clamp(drag, -1.0f, 1.0f);
                f32 attackAngle = RAD_TO_DEG * lmath::asin(drag);

                f32 force = 0.5f * AirDensity * localSpeed * localSpeed * elements_[i].area_;
                if(6 == i){
                    //tail/rudder
                    result = liftVector;
                    result *= RudderLiftCoefficient(attackAngle);
                    result.muladd(force * RudderDragCoefficient(attackAngle), dragVector, result);

                }else if(7 == i){
                    result = dragVector;
                    result *= 0.5f * force;

                }else{
                    f32 lift = LiftCoefficient(attackAngle, elements_[i].flap_);
                    result = liftVector;
                    result *= lift;
                    result.muladd(force * DragCoefficient(attackAngle, elements_[i].flap_), dragVector, result);

                    if(i<=3 && lift<0.0001f){
                        stalling_ = true;
                    }
                }

                tmpForce += result;
                lmath::Vector4 moment;
                moment.zero();
                moment.cross3(cgCoord, result);
                tmpMoment += moment;
            }else{
                dragVector.zero();
                liftVector.zero();
                result.zero();
            }
        }

        tmpForce += thrust;

        force_ = tmpForce;
        force_.rotate(orientation_);
        force_.y_ += Gravity * totalMass_;

        moment_ += tmpMoment;
    }

    void Airplane::recalcNormal(Element& element)
    {
        f32 snIncidence, csIncidence;
        lmath::sincos(snIncidence, csIncidence, DEG_TO_RAD*element.incidence_);

        f32 snDihedral, csDihedral;
        lmath::sincos(snDihedral, csDihedral, DEG_TO_RAD*element.dihedral_);

        //element.normal_.set(snIncidence, csIncidence*snDihedral, csIncidence*csDihedral);
        element.normal_.set(csIncidence*snDihedral, csIncidence*csDihedral, snIncidence);
        element.normal_.normalize();
    }

    void Airplane::incThrust()
    {
        thrust_ += Thrust;
        thrust_ = lcore::minimum(thrust_, MaxThrust);
    }

    void Airplane::decThrust()
    {
        thrust_ -= Thrust;
        thrust_ = lcore::maximum(thrust_, MinThrust);
    }

    void Airplane::leftRudder()
    {
        elements_[6].incidence_ = 16.0f;
        recalcNormal(elements_[6]);
    }

    void Airplane::rightRudder()
    {
        elements_[6].incidence_ = -16.0f;
        recalcNormal(elements_[6]);
    }

    void Airplane::zeroRudder()
    {
        elements_[6].incidence_ = 0.0f;
        recalcNormal(elements_[6]);
    }

    void Airplane::rollLeft(void)
    {
        //elements_[0].flap_ = 1;
        //elements_[3].flap_ = -1;

        elements_[0].incidence_ = 0.0f;
        recalcNormal(elements_[0]);

        elements_[3].incidence_ = -7.0f;
        recalcNormal(elements_[3]);
    }

    void Airplane::rollRight(void)
    {
        //elements_[0].flap_ = -1;
        //elements_[3].flap_ = 1;

        elements_[0].incidence_ = -7.0f;
        recalcNormal(elements_[0]);

        elements_[3].incidence_ = 0.0f;
        recalcNormal(elements_[3]);
    }

    void Airplane::zeroAilerons(void)
    {
        elements_[0].flap_ = 0;
        elements_[3].flap_ = 0;

        elements_[0].incidence_ = -3.5f;
        recalcNormal(elements_[0]);

        elements_[3].incidence_ = -3.5f;
        recalcNormal(elements_[3]);
    }

    void Airplane::pitchUp(void)
    {
        //elements_[4].flap_ = 1;
        //elements_[5].flap_ = 1;

        elements_[4].incidence_ = 10.0f;
        recalcNormal(elements_[4]);

        elements_[5].incidence_ = 10.0f;
        recalcNormal(elements_[5]);
    }

    void Airplane::pitchDown(void)
    {
        //elements_[4].flap_ = -1;
        //elements_[5].flap_ = -1;

        elements_[4].incidence_ = -10.0f;
        recalcNormal(elements_[4]);

        elements_[5].incidence_ = -10.0f;
        recalcNormal(elements_[5]);
    }

    void Airplane::zeroElevators(void)
    {
        elements_[4].flap_ = 0;
        elements_[5].flap_ = 0;

        elements_[4].incidence_ = 0.0f;
        recalcNormal(elements_[4]);

        elements_[5].incidence_ = 0.0f;
        recalcNormal(elements_[5]);
    }

    void Airplane::flapsDown(void)
    {
        elements_[1].flap_ = -1;
        elements_[2].flap_ = -1;
    }

    void Airplane::zeroFlaps(void)
    {
        elements_[1].flap_ = 0;
        elements_[2].flap_ = 0;
    }
}
