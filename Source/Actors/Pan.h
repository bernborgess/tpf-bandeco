#pragma once

#include <optional>
#include <string>

#include "Actor.h"
#include "Item.h"
#include "Pot.h"

class Pan : public Pot {
   public:
    static Pan* NewPan(Game* game);

    // Add items to the Pan, return the item if rejected
    Item* PutItem(Item* item) override;

    // Transfer *done* food, to the plate
    std::optional<ItemType> PickItem() override;

    // Empties itself, like when interacted with Trash
    void Clear() override;

    // Only used when transfer to plate didn't work
    void ReturnItem(ItemType item) override;

    void OnUpdate(float deltaTime) override;

    void OnCook(float deltaTime) override;

   private:
    Pan(Game* game, const std::string& texturePath);

    // Return true if item was accepted
    bool AddItem(ItemType itemType) override;

    std::optional<ItemType> mItemInside;
    int mItemCounter;
    // Cook & Burn times are proportional to `mItemCounter`
    static constexpr float COOK_TIME_MAX = 8.0f;
    static constexpr float BURN_TIME_MAX = 16.0f;
    float mCookTime;
    bool mIsCooked;
    bool mIsBurnt;

    static const std::string PAN_EMPTY_PATH;
    static const std::string PAN_MEAT_CUT_PATH;
    static const std::string PAN_MEAT_COOK_PATH;
    static const std::string PAN_BURNT_PATH;
};