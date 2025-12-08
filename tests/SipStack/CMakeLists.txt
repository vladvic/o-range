file(GLOB_RECURSE STACK_TEST_SOURCES *.cpp)

add_executable(stack-test
  ${STACK_TEST_SOURCES}
)

set_property(TARGET stack-test PROPERTY POSITION_INDEPENDENT_CODE FALSE)

target_link_libraries(stack-test PUBLIC
  sip_stack
)

target_include_directories(sip_stack PUBLIC
  ${CMAKE_SOURCE_DIR}/include/SipStack
  ${CMAKE_SOURCE_DIR}/include
)
