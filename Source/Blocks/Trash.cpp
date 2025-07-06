#include "Trash.h"

#include <SDL.h>

#include "../Actors/Pan.h"
#include "../Actors/Plate.h"
#include "../Actors/Pot.h"

const std::string Trash::TRASH_PATH = "../Assets/Prototype/Trash.png";

Trash::Trash(Game* game, std::pair<int, int> gridPos)
    : Block(game, TRASH_PATH, gridPos) {}

Item* Trash::SetItemOnTop(Item* item) {
    if (!item) return nullptr;
    switch (item->GetItemType()) {
        case ItemType::Pan: {
            Pan* pan = (Pan*)item;
            pan->Clear();
            return pan;
        }
        case ItemType::Pot: {
            Pot* pot = (Pot*)item;
            pot->Clear();
            return pot;
        }
        case ItemType::Plate: {
            Plate* plate = (Plate*)item;
            plate->PickItems();
            return plate;
        }
    }

    item->SetState(ActorState::Destroy);
    return nullptr;
}