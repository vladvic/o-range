set(SRC
        SipStack.cpp
        ORangeSessionHandler.cpp
   )

find_library(Boost COMPONENTS asio)

add_library(sip_stack
  ${SRC}
)

set_property(TARGET sip_stack PROPERTY POSITION_INDEPENDENT_CODE FALSE)

target_link_libraries(sip_stack PUBLIC
  ${BOOST_LIBS}
  dum
  resip
  rutil
  logger
)

target_include_directories(sip_stack PRIVATE
  ${CMAKE_SOURCE_DIR}/include/SipStack
)