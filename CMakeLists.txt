cmake_minimum_required(VERSION 4.0)
project(HW1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
add_executable(HW1 
    src/main.cpp
    src/DataStructureA.cpp
    src/DataStructureB.cpp
    src/DataStructureC.cpp
)