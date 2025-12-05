file(GLOB_RECURSE S2B_SOURCES *.cpp)

find_library(Boost COMPONENTS asio)

add_library(s2b
  ${S2B_SOURCES}
)

set_property(TARGET s2b PROPERTY POSITION_INDEPENDENT_CODE FALSE)

target_link_libraries(s2b PUBLIC
  ${BOOST_LIBS}
  logger
)

target_include_directories(s2b PUBLIC
  ${CMAKE_SOURCE_DIR}/src
)
