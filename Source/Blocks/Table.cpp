#include "Table.h"

#include <SDL.h>

#include "../Actors/Pan.h"
#include "../Actors/Plate.h"
#include "../Actors/Pot.h"

const std::string Table::TABLE_CENTER_PATH =
    "../Assets/Prototype/TableCenter.png";
const std::string Table::TABLE_FRONT_PATH = "../Assets/Prototype/Table.png";
const std::string Table::TABLE_LEFT_PATH = "../Assets/Prototype/TableLeft.png";
const std::string Table::TABLE_PLAIN_PATH =
    "../Assets/Prototype/TablePlain.png";
const std::string Table::TABLE_RIGHT_PATH =
    "../Assets/Prototype/TableRight.png";

Table::Table(Game* game, const std::string& texturePath,
             std::pair<int, int> gridPos)
    : Block(game, texturePath, gridPos), mItemOnTop(nullptr) {}

Table* Table::NewTable(Game* game, LevelTile tile,
                       std::pair<int, int> gridPos) {
    const std::map<LevelTile, const std::string> tileMap = {
        {LevelTile::TileTable, TABLE_FRONT_PATH},
        {LevelTile::TileTablePlate, TABLE_CENTER_PATH},
        {LevelTile::TileTableLeft, TABLE_LEFT_PATH},
        {LevelTile::TileTableRight, TABLE_RIGHT_PATH},
        {LevelTile::TileTableCenter, TABLE_CENTER_PATH},
        {LevelTile::TileTablePlain, TABLE_PLAIN_PATH},
    };

    auto it = tileMap.find(tile);
    if (it == tileMap.end()) return nullptr;

    Table* table = new Table(game, it->second, gridPos);

    // When we need a plate on top
    // switch (tile) {
    //     case LevelTile::TileTablePlate: {
    //         Plate* plate = Plate::NewPlate(game);
    //         table->SetItemOnTop(plate);
    //     }
    // }

    Plate* plate = Plate::NewPlate(game);
    plate->PutItem(ItemType::TomatoSoup);
    table->SetItemOnTop(plate);

    return table;
}

Item* Table::PickItemOnTop() {
    if (!mItemOnTop) return nullptr;
    Item* item = mItemOnTop;
    mItemOnTop = nullptr;
    return item;
}

bool Table::HasItemOnTop() { return mItemOnTop != nullptr; }

Item* Table::SetItemOnTop(Item* item) {
    if (!item) return item;

    if (mItemOnTop) {
        // Check if item on top is a Pot or Plate
        if (mItemOnTop->GetItemType() == ItemType::Pot) {
            Pot* pot = (Pot*)mItemOnTop;
            // Check if player has a plate on his hand
            if (item->GetItemType() == ItemType::Plate) {
                Plate* plate = (Plate*)item;
                // Will move food from pot to plate, if plate accepts
                auto itemInside = pot->PickItem();
                if (itemInside) {
                    auto rejected = plate->PutItem(*itemInside);
                    if (rejected) pot->ReturnItem(*rejected);
                }
                return plate;
            }
            return pot->PutItem(item);
        }
        if (mItemOnTop->GetItemType() == ItemType::Plate) {
            Plate* plate = (Plate*)mItemOnTop;
            if (item->GetItemType() == ItemType::Pan) {
                Pan* pan = (Pan*)item;
                auto itemInside = pan->PickItem();
                if (itemInside) {
                    auto rejected = plate->PutItem(*itemInside);
                    if (rejected) pan->ReturnItem(*rejected);
                }
                return pan;
            }
            // Check if player has a Pot on his hand
            if (item->GetItemType() == ItemType::Pot) {
                Pot* pot = (Pot*)item;
                auto itemInside = pot->PickItem();
                if (itemInside) {
                    auto rejected = plate->PutItem(*itemInside);
                    if (rejected) pot->ReturnItem(*rejected);
                }
                return pot;
            }
            return plate->PutItem(item);
        }

        // Rejected the item (table full)
        return item;
    }
    // Accepted the item, returns nothing
    mItemOnTop = item;
    Vector2 offset = Vector2(0, 0);
    if (item->GetItemType() == ItemType::Pot) offset = Vector2(0, -16);
    if (item->GetItemType() == ItemType::Plate) offset = Vector2(6, 6);

    mItemOnTop->SetPosition(GetPosition() + offset);
    return nullptr;
}