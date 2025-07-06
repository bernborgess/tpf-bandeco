#include "OrderManager.h"

#include <SDL.h>

#include <algorithm>

OrderManager::OrderManager(Game* game)
    : mGame(game), mOrderQueueScreen(nullptr), mUIChanged(false) {}

void OrderManager::Clear() {
    mCurrentOrders.clear();
    while (!mPlannedOrders.empty()) {
        mPlannedOrders.pop();
    }
    mOrderQueueScreen = new UIScreen(mGame, "../Assets/Fonts/Chewy.ttf");
}

void OrderManager::AddOrder(Order order) { mPlannedOrders.push(order); }

void OrderManager::TimeTick(int time) {
    if (mPlannedOrders.empty()) return;
    Order firstOrder = mPlannedOrders.top();
    if (firstOrder.startTime >= time) {
        mPlannedOrders.pop();
        // Maybe add some animation to this transition
        // TODO: Play a sound?
        SDL_Log("New order now");
        mCurrentOrders.push_back(firstOrder);
        mUIChanged = true;
    }

    // Shouldn't happen
    if (!mOrderQueueScreen) {
        SDL_Log("No OrderQueueScreen found!");
        return;
    }

    // No need to redraw
    if (!mUIChanged) return;

    // Redraw the orders
    mOrderQueueScreen->mImages.clear();
    int i = 0;
    SDL_Log("There are %d orders now", mCurrentOrders.size());
    for (auto& k : mCurrentOrders) {
        std::string recipePath = "";
        if (k.recipe == std::set<ItemType>{ItemType::TomatoSoup}) {
            recipePath = "../Assets/Recipes/TomatoSoup.png";
        }
        if (k.recipe ==
            std::set<ItemType>{ItemType::Bread, ItemType::MeatCook}) {
            recipePath = "../Assets/Recipes/Burger.png";
        }
        if (k.recipe == std::set<ItemType>{ItemType::Bread, ItemType::MeatCook,
                                           ItemType::LettuceCut}) {
            recipePath = "../Assets/Recipes/BurgerLettuce.png";
        }
        if (k.recipe == std::set<ItemType>{ItemType::Bread, ItemType::MeatCook,
                                           ItemType::LettuceCut,
                                           ItemType::TomatoCut}) {
            recipePath = "../Assets/Recipes/BurgerLettuceTomato.png";
        }

        if (recipePath == "") {
            SDL_Log("No path found for this recipe");
            continue;
        }

        mOrderQueueScreen->AddImage(recipePath, Vector2(16 + 144 * i, 16),
                                    Vector2(128, 128));
        i++;
    }
    mUIChanged = false;
}

int OrderManager::DeliverOrder(std::set<ItemType> recipe) {
    // Look for some occurrence of recipe
    auto it = mCurrentOrders.begin();
    for (; it != mCurrentOrders.end(); ++it) {
        // This `recipe` is accepted if all ingredients of
        // `it->recipe` are present in `recipe`
        if (it->recipe == recipe) break;
    }

    // Didn't find such recipe!
    if (it == mCurrentOrders.end()) {
        // No points :(
        return 0;
    }

    // A needed order

    // Remove it from the list
    mCurrentOrders.erase(it);
    mUIChanged = true;

    // TODO: Calculate points in relation to time
    return 20;
}
