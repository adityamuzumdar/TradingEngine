#pragma once
/*
The Problem: In standard C++, every time you do new Order(), the OS has to find free memory. This is slow (takes microseconds) and unpredictable.

The Solution: We pre-allocate a huge block of memory (e.g., 10,000 orders) at startup.

When we need an order, we take one from our "Free List."

When a trade is done, we don't delete the order; we just put it back in the "Free List."

Cost: Nanoseconds (extremely fast). */

#include <vector>
#include <memory>
#include <iostream>

template<typename T> // this means that class can be used with any type - order, trade, etc
class ObjectPool {
private:
    std::vector<T> pool;
    std::vector<T*> free_list;

public:
    // Constructor: Pre-allocate memory immediately
    ObjectPool(size_t size) {
        // Reserve memory to prevent re-allocation
        pool.reserve(size);
        free_list.reserve(size);

        // Create the objects and add them to the free list
        for (size_t i = 0; i < size; ++i) {
            pool.emplace_back(); // Construct object in place
            free_list.push_back(&pool.back());
        }
    }

    // Get an object from the pool
    template<typename... Args>
    T* acquire(Args&&... args) {
        if (free_list.empty()) {
            // In a real HFT system, you might crash here or expand.
            // For this project, we'll return nullptr and log error.
            std::cerr << "CRITICAL: Object Pool exhausted!\n";
            return nullptr;
        }

        // Pop the last available object (Fastest operation: O(1))
        T* obj = free_list.back();
        free_list.pop_back();

        // "Construct" it with new arguments
        // We use 'placement new' syntax or just re-assign.
        // For simplicity here, we assume T has a reset/reinit capability or we just assign directly.
        *obj = T(std::forward<Args>(args)...);
        
        return obj;
    }

    // Return an object to the pool
    void release(T* obj) {
        // Just put the pointer back in the free list.
        // No memory is actually freed to the OS.
        free_list.push_back(obj);
    }
    
    size_t available() const {
        return free_list.size();
    }
};