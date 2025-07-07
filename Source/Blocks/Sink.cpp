#include "Sink.h"

#include <SDL.h>

#include <algorithm>
#include <vector>

#include "../Game.h"

const std::string Sink::SINK_PATH = "../Assets/Prototype/Sink.png";
const std::string Sink::DRAINER_PATH = "../Assets/Prototype/Sink.png";

Sink::Sink(Game* game, const std::string& texturePath,
           std::pair<int, int> gridPos)
    : Table(game, texturePath, gridPos),
      washLevel(0),
      dirtyPlateCount(0),
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

Item* Sink::SetItemOnTop(Item* item) {
    if (item->GetItemType() != ItemType::PlateDirty) dirtyPlateCount++;
    if (dirtyPlateCount == 1) washLevel = 0;
    item->SetState(ActorState::Destroy);
    return nullptr;
}

void Sink::OnItemWash() {
    SDL_Log("TRY WASH THE DISHES");
    if (dirtyPlateCount < 1) return;

    washLevel++;

    mProgressBar->SetShow(true);
    mProgressBar->SetProgress(washLevel / (double)WASH_LEVEL_MAX);

    if (washLevel == WASH_LEVEL_MAX) {
        // Send clean plate to the drainer
        if (mDrainer == nullptr) return;
        dirtyPlateCount--;
        mDrainer->mCleanPlateCount++;
    }
}