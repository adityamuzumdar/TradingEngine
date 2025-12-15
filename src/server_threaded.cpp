#include <iostream>
#include <thread>
#include <atomic>
#include "../include/core/MatchingEngine.hpp"
#include "../include/utils/ObjectPool.hpp"
#include "../include/network/TcpServer.hpp"
#include "../include/network/ClientMessage.hpp"
#include "../include/utils/LockFreeQueue.hpp"

// This is the internal event passed between threads
struct EngineEvent {
    ClientMessage msg;
};

// Global flag to stop threads safely
std::atomic<bool> running(true);

void network_thread_func(LockFreeQueue<EngineEvent>& queue) {
    TcpServer server(12345);
    server.start();
    server.wait_for_connection();

    ClientMessage msg;
    while (running) {
        // BLOCKING READ: This thread sleeps until bytes arrive
        if (server.read_message(msg)) {
            EngineEvent event = { msg };
            
            // Push to Queue (Keep trying if full)
            while (!queue.push(event)) {
                // If full, spin-wait (or yield)
                std::this_thread::yield();
            }
        } else {
            std::cout << "Client disconnected.\n";
            running = false;
            break;
        }
    }
}

void engine_thread_func(LockFreeQueue<EngineEvent>& queue) {
    MatchingEngine engine;
    ObjectPool<Order> pool(100000);

    std::cout << "Engine Warmup Complete. Waiting for orders...\n";

    EngineEvent event;
    while (running) {
        // NON-BLOCKING POP: This loop runs at 100% CPU speed
        if (queue.pop(event)) {
            // 1. Decode
            ClientMessage& msg = event.msg;
            Side side = (msg.side == 0) ? Side::BUY : Side::SELL;
            OrderType type = (msg.type == 'M') ? OrderType::MARKET : OrderType::LIMIT;

            // 2. Acquire & Process
            Order* order = pool.acquire(msg.id, side, msg.price, msg.qty, type);
            std::vector<Trade> trades = engine.process_order(order);

            // 3. Output (In real HFT, we would push this to an output queue)
            std::cout << "[ENG] Processed Order #" << msg.id << ". Executed " << trades.size() << " trades.\n";
            if (!trades.empty()) {
                engine.print_book();
            }
        } else {
            // Queue is empty. 
            // In low-latency, we BUSY WAIT (Spin) to react instantly.
            // We do NOT sleep.
             std::this_thread::yield(); // Optional: Be nice to CPU if on laptop
        }
    }
}

int main() {
    // The Queue connects the two threads
    LockFreeQueue<EngineEvent> queue(1024);

    // Start the Network Thread
    std::thread netThread(network_thread_func, std::ref(queue));

    // Run the Engine on the Main Thread (to keep it on the main core)
    engine_thread_func(queue);

    // Cleanup
    if (netThread.joinable()) netThread.join();
    return 0;
}