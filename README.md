# TradingEngine

A high-performance trading engine built in C++.

## Project Structure

- `bin/`: Executable files after compilation
- `build/`: Intermediate build files (CMake artifacts)
- `data/`: Sample input files (e.g., orders.csv for testing)
- `docs/`: Documentation (Design docs, latency graphs)
- `include/`: Header files (.hpp) - The "Interface"
  - `core/`: Trading logic (OrderBook, MatchingEngine)
  - `network/`: Networking code (TCP Server, parsing)
  - `utils/`: Helper tools (Logging, Memory Pools, Time)
- `src/`: Source files (.cpp) - The "Implementation"
  - `core/`
  - `network/`
  - `utils/`
  - `main.cpp`: Entry point
- `tests/`: Unit tests and Benchmarks
  - `test_orderbook.cpp`
  - `benchmark.cpp`
- `CMakeLists.txt`: Build configuration
- `README.md`: This file

