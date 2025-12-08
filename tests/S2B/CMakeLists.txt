file(GLOB_RECURSE S2B_TEST_SOURCES *.cpp)

add_executable(s2b-test
  ${S2B_TEST_SOURCES}
)

set_property(TARGET s2b-test PROPERTY POSITION_INDEPENDENT_CODE FALSE)

target_link_libraries(s2b-test PUBLIC
  s2b
)

target_include_directories(s2b PUBLIC
  ${CMAKE_SOURCE_DIR}/include
)
