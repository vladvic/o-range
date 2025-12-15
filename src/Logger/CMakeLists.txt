file(GLOB_RECURSE LOGGER_SOURCES *.cpp)

find_package(spdlog REQUIRED)

add_library(logger
  ${LOGGER_SOURCES}
)

set_property(TARGET logger PROPERTY POSITION_INDEPENDENT_CODE FALSE)

target_include_directories(logger PRIVATE
  ${CMAKE_SOURCE_DIR}/include/Logger
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(logger PUBLIC spdlog::spdlog)
