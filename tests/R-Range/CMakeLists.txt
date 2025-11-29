file(GLOB_RECURSE R_RANGE_TEST_SOURCES *.cpp)

add_executable(r-range-test
  ${R_RANGE_TEST_SOURCES}
)

set_property(TARGET r-range-test PROPERTY POSITION_INDEPENDENT_CODE FALSE)

target_link_libraries(r-range-test PUBLIC
  r-range
)

target_include_directories(r-range PUBLIC
  ${CMAKE_SOURCE_DIR}/src
)
