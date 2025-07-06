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
    TileFoodTomato = 24,
    TileTable = 6,
    TileTableCut = 7,
    TileTrash = 8,
    TileSink = 9,
    TileDeliver = 10,
    TilePlayerBStart = 41,
    TilePlayerDStart = 40,
    TileStove = 13,
    TileTablePlate = 14,
    TileTableLeft = 15,
    TileTableRight = 16,
    TileTableCenter = 17,
    TileTablePlain = 18,
    TileStovePotTomatoSoup = 19,
    TileStovePot = 22,
    TileTableCutRight = 23,
    TileDeliverDownRight = 25,
    TileDeliverDownLeft = 26,
    TileCeil = 27,
    TileCeilCorner = 28,
    TileCeilRotate = 29,
    TileDishDrainer = 31
};

class Level {
   public:
    Level(class Game* game, int levelWidth, int levelHeight);

    void LoadMainMenu();

    void LoadHowToPlay();

    // Shows how the player performed
    void LoadLevelResult();

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
