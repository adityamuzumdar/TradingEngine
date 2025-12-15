#pragma once

#include <cstdint>

// Definitions
using Price = uint64_t;
using Quantity = uint32_t;
using OrderId = uint64_t;

enum class Side : uint8_t { BUY = 0, SELL = 1 };
enum class OrderType : uint8_t { LIMIT = 0, MARKET = 1 };

struct Order {
    OrderId id;
    OrderId client_id;
    Side side;
    Price price;
    Quantity quantity;  
    
    Order* next;
    Order* prev;

    // 1. DEFAULT CONSTRUCTOR (ADDED)
    // This allows the ObjectPool to create "blank" orders in memory.
    Order() 
        : id(0), client_id(0), side(Side::BUY), price(0), quantity(0), 
          next(nullptr), prev(nullptr) {}

    // 2. Parameterized Constructor
    Order(OrderId _id, Side _side, Price _price, Quantity _qty)
        : id(_id), client_id(0), side(_side), price(_price), quantity(_qty), 
          next(nullptr), prev(nullptr) {}
        
    void reset() {
        id = 0;
        price = 0;
        quantity = 0;
        next = nullptr;
        prev = nullptr;
    }
};

struct Trade {
    OrderId maker_order_id; 
    OrderId taker_order_id; 
    Price price;
    Quantity quantity;
};