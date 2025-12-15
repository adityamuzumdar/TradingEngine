#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include "../include/core/MatchingEngine.hpp"
#include "../include/utils/ObjectPool.hpp"

// A simple random number generator helper
// (Standard rand() is too slow for benchmarking)
struct RandomGen {
    std::mt19937 rng;
    std::uniform_int_distribution<uint64_t> price_dist;
    std::uniform_int_distribution<uint32_t> qty_dist;
    std::uniform_int_distribution<int> side_dist;

    RandomGen() : rng(42), price_dist(90, 110), qty_dist(1, 100), side_dist(0, 1) {}

    Side next_side() { return side_dist(rng) == 0 ? Side::BUY : Side::SELL; }
    Price next_price() { return price_dist(rng); }
    Quantity next_qty() { return qty_dist(rng); }
};

int main() {
    // 1. Setup
    const int NUM_ORDERS = 1000000; // 1 Million Orders
    MatchingEngine engine;
    ObjectPool<Order> pool(NUM_ORDERS + 1000); // Pre-allocate everything
    RandomGen gen;

    std::cout << "Generating " << NUM_ORDERS << " orders in memory...\n";

    // 2. Pre-generate orders (so we measure Engine speed, not Random Generation speed)
    std::vector<Order*> workload;
    workload.reserve(NUM_ORDERS);

    for (int i = 0; i < NUM_ORDERS; ++i) {
        Order* o = pool.acquire(i, gen.next_side(), gen.next_price(), gen.next_qty());
        workload.push_back(o);
    }

    std::cout << "Starting Benchmark...\n";

    // 3. Start Timer
    auto start = std::chrono::high_resolution_clock::now();

    // 4. Run the Hot Path
    for (Order* o : workload) {
        engine.process_order(o);
        // In a real system, you would release orders here if they are filled
    }

    // 5. Stop Timer
    auto end = std::chrono::high_resolution_clock::now();

    // 6. Calculate Results
    auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double duration_seconds = duration_ns / 1e9;
    double orders_per_sec = NUM_ORDERS / duration_seconds;
    double avg_latency_ns = duration_ns / (double)NUM_ORDERS;

    std::cout << "--------------------------------\n";
    std::cout << "Done!\n";
    std::cout << "Time Elapsed: " << duration_seconds << " s\n";
    std::cout << "Throughput:   " << (long)orders_per_sec << " orders/sec\n";
    std::cout << "Avg Latency:  " << avg_latency_ns << " ns/order\n";
    std::cout << "--------------------------------\n";

    return 0;
}