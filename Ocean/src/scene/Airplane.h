#ifndef INC_SCENE_AIRPLANE_H__
#define INC_SCENE_AIRPLANE_H__
/**
@file Airplane.h
@author t-sakai
@date 2013/07/16 create
*/
#include "scene.h"
#include <lmath/lmath.h>
#include <lmath/Quaternion.h>
#include "render/Node.h"
#include "render/Geometry.h"
#include "render/Mesh.h"
#include "render/Material.h"

namespace scene
{
    class Airplane
    {
    public:
        struct Element
        {
            f32 mass_;
            f32 incidence_;
            f32 dihedral_;
            f32 area_;
            lmath::Vector3 offset_;
            lmath::Vector3 cgCoord_;
            lmath::Vector3 localInertia_;
            lmath::Vector3 normal_;
            s32 flap_;
        };

        Airplane();
        ~Airplane();

        void reset();
        void initialize();
        void update(f32 deltaTime);
        void draw();

        const lmath::Vector4& getPosition() const{ return position_;}
        const lmath::Quaternion& getOrientation() const{ return orientation_;}
        const lmath::Vector4& getMoment() const{ return moment_;}
        f32 getSpeed() const{ return speed_;}
        f32 getThrust() const{ return thrust_;}

        void setPosition(const lmath::Vector4& position){ position_ = position;}

        void incThrust();
        void decThrust();

        void leftRudder();
        void rightRudder();
        void zeroRudder();

        void rollLeft();
        void rollRight();
        void pitchUp();
        void pitchDown();

        void zeroAilerons();
        void zeroElevators();

        void flapsDown();
        void zeroFlaps();

        f32 getYaw() const{ return yaw_;}
        f32 getPitch() const{ return pitch_;}
        f32 getRoll() const{ return roll_;}

    private:
        struct Vertex
        {
            lmath::Vector3 position_;
            lmath::Vector3 normal_;
            lmath::Vector2 texcoord_;
        };
        void createMesh();

        void calcLoads();

        void recalcNormal(Element& element);

        f32 speed_;
        f32 thrust_;
        f32 totalMass_;
        bool stalling_;
        lmath::Vector4 position_;
        lmath::Vector4 velocity_;
        lmath::Vector4 velocityBody_;
        lmath::Vector4 force_;

        lmath::Quaternion orientation_;
        lmath::Vector4 angularVelocity_;
        lmath::Vector4 moment_;

        lmath::Matrix44 inertia_;
        lmath::Matrix44 invInertia_;

        Element elements_[8];

        render::Geometry geometry_;
        render::Mesh mesh_;
        render::Material material_;
        render::NodeBase node_[8];

        f32 yaw_;
        f32 pitch_;
        f32 roll_;
    };
}
#endif //INC_SCENE_AIRPLANE_H__
