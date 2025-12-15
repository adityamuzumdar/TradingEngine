#include <iostream>
#include <cassert>
#include "include/core/OrderBook.hpp"
#include "include/utils/ObjectPool.hpp"

int main() {
    // 1. Setup
    ObjectPool<Order> pool(100);
    OrderBook book;

    std::cout << "Testing OrderBook...\n";

    // 2. Create Orders
    // Buy 100 @ 10, Buy 100 @ 11 (Better price)
    Order* o1 = pool.acquire(1, Side::BUY, 10, 100);
    Order* o2 = pool.acquire(2, Side::BUY, 11, 100); 

    // 3. Add to Book
    book.add_order(o1);
    book.add_order(o2);

    // 4. Verify Priority
    // Best bid should be o2 (Price 11)
    Order* best = book.get_best_bid();
    if (best->id == 2) {
        std::cout << "[PASS] Best Bid Priority Correct (ID 2 is top).\n";
    } else {
        std::cout << "[FAIL] Wrong Best Bid.\n";
    }

    // 5. Verify Removal
    book.remove_order(o2);
    best = book.get_best_bid();
    if (best->id == 1) {
        std::cout << "[PASS] Removal Correct (ID 2 removed, ID 1 is new top).\n";
    } else {
        std::cout << "[FAIL] Removal Logic Error.\n";
    }

    return 0;
}