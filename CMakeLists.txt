cmake_minimum_required(VERSION 3.16)
project(life)

set(CMAKE_CXX_STANDARD 14)


add_library(life
        include/life/nibbles.h src/life/nibbles.cpp
        src/life/state.cpp include/life/state.h
        src/life/transform.cpp include/life/transform.h)
target_include_directories(life PUBLIC include/)

add_subdirectory(test)
add_subdirectory(example)
add_subdirectory(sfml)