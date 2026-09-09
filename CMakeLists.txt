cmake_minimum_required(VERSION 3.21)
project(Leveller VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Geode REQUIRED)

add_geode_mod(Leveller
    SOURCES
        src/main.cpp
)
