
#pragma once
#include <SDL.h>

#include <unordered_map>
#include <vector>

#include "AudioSystem.h"
#include "Blocks/Block.h"
#include "Level.h"
#include "Math.h"
#include "OrderManager.h"

class Game {
   public:
    static const int LEVEL_WIDTH = 25;
    static const int LEVEL_HEIGHT = 14;
    static const int TILE_SIZE = 64;
    static const int TRANSITION_TIME = 1;

    enum class GameScene { MainMenu, Level1, Level2 };

    enum class SceneManagerState { None, Entering, Active, Exiting };

    enum class GamePlayState {
        Playing,
        Paused,
        GameOver,
        LevelComplete,
        Leaving
    };

    Game(int windowWidth, int windowHeight);

    bool Initialize();
    void RunLoop();
    void Shutdown();
    void Quit() { mIsRunning = false; }

    // Actor functions
    void UpdateActors(float deltaTime);
    void AddActor(class Actor *actor);
    void RemoveActor(class Actor *actor);
    void ProcessInputActors();
    void HandleKeyPressActors(const int key, const bool isPressed);

    std::vector<class AABBColliderComponent *> GetNearbyColliders(
        const Vector2 &position, const int range = 2);

    void Reinsert(Actor *actor);

    // Camera functions
    Vector2 &GetCameraPos() { return mCameraPos; };
    void SetCameraPos(const Vector2 &position) { mCameraPos = position; };

    // Audio functions
    class AudioSystem *GetAudio() { return mAudio; }

    // UI functions
    void PushUI(class UIScreen *screen) { mUIStack.emplace_back(screen); }
    const std::vector<class UIScreen *> &GetUIStack() { return mUIStack; }

    // Window functions
    int GetWindowWidth() const { return mWindowWidth; }
    int GetWindowHeight() const { return mWindowHeight; }
    SDL_Renderer *GetRenderer() { return mRenderer; }

    // Loading functions
    class UIFont *LoadFont(const std::string &fileName);
    SDL_Texture *LoadTexture(const std::string &texturePath);

    void SetGameScene(GameScene scene, float transitionTime = .0f);
    void ResetGameScene(float transitionTime = .0f);
    void UnloadScene();

    void SetBackgroundImage(const std::string &imagePath,
                            const Vector2 &position = Vector2::Zero,
                            const Vector2 &size = Vector2::Zero);
    void TogglePause();

    // Game-specific
    const class Player *GetPlayerB() { return mPlayerB; }
    const class Player *GetPlayerD() { return mPlayerD; }
    OrderManager &GetOrderManager() { return mOrderManager; }
    std::pair<LevelTile, Block *> GetLevelTileAt(int x, int y) {
        return mLevelManager.GetLevelTileAt(x, y);
    }
    void GivePoints(int points);

    int GetPoints() { return mLevelPoints; }

    void SetGamePlayState(GamePlayState state) { mGamePlayState = state; }
    GamePlayState GetGamePlayState() const { return mGamePlayState; }

   private:
    void ProcessInput();
    void UpdateGame();
    void UpdateCamera();
    void GenerateOutput();

    // Scene Manager
    void UpdateSceneManager(float deltaTime);
    void ChangeScene();
    SceneManagerState mSceneManagerState;
    float mSceneManagerTimer;

    // HUD functions
    void UpdateLevelTime(float deltaTime);

    // Load the level from a CSV file as a 2D array
    friend class Level;
    Level mLevelManager;

    // Spatial Hashing for collision detection
    class SpatialHashing *mSpatialHashing;

    // All the UI elements
    std::vector<class UIScreen *> mUIStack;
    std::unordered_map<std::string, class UIFont *> mFonts;

    // SDL stuff
    SDL_Window *mWindow;
    SDL_Renderer *mRenderer;
    AudioSystem *mAudio;

    // Window properties
    int mWindowWidth;
    int mWindowHeight;

    // Track elapsed time since game start
    Uint32 mTicksCount;

    // Track actors state
    bool mIsRunning;
    GamePlayState mGamePlayState;

    // Track level state
    GameScene mGameScene;
    GameScene mNextScene;

    // Background and camera
    Vector3 mBackgroundColor;
    Vector3 mModColor;
    Vector2 mCameraPos;

    // Game-specific
    class Player *mPlayerB;
    class Player *mPlayerD;
    OrderManager mOrderManager;

    class HUD *mHUD;
    SoundHandle mMusicHandle;

    float mGameTimer;
    int mGameTimeLimit;
    int mLevelPoints;
    bool mLevelOver;

    SDL_Texture *mBackgroundTexture;
    Vector2 mBackgroundSize;
    Vector2 mBackgroundPosition;
};
