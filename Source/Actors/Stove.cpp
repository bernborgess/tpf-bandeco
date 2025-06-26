#include "Stove.h"

#include <SDL.h>

#include "../Game.h"

Stove::Stove(Game* game, const std::string& texturePath,
             std::pair<int, int> gridPos)
    : Block(game, texturePath, gridPos), mPotOnTop(nullptr) {
    // Also create a Pot on top of it!
    Pot* pot = Pot::NewPot(mGame);
    pot->SetPosition(GetPosition() + Vector2(16, 24));
    mPotOnTop = pot;
}

Pot* Stove::PickPotOnTop() {
    if (mPotOnTop) {
        Pot* pot = mPotOnTop;
        mPotOnTop = nullptr;
        return pot;
    }
    return nullptr;
}

Pot* Stove::PutPotOnTop(Pot* pot) {
    if (mPotOnTop) {
        // Reject, it's already occupied
        return pot;
    }
    // Accept the pot
    mPotOnTop = pot;
    mPotOnTop->SetPosition(GetPosition() + Vector2(16, 24));
    return nullptr;
}