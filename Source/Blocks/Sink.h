#pragma once

#include <string>

#include "../Actors/ProgressBar.h"
#include "Drainer.h"
#include "Table.h"

class Sink : public Table {
   public:
    static Sink* NewSink(Game*, LevelTile tile, std::pair<int, int> gridPos);

    // Changes item if finished cutting
    Item* PickItemOnTop() override;

    Item* SetItemOnTop(Item* item) override;

    void OnItemWash();

    Drainer* mDrainer;

   private:
    Sink(Game* game, const std::string& texturePath,
         std::pair<int, int> gridPos);

    // Pointer to where the plates are sent

    static const int WASH_LEVEL_MAX = 5;
    int washLevel;
    int mDirtyPlateCount;
    int cleanPlateCount;

    void UpdateTexture();

    static const std::string SINK_PATH;
    static const std::string SINK_PLATE_PATH;
    static const std::string SINK_PLATES_PATH;

    ProgressBar* mProgressBar;
};