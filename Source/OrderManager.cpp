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
}

void OrderManager::AddOrder(Order order) { mPlannedOrders.push(order); }

void OrderManager::TimeTick(int time) {
    if (mPlannedOrders.empty()) return;
    Order firstOrder = mPlannedOrders.top();
    if (firstOrder.startTime >= time) {
        mPlannedOrders.pop();
        // Maybe add some animation to this transition
        // TODO: Play a sound?
        mCurrentOrders.push_back(firstOrder);
        mUIChanged = true;
    }

    // Shouldn't happen
    if (!mOrderQueueScreen) return;

    // No need to redraw
    if (!mUIChanged) return;

    // Redraw the orders
    mOrderQueueScreen->mImages.clear();
    int i = 0;
    for (auto& k : mCurrentOrders) {
        mOrderQueueScreen->AddImage("../Assets/Recipes/TomatoSoup.png",
                                    Vector2(16 + 144 * i, 16),
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
