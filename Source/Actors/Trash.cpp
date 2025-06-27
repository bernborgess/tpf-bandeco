#include "Trash.h"

#include "Pot.h"

Item* Trash::SetItemOnTop(Item* item) {
    if (!item) return nullptr;
    if (item->GetItemType() == ItemType::Pot) {
        Pot* pot = (Pot*)item;
        Item* itemInside = pot->PickItem();
        if (itemInside) {
            itemInside->SetState(ActorState::Destroy);
        }
        return item;
    } else {
        item->SetState(ActorState::Destroy);
        return nullptr;
    }
}