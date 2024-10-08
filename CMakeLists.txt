cmake_minimum_required(VERSION 3.28)
project(Calc)

set(CMAKE_CXX_STANDARD 17)

add_executable(Calc main.cpp )

find_package(Catch2 REQUIRED)
target_link_libraries(Calc PRIVATE Catch2::Catch2)
