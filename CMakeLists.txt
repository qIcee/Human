cmake_minimum_required(VERSION 3.20)

project(Human)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Human
    src/main.cpp
    src/human.cpp
    src/simulation.cpp

    src/organs/heart.cpp
    src/organs/brain.cpp
    src/muscles/musclegroup.cpp

    src/systems/muscularsystem.cpp
    src/systems/nervoussystem.cpp
    src/systems/cardiovascularsystem.cpp
)

target_include_directories(Human PRIVATE
    include
)