//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once

#include <string>

#include "../Actors/Actor.h"
#include "../Actors/Item.h"

class Block : public Actor {
   public:
    explicit Block(Game* game, const std::string& texturePath,
                   std::pair<int, int> gridPos);

    void SetPosition(const Vector2& position) {
        Actor::SetPosition(position);
        mOriginalPosition.Set(position.x, position.y);
    }

    void OnUpdate(float deltaTime) override;
    void OnVerticalCollision(const float minOverlap,
                             AABBColliderComponent* other) override;

    std::pair<int, int> GetGridPosition() { return mGridPosition; }

    virtual Item* PickItemOnTop();
    virtual Item* SetItemOnTop(Item* item);

   private:
    const int BUMP_FORCE = 200;

    Vector2 mOriginalPosition;

    std::pair<int, int> mGridPosition;  // (x,y) in the grid

    class AABBColliderComponent* mColliderComponent;
    class RigidBodyComponent* mRigidBodyComponent;
    class DrawSpriteComponent* mDrawComponent;
};
