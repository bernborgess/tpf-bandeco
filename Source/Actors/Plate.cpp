#include "Plate.h"

#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Game.h"

// Paths to the plate variants
const std::string Plate::PLATE_EMPTY_PATH =
    "../Assets/Sprites/Plates/Empty.png";
const std::string Plate::PLATE_TOMATO_SOUP_PATH =
    "../Assets/Sprites/Plates/TomatoSoup.png";
const std::string Plate::PLATE_T_PATH = "../Assets/Sprites/Plates/PlateT.png";
const std::string Plate::PLATE_M_PATH = "../Assets/Sprites/Plates/PlateM.png";
const std::string Plate::PLATE_MT_PATH = "../Assets/Sprites/Plates/PlateMT.png";
const std::string Plate::PLATE_L_PATH = "../Assets/Sprites/Plates/PlateL.png";
const std::string Plate::PLATE_LT_PATH = "../Assets/Sprites/Plates/PlateLT.png";
const std::string Plate::PLATE_LM_PATH = "../Assets/Sprites/Plates/PlateLM.png";
const std::string Plate::PLATE_LMT_PATH =
    "../Assets/Sprites/Plates/PlateLMT.png";
const std::string Plate::PLATE_B_PATH = "../Assets/Sprites/Plates/PlateB.png";
const std::string Plate::PLATE_BT_PATH = "../Assets/Sprites/Plates/PlateBT.png";
const std::string Plate::PLATE_BM_PATH = "../Assets/Sprites/Plates/PlateBM.png";
const std::string Plate::PLATE_BMT_PATH =
    "../Assets/Sprites/Plates/PlateBMT.png";
const std::string Plate::PLATE_BL_PATH = "../Assets/Sprites/Plates/PlateBL.png";
const std::string Plate::PLATE_BLT_PATH =
    "../Assets/Sprites/Plates/PlateBLT.png";
const std::string Plate::PLATE_BLM_PATH =
    "../Assets/Sprites/Plates/PlateBLM.png";
const std::string Plate::PLATE_BLMT_PATH =
    "../Assets/Sprites/Plates/PlateBLMT.png";

Plate::Plate(Game* game, const std::string& texturePath)
    : Item(game, texturePath, ItemType::Plate,
           /* width = */ 52,
           /* height = */ 52,
           /* drawOrder = */ 200) {
    mItems.clear();
}

// Public Constructor that handles choosing the textures
Plate* Plate::NewPlate(Game* game) { return new Plate(game, PLATE_EMPTY_PATH); }

void Plate::UpdateTexture() {
    if (mItems.empty()) {
        return mDrawComponent->UpdateTexture(PLATE_EMPTY_PATH);
    }
    if (mItems == std::set<ItemType>{ItemType::TomatoSoup}) {
        return mDrawComponent->UpdateTexture(PLATE_TOMATO_SOUP_PATH);
    }

    const int BREAD_MASK = 1;
    const int LETTUCE_MASK = 2;
    const int MEAT_MASK = 4;
    const int TOMATO_MASK = 8;

    int mask = 0;
    if (mItems.find(ItemType::Bread) != mItems.end()) mask |= BREAD_MASK;
    if (mItems.find(ItemType::LettuceCut) != mItems.end()) mask |= LETTUCE_MASK;
    if (mItems.find(ItemType::MeatCook) != mItems.end()) mask |= MEAT_MASK;
    if (mItems.find(ItemType::TomatoCut) != mItems.end()) mask |= TOMATO_MASK;

    switch (mask) {
        case 1:
            return mDrawComponent->UpdateTexture(PLATE_B_PATH);
        case 2:
            return mDrawComponent->UpdateTexture(PLATE_L_PATH);
        case 3:
            return mDrawComponent->UpdateTexture(PLATE_BL_PATH);
        case 4:
            return mDrawComponent->UpdateTexture(PLATE_M_PATH);
        case 5:
            return mDrawComponent->UpdateTexture(PLATE_BM_PATH);
        case 6:
            return mDrawComponent->UpdateTexture(PLATE_LM_PATH);
        case 7:
            return mDrawComponent->UpdateTexture(PLATE_BLM_PATH);
        case 8:
            return mDrawComponent->UpdateTexture(PLATE_T_PATH);
        case 9:
            return mDrawComponent->UpdateTexture(PLATE_BT_PATH);
        case 10:
            return mDrawComponent->UpdateTexture(PLATE_LT_PATH);
        case 11:
            return mDrawComponent->UpdateTexture(PLATE_BLT_PATH);
        case 12:
            return mDrawComponent->UpdateTexture(PLATE_MT_PATH);
        case 13:
            return mDrawComponent->UpdateTexture(PLATE_BMT_PATH);
        case 14:
            return mDrawComponent->UpdateTexture(PLATE_LMT_PATH);
        case 15:
            return mDrawComponent->UpdateTexture(PLATE_BLMT_PATH);
    }
}

Item* Plate::PutItem(Item* item) {
    if (!item) return item;

    ItemType itemType = item->GetItemType();
    switch (itemType) {
        case ItemType::TomatoCut:
        case ItemType::Bread:
        case ItemType::LettuceCut:
        case ItemType::MeatCook: {
            if (mItems.find(ItemType::TomatoSoup) == mItems.end() &&
                mItems.find(item->GetItemType()) == mItems.end()

            ) {
                item->SetState(ActorState::Destroy);
                mItems.insert(item->GetItemType());
                UpdateTexture();
                return nullptr;
            }
            return item;
        }
    }

    // Can't use it, reject
    return item;
}

std::optional<ItemType> Plate::PutItem(ItemType itemType) {
    switch (itemType) {
        case ItemType::TomatoSoup: {
            if (mItems.empty()) {
                mItems.insert(itemType);
                UpdateTexture();
                return {};
            }
            // Can't put two soups in the same plate.
            return itemType;
        }
        // Food will combine, if there's no soup in the plate
        case ItemType::Bread:
        case ItemType::LettuceCut:
        case ItemType::MeatCook:
        case ItemType::TomatoCut: {
            if (mItems.find(ItemType::TomatoSoup) == mItems.end()) {
                mItems.insert(itemType);
                UpdateTexture();
                return {};
            }
            return itemType;
        }
    }
    return itemType;
}

std::set<ItemType> Plate::PickItems() {
    std::set<ItemType> items = mItems;
    mItems.clear();
    UpdateTexture();
    return items;
}
