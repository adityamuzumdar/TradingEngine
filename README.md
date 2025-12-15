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

## Build Instructions

1. Create a build directory: `mkdir build && cd build`
2. Run CMake: `cmake ..`
3. Build: `make`

## Usage

Run the executable from `bin/`.

## Contributing

Add your contributions here.