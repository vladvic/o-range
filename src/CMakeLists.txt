add_executable(o-range main.cpp)

set_property(TARGET o-range PROPERTY POSITION_INDEPENDENT_CODE FALSE)

target_link_libraries(o-range PUBLIC
  logger
  sip_stack
  r-range
  s2b
)

target_include_directories(sip_stack PUBLIC
  ${CMAKE_SOURCE_DIR}/include
)

