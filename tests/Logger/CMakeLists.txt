file(GLOB_RECURSE LOGGER_TEST_SOURCES *.cpp)

add_executable(logger-test
  ${LOGGER_TEST_SOURCES}
)

set_property(TARGET logger-test PROPERTY POSITION_INDEPENDENT_CODE FALSE)

target_link_libraries(logger-test PUBLIC
  logger
)

target_include_directories(logger PUBLIC
  ${CMAKE_SOURCE_DIR}/include
)
