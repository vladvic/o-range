cmake_minimum_required(VERSION 3.16)
project(O-Range)

find_package(Boost REQUIRED)
find_package(magic_enum CONFIG REQUIRED)

get_target_property(INCLUDE_PATHS magic_enum::magic_enum INTERFACE_INCLUDE_DIRECTORIES)
include_directories(${Boost_INCLUDE_DIRS} ${INCLUDE_PATHS} ${INCLUDE_PATHS}/magic_enum)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(src/Logger)
add_subdirectory(src/R-Range)
add_subdirectory(src/S2B)
add_subdirectory(src/SipStack)
add_subdirectory(tests/R-Range)
add_subdirectory(tests/S2B)
add_subdirectory(tests/SipStack)
add_subdirectory(tests/util)
