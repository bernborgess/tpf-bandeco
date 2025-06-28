#include "Trash.h"

#include "Plate.h"
#include "Pot.h"

Item* Trash::SetItemOnTop(Item* item) {
    if (!item) return nullptr;
    switch (item->GetItemType()) {
        case ItemType::Pot: {
            Pot* pot = (Pot*)item;
            Item* itemInside = pot->PickItem();
            if (itemInside) {
                itemInside->SetState(ActorState::Destroy);
            }
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