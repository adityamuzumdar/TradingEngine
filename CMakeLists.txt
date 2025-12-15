cmake_minimum_required(VERSION 3.10)

# Project Name
project(TradingEngine VERSION 1.0)

# Use C++17 or C++20 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories (where to look for .hpp files)
include_directories(include)

# Gather all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create the executable named "Exchange"
add_executable(Exchange ${SOURCES})

# (Optional) Optimization flags for "Release" mode
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # -O3: Max optimization
    # -march=native: Use instructions specific to your CPU (AVX, etc.)
    target_compile_options(Exchange PRIVATE -O3 -march=native)
endif()