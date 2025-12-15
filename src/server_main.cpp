#include <iostream>
#include "../include/core/MatchingEngine.hpp"
#include "../include/utils/ObjectPool.hpp"
#include "../include/network/TcpServer.hpp"
#include "../include/network/ClientMessage.hpp"

int main() {
    // 1. Setup Engine
    MatchingEngine engine;
    ObjectPool<Order> pool(100000);
    TcpServer server(12345); // Listen on port 12345

    // 2. Start Network
    server.start();
    server.wait_for_connection();

    // 3. Event Loop
    ClientMessage msg;
    while (true) {
        // A. Read raw bytes from network
        if (!server.read_message(msg)) {
            std::cout << "Client disconnected. Shutting down.\n";
            break;
        }

        // B. Decode logic
        Side side = (msg.side == 0) ? Side::BUY : Side::SELL;
        OrderType type = (msg.type == 'M') ? OrderType::MARKET : OrderType::LIMIT;

        std::cout << "Received Order #" << msg.id << " " 
                  << (side == Side::BUY ? "BUY" : "SELL") << " " 
                  << msg.qty << " @ " << msg.price << "\n";

        // C. Acquire Order Object from Pool
        Order* order = pool.acquire(msg.id, side, msg.price, msg.qty, type);

        // D. Process
        auto trades = engine.process_order(order);

        // E. Print Trades
        for (const auto& t : trades) {
            std::cout << "  >>> TRADE: " << t.quantity << " @ " << t.price << "\n";
        }
        
        // F. Show Book
        engine.print_book();
    }

    return 0;
}