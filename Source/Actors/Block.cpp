//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Block.h"

#include "../Components/ColliderComponents/AABBColliderComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Game.h"

Block::Block(Game* game, const std::string& texturePath,
             std::pair<int, int> gridPos)
    : Actor(game), mGridPosition(gridPos) {
    mDrawComponent = new DrawSpriteComponent(this, texturePath, Game::TILE_SIZE,
                                             Game::TILE_SIZE, 10);
    mColliderComponent = new AABBColliderComponent(
        this, 0, 0, Game::TILE_SIZE, Game::TILE_SIZE, ColliderLayer::Blocks);
    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f, 0.0f, false);

    SetPosition(Vector2(gridPos.first * Game::TILE_SIZE,
                        gridPos.second * Game::TILE_SIZE));
}

void Block::OnUpdate(float deltaTime) {
    // If the block is moving, check if it has reached its original position
    if (mRigidBodyComponent->GetVelocity().y > 0.0f &&
        mPosition.y >= mOriginalPosition.y) {
        mPosition.Set(mOriginalPosition.x, mOriginalPosition.y);
        mRigidBodyComponent->SetVelocity(Vector2::Zero);
        mColliderComponent->SetStatic(true);
    }
}

void Block::OnVerticalCollision(const float minOverlap,
                                AABBColliderComponent* other) {
    // If collide against enemy, apply bump force
    if (other->GetLayer() == ColliderLayer::Enemy) {
        mRigidBodyComponent->SetVelocity(Vector2::NegUnitY * BUMP_FORCE);
        mOriginalPosition.Set(mPosition.x, mPosition.y);
    }
}
