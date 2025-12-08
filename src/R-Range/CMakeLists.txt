file(GLOB_RECURSE R_RANGE_SOURCES *.cpp)

find_library(Boost COMPONENTS asio)

add_library(r-range
  ${R_RANGE_SOURCES}
)

set_property(TARGET r-range PROPERTY POSITION_INDEPENDENT_CODE FALSE)

target_link_libraries(r-range PUBLIC
  ${BOOST_LIBS}
  logger
)

target_include_directories(r-range PRIVATE
  ${CMAKE_SOURCE_DIR}/include/R-Range
)
