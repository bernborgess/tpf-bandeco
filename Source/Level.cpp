#include "Level.h"

#include <fstream>

// Blocks
#include "Blocks/Block.h"
#include "Blocks/Deliver.h"
#include "Blocks/FoodBox.h"
#include "Blocks/Stove.h"
#include "Blocks/Table.h"
#include "Blocks/TableCut.h"
#include "Blocks/Trash.h"

// Player
#include "Actors/Plate.h"
#include "Actors/Player.h"
#include "CSV.h"
#include "Game.h"
#include "UIElements/UIScreen.h"

Level::Level(Game *game, int levelWidth, int levelHeight)
    : mGame(game), LEVEL_WIDTH(levelWidth), LEVEL_HEIGHT(levelHeight) {}

void Level::LoadMainMenu() {
    auto mainMenu = new UIScreen(mGame, "../Assets/Fonts/Chewy.ttf");
    const Vector2 titleSize = Vector2(178.0f, 88.0f) * 2.0f;

    const int windowWidth = mGame->GetWindowWidth();
    const Vector2 titlePos =
        Vector2(windowWidth / 2.0f - titleSize.x / 2.0f, 50.0f);

    // TODO: Main menu
    // mainMenu->AddImage("../Assets/Sprites/Logo.png", titlePos, titleSize);

    mainMenu->AddText("Overcooked!", Vector2(400, 60), Vector2(800, 200),
                      Color::Blue);
    mainMenu->AddText("Pesadelo no Bandeco!", Vector2(600, 225),
                      Vector2(600, 100));

    mainMenu->AddText("Versão PlayTest", Vector2(400, 300), Vector2(300, 80),
                      Color::Red);

    auto button1 = mainMenu->AddButton(
        "Começar", Vector2(600, 400), Vector2(60 * 6, 90),
        [this]() { mGame->SetGameScene(Game::GameScene::Level1); },
        Color::Black, 72, 1024, Vector2::Zero, Vector2(200, 80), Color::White);

    auto button2 = mainMenu->AddButton(
        "Como Jogar?", Vector2(600, 500), Vector2(60 * 6, 90),
        [this]() { SDL_Log("TODO: How to play"); }, Color::Black, 72, 1024,
        Vector2::Zero, Vector2(200, 80), Color::White);
}

void Level::LoadLevel(const std::string &levelName, const int levelWidth,
                      const int levelHeight) {
    // Load level data
    mLevelData = ReadLevelData(levelName, levelWidth, levelHeight);

    if (!mLevelData) {
        SDL_Log("Failed to load level data");
        return;
    }

    // Instantiate level actors
    BuildLevel(levelWidth, levelHeight);
}

void Level::NewDecorativeBlock(LevelTile tile, std::pair<int, int> gridPos) {
    const std::map<LevelTile, const std::string> tileMap = {
        {LevelTile::TileWall, "../Assets/Prototype/Wall.png"},
        {LevelTile::TileSink, "../Assets/Prototype/Sink.png"},
    };

    auto it = tileMap.find(tile);
    if (it != tileMap.end()) {
        Block *block = new Block(mGame, it->second, gridPos);
        mLevelBlocks.push_back(block);
    }
}

void Level::BuildTile(LevelTile &tile, int x, int y) {
    switch (tile) {
        case LevelTile::TilePlayerBStart: {
            if (mGame->mPlayerB) return;
            mGame->mPlayerB = new Player(mGame, PlayerType::PlayerB);
            mGame->mPlayerB->SetPosition(Vector2(x * TILE_SIZE, y * TILE_SIZE));
            return;
        }
        case LevelTile::TilePlayerDStart: {
            if (mGame->mPlayerD) return;
            mGame->mPlayerD = new Player(mGame, PlayerType::PlayerD);
            mGame->mPlayerD->SetPosition(Vector2(x * TILE_SIZE, y * TILE_SIZE));
            return;
        }
        case LevelTile::TileFoodTomato: {
            FoodBox *fBblock =
                FoodBox::NewFoodBox(mGame, ItemType::Tomato, {x, y});
            mLevelBlocks.push_back(fBblock);
            return;
        }
        case LevelTile::TileTable:
        case LevelTile::TileTablePlate:
        case LevelTile::TileTableLeft:
        case LevelTile::TileTableRight:
        case LevelTile::TileTableCenter:
        case LevelTile::TileTablePlain: {
            Table *table = Table::NewTable(mGame, tile, {x, y});
            mLevelBlocks.push_back(table);
            tile = LevelTile::TileTable;
            return;
        }
        case LevelTile::TileTableCut:
        case LevelTile::TileTableCutRight: {
            TableCut *tableCut = TableCut::NewTableCut(mGame, tile, {x, y});
            mLevelBlocks.push_back(tableCut);
            tile = LevelTile ::TileTableCut;
            return;
        }
        case LevelTile::TileStove:
        case LevelTile::TileStovePotTomatoSoup:
        case LevelTile::TileStovePot: {
            Stove *stove = Stove::NewStove(mGame, tile, {x, y});
            mLevelBlocks.push_back(stove);
            tile = LevelTile::TileStove;
            return;
        }
        case LevelTile::TileTrash: {
            Trash *trash = new Trash(mGame, {x, y});
            mLevelBlocks.push_back(trash);
            return;
        }
        case LevelTile::TileDeliver: {
            Deliver *deliver = Deliver::NewDeliver(mGame, tile, {x, y});
            mLevelBlocks.push_back(deliver);
            return;
        }
        default: {
            NewDecorativeBlock(tile, {x, y});
        }
    }
}

void Level::BuildLevel(int width, int height) {
    for (int y = 0; y < LEVEL_HEIGHT; ++y) {
        for (int x = 0; x < LEVEL_WIDTH; ++x) {
            LevelTile &tile = mLevelData[y][x];
            BuildTile(tile, x, y);
        }
    }
}

LevelTile **Level::ReadLevelData(const std::string &fileName, int width,
                                 int height) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        SDL_Log("Failed to load paths: %s", fileName.c_str());
        return nullptr;
    }

    // Create a 2D array of size width and height to store the level data
    int **levelData = new int *[height];
    for (int i = 0; i < height; ++i) {
        levelData[i] = new int[width];
    }

    // Read the file line by line
    int row = 0;

    std::string line;
    while (!file.eof()) {
        std::getline(file, line);
        if (!line.empty()) {
            auto tiles = CSVHelper::Split(line);

            if (tiles.size() != width) {
                SDL_Log("Invalid level data");
                return nullptr;
            }

            for (int i = 0; i < width; ++i) {
                levelData[row][i] = tiles[i];
            }
        }

        ++row;
    }

    // Close the file
    file.close();

    return (LevelTile **)levelData;
}

std::pair<LevelTile, Block *> Level::GetLevelTileAt(int x, int y) {
    if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) {
        return {LevelTile::TileNothing, nullptr};
    }

    LevelTile tile = mLevelData[y][x];
    for (Block *block : mLevelBlocks) {
        auto [bx, by] = block->GetGridPosition();
        if (x == bx && y == by) {
            return {tile, block};
        }
    }
    return {tile, nullptr};
}
