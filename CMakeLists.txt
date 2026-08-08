cmake_minimum_required(VERSION 3.16)

project(limit_order_book VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(limit_order_book
    src/main.cpp
)

target_include_directories(limit_order_book PRIVATE include)