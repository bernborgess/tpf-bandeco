#pragma once
#include <list>
#include <queue>

#include "Actors/Item.h"

struct Order {
    int startTime;  // in Seconds to the end of the level
    std::set<ItemType> recipe;
    bool operator<(const Order& other) const {
        return startTime < other.startTime;
    }
};

class OrderManager {
   public:
    OrderManager() {};

    // Clears the planned and current order lists
    void Clear();

    // Add to the `planned orders`, that will emerge when the `startTime`
    // timestamp is reached
    void AddOrder(Order order);

    // Check the time to move orders from `planned orders` to the `current
    // orders`
    void TimeTick(int time);

    // Checks out the first order of this `recipe` and give the points.
    // If it doesn't exits, returns zero points for the failure.
    int DeliverOrder(std::set<ItemType> recipe);

   private:
    std::list<Order> mCurrentOrders;

    // I need a data structure to hold the orders, sorted by start time
    // decreasing, so that I can pick the first one and move it to the
    // `mCurrentOrders` vector
    std::priority_queue<Order> mPlannedOrders;
};