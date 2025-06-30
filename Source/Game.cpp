#include "Game.h"

#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "Actors/Player.h"
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
      mPlayerB(nullptr),
      mPlayerD(nullptr),
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
      mBackgroundPosition(Vector2::Zero),
      mLevelManager(this, LEVEL_WIDTH, LEVEL_HEIGHT) {}

bool Game::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow(
        "Pesadelo no Bandeco!", 500, 0, mWindowWidth, mWindowHeight, 0
        // SDL_WINDOW_FULLSCREEN
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

    // Scene Manager FSM
    switch (mNextScene) {
        case GameScene::MainMenu: {
            // Set background color
            mBackgroundColor.Set(107.0f, 140.0f, 255.0f);
            SDL_SetRenderDrawColor(mRenderer, mBackgroundColor.x,
                                   mBackgroundColor.y, mBackgroundColor.z, 255);

            // Initialize main menu actors
            mLevelManager.LoadMainMenu();
            break;
        }
        case GameScene::HowToPlay: {
            // Set background color
            mBackgroundColor.Set(107.0f, 140.0f, 255.0f);
            SDL_SetRenderDrawColor(mRenderer, mBackgroundColor.x,
                                   mBackgroundColor.y, mBackgroundColor.z, 255);

            mLevelManager.LoadHowToPlay();
            break;
        }
        case GameScene::Level1: {
            mHUD = new HUD(this, "../Assets/Fonts/Chewy.ttf");
            mGameTimeLimit = 180;  // debug: level time
            mHUD->SetTime(mGameTimeLimit);
            mHUD->SetLevelName("Cantina do ICEx");
            mLevelPoints = 0;  // debug: raise point to win screen
            mLevelOver = false;

            // Add level music
            mMusicHandle = mAudio->PlaySound("a_cozinha.ogg", false);

            // Set background color
            mBackgroundColor.Set(250.0f, 175.0f, 72.0f);
            SDL_SetRenderDrawColor(mRenderer, mBackgroundColor.x,
                                   mBackgroundColor.y, mBackgroundColor.z, 255);

            // Set background image
            SetBackgroundImage("../Assets/Prototype/BackgroundLevel1.png",
                               Vector2(0, 0), Vector2(1600, 900));

            // Adding all the planned orders for this level
            mOrderManager.Clear();
            std::array<int, 10> soupStartTimes = {180, 160, 150, 120, 100,
                                                  90,  70,  50,  30,  10};
            for (int startTime : soupStartTimes) {
                mOrderManager.AddOrder(
                    {.startTime = startTime, .recipe = {ItemType::TomatoSoup}});
            }

            // Initialize actors
            mLevelManager.LoadLevel("../Assets/Levels/level1-1.csv",
                                    LEVEL_WIDTH, LEVEL_HEIGHT);
            break;
        }
        case GameScene::Level2: {
            mHUD = new HUD(this, "../Assets/Fonts/SMB.ttf");
            mGameTimeLimit = 400;
            mHUD->SetTime(400);
            mHUD->SetLevelName("1-2");

            // TODO: Add level music
            // mMusicHandle = mAudio->PlaySound("MusicUnderground.ogg", true);

            // Set background color
            mBackgroundColor.Set(0.0f, 0.0f, 0.0f);

            // Initialize actors
            mLevelManager.LoadLevel("../Assets/Levels/level1-2.csv",
                                    LEVEL_WIDTH, LEVEL_HEIGHT);
            break;
        }
        case GameScene::LevelResult: {
            // Set background color
            mBackgroundColor.Set(107.0f, 140.0f, 255.0f);
            SDL_SetRenderDrawColor(mRenderer, mBackgroundColor.x,
                                   mBackgroundColor.y, mBackgroundColor.z, 255);

            // Initialize main menu actors
            mLevelManager.LoadLevelResult();
            break;
        }
    }

    // Set new scene
    mGameScene = mNextScene;
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
                    mUIStack.back()->HandleKeyPress(event.key.keysym.scancode);
                }

                if (mGameScene == GameScene::Level1 ||
                    mGameScene == GameScene::Level2) {
                    HandleKeyPressActors(event.key.keysym.scancode,
                                         event.key.repeat == 0);
                }

                // Check if the P key has been pressed to pause/unpause the
                // game
                if (event.key.keysym.scancode == SDL_SCANCODE_P) {
                    TogglePause();
                }
                // Quickly end the game
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    Quit();
                }
                break;
        }
    }

    if (mGameScene == GameScene::Level1 || mGameScene == GameScene::Level2) {
        ProcessInputActors();
    }
    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    mAudio->ProcessInput(state);
}

void Game::ProcessInputActors() {
    if (mGamePlayState == GamePlayState::Playing && !mLevelOver) {
        // Get actors on camera
        std::vector<Actor *> actorsOnCamera = mSpatialHashing->QueryOnCamera(
            mCameraPos, mWindowWidth, mWindowHeight);

        const Uint8 *state = SDL_GetKeyboardState(nullptr);

        bool isPlayerOnCamera = false;
        for (auto actor : actorsOnCamera) {
            actor->ProcessInput(state);

            if (actor == mPlayerB) {
                isPlayerOnCamera = true;
            }
        }

        // If Player is not on camera, process input for him
        if (!isPlayerOnCamera && mPlayerB) {
            mPlayerB->ProcessInput(state);
        }
    }
}

void Game::HandleKeyPressActors(const int scanCode, const bool isPressed) {
    if (mGamePlayState == GamePlayState::Playing) {
        // Get actors on camera
        std::vector<Actor *> actorsOnCamera = mSpatialHashing->QueryOnCamera(
            mCameraPos, mWindowWidth, mWindowHeight);

        // Handle key press for actors
        for (auto actor : actorsOnCamera) {
            if (!actor) {
                SDL_Log("Null actor...");
                continue;
            }
            if (actor == mPlayerB || actor == mPlayerD) {
                actor->HandleKeyPress(scanCode, isPressed);
            }
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

void Game::GivePoints(int points) {
    mLevelPoints += points;
    mAudio->PlaySound("Bell.wav");
    mHUD->SetPoints(mLevelPoints);
}

void Game::UpdateGame() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    mTicksCount = SDL_GetTicks();

    if (mGameScene == GameScene::Level1 || mGameScene == GameScene::Level2) {
        if (mGamePlayState != GamePlayState::Paused &&
            mGamePlayState != GamePlayState::GameOver) {
            // Reinsert all actors and pending actors
            UpdateActors(deltaTime);
        }
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

    if ((mGameScene == GameScene::Level1 || mGameScene == GameScene::Level2) &&
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

        if (mGameTimeLimit >= 0) mHUD->SetTime(mGameTimeLimit);

        if (mGameTimeLimit <= 0 && !mLevelOver) {
            // TODO: Show TIME'S UP ui and change the scene
            SDL_Log("LEVEL OVER WITH %d points.", mLevelPoints);
            mLevelOver = true;
            auto levelOver = new UIScreen(this, "../Assets/Fonts/Chewy.ttf");
            levelOver->AddText("ACABOU!", Vector2(360, 360), Vector2(800, 200),
                               Color::Blue);
        }
        mOrderManager.TimeTick(mGameTimeLimit);
        if (mGameTimeLimit <= -5) {
            if (mPlayerB) {
                delete mPlayerB;
                mPlayerB = nullptr;
            }
            if (mPlayerD) {
                delete mPlayerD;
                mPlayerD = nullptr;
            }
            SetGameScene(GameScene::LevelResult);
            SetGamePlayState(GamePlayState::GameOver);
        }
    }
}

void Game::UpdateCamera() {
    if (!mPlayerB) return;

    float horizontalCameraPos =
        mPlayerB->GetPosition().x - (mWindowWidth / 2.0f);

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
        if (actor == mPlayerB) {
            isPlayerOnCamera = true;
        }
    }

    // If Player is not on camera, reset camera position
    if (!isPlayerOnCamera && mPlayerB) {
        mPlayerB->Update(deltaTime);
    }

    for (auto actor : actorsOnCamera) {
        if (actor->GetState() == ActorState::Destroy) {
            delete actor;
            if (actor == mPlayerB) {
                mPlayerB = nullptr;
            }
            if (actor == mPlayerD) {
                mPlayerD = nullptr;
            }
        }
    }
}

void Game::AddActor(Actor *actor) { mSpatialHashing->Insert(actor); }

void Game::RemoveActor(Actor *actor) { mSpatialHashing->Remove(actor); }
void Game::Reinsert(Actor *actor) { mSpatialHashing->Reinsert(actor); }

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

    if (mGameScene == GameScene::Level1 || mGameScene == GameScene::Level2) {
        // Get actors on camera
        std::vector<Actor *> actorsOnCamera = mSpatialHashing->QueryOnCamera(
            mCameraPos, mWindowWidth, mWindowHeight);

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
