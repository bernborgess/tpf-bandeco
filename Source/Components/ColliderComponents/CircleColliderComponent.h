//
// Created by Lucas N. Ferreira on 10/09/23.
//

#pragma once
#include <vector>

#include "../../Math.h"
#include "../Component.h"

class CircleColliderComponent : public Component {
   public:
    CircleColliderComponent(class Actor* owner, float radius,
                            int updateOrder = 10);

    void SetRadius(float radius) { mRadius = radius; }
    float GetRadius() const;

    const Vector2& GetCenter() const;

    bool Intersect(const CircleColliderComponent& b) const;

   private:
    float mRadius;
};
