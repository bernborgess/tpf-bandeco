#include "OrderManager.h"

#include <SDL.h>

#include <algorithm>

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
        SDL_Log("New order now at %d seconds", time);
        // Move this one
        mPlannedOrders.pop();
        // Maybe add some animation to this transition
        // Play a sound?
        mCurrentOrders.push_back(firstOrder);
    }
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

    // TODO: Calculate points in relation to time
    return 20;
}
