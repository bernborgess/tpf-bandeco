#pragma once

#include <optional>
#include <string>

#include "Actor.h"
#include "Item.h"

class Pot : public Item {
   public:
    static Pot* NewPot(Game* game);

    // Add items to the pot, return the item if rejected
    virtual Item* PutItem(Item* item);

    // Transfer *done* food, to the plate
    virtual std::optional<ItemType> PickItem();

    // Empties itself, like when interacted with Trash
    virtual void Clear();

    // Only used when transfer to plate didn't work
    virtual void ReturnItem(ItemType item);

    void OnUpdate(float deltaTime) override;

    virtual void OnCook(float deltaTime);

   protected:
    Pot(Game* game, const std::string& texturePath);

    // Return true if item was accepted
    virtual bool AddItem(ItemType itemType);

    std::optional<ItemType> mItemInside;
    int mItemCounter;
    // Cook & Burn times are proportional to `mItemCounter`
    static constexpr float COOK_TIME_MAX = 5.0f;
    static constexpr float BURN_TIME_MAX = 10.0f;
    float mCookTime;
    bool mIsCooked;
    bool mIsBurnt;

    static const std::string POT_EMPTY_PATH;
    static const std::string POT_TOMATO_1_PATH;
    static const std::string POT_TOMATO_2_PATH;
    static const std::string POT_TOMATO_3_PATH;
    static const std::string POT_TOMATO_SOUP_PATH;
    static const std::string POT_BURNT_PATH;
};