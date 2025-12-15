#pragma once

#include <vector>
#include <atomic>

template<typename T>
class LockFreeQueue {
private:
    std::vector<T> buffer;
    std::atomic<size_t> head; // Write index
    std::atomic<size_t> tail; // Read index
    size_t capacity;

public:
    LockFreeQueue(size_t size) : buffer(size), capacity(size), head(0), tail(0) {}

    // Called by Network Thread
    bool push(const T& item) {
        size_t current_head = head.load(std::memory_order_relaxed);
        size_t next_head = (current_head + 1) % capacity;

        // If next_head hits tail, the buffer is full!
        if (next_head == tail.load(std::memory_order_acquire)) {
            return false; // Queue Full
        }

        buffer[current_head] = item;
        
        // Publish the data (Release memory fence)
        head.store(next_head, std::memory_order_release);
        return true;
    }

    // Called by Matching Engine Thread
    bool pop(T& item) {
        size_t current_tail = tail.load(std::memory_order_relaxed);

        // If tail == head, the buffer is empty
        if (current_tail == head.load(std::memory_order_acquire)) {
            return false; // Queue Empty
        }

        item = buffer[current_tail];

        // Mark as read (Release memory fence)
        tail.store((current_tail + 1) % capacity, std::memory_order_release);
        return true;
    }
};