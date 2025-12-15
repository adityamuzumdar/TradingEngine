#pragma once

#include <map>
#include <iostream>
#include "Order.hpp"

// Represents a queue of orders at a specific price level
struct OrderLevel {
    Price price;
    Order* head; // First order (highest priority)
    Order* tail; // Last order

    OrderLevel() : price(0), head(nullptr), tail(nullptr) {}

    // Add order to the end of the queue (Time Priority)
    void push_back(Order* order) {
        if (!head) {
            head = order;
            tail = order;
            order->next = nullptr;
            order->prev = nullptr;
        } else {
            tail->next = order;
            order->prev = tail;
            order->next = nullptr;
            tail = order;
        }
    }

    // Remove specific order (Cancellation or Execution)
    // Because we have next/prev pointers, this is O(1) - Instant!
    void remove(Order* order) {
        if (order->prev) {
            order->prev->next = order->next;
        } else {
            // It was the head
            head = order->next;
        }

        if (order->next) {
            order->next->prev = order->prev;
        } else {
            // It was the tail
            tail = order->prev;
        }
        
        // Safety: Unlink the order completely
        order->next = nullptr;
        order->prev = nullptr;
    }

    bool empty() const {
        return head == nullptr;
    }
};

class OrderBook {
public:
    // Bids: High Price Priority (Descending)
    std::map<Price, OrderLevel, std::greater<Price>> bids;
    
    // Asks: Low Price Priority (Ascending)
    std::map<Price, OrderLevel, std::less<Price>> asks;

    // Add an order to the book
    void add_order(Order* order) {
        if (order->side == Side::BUY) {
            // This creates the OrderLevel if it doesn't exist yet
            bids[order->price].push_back(order);
            bids[order->price].price = order->price;
        } else {
            asks[order->price].push_back(order);
            asks[order->price].price = order->price;
        }
    }

    // Remove an order from the book
    void remove_order(Order* order) {
        if (order->side == Side::BUY) {
            auto it = bids.find(order->price);
            if (it != bids.end()) {
                it->second.remove(order);
                // If this price level is now empty, delete the level to keep map small
                if (it->second.empty()) {
                    bids.erase(it);
                }
            }
        } else {
            auto it = asks.find(order->price);
            if (it != asks.end()) {
                it->second.remove(order);
                if (it->second.empty()) {
                    asks.erase(it);
                }
            }
        }
    }
    
    // View the best prices
    Order* get_best_bid() {
        if (bids.empty()) return nullptr;
        return bids.begin()->second.head; // The first order at the best price
    }

    Order* get_best_ask() {
        if (asks.empty()) return nullptr;
        return asks.begin()->second.head;
    }
};