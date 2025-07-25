
#pragma once

#include <cstdint>
#include <list>
#include <string>
#include <vector>

#include "../Math.h"
#include "UIButton.h"
#include "UIImage.h"
#include "UIText.h"

class UIScreen {
   public:
    // Tracks if the UI is active or closing
    enum class UIState { Active, Closing };

    UIScreen(class Game *game, const std::string &fontName);
    virtual ~UIScreen();

    // UIScreen subclasses can override these
    virtual void Update(float deltaTime);
    virtual void Draw(class SDL_Renderer *renderer);
    virtual void ProcessInput(const uint8_t *keys);
    virtual void HandleKeyPress(int scanCode);

    // Set state to closing
    void Close();

    // Get state of UI screen
    UIState GetState() const { return mState; }

    // Game getter
    class Game *GetGame() { return mGame; }

    // Add a button to this screen
    UIButton *AddButton(const std::string &name, const Vector2 &pos, const Vector2 &dims,
                        std::function<void()> onClick,
                        const Vector3 &backgroundColor = Color::Green, int pointSize = 40,
                        int wrapLength = 1024, const Vector2 &textPos = Vector2::Zero,
                        const Vector2 &textSize = Vector2(140, 20),
                        const Vector3 &textColor = Color::White);
    UIText *AddText(const std::string &name, const Vector2 &pos, const Vector2 &dims,
                    const Vector3 &color = Color::White, const int pointSize = 72,
                    const int unsigned wrapLength = 1024);
    UIImage *AddImage(const std::string &imagePath, const Vector2 &pos, const Vector2 &dims,
                      const Vector3 &color = Color::White);

    // Sets the mouse mode to relative or not
    class Game *mGame;
    class UIFont *mFont;

    // Configure positions
    Vector2 mPos;
    Vector2 mSize;

    // State
    UIState mState;

    // List of buttons, texts, and images
    int mSelectedButtonIndex;
    std::vector<UIButton *> mButtons;
    std::vector<UIText *> mTexts;
    std::vector<UIImage *> mImages;
};
