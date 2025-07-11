//
// Created by Lucas N. Ferreira on 08/09/23.
//

#pragma once
#include "../Math.h"
#include "Component.h"

class RigidBodyComponent : public Component {
   public:
    // Lower update order to update first
    RigidBodyComponent(class Actor* owner, float mass = 1.0f,
                       float friction = 0.0f, int updateOrder = 10);

    void Update(float deltaTime) override;

    const Vector2& GetVelocity() const { return mVelocity; }
    void SetVelocity(const Vector2& velocity) { mVelocity = velocity; }

    const Vector2& GetAcceleration() const { return mAcceleration; }
    void SetAcceleration(const Vector2& acceleration) {
        mAcceleration = acceleration;
    }

    void SetApplyFriction(const bool applyFriction) {
        mApplyFriction = applyFriction;
    }

    void ApplyForce(const Vector2& force);

   private:
    bool mApplyFriction;

    // Physical properties
    float mFrictionCoefficient;
    float mMass;
    float m;

    Vector2 mVelocity;
    Vector2 mAcceleration;
};
