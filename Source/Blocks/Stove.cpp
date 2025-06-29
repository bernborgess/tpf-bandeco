#include "Stove.h"

#include <SDL.h>

#include "../Actors/Plate.h"
#include "../Game.h"

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
        case LevelTile::TileStovePotTomatoSoup:
        case LevelTile::TileStovePot:
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

            // From plate to the pot
            if (plate->HasFood()) {
                return mPotOnTop->PutItem(plate);
            }

            // Transfer food to the plate if accepts

            // Get what's in the pot
            auto foodOfPot = mPotOnTop->PickItem();

            if (foodOfPot) {
                auto refusedItem = plate->PutItem(*foodOfPot);

                // Give back to pot, in case it was rejected by the plate
                if (refusedItem) mPotOnTop->ReturnItem(*refusedItem);
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