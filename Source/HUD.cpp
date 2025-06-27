#include "HUD.h"

#include <iomanip>
#include <sstream>

#include "Game.h"
#include "UIElements/UIText.h"

HUD::HUD(class Game *game, const std::string &fontName)
    : UIScreen(game, fontName) {
    // TODO: Coins counter
    mTimeText = AddText("", Vector2(1260, 820), Vector2(100, 80), Color::Blue);
    mLevelName = AddText("", Vector2(1000, 6), Vector2(360, 60), Color::Blue);
}

HUD::~HUD() {}

void HUD::SetTime(int time) {
    int minutes = time / 60;
    int second = time % 60;
    std::stringstream ss;
    ss << std::setw(1) << std::setfill('0') << minutes << ':';
    ss << std::setw(2) << std::setfill('0') << second;
    std::string time_string = ss.str();
    mTimeText->SetText(time_string);
}

void HUD::SetLevelName(const std::string &levelName) {
    mLevelName->SetText(levelName);
}