file(GLOB_RECURSE LOGGER_SOURCES *.cpp)

add_library(logger
  ${LOGGER_SOURCES}
)

set_property(TARGET logger PROPERTY POSITION_INDEPENDENT_CODE FALSE)

target_include_directories(logger PUBLIC
  ${CMAKE_SOURCE_DIR}/src
)
