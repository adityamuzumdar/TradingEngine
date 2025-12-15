#include <iostream>
#include <chrono>
#include "include/core/Order.hpp"
#include "include/utils/ObjectPool.hpp"

int main() {
    const int TEST_SIZE = 10000;
    
    // 1. Initialize the Pool for 10,000 orders
    std::cout << "Initializing Pool...\n";
    ObjectPool<Order> orderPool(TEST_SIZE);
    
    std::cout << "Pool initialized. Available: " << orderPool.available() << "\n";

    // 2. Measure Allocation Speed
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<Order*> active_orders;
    active_orders.reserve(TEST_SIZE);

    // Simulate high-frequency "New Order" requests
    for(int i = 0; i < TEST_SIZE; ++i) {
        // Acquire order from pool
        Order* o = orderPool.acquire(i, Side::BUY, 100 + i, 10);
        if (o) active_orders.push_back(o);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Allocated " << TEST_SIZE << " orders in " << duration << " microseconds.\n";
    std::cout << "Pool Available: " << orderPool.available() << "\n";

    // 3. Release them back
    for(Order* o : active_orders) {
        orderPool.release(o);
    }

    std::cout << "Released all. Pool Available: " << orderPool.available() << "\n";

    return 0;
}