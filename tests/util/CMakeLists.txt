file(GLOB_RECURSE UTIL_TEST_SOURCES *.cpp)

add_executable(util-test
  ${UTIL_TEST_SOURCES}
)

set_property(TARGET util-test PROPERTY POSITION_INDEPENDENT_CODE FALSE)

target_include_directories(util-test PUBLIC
  ${CMAKE_SOURCE_DIR}/include
)
