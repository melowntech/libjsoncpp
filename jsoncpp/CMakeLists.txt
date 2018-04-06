#dump_variables()

# bump version here
set(jsoncpp_VERSION 2.3)

# find_package(jsoncpp) sets jsoncpp_FOUND which conflicts with this module --
# get rid of it
unset(jsoncpp_FOUND)
unset(jsoncpp_FOUND PARENT_SCOPE)

if(WIN32)
  set(JSONCPP_LIB_TARGET jsoncpp_lib_static)
else()
  set(JSONCPP_LIB_TARGET jsoncpp_lib)
endif()

define_module(LIBRARY jsoncpp=${jsoncpp_VERSION}
  DEPENDS utility>=1.11
  ${JSONCPP_LIB_TARGET}
  Boost)

set(jsoncpp_SOURCES
  as.hpp
  json.hpp
  io.hpp io.cpp
)

add_library(jsoncpp STATIC ${jsoncpp_SOURCES})
target_link_libraries(jsoncpp ${MODULE_LIBRARIES})
buildsys_target_compile_definitions(jsoncpp ${MODULE_DEFINITIONS})
buildsys_library(jsoncpp)
