#include <iostream>
#include <vector>
#include "../include/core/MatchingEngine.hpp"
#include "../include/utils/ObjectPool.hpp"

int main() {
    MatchingEngine engine;
    ObjectPool<Order> order_pool(10000); 
    
    std::cout << "--- Trading Engine Started ---\n";

    struct RawOrder {
        OrderId id;
        Side side;
        Price price;
        Quantity qty;
    };

    // Scenario:
    // 1. Sell 10 @ 100 
    // 2. Sell 20 @ 101 
    // 3. Buy 10 @ 99  
    // 4. Buy 35 @ 102 (Aggressive: Eats #1, #2, and rests 5 @ 102)
    std::vector<RawOrder> scenario = {
        {1, Side::SELL, 100, 10}, 
        {2, Side::SELL, 101, 20}, 
        {3, Side::BUY,  99,  10}, 
        {4, Side::BUY,  102, 35}  
    };

    for (const auto& raw : scenario) {
        Order* order = order_pool.acquire(raw.id, raw.side, raw.price, raw.qty);
        
        std::cout << "Processing Order #" << raw.id << " (" 
                  << (raw.side == Side::BUY ? "BUY" : "SELL") 
                  << " " << raw.qty << " @ " << raw.price << ")\n";

        std::vector<Trade> trades = engine.process_order(order);

        for (const auto& trade : trades) {
            std::cout << "  >>> TRADE EXECUTION: Maker #" << trade.maker_order_id 
                      << " matched with Taker #" << trade.taker_order_id 
                      << " | " << trade.quantity << " @ " << trade.price << "\n";
        }
        
        // NEW: Print the book state after this order
        engine.print_book();
    }

    return 0;
}