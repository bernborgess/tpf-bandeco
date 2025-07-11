#include "Sink.h"

#include <SDL.h>

#include <algorithm>
#include <vector>

#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Game.h"

const std::string Sink::SINK_PATH = "../Assets/Prototype/Sink.png";
const std::string Sink::SINK_PLATE_PATH = "../Assets/Prototype/SinkPlate.png";
const std::string Sink::SINK_PLATES_PATH = "../Assets/Prototype/SinkPlates.png";

Sink::Sink(Game* game, const std::string& texturePath,
           std::pair<int, int> gridPos)
    : Table(game, texturePath, gridPos),
      washLevel(0),
      mDirtyPlateCount(0),
      cleanPlateCount(0),
      mDrainer(nullptr) {
    mProgressBar = new ProgressBar(game);
    mProgressBar->SetPosition(GetPosition() + Vector2(20, 50));
}

Sink* Sink::NewSink(Game* game, LevelTile tile, std::pair<int, int> gridPos) {
    switch (tile) {
        case LevelTile::TileSink: {
            Sink* sink = new Sink(game, SINK_PATH, gridPos);
            game->mLevelManager.mLevelBlocks.push_back(sink);
            return sink;
        }
    }
    return nullptr;
}

// You have to pick items from the Drainer!
Item* Sink::PickItemOnTop() { return nullptr; }

void Sink::UpdateTexture() {
    switch (mDirtyPlateCount) {
        case 0:
            mProgressBar->SetShow(false);
            return mDrawComponent->UpdateTexture(SINK_PATH);
        case 1:
            mProgressBar->SetShow(true);
            mProgressBar->SetProgress(washLevel / (double)WASH_LEVEL_MAX);
            return mDrawComponent->UpdateTexture(SINK_PLATE_PATH);
        default:
            mProgressBar->SetShow(true);
            mProgressBar->SetProgress(washLevel / (double)WASH_LEVEL_MAX);
            return mDrawComponent->UpdateTexture(SINK_PLATES_PATH);
    }
}

Item* Sink::SetItemOnTop(Item* item) {
    if (item->GetItemType() != ItemType::PlateDirty) return item;
    mDirtyPlateCount++;
    if (mDirtyPlateCount == 1) washLevel = 0;
    item->SetState(ActorState::Destroy);
    UpdateTexture();

    return nullptr;
}

void Sink::OnItemWash() {
    SDL_Log("TRY WASH THE DISHES");
    if (mDirtyPlateCount < 1) return;

    washLevel++;

    if (washLevel == WASH_LEVEL_MAX) {
        if (mDrainer == nullptr) return;
        mDirtyPlateCount--;
        washLevel = 0;
        mDrainer->mCleanPlateCount++;
    }
    UpdateTexture();
}