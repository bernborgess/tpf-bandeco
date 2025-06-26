#include "Stove.h"

#include <SDL.h>

Stove::Stove(Game* game, const std::string& texturePath,
             std::pair<int, int> gridPos)
    : Table(game, texturePath, gridPos) {
    // Also create a Pot on top of it!
    // Pot* pot = new Pot();
    // mItemOnTop = pot;
}
