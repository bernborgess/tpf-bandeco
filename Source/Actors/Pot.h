#pragma once

#include <optional>
#include <string>

#include "Actor.h"
#include "Item.h"

class Pot : public Item {
   public:
    static Pot* NewPot(Game* game);

    // Add items to the pot, return the item if rejected
    Item* PutItem(Item* item);

    // Transfer *done* food, to the plate
    std::optional<ItemType> PickItem();

    // Empties itself, like when interacted with Trash
    void Clear();

    // Only used when transfer to plate didn't work
    void ReturnItem(ItemType item);

    void OnUpdate(float deltaTime) override;

    void OnCook(float deltaTime);

   private:
    Pot(Game* game, const std::string& texturePath);

    // Return true if item was accepted
    bool AddItem(ItemType itemType);

    std::optional<ItemType> mItemInside;
    int mItemCounter;
    // Cook & Burn times are proportional to `mItemCounter`
    static constexpr float COOK_TIME_MAX = 5.0f;
    static constexpr float BURN_TIME_MAX = 10.0f;
    float mCookTime;
    bool mIsCooked;
    bool mIsBurnt;

    static const std::string POT_EMPTY_PATH;
    static const std::string POT_TOMATO_SOUP_PATH;
    static const std::string POT_BURNT_PATH;
};