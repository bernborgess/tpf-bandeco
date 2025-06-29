#pragma once

enum class LevelDataEntry {
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

   private:
    class Game* mGame;

    const int LEVEL_WIDTH;
    const int LEVEL_HEIGHT;
    const int TILE_SIZE = 64;
};

// Defines LevelDataEntry
// LoadLevel
// LoadMainMenu
// GetBlockAt
// ReadLevelData
// BuildLevel
