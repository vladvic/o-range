cmake_minimum_required(VERSION 3.16)
project(O-Range)

find_package(Boost REQUIRED)
find_package(magic_enum REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${magic_enum_INCLUDE_DIR} ${magic_enum_INCLUDE_DIR}/magic_enum)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(src/Logger)
add_subdirectory(src/R-Range)
add_subdirectory(src/S2B)
#add_subdirectory(src/SipStack)
add_subdirectory(tests/R-Range)
add_subdirectory(tests/S2B)
#add_subdirectory(tests/SipStack)
