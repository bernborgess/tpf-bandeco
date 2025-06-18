//
// Created by Lucas N. Ferreira on 29/05/25.
//

#pragma once

#include <unordered_map>
#include <vector>

#include "Actors/Actor.h"
#include "Math.h"

class SpatialHashing {
   public:
    SpatialHashing(int cellSize, int width, int height);
    ~SpatialHashing();

    void Insert(Actor *actor);
    void Remove(Actor *actor);
    void Reinsert(Actor *actor);

    std::vector<AABBColliderComponent *> QueryColliders(
        const Vector2 &position, const int range = 1) const;

    std::vector<Actor *> Query(const Vector2 &position,
                               const int range = 1) const;
    std::vector<Actor *> QueryOnCamera(const Vector2 &cameraPosition,
                                       const float screenWidth,
                                       const float screenHeight,
                                       const float extraRadius = 0.0f) const;

   private:
    int mCellSize;
    int mWidth;
    int mHeight;

    std::vector<std::vector<std::vector<Actor *>>>
        mGrid;  // 2D grid of colliders
    std::unordered_map<Actor *, Vector2>
        mPositions;  // Maps collider to its position
    std::unordered_map<Actor *, std::pair<int, int>>
        mCellIndices;  // Maps collider to its grid cell indices
};
