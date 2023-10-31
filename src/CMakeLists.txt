cmake_minimum_required(VERSION 3.10)

project(Lab_02)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(assync AssyncMain.cpp)
add_executable(sync SyncMain.cpp)