#pragma once

#include <string>
#include <vector>

#include "Blocks/Block.h"

enum class LevelTile {
    TileNothing = -1,
    TileWall = 1,
    TileFoodBread = 2,
    TileFoodLettuce = 3,
    TileFoodMeat = 4,
    TileFoodTomato = 5,
    TileTable = 6,
    TileTableCut = 7,
    TileTrash = 8,
    TileSink = 9,
    TileDeliver = 10,
    TilePlayerBStart = 11,
    TilePlayerDStart = 12,
    TileStove = 13,
    TileTablePlate = 14
};

class Level {
   public:
    Level(class Game* game, int levelWidth, int levelHeight);

    void LoadMainMenu();

    // Loads the level, populating the tiles and returns a pair with the
    // instances for both players
    void LoadLevel(const std::string& levelName, const int levelWidth,
                   const int levelHeight);

    std::pair<LevelTile, Block*> GetLevelTileAt(int x, int y);

   private:
    LevelTile** ReadLevelData(const std::string& fileName, int width,
                              int height);
    void BuildTile(LevelTile& tile, int x, int y);
    void BuildLevel(int width, int height);

    // Helper to avoid hard coding paths in multiple places
    void NewDecorativeBlock(LevelTile tile, std::pair<int, int> gridPos);

    class Game* mGame;
    LevelTile** mLevelData;
    std::vector<Block*> mLevelBlocks;
    int LEVEL_WIDTH;
    int LEVEL_HEIGHT;
    const int TILE_SIZE = 64;
};
