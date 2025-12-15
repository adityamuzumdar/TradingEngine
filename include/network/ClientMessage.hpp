#pragma once
#include <cstdint>

// This ensures the compiler packs the bytes tightly (no gaps)
#pragma pack(push, 1)

struct ClientMessage {
    char type;          // 1 byte: 'L' for Limit, 'M' for Market
    uint64_t id;        // 8 bytes
    uint64_t price;     // 8 bytes
    uint32_t qty;       // 4 bytes
    uint32_t side;      // 4 bytes: 0=Buy, 1=Sell
};

#pragma pack(pop)