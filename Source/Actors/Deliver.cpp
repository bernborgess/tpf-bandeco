#include "Deliver.h"

#include <SDL.h>

#include "../Game.h"
#include "Plate.h"

const std::string Deliver::DELIVER_FRONT_PATH =
    "../Assets/Prototype/Deliver.png";

Deliver* Deliver::NewDeliver(Game* game, LevelTile tile,
                             std::pair<int, int> gridPos) {
    switch (tile) {
        case LevelTile::TileDeliver: {
            return new Deliver(game, DELIVER_FRONT_PATH, gridPos);
        }
    }
    return nullptr;
}

Item* Deliver::SetItemOnTop(Item* item) {
    if (item->GetItemType() != ItemType::Plate) {
        SDL_Log("Needs plate!");
        return item;
    }
    Plate* plate = (Plate*)item;
    std::set<ItemType> recipe = plate->PickItems();

    // Are the items are a wanted dish?
    int points = mGame->GetOrderManager().DeliverOrder(recipe);
    if (points > 0) {
        // TODO
        // YES: Give points
        SDL_Log("Got %d points", points);
        mGame->GivePoints(points);
    } else {
        // NO: Beep a sound of mistake
        SDL_Log("Wrong recipe, 0 points");
    }

    return plate;
}