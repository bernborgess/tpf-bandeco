//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once
#include <map>
#include <set>
#include <vector>

#include "../../Math.h"
#include "../Component.h"
#include "../RigidBodyComponent.h"

enum class ColliderLayer { Player, Enemy, Blocks, Pole };

class AABBColliderComponent : public Component {
   public:
    // Collider ignore map
    const std::map<ColliderLayer, const std::set<ColliderLayer>>
        ColliderIgnoreMap = {{ColliderLayer::Player, {}},
                             {ColliderLayer::Enemy, {}},
                             {ColliderLayer::Blocks, {ColliderLayer::Blocks}},
                             {ColliderLayer::Pole, {}}};

    AABBColliderComponent(class Actor* owner, int dx, int dy, int w, int h,
                          ColliderLayer layer, bool isStatic = false,
                          int updateOrder = 10);
    ~AABBColliderComponent() override;

    bool Intersect(const AABBColliderComponent& b) const;

    float DetectHorizontalCollision(RigidBodyComponent* rigidBody);
    float DetectVerticalCollision(RigidBodyComponent* rigidBody);

    void SetStatic(bool isStatic) { mIsStatic = isStatic; }

    Vector2 GetMin() const;
    Vector2 GetMax() const;
    Vector2 GetCenter() const;
    ColliderLayer GetLayer() const { return mLayer; }

   private:
    float GetMinVerticalOverlap(AABBColliderComponent* b) const;
    float GetMinHorizontalOverlap(AABBColliderComponent* b) const;

    void ResolveHorizontalCollisions(RigidBodyComponent* rigidBody,
                                     const float minOverlap);
    void ResolveVerticalCollisions(RigidBodyComponent* rigidBody,
                                   const float minOverlap);

    Vector2 mOffset;
    int mWidth;
    int mHeight;
    bool mIsStatic;

    ColliderLayer mLayer;
};