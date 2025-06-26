#include "Stove.h"

#include <SDL.h>

#include "../Game.h"

Stove::Stove(Game* game, const std::string& texturePath,
             std::pair<int, int> gridPos)
    : Block(game, texturePath, gridPos), mPotOnTop(nullptr) {
    auto [x, y] = gridPos;
    SetPosition(Vector2(x * Game::TILE_SIZE, y * Game::TILE_SIZE));
    // Also create a Pot on top of it!
    Pot* pot = Pot::NewPot(mGame);
    pot->SetPosition(GetPosition() + Vector2(16, 24));
    // mItemOnTop = pot;
}
