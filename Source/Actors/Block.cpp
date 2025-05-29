//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Block.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

Block::Block(Game *game, const std::string &texturePath,
    TileType blockType, Vector2 originalPosition) :
      Actor(game)
    , mBlockType(blockType)
    , mIsHit(false)
    , mHitTimer(0)
    , mOriginalPosition(originalPosition)
    {
    
    mPosition = mOriginalPosition;

    new DrawSpriteComponent(this, texturePath, Game::TILE_SIZE, Game::TILE_SIZE);

    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 6 (1 linha): Crie o `AABBColliderComponent` com dimensões `(0,0,GAME::TILE_SIZE, GAME::TILE_SIZE)`
    //  e tipo de colisão `ColliderLayer::Blocks`. Marque o collider como estático.
    new AABBColliderComponent(this, 0, 0, Game::TILE_SIZE, Game::TILE_SIZE, ColliderLayer::Blocks, true);
}


void Block::OnVerticalCollision(const float minOverlap, AABBColliderComponent *other) {
    if(minOverlap>0) return;
    if(mBlockType != TileType::BrickBlock) return;
    
    // TODO: Animate the UP and DOWN
    SDL_Log("Tijolo got hit!");
    mIsHit = true;
    mHitTimer = HIT_TIMER;
}

void Block::OnUpdate(float deltaTime) {
    if(!mIsHit) return;

    mHitTimer -= deltaTime;
    if(mHitTimer <= 0) {
        SDL_Log("Hit over");
        mPosition = mOriginalPosition;
        mIsHit = false;
    }

    // Parabola: y = (-HIT_HEIGHT)t(t-HIT_TIMER)
    auto hitHeight = HIT_HEIGHT * mHitTimer * (mHitTimer - HIT_TIMER);
    mPosition.y = mOriginalPosition.y + hitHeight;



}
