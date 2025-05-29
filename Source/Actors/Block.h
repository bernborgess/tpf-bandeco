//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once

#include "Actor.h"
#include "../Game.h"
#include "../Math.h"
#include<string>

const float HIT_TIMER = 0.4;
const float HIT_HEIGHT = 170.0;

class Block : public Actor
{
public:

    explicit Block(Game* game, const std::string &texturePath, TileType blockType, Vector2 originalPosition);
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void OnUpdate(float deltaTime) override;
    TileType mBlockType;
private:
    bool mIsHit;
    float mHitTimer;
    Vector2 mOriginalPosition;
};
