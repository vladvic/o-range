file(GLOB_RECURSE D2L_SOURCES *.cpp)

find_package(Lua REQUIRED)

add_library(d2l
  ${D2L_SOURCES}
)

set_property(TARGET d2l PROPERTY POSITION_INDEPENDENT_CODE FALSE)

target_link_libraries(d2l PUBLIC
  r-range
  ${LUA_LIBRARIES}
)

target_include_directories(d2l PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/include/D2L
  ${LUA_INCLUDE_DIR}
)
