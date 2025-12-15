// src/main.cpp
#include <iostream>
#include <vector>
#include "../include/core/MatchingEngine.hpp"
#include "../include/utils/ObjectPool.hpp"

int main() {
    // 1. Initialize System
    MatchingEngine engine;
    ObjectPool<Order> order_pool(10000); 
    
    std::cout << "--- Trading Engine Started ---\n";

    // 2. Define a scenario
    struct RawOrder {
        OrderId id;
        Side side;
        Price price;
        Quantity qty;
    };

    // Scenario:
    // 1. Sell 10 @ 100 (Maker)
    // 2. Sell 20 @ 101 (Maker)
    // 3. Buy 10 @ 99  (No match, rests in book)
    // 4. Buy 35 @ 102 (Aggressive - should eat the 100s and 101s)
    std::vector<RawOrder> scenario = {
        {1, Side::SELL, 100, 10}, 
        {2, Side::SELL, 101, 20}, 
        {3, Side::BUY,  99,  10}, 
        {4, Side::BUY,  102, 35}  
    };

    // 3. Process the scenario
    for (const auto& raw : scenario) {
        // Acquire fresh memory from pool
        Order* order = order_pool.acquire(raw.id, raw.side, raw.price, raw.qty);
        
        std::cout << "Processing Order #" << raw.id << " (" 
                  << (raw.side == Side::BUY ? "BUY" : "SELL") 
                  << " " << raw.qty << " @ " << raw.price << ")\n";

        // Pass to Engine
        std::vector<Trade> trades = engine.process_order(order);

        // Report Trades
        for (const auto& trade : trades) {
            std::cout << "  >>> TRADE EXECUTION: Maker #" << trade.maker_order_id 
                      << " matched with Taker #" << trade.taker_order_id 
                      << " | " << trade.quantity << " @ " << trade.price << "\n";
        }
    }

    return 0;
}