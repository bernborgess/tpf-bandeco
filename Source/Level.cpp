#include "Level.h"

#include <fstream>
#include <iomanip>
#include <sstream>

// Blocks
#include "Blocks/Block.h"
#include "Blocks/Cabinet.h"
#include "Blocks/Deliver.h"
#include "Blocks/Drainer.h"
#include "Blocks/FoodBox.h"
#include "Blocks/Sink.h"
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

    mainMenu->AddImage("../Assets/Prototype/MainMenuBackground.png",
                       Vector2::Zero, Vector2(1792, 1024));

    mainMenu->AddImage("../Assets/Sprites/OvercookedLogo.png", Vector2(200, 40),
                       Vector2(1189, 270));

    mainMenu->AddText("Pesadelo no Bandeco!", Vector2(625, 245),
                      Vector2(600, 100), Color::Black);

    mainMenu->AddText("Pesadelo no Bandeco!", Vector2(630, 250),
                      Vector2(600, 100), Color::Yellow);

    auto button1 = mainMenu->AddButton(
        "Começar", Vector2(600, 480), Vector2(60 * 6, 90),
        [this]() { mGame->SetGameScene(Game::GameScene::Level1); }, Color::Blue,
        72, 1024, Vector2::Zero, Vector2(200, 80), Color::White);

    auto button2 = mainMenu->AddButton(
        "Como Jogar?", Vector2(600, 600), Vector2(60 * 6, 90),
        [this]() { mGame->SetGameScene(Game::GameScene::HowToPlay); },
        Color::Blue, 72, 1024, Vector2::Zero, Vector2(200, 80), Color::White);
}

void Level::LoadHowToPlay() {
    auto howToPlayScreen = new UIScreen(mGame, "../Assets/Fonts/Chewy.ttf");

    howToPlayScreen->AddImage("../Assets/Prototype/MainMenuBackground.png",
                              Vector2::Zero, Vector2(1792, 1024));

    howToPlayScreen->AddImage("../Assets/Sprites/ControlsTitle.png",
                              Vector2(-10, -30), 1.0 * Vector2(1173, 171));

    howToPlayScreen->AddImage("../Assets/Sprites/Controls.png",
                              Vector2(400, 250), 1.5 * Vector2(518, 239));

    howToPlayScreen->AddButton(
        "Voltar", Vector2(640, 700), Vector2(60 * 6, 90),
        [this]() { mGame->SetGameScene(Game::GameScene::MainMenu); },
        Color::Blue, 72, 1024, Vector2(0, 0), Vector2(200, 80), Color::White);
}

void Level::LoadLevelResult() {
    auto resultsScreen = new UIScreen(mGame, "../Assets/Fonts/Chewy.ttf");

    // TODO

    resultsScreen->AddImage("../Assets/Prototype/MainMenuBackground.png",
                            Vector2::Zero, Vector2(1792, 1024));

    resultsScreen->AddImage("../Assets/Prototype/WinLogo.png", Vector2(300, 10),
                            0.5 * Vector2(1920, 321));

    resultsScreen->AddImage("../Assets/Prototype/Coin.png", Vector2(470, 210),
                            Vector2(250, 300));

    int points = mGame->GetPoints();
    std::stringstream ss;
    ss << std::setw(3) << std::setfill('0') << points;

    resultsScreen->AddText(ss.str(), Vector2(508, 300), Vector2(160, 120),
                           Color::Black);
    resultsScreen->AddText(ss.str(), Vector2(514, 304), Vector2(160, 120),
                           Color::White);

    resultsScreen->AddText("PONTOS", Vector2(780, 300), Vector2(300, 120),
                           Color::Blue);

    if (points < 100) {
        resultsScreen->AddText(
            "Você precisa de pelo menos 100 pontos para progredir.",
            Vector2(405, 500), Vector2(900, 100), Color::Black);
        resultsScreen->AddText(
            "Você precisa de pelo menos 100 pontos para progredir.",
            Vector2(400, 500), Vector2(900, 100));
        // TODO: Sons de derrota
    } else {
        resultsScreen->AddText("Parabéns! Você venceu o primeiro desafio!",
                               Vector2(400, 500), Vector2(900, 100));
        // TODO: Sons de vitoria
    }

    resultsScreen->AddButton(
        "Continuar", Vector2(600, 660), Vector2(400, 100),
        [this]() { mGame->SetGameScene(Game::GameScene::MainMenu); },
        Color::Blue, 72, 1024, Vector2::Zero, Vector2(300, 100));
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
        {LevelTile::TileCeil, "../Assets/Prototype/Ceil.png"},
        {LevelTile::TileCeilCorner, "../Assets/Prototype/CeilCorner.png"},
        {LevelTile::TileCeilRotate, "../Assets/Prototype/CeilRotate.png"},
        {LevelTile::TileDishDrainer, "../Assets/Prototype/DishDrainer.png"},
    };

    auto it = tileMap.find(tile);
    if (it != tileMap.end()) {
        Block *block = new Block(mGame, it->second, gridPos);
        mLevelBlocks.push_back(block);
    }
}

void Level::BuildTile(LevelTile &tile, int x, int y, Sink *&theSink) {
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
        case LevelTile::TileFoodBread: {
            FoodBox *fBblock =
                FoodBox::NewFoodBox(mGame, ItemType::Bread, {x, y});
            mLevelBlocks.push_back(fBblock);
            return;
        }
        case LevelTile::TileFoodLettuce: {
            FoodBox *fBblock =
                FoodBox::NewFoodBox(mGame, ItemType::Lettuce, {x, y});
            mLevelBlocks.push_back(fBblock);
            return;
        }
        case LevelTile::TileFoodMeat: {
            FoodBox *fBblock =
                FoodBox::NewFoodBox(mGame, ItemType::Meat, {x, y});
            mLevelBlocks.push_back(fBblock);
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
        case LevelTile::TileStovePan:
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
        case LevelTile::TileDeliver:
        case LevelTile::TileDeliverDownRight:
        case LevelTile::TileDeliverDownLeft: {
            Deliver *deliver = Deliver::NewDeliver(mGame, tile, {x, y});
            mLevelBlocks.push_back(deliver);
            return;
        }
        case LevelTile::TileSink: {
            theSink = Sink::NewSink(mGame, tile, {x, y});
            return;
        }
        case LevelTile::TileDishDrainer: {
            Drainer *drainer = new Drainer(mGame, {x, y});
            // Bind to the sink
            if (theSink) {
                theSink->mDrainer = drainer;
            }
            mLevelBlocks.push_back(drainer);
            return;
        }
        case LevelTile::TileCabinet: {
            Cabinet *cabinet = new Cabinet(mGame, {x, y});
            mLevelBlocks.push_back(cabinet);
            mGame->mOrderManager.mCabinet = cabinet;
            return;
        }
        default: {
            NewDecorativeBlock(tile, {x, y});
        }
    }
}

void Level::BuildLevel(int width, int height) {
    Sink *theSink = nullptr;
    for (int y = 0; y < LEVEL_HEIGHT; ++y) {
        for (int x = 0; x < LEVEL_WIDTH; ++x) {
            LevelTile &tile = mLevelData[y][x];
            BuildTile(tile, x, y, theSink);
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
