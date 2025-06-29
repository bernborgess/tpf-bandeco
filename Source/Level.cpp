#include "Level.h"

#include "Game.h"
#include "UIElements/UIScreen.h"

Level::Level(Game* game, int levelWidth, int levelHeight)
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