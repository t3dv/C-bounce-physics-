cmake_minimum_required(VERSION 4.0)
project(physics)

include(FetchContent)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
)
FetchContent_MakeAvailable(SFML)

add_executable(physics main.cpp)


target_link_libraries(physics PRIVATE sfml-graphics sfml-window sfml-system)
