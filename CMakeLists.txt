cmake_minimum_required(VERSION 3.29)
project(f1_test C)

set(CMAKE_C_STANDARD 11)

add_executable(f1_test main.c
        f1_championship.h
        time.c
        f1_championship.c
        car.h)
