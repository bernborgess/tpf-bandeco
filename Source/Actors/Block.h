//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once

#include <string>

#include "Actor.h"

class Block : public Actor {
   public:
    explicit Block(Game* game, const std::string& texturePath,
                   const bool isStatic = true);

    void SetPosition(const Vector2& position) {
        Actor::SetPosition(position);
        mOriginalPosition.Set(position.x, position.y);
    }

    void OnUpdate(float deltaTime) override;
    void OnVerticalCollision(const float minOverlap,
                             AABBColliderComponent* other) override;

   private:
    const int BUMP_FORCE = 200;

    Vector2 mOriginalPosition;

    class AABBColliderComponent* mColliderComponent;
    class RigidBodyComponent* mRigidBodyComponent;
};
