cmake_minimum_required(VERSION 3.25.0)
project(learn_engine_dev C)

set(CMAKE_C_STANDARD 11)

option(BUILD_CHAPTER01 "Build Chapter 01" ON)

include(FetchContent)

if (BUILD_CHAPTER01)
    add_subdirectory(src2/1.game_loop)
endif ()
