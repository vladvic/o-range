cmake_minimum_required(VERSION 3.16)
project(O-Range)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(src/Logger)
add_subdirectory(src/R-Range)
add_subdirectory(src/S2B)
add_subdirectory(tests/R-Range)
add_subdirectory(tests/S2B)
