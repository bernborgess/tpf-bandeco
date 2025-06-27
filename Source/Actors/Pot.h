#pragma once

#include <string>

#include "Actor.h"
#include "Item.h"

class Pot : public Item {
   public:
    static Pot* NewPot(Game* game);
    Item* PutItem(Item* item);
    Item* PickItem();
    void OnUpdate(float deltaTime) override;
    void OnCook(float deltaTime);

   private:
    Pot(Game* game, const std::string& texturePath);
    Item* mItemInside;
    int mItemCounter;
    // Cook & Burn times are proportional to `mItemCounter`
    static constexpr float COOK_TIME_MAX = 5.0f;
    static constexpr float BURN_TIME_MAX = 10.0f;
    float mCookTime;
    bool mIsCooked;
    bool mIsBurnt;
};