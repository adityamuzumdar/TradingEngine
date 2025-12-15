#pragma once

#include "OrderBook.hpp"
#include <vector>

class MatchingEngine {
private:
    OrderBook order_book;

public:
    // Process an incoming order and return any trades that happened
    std::vector<Trade> process_order(Order* incoming_order) {
        std::vector<Trade> trades;
        
        if (incoming_order->side == Side::BUY) {
            match_buy_order(incoming_order, trades);
        } else {
            match_sell_order(incoming_order, trades);
        }

        // If the order was not fully filled, add it to the book
        if (incoming_order->quantity > 0) {
            order_book.add_order(incoming_order);
        }

        return trades;
    }

    void print_book() const {
        order_book.print();
    }

private:
    void match_buy_order(Order* incoming, std::vector<Trade>& trades) {
        while (incoming->quantity > 0) {
            Order* best_ask = order_book.get_best_ask();

            // 1. Is the book empty?
            if (!best_ask) break;

            // 2. Is the price good? (Ask Price <= My Limit)
            if (best_ask->price > incoming->price) break;

            // 3. Execute Trade
            create_trade(incoming, best_ask, trades);
        }
    }

    void match_sell_order(Order* incoming, std::vector<Trade>& trades) {
        while (incoming->quantity > 0) {
            Order* best_bid = order_book.get_best_bid();

            if (!best_bid) break;
            
            // Is the price good? (Bid Price >= My Limit)
            if (best_bid->price < incoming->price) break;

            create_trade(incoming, best_bid, trades);
        }
    }

    void create_trade(Order* incoming, Order* resting, std::vector<Trade>& trades) {
        Quantity trade_qty = std::min(incoming->quantity, resting->quantity);

        // Record the trade
        trades.push_back({
            resting->id,    // Maker
            incoming->id,   // Taker
            resting->price, // Trade always happens at the RESTING price
            trade_qty
        });

        // Update quantities
        incoming->quantity -= trade_qty;
        resting->quantity -= trade_qty;

        // Clean up resting order if filled
        if (resting->quantity == 0) {
            order_book.remove_order(resting);
            // NOTE: In a real system, you would release 'resting' back to the ObjectPool here.
            // We will handle that in the main loop to keep this class clean.
        }
    }
};