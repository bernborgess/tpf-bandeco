#pragma once

#include <string>

#include "Actor.h"
#include "Item.h"

class Pot : public Item {
   public:
    static Pot* NewPot(Game* game);
    Item* PutItem(Item* item);
    void OnUpdate(float deltaTime) override;

   private:
    Pot(Game* game, const std::string& texturePath);
    Item* mItemInside;
    int mItemCounter;
};