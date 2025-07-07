#include "Deliver.h"

#include <SDL.h>

#include "../Actors/Plate.h"
#include "../Game.h"

const std::string Deliver::DELIVER_FRONT_PATH =
    "../Assets/Prototype/Deliver.png";
const std::string Deliver::DELIVER_DOWN_LEFT_PATH =
    "../Assets/Prototype/DeliverDownLeft.png";
const std::string Deliver::DELIVER_DOWN_RIGHT_PATH =
    "../Assets/Prototype/DeliverDownRight.png";

Deliver* Deliver::NewDeliver(Game* game, LevelTile tile,
                             std::pair<int, int> gridPos) {
    const std::map<LevelTile, const std::string> tileMap = {
        {LevelTile::TileDeliver, DELIVER_FRONT_PATH},
        {LevelTile::TileDeliverDownLeft, DELIVER_DOWN_LEFT_PATH},
        {LevelTile::TileDeliverDownRight, DELIVER_DOWN_RIGHT_PATH},
    };
    auto it = tileMap.find(tile);
    if (it == tileMap.end()) return nullptr;
    return new Deliver(game, it->second, gridPos);
}

Item* Deliver::SetItemOnTop(Item* item) {
    if (item->GetItemType() != ItemType::Plate) {
        SDL_Log("Needs plate!");
        return item;
    }
    Plate* plate = (Plate*)item;
    std::set<ItemType> recipe = plate->PickItems();

    // Are the items are a wanted dish?

    // TODO: After orders are shown, enable this validation
    int points = mGame->GetOrderManager().DeliverOrder(recipe);
    if (points > 0) {
        // YES: Give points
        SDL_Log("Got %d points", points);
        mGame->GivePoints(points);
    } else {
        // NO: Beep a sound of mistake
        SDL_Log("Wrong recipe, 0 points");
    }

    return plate;
}