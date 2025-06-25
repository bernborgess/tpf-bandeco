#include "Game.h"

#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "Actors/Actor.h"
#include "Actors/Block.h"
#include "Actors/Player.h"
#include "Actors/Spawner.h"
#include "CSV.h"
#include "Components/ColliderComponents/AABBColliderComponent.h"
#include "Components/DrawComponents/DrawComponent.h"
#include "Components/DrawComponents/DrawPolygonComponent.h"
#include "Components/DrawComponents/DrawSpriteComponent.h"
#include "HUD.h"
#include "Random.h"
#include "SpatialHashing.h"
#include "UIElements/UIScreen.h"

Game::Game(int windowWidth, int windowHeight)
    : mWindow(nullptr),
      mRenderer(nullptr),
      mTicksCount(0),
      mIsRunning(true),
      mWindowWidth(windowWidth),
      mWindowHeight(windowHeight),
      mPlayer(nullptr),
      mHUD(nullptr),
      mBackgroundColor(0, 0, 0),
      mModColor(255, 255, 255),
      mCameraPos(Vector2::Zero),
      mAudio(nullptr),
      mGameTimer(0.0f),
      mGameTimeLimit(0),
      mSceneManagerTimer(0.0f),
      mSceneManagerState(SceneManagerState::None),
      mGameScene(GameScene::MainMenu),
      mNextScene(GameScene::MainMenu),
      mBackgroundTexture(nullptr),
      mBackgroundSize(Vector2::Zero),
      mBackgroundPosition(Vector2::Zero) {}

bool Game::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow =
        SDL_CreateWindow("Pesadelo no Bandeco!", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, mWindowWidth, mWindowHeight, 0
                         // TODO: SDL_WINDOW_FULLSCREEN);
        );
    if (!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(
        mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
        return false;
    }

    // Initialize SDL_ttf
    if (TTF_Init() != 0) {
        SDL_Log("Failed to initialize SDL_ttf");
        return false;
    }

    mAudio = new AudioSystem();

    // Start random number generator
    Random::Init();

    mSpatialHashing = new SpatialHashing(
        TILE_SIZE * 4.0f, LEVEL_WIDTH * TILE_SIZE, LEVEL_HEIGHT * TILE_SIZE);
    mTicksCount = SDL_GetTicks();

    // Init all game actors
    SetGameScene(GameScene::MainMenu);

    ChangeScene();

    return true;
}

void Game::SetGameScene(Game::GameScene scene, float transitionTime) {
    if (mSceneManagerState != SceneManagerState::None) {
        SDL_Log("Incorrect state");
        return;
    }

    mNextScene = scene;
    mSceneManagerState = SceneManagerState::Entering;
    mSceneManagerTimer = transitionTime;
}

void Game::ResetGameScene(float transitionTime) {
    SetGameScene(mGameScene, transitionTime);
}

void Game::ChangeScene() {
    // Unload current Scene
    UnloadScene();

    // Reset camera position
    mCameraPos.Set(0.0f, 0.0f);

    // Reset game timer
    mGameTimer = 0.0f;

    // Reset gameplay state
    mGamePlayState = GamePlayState::Playing;

    // Reset scene manager state
    mSpatialHashing = new SpatialHashing(
        TILE_SIZE * 4.0f, LEVEL_WIDTH * TILE_SIZE, LEVEL_HEIGHT * TILE_SIZE);

    // Scene Manager FSM: using if/else instead of switch
    if (mNextScene == GameScene::MainMenu) {
        // Set background color
        mBackgroundColor.Set(107.0f, 140.0f, 255.0f);
        SDL_SetRenderDrawColor(mRenderer, mBackgroundColor.x,
                               mBackgroundColor.y, mBackgroundColor.z, 255);

        // Initialize main menu actors
        LoadMainMenu();
    } else if (mNextScene == GameScene::Level1) {
        mHUD = new HUD(this, "../Assets/Fonts/SMB.ttf");
        mGameTimeLimit = 400;
        mHUD->SetTime(400);
        mHUD->SetLevelName("1-1");

        mMusicHandle = mAudio->PlaySound("MusicMain.ogg", true);

        // Set background color
        mBackgroundColor.Set(250.0f, 175.0f, 72.0f);
        SDL_SetRenderDrawColor(mRenderer, mBackgroundColor.x,
                               mBackgroundColor.y, mBackgroundColor.z, 255);

        // Set background color
        SetBackgroundImage("../Assets/Sprites/Background.png",
                           Vector2(TILE_SIZE, 0), Vector2(6784, 448));

        // Draw Flag
        auto flag = new Actor(this);
        flag->SetPosition(Vector2(
            LEVEL_WIDTH * TILE_SIZE - (16 * TILE_SIZE) - 16, 3 * TILE_SIZE));

        // Initialize actors
        LoadLevel("../Assets/Levels/level1-1.csv", LEVEL_WIDTH, LEVEL_HEIGHT);
    } else if (mNextScene == GameScene::Level2) {
        mHUD = new HUD(this, "../Assets/Fonts/SMB.ttf");
        mGameTimeLimit = 400;
        mHUD->SetTime(400);
        mHUD->SetLevelName("1-2");

        mMusicHandle = mAudio->PlaySound("MusicUnderground.ogg", true);

        // Set background color
        mBackgroundColor.Set(0.0f, 0.0f, 0.0f);

        // Initialize actors
        LoadLevel("../Assets/Levels/level1-2.csv", LEVEL_WIDTH, LEVEL_HEIGHT);
    }

    // Set new scene
    mGameScene = mNextScene;
}

void Game::LoadMainMenu() {
    auto mainMenu = new UIScreen(this, "../Assets/Fonts/SMB.ttf");
    const Vector2 titleSize = Vector2(178.0f, 88.0f) * 2.0f;
    const Vector2 titlePos =
        Vector2(mWindowWidth / 2.0f - titleSize.x / 2.0f, 50.0f);
    mainMenu->AddImage("../Assets/Sprites/Logo.png", titlePos, titleSize);

    mainMenu->AddText("1985 NINTENDO", Vector2(300, 225), Vector2(200, 18));

    mainMenu->AddText("TOP - 000000", Vector2(230, 380), Vector2(180, 18));

    mainMenu->AddImage("../Assets/Sprites/Bush.png", Vector2(350, 370),
                       1.2 * Vector2(255, 86));

    auto button1 = mainMenu->AddButton(
        "Level 1", Vector2(mWindowWidth / 2.0f - 100.0f, 270.0f),
        Vector2(200.0f, 40.0f), [this]() { SetGameScene(GameScene::Level1); });

    auto button2 = mainMenu->AddButton(
        "Level 2", Vector2(mWindowWidth / 2.0f - 100.0f, 320.0f),
        Vector2(200.0f, 40.0f), [this]() { SetGameScene(GameScene::Level2); });
}

void Game::LoadLevel(const std::string &levelName, const int levelWidth,
                     const int levelHeight) {
    // Load level data
    int **mLevelData = ReadLevelData(levelName, levelWidth, levelHeight);

    if (!mLevelData) {
        SDL_Log("Failed to load level data");
        return;
    }

    // Instantiate level actors
    BuildLevel(mLevelData, levelWidth, levelHeight);
}

void Game::BuildLevel(int **levelData, int width, int height) {
    // Const map to convert tile ID to block type
    const std::map<int, const std::string> tileMap = {
        {0, "../Assets/Sprites/Blocks/BlockA.png"},
        {1, "../Assets/Sprites/Blocks/BlockC.png"},
        {2, "../Assets/Sprites/Blocks/BlockF.png"},
        {4, "../Assets/Sprites/Blocks/BlockB.png"},
        {6, "../Assets/Sprites/Blocks/BlockI.png"},
        {8, "../Assets/Sprites/Blocks/BlockD.png"},
        {9, "../Assets/Sprites/Blocks/BlockH.png"},
        {12, "../Assets/Sprites/Blocks/BlockG.png"}};

    for (int y = 0; y < LEVEL_HEIGHT; ++y) {
        for (int x = 0; x < LEVEL_WIDTH; ++x) {
            int tile = levelData[y][x];

            if (tile == 16)  // Player
            {
                mPlayer = new Player(this);
                mPlayer->SetPosition(Vector2(x * TILE_SIZE, y * TILE_SIZE));
            } else if (tile == 10)  // Spawner
            {
                Spawner *spawner = new Spawner(this, SPAWN_DISTANCE);
                spawner->SetPosition(Vector2(x * TILE_SIZE, y * TILE_SIZE));
            } else  // Blocks
            {
                auto it = tileMap.find(tile);
                if (it != tileMap.end()) {
                    // Create a block actor
                    Block *block = new Block(this, it->second);
                    block->SetPosition(Vector2(x * TILE_SIZE, y * TILE_SIZE));
                }
            }
        }
    }
}

int **Game::ReadLevelData(const std::string &fileName, int width, int height) {
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

    return levelData;
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                Quit();
                break;
            case SDL_KEYDOWN:
                // Handle key press for UI screens
                if (!mUIStack.empty()) {
                    mUIStack.back()->HandleKeyPress(event.key.keysym.sym);
                }

                HandleKeyPressActors(event.key.keysym.sym,
                                     event.key.repeat == 0);

                // Check if the P key has been pressed to pause/unpause the
                // game
                if (event.key.keysym.sym == SDLK_p) {
                    TogglePause();
                }
                // Quickly end the game
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    Quit();
                }
                break;
        }
    }

    ProcessInputActors();
    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    mAudio->ProcessInput(state);
}

void Game::ProcessInputActors() {
    if (mGamePlayState == GamePlayState::Playing) {
        // Get actors on camera
        std::vector<Actor *> actorsOnCamera = mSpatialHashing->QueryOnCamera(
            mCameraPos, mWindowWidth, mWindowHeight);

        const Uint8 *state = SDL_GetKeyboardState(nullptr);

        bool isPlayerOnCamera = false;
        for (auto actor : actorsOnCamera) {
            actor->ProcessInput(state);

            if (actor == mPlayer) {
                isPlayerOnCamera = true;
            }
        }

        // If Player is not on camera, process input for him
        if (!isPlayerOnCamera && mPlayer) {
            mPlayer->ProcessInput(state);
        }
    }
}

void Game::HandleKeyPressActors(const int key, const bool isPressed) {
    if (mGamePlayState == GamePlayState::Playing) {
        // Get actors on camera
        std::vector<Actor *> actorsOnCamera = mSpatialHashing->QueryOnCamera(
            mCameraPos, mWindowWidth, mWindowHeight);

        // Handle key press for actors
        bool isPlayerOnCamera = false;
        for (auto actor : actorsOnCamera) {
            actor->HandleKeyPress(key, isPressed);

            if (actor == mPlayer) {
                isPlayerOnCamera = true;
            }
        }

        // If Player is not on camera, handle key press for him
        if (!isPlayerOnCamera && mPlayer) {
            mPlayer->HandleKeyPress(key, isPressed);
        }
    }
}

void Game::TogglePause() {
    if (mGameScene != GameScene::MainMenu) {
        if (mGamePlayState == GamePlayState::Playing) {
            mGamePlayState = GamePlayState::Paused;

            mAudio->PauseSound(mMusicHandle);
            mAudio->PlaySound("Coin.wav");
        } else if (mGamePlayState == GamePlayState::Paused) {
            mGamePlayState = GamePlayState::Playing;

            mAudio->ResumeSound(mMusicHandle);
            mAudio->PlaySound("Coin.wav");
        }
    }
}

void Game::UpdateGame() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    mTicksCount = SDL_GetTicks();

    if (mGamePlayState != GamePlayState::Paused &&
        mGamePlayState != GamePlayState::GameOver) {
        // Reinsert all actors and pending actors
        UpdateActors(deltaTime);
    }

    // Reinsert audio system
    mAudio->Update(deltaTime);

    // Reinsert UI screens
    for (auto ui : mUIStack) {
        if (ui->GetState() == UIScreen::UIState::Active) {
            ui->Update(deltaTime);
        }
    }

    // Delete any UIElements that are closed
    auto iter = mUIStack.begin();
    while (iter != mUIStack.end()) {
        if ((*iter)->GetState() == UIScreen::UIState::Closing) {
            delete *iter;
            iter = mUIStack.erase(iter);
        } else {
            ++iter;
        }
    }

    // ---------------------
    // Game Specific Updates
    // ---------------------
    UpdateCamera();

    UpdateSceneManager(deltaTime);

    if (mGameScene != GameScene::MainMenu &&
        mGamePlayState == GamePlayState::Playing) {
        UpdateLevelTime(deltaTime);
    }
}

void Game::UpdateSceneManager(float deltaTime) {
    if (mSceneManagerState == SceneManagerState::Entering) {
        mSceneManagerTimer -= deltaTime;
        if (mSceneManagerTimer <= 0) {
            mSceneManagerTimer = TRANSITION_TIME;
            mSceneManagerState = SceneManagerState::Active;
        }
    }
    if (mSceneManagerState == SceneManagerState::Active) {
        mSceneManagerTimer -= deltaTime;
        if (mSceneManagerTimer <= 0) {
            ChangeScene();
            mSceneManagerState = SceneManagerState::None;
            mSceneManagerTimer = TRANSITION_TIME;
        }
    }
}

void Game::UpdateLevelTime(float deltaTime) {
    mGameTimer += deltaTime;
    if (mGameTimer >= 1.0) {
        mGameTimer = 0.0f;
        mGameTimeLimit -= 1;
        mHUD->SetTime(mGameTimeLimit);
        if (mGameTimeLimit <= 0) {
            mPlayer->Kill();
        }
    }
}

void Game::UpdateCamera() {
    if (!mPlayer) return;

    float horizontalCameraPos =
        mPlayer->GetPosition().x - (mWindowWidth / 2.0f);

    float maxCameraPos = (LEVEL_WIDTH * TILE_SIZE) - mWindowWidth;
    horizontalCameraPos = Math::Clamp(horizontalCameraPos, 0.0f, maxCameraPos);

    // Not following player
    // mCameraPos.x = horizontalCameraPos;
}

void Game::UpdateActors(float deltaTime) {
    // Get actors on camera
    std::vector<Actor *> actorsOnCamera =
        mSpatialHashing->QueryOnCamera(mCameraPos, mWindowWidth, mWindowHeight);

    bool isPlayerOnCamera = false;
    for (auto actor : actorsOnCamera) {
        actor->Update(deltaTime);
        if (actor == mPlayer) {
            isPlayerOnCamera = true;
        }
    }

    // If Player is not on camera, reset camera position
    if (!isPlayerOnCamera && mPlayer) {
        mPlayer->Update(deltaTime);
    }

    for (auto actor : actorsOnCamera) {
        if (actor->GetState() == ActorState::Destroy) {
            delete actor;
            if (actor == mPlayer) {
                mPlayer = nullptr;
            }
        }
    }
}

void Game::AddActor(Actor *actor) { mSpatialHashing->Insert(actor); }

void Game::RemoveActor(Actor *actor) { mSpatialHashing->Remove(actor); }
void Game::Reinsert(Actor *actor) { mSpatialHashing->Reinsert(actor); }

std::vector<Actor *> Game::GetNearbyActors(const Vector2 &position,
                                           const int range) {
    return mSpatialHashing->Query(position, range);
}

std::vector<AABBColliderComponent *> Game::GetNearbyColliders(
    const Vector2 &position, const int range) {
    return mSpatialHashing->QueryColliders(position, range);
}

void Game::GenerateOutput() {
    // Clear frame with background color
    SDL_SetRenderDrawColor(mRenderer, mBackgroundColor.x, mBackgroundColor.y,
                           mBackgroundColor.z, 255);

    // Clear back buffer
    SDL_RenderClear(mRenderer);

    // Draw background texture considering camera position
    if (mBackgroundTexture) {
        SDL_Rect dstRect = {
            static_cast<int>(mBackgroundPosition.x - mCameraPos.x),
            static_cast<int>(mBackgroundPosition.y - mCameraPos.y),
            static_cast<int>(mBackgroundSize.x),
            static_cast<int>(mBackgroundSize.y)};

        SDL_RenderCopy(mRenderer, mBackgroundTexture, nullptr, &dstRect);
    }

    // Get actors on camera
    std::vector<Actor *> actorsOnCamera =
        mSpatialHashing->QueryOnCamera(mCameraPos, mWindowWidth, mWindowHeight);

    // Get list of drawables in draw order
    std::vector<DrawComponent *> drawables;

    for (auto actor : actorsOnCamera) {
        auto drawable = actor->GetComponent<DrawComponent>();
        if (drawable && drawable->IsVisible()) {
            drawables.emplace_back(drawable);
        }
    }

    // Sort drawables by draw order
    std::sort(drawables.begin(), drawables.end(),
              [](const DrawComponent *a, const DrawComponent *b) {
                  return a->GetDrawOrder() < b->GetDrawOrder();
              });

    // Draw all drawables
    for (auto drawable : drawables) {
        drawable->Draw(mRenderer, mModColor);
    }

    // Draw all UI screens
    for (auto ui : mUIStack) {
        ui->Draw(mRenderer);
    }

    if (mSceneManagerState == SceneManagerState::Active) {
        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
        SDL_Rect bigRect = {
            .x = 0, .y = 0, .w = mWindowWidth, .h = mWindowHeight};
        SDL_RenderFillRect(mRenderer, &bigRect);
    }

    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);
}

void Game::SetBackgroundImage(const std::string &texturePath,
                              const Vector2 &position, const Vector2 &size) {
    if (mBackgroundTexture) {
        SDL_DestroyTexture(mBackgroundTexture);
        mBackgroundTexture = nullptr;
    }

    // Load background texture
    mBackgroundTexture = LoadTexture(texturePath);
    if (!mBackgroundTexture) {
        SDL_Log("Failed to load background texture: %s", texturePath.c_str());
    }

    // Set background position
    mBackgroundPosition.Set(position.x, position.y);

    // Set background size
    mBackgroundSize.Set(size.x, size.y);
}

SDL_Texture *Game::LoadTexture(const std::string &texturePath) {
    SDL_Surface *surface = IMG_Load(texturePath.c_str());

    if (!surface) {
        SDL_Log("Failed to load image: %s", IMG_GetError());
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        return nullptr;
    }

    return texture;
}

UIFont *Game::LoadFont(const std::string &fileName) {
    if (mFonts.count(fileName) != 0) {
        return mFonts[fileName];
    }
    auto font = new UIFont(mRenderer);
    if (!font->Load(fileName)) {
        font->Unload();
        delete font;
        return nullptr;
    }
    return mFonts[fileName] = font;
}

void Game::UnloadScene() {
    // Delete actors
    delete mSpatialHashing;

    // Delete UI screens
    for (auto ui : mUIStack) {
        delete ui;
    }
    mUIStack.clear();

    // Delete background texture
    if (mBackgroundTexture) {
        SDL_DestroyTexture(mBackgroundTexture);
        mBackgroundTexture = nullptr;
    }
}

void Game::Shutdown() {
    UnloadScene();

    for (auto font : mFonts) {
        font.second->Unload();
        delete font.second;
    }
    mFonts.clear();

    delete mAudio;
    mAudio = nullptr;

    Mix_CloseAudio();

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
