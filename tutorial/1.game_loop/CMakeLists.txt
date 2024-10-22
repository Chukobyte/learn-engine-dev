cmake_minimum_required(VERSION 3.25.0)
project(1.0_gameloop C)

set(CMAKE_C_STANDARD 11)

include(FetchContent)

# Dependencies
set(SEIKA_STATIC_LIB ON)
set(SEIKA_AUDIO ON)
FetchContent_Declare(
        seika_content
        GIT_REPOSITORY https://github.com/Chukobyte/seika.git
        GIT_TAG 1.0
)
FetchContent_MakeAvailable(seika_content)

file(GLOB_RECURSE LESSON_SRC ./*.c ./*.h)
add_executable(${PROJECT_NAME} ${LESSON_SRC})
target_link_libraries(${PROJECT_NAME} seika)
