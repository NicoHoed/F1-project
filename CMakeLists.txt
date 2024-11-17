# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name
project(F1Simulation)

# Specify the C standard (optional but recommended)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Specify source files
set(SOURCES
        src/drivers.c
        src/main.c
        src/practices.c
        src/quali.c
        src/races.c
        src/season.c
        src/tracks.c
        src/utils.c
        src/weekends.c
)

# Specify the executable to be created
add_executable(F1Simulation ${SOURCES})

# Optionally, enable warnings during compilation
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(F1Simulation PRIVATE -Wall -Wextra -pedantic)
endif()
