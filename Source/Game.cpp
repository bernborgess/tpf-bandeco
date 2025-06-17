
#include "Game.h"

#include <SDL_image.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "Actors/Actor.h"
#include "Actors/Block.h"
#include "Actors/Goomba.h"
#include "Actors/Player.h"
#include "Actors/Spawner.h"
#include "CSV.h"
#include "Components/ColliderComponents/AABBColliderComponent.h"
#include "Components/DrawComponents/DrawComponent.h"

Game::Game(int windowWidth, int windowHeight)
    : mWindow(nullptr),
      mRenderer(nullptr),
      mWindowWidth(windowWidth),
      mWindowHeight(windowHeight),
      mTicksCount(0),
      mIsRunning(true),
      mUpdatingActors(false),
      mMario(nullptr),
      mLuigi(nullptr),
      mLevelData(nullptr) {}

bool Game::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("TP3: Super Mario Bros", 0, 0, mWindowWidth,
                               mWindowHeight, 0);
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

    mTicksCount = SDL_GetTicks();

    // Init all game actors
    InitializeActors();

    return true;
}

void Game::InitializeActors() {
    const auto levelData = LoadLevel("../Assets/Levels/Level1-1/level1-1.csv",
                                     Game::LEVEL_WIDTH, Game::LEVEL_HEIGHT);
    BuildLevel(levelData, Game::LEVEL_WIDTH, Game::LEVEL_HEIGHT);

    // TODO 7.1 (~1 linha): Utilize a função LoadLevel para carregar o primeiro
    // nível (Level1.txt) do jogo.
    //  Note que a classe Game tem constantes LEVEL_WIDTH e LEVEL_HEIGHT que
    //  definem a largura e altura

    // TODO 7.2 (~4 linhas): Verifique se a matriz de tiles foi carregada
    // corretamente. Se não, retorne.
    //  Se foi, chame a função BuildLevel passando a matriz de tiles, a largura
    //  e altura do nível.
}

void Game::applyTexture(std::string *texture_path, const char *str,
                        bool *unset_texture) {
    if (*unset_texture) {
        texture_path->append(str);
        *unset_texture = false;
    }
}

void Game::BuildLevel(int **levelData, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::string texturePath = "../Assets/Sprites/Blocks/Block";
            bool unsetTexture = true;
            switch (levelData[i][j]) {
                case CobbleStoneBlock:
                    applyTexture(&texturePath, "A.png", &unsetTexture);
                case QuestionBlock:
                    applyTexture(&texturePath, "C.png", &unsetTexture);
                case PipeTopLeft:
                    applyTexture(&texturePath, "F.png", &unsetTexture);
                case BrickBlock:
                    applyTexture(&texturePath, "B.png", &unsetTexture);
                case PipeBottomRight:
                    applyTexture(&texturePath, "I.png", &unsetTexture);
                case StoneBlock:
                    applyTexture(&texturePath, "D.png", &unsetTexture);
                case PipeBottomLeft:
                    applyTexture(&texturePath, "H.png", &unsetTexture);
                case PipeTopRight: {
                    applyTexture(&texturePath, "G.png", &unsetTexture);
                    Vector2 pos = Vector2(j * TILE_SIZE, i * TILE_SIZE);
                    auto blk = new Block(this, texturePath,
                                         (TileType)levelData[i][j], pos);
                    break;
                }
                case MarioTile: {
                    if (!mMario) {
                        mMario = new Player(this);
                        mMario->SetPosition(
                            Vector2(j * TILE_SIZE, i * TILE_SIZE));
                    }
                    break;
                }
                case LuigiTile: {
                    if (!mLuigi) {
                        mLuigi = new Player(this, PlayerLuigi);
                        mLuigi->SetPosition(
                            Vector2(j * TILE_SIZE, i * TILE_SIZE));
                    }
                    break;
                }

                case GoombaTile: {
                    auto spawner = new Spawner(this, SPAWN_DISTANCE);
                    spawner->SetPosition(Vector2(j * TILE_SIZE, i * TILE_SIZE));
                }
            }
        }
    }
}

int **Game::LoadLevel(const std::string &fileName, int width, int height) {
    int **M = new int *[height];
    for (int i = 0; i < height; i++) {
        M[i] = new int[width];
    }

    std::ifstream infile(fileName.c_str());
    if (!infile.is_open()) {
        SDL_Log("Couldn't open the %s\n", fileName.c_str());
        return nullptr;
    }

    std::string line;
    for (int row = 0; std::getline(infile, line); row++) {
        std::vector<int> v = CSVHelper::Split(line);
        for (int j = 0; j < width; j++) {
            M[row][j] = v[j];
        }
    }

    return M;
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
            case SDL_QUIT: {
                Quit();
                break;
            }
            case SDL_KEYDOWN: {
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE: {
                        Quit();
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(nullptr);

    for (auto actor : mActors) {
        actor->ProcessInput(state);
    }
}

void Game::UpdateGame() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    mTicksCount = SDL_GetTicks();

    // Update all actors and pending actors
    UpdateActors(deltaTime);

    // Update camera position
    UpdateCamera();
}

void Game::UpdateCamera() {
    float w = mMario->GetPosition().x - float(mWindowWidth) / 2.0f;
    if (mCameraPos.x > w) return;
    mCameraPos.x = Math::Clamp(w, 0.0f, float(LEVEL_WIDTH * TILE_SIZE));
}

void Game::UpdateActors(float deltaTime) {
    mUpdatingActors = true;
    for (auto actor : mActors) {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;

    for (auto pending : mPendingActors) {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    std::vector<Actor *> deadActors;
    for (auto actor : mActors) {
        if (actor->GetState() == ActorState::Destroy) {
            deadActors.emplace_back(actor);
        }
    }

    for (auto actor : deadActors) {
        delete actor;
    }
}

void Game::AddActor(Actor *actor) {
    if (mUpdatingActors) {
        mPendingActors.emplace_back(actor);
    } else {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor *actor) {
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end()) {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end()) {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

void Game::AddDrawable(class DrawComponent *drawable) {
    mDrawables.emplace_back(drawable);

    std::sort(mDrawables.begin(), mDrawables.end(),
              [](DrawComponent *a, DrawComponent *b) {
                  return a->GetDrawOrder() < b->GetDrawOrder();
              });
}

void Game::RemoveDrawable(class DrawComponent *drawable) {
    auto iter = std::find(mDrawables.begin(), mDrawables.end(), drawable);
    mDrawables.erase(iter);
}

void Game::AddCollider(class AABBColliderComponent *collider) {
    mColliders.emplace_back(collider);
}

void Game::RemoveCollider(AABBColliderComponent *collider) {
    auto iter = std::find(mColliders.begin(), mColliders.end(), collider);
    mColliders.erase(iter);
}

void Game::GenerateOutput() {
    // Set draw color to black
    // SDL_SetRenderDrawColor(mRenderer, 107, 140, 255, 255);
    SDL_SetRenderDrawColor(mRenderer, 119, 136, 153, 255);

    // Clear back buffer
    SDL_RenderClear(mRenderer);

    for (auto drawable : mDrawables) {
        if (drawable->IsVisible()) {
            drawable->Draw(mRenderer);
        }
    }

    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);
}

SDL_Texture *Game::LoadTexture(const std::string &texturePath) {
    // --------------
    // TODO - PARTE 1
    // --------------

    // TODO 4.1 (~4 linhas): Utilize a função `IMG_Load` para carregar a imagem
    // passada como parâmetro
    //  `texturePath`. Esse função retorna um ponteiro para `SDL_Surface*`.
    //  Retorne `nullptr` se a imagem não foi carregada com sucesso.
    SDL_Surface *img = IMG_Load(texturePath.c_str());
    if (!img) return nullptr;

    // TODO 4.2 (~6 linhas): Utilize a função `SDL_CreateTextureFromSurface`
    // para criar uma textura a partir
    //  da imagem carregada anteriormente. Essa função retorna um ponteiro para
    //  `SDL_Texture*`. Logo após criar a textura, utilize a função
    //  `SDL_FreeSurface` para liberar a imagem carregada. Se a textura foi
    //  carregada com sucesso, retorne o ponteiro para a textura. Caso
    //  contrário, retorne `nullptr`.
    SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer, img);
    SDL_FreeSurface(img);
    return texture;
}

void Game::Shutdown() {
    // Delete actors
    while (!mActors.empty()) {
        delete mActors.back();
    }

    // Delete level data
    if (mLevelData != nullptr) {
        for (int i = 0; i < LEVEL_HEIGHT; ++i) {
            if (mLevelData[i] != nullptr) delete[] mLevelData[i];
        }
    }
    delete[] mLevelData;

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
