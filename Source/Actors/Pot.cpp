#include "Pot.h"

#include "../Components/DrawComponents/DrawSpriteComponent.h"

Pot::Pot(Game* game, const std::string& texturePath)
    : Actor(game), mItemCounter(0), mItemInside(nullptr) {
    new DrawSpriteComponent(this, texturePath, 32, 32, 200);
}

// Public Constructor that handles choosing the textures
Pot* Pot::NewPot(Game* game) {
    return new Pot(game, "../Assets/Prototype/Pot.png");
}
