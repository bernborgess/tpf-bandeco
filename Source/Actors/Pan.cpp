#include "Pan.h"

#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Game.h"
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
    mProgressBar = new ProgressBar(game);
}

// Public Constructor that handles choosing the textures
Pan* Pan::NewPan(Game* game) { return new Pan(game, PAN_EMPTY_PATH); }

bool Pan::AddItem(ItemType itemType) {
    // Only one item in the pan
    if (mItemInside) return false;

    switch (itemType) {
        case ItemType::MeatCut: {
            mItemInside = ItemType::MeatCut;
            mItemCounter = 1;
            mCookTime = 0.0f;
            mIsCooked = mIsBurnt = false;
            mDrawComponent->UpdateTexture(PAN_MEAT_CUT_PATH);
            mProgressBar->SetShow(true);
            return true;
        }
        case ItemType::MeatCook: {
            mItemInside = ItemType::MeatCook;
            mItemCounter = 1;
            mCookTime = COOK_TIME_MAX;
            mIsCooked = true;
            mIsBurnt = false;
            mDrawComponent->UpdateTexture(PAN_MEAT_COOK_PATH);
            mProgressBar->SetShow(true);
            return true;
        }
        case ItemType::MeatBurn: {
            mItemInside = ItemType::MeatBurn;
            mItemCounter = 1;
            mCookTime = BURN_TIME_MAX;
            mIsBurnt = mIsCooked = true;
            mDrawComponent->UpdateTexture(PAN_BURNT_PATH);
            return true;
        }
    }

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
            mProgressBar->SetShow(false);
            return ItemType::MeatCook;
        }
    }
    // Not done or burnt.
    return {};
}

void Pan::Clear() {
    mItemInside = {};
    mProgressBar->SetShow(false);
    mDrawComponent->UpdateTexture(PAN_EMPTY_PATH);
}

void Pan::ReturnItem(ItemType item) {
    mItemInside = item;
    switch (item) {
        case ItemType::MeatCut: {
            mDrawComponent->UpdateTexture(PAN_MEAT_CUT_PATH);
            break;
        }
        case ItemType::MeatCook: {
            mDrawComponent->UpdateTexture(PAN_MEAT_COOK_PATH);
            break;
        }
        case ItemType::MeatBurn: {
            mDrawComponent->UpdateTexture(PAN_BURNT_PATH);
            break;
        }
    }
}

void Pan::OnUpdate(float deltaTime) {
    if (!mItemInside) return;
    mProgressBar->SetPosition(GetPosition() + Vector2(16, 72));

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
                mGame->GetAudio()->PlaySound("burnout.ogg");
                mItemInside = ItemType::MeatBurn;
                mIsBurnt = true;
                mDrawComponent->UpdateTexture(PAN_BURNT_PATH);
                mProgressBar->SetShow(false);
                break;
            }
        }
    }
}

void Pan::OnCook(float deltaTime) {
    if (mItemInside) {
        mCookTime += deltaTime;
        if (mCookTime > COOK_TIME_MAX) {
            mProgressBar->SetProgress(
                (mCookTime - COOK_TIME_MAX) / (BURN_TIME_MAX - COOK_TIME_MAX),
                true);
        } else {
            mProgressBar->SetProgress(mCookTime / COOK_TIME_MAX);
        }
    }
}