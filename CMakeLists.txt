cmake_minimum_required(VERSION 3.25.0)
project(learn_engine_dev C)

set(CMAKE_C_STANDARD 11)

include(FetchContent)

# Dependencies
set(SEIKA_STATIC_LIB ON)
FetchContent_Declare(
        seika_content
        GIT_REPOSITORY https://github.com/Chukobyte/seika.git
        GIT_TAG 1.0
)
FetchContent_MakeAvailable(seika_content)

add_executable(${PROJECT_NAME} src2/main.c)
target_link_libraries(${PROJECT_NAME} seika)
