#include "Pan.h"

#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "Plate.h"

const std::string Pan::PAN_EMPTY_PATH = "../Assets/Prototype/Pan.png";
const std::string Pan::PAN_MEAT_CUT_PATH = "../Assets/Prototype/PanMeatCut.png";
const std::string Pan::PAN_MEAT_COOK_PATH =
    "../Assets/Prototype/PanMeatCook.png";
const std::string Pan::PAN_BURNT_PATH = "../Assets/Prototype/PanBurn.png";

Pan::Pan(Game* game, const std::string& texturePath)
    : Pot(game, texturePath), mItemCounter(0) {
    mItemInside = {};
    mItemType = ItemType::Pan;
}

// Public Constructor that handles choosing the textures
Pan* Pan::NewPan(Game* game) { return new Pan(game, PAN_EMPTY_PATH); }

bool Pan::AddItem(ItemType itemType) {
    if (!mItemInside) {
        switch (itemType) {
            // TODO
        }
        // Not a supported item
        return false;
    }

    // Only accept if it's the same type
    if (*mItemInside != itemType) {
        SDL_Log("Have to refuse,since types are different");
        return false;
    }

    // Stacking more items
    switch (*mItemInside) {
        // TODO
    }
    // Can't use it
    return false;
}

Item* Pan::PutItem(Item* item) {
    if (!item) return item;

    bool accepted = AddItem(item->GetItemType());

    if (accepted) {
        item->SetState(ActorState::Destroy);
        return nullptr;
    } else if (item->GetItemType() == ItemType::Plate) {
        Plate* plate = (Plate*)item;
        auto items = plate->PickItems();
        for (auto& itemType : items) {
            bool accepted = AddItem(itemType);
            if (!accepted) {
                // Give back to the plate
                plate->PutItem(itemType);
            }
        }
        return plate;
    } else {
        return item;
    }
}

std::optional<ItemType> Pan::PickItem() {
    if (!mItemInside) return {};

    // Only *done* food is returned
    switch (*mItemInside) {
        case ItemType::MeatCook: {
            mItemInside = {};
            mDrawComponent->UpdateTexture(PAN_EMPTY_PATH);
            return ItemType::TomatoSoup;
        }
    }
    // Not done or burnt.
    return {};
}

void Pan::Clear() {
    mItemInside = {};
    mDrawComponent->UpdateTexture(PAN_EMPTY_PATH);
}

void Pan::ReturnItem(ItemType item) {
    mItemInside = item;
    switch (item) {
        // TODO
    }
}

void Pan::OnUpdate(float deltaTime) {
    if (!mItemInside) return;

    // Check if cooking is done!
    if (mCookTime >= mItemCounter * COOK_TIME_MAX && !mIsCooked) {
        switch (*mItemInside) {
            case ItemType::MeatCut: {
                mItemInside = ItemType::MeatCook;
                mIsCooked = true;
                mDrawComponent->UpdateTexture(PAN_MEAT_COOK_PATH);
                break;
            }
        }
    }

    // Check if burning!
    if (mCookTime >= mItemCounter * BURN_TIME_MAX && !mIsBurnt) {
        switch (*mItemInside) {
            case ItemType::MeatCut:
            case ItemType::MeatCook: {
                // Swap the TomatoSoup to TomatoBurn
                mItemInside = ItemType::MeatBurn;
                mIsBurnt = true;
                mDrawComponent->UpdateTexture(PAN_BURNT_PATH);
                break;
            }
        }
    }
}

void Pan::OnCook(float deltaTime) {
    if (mItemInside) {
        mCookTime += deltaTime;
    }
}