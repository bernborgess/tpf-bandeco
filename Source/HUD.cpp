#include "HUD.h"

#include <iomanip>
#include <sstream>

#include "Game.h"
#include "UIElements/UIText.h"

HUD::HUD(class Game *game, const std::string &fontName)
    : UIScreen(game, fontName) {
    // Level Time
    AddImage("../Assets/Prototype/Time.png", Vector2(1170, 690),
             Vector2(200, 200));
    mTimeTextAux =
        AddText("", Vector2(1196, 756), Vector2(150, 80), Color::White);
    mTimeText = AddText("", Vector2(1200, 760), Vector2(150, 80), Color::Blue);

    // Level Name
    mLevelNameAux =
        AddText("", Vector2(998, 2), Vector2(360, 60), Color::White);
    mLevelName = AddText("", Vector2(1000, 6), Vector2(360, 60), Color::Blue);

    //  Points
    AddImage("../Assets/Prototype/Coin.png", Vector2(60, 710),
             Vector2(200, 200));
    mPointsCounterAux =
        AddText("  0", Vector2(108, 765), Vector2(80, 80), Color::Blue);
    mPointsCounter =
        AddText("  0", Vector2(112, 769), Vector2(80, 80), Color::White);
}

HUD::~HUD() {}

void HUD::SetTime(int time) {
    int minutes = time / 60;
    int second = time % 60;
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << minutes << ':';
    ss << std::setw(2) << std::setfill('0') << second;
    std::string time_string = ss.str();
    mTimeText->SetText(time_string);
    mTimeTextAux->SetText(time_string);
}

void HUD::SetLevelName(const std::string &levelName) {
    mLevelNameAux->SetText(levelName);
    mLevelName->SetText(levelName);
}

void HUD::SetPoints(int points) {
    std::stringstream ss;
    ss << std::setw(3) << std::setfill(' ') << points;
    std::string points_string = ss.str();
    mPointsCounter->SetText(points_string);
    mPointsCounterAux->SetText(points_string);
}