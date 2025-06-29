#include "Stove.h"

#include <SDL.h>

#include "../Game.h"
#include "Plate.h"

const std::string Stove::STOVE_FRONT_PATH = "../Assets/Prototype/Stove.png";
const std::string Stove::STOVE_RIGHT_PATH = "../Assets/Prototype/Stove.png";

Stove::Stove(Game* game, const std::string& texturePath,
             std::pair<int, int> gridPos)
    : Block(game, texturePath, gridPos), mPotOnTop(nullptr) {
    // Also create a Pot on top of it!
    Pot* pot = Pot::NewPot(mGame);
    pot->SetPosition(GetPosition() + Vector2(16, 24));
    mPotOnTop = pot;
}

Stove* Stove::NewStove(Game* game, LevelTile tile,
                       std::pair<int, int> gridPos) {
    switch (tile) {
        case LevelTile::TileStove:
            return new Stove(game, STOVE_FRONT_PATH, gridPos);
    }
    return nullptr;
}

Item* Stove::PickItemOnTop() {
    if (mPotOnTop) {
        Pot* pot = mPotOnTop;
        mPotOnTop = nullptr;
        return pot;
    }
    return nullptr;
}

Item* Stove::SetItemOnTop(Item* item) {
    if (!item) return item;

    switch (item->GetItemType()) {
        case ItemType::Pot: {
            Pot* pot = (Pot*)item;
            if (mPotOnTop) {  // Reject, already got a pot
                return pot;
            }
            // Accept the pot
            mPotOnTop = pot;
            mPotOnTop->SetPosition(GetPosition() + Vector2(16, 24));
            return nullptr;
        }
        case ItemType::Plate: {
            Plate* plate = (Plate*)item;

            if (!mPotOnTop) {
                return plate;
            }

            // Transfer food to the plate if accepts

            // Get what's in the pot
            Item* foodOfPot = mPotOnTop->PickItem();

            // Try put what's in the pot in the plate
            foodOfPot = plate->PutItem(foodOfPot);

            if (foodOfPot) {
                // Plate rejected the item
                SDL_Log("Plate rejected the item");
                mPotOnTop->PutItem(foodOfPot);
            } else {
                // Plate accepted the item
                SDL_Log("Plate accepted the item");
                mPotOnTop->PutItem(foodOfPot);
            }

            return plate;
        }
        default: {
            if (!mPotOnTop) {  // Can't put food without pot
                return item;
            }
            return mPotOnTop->PutItem(item);
        }
    }
}

void Stove::OnUpdate(float deltaTime) {
    // Cook it!
    if (mPotOnTop) {
        mPotOnTop->OnCook(deltaTime);
    }
}