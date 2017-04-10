#dump_variables()

# bump version here
set(jsoncpp_VERSION 2.0)

define_module(LIBRARY jsoncpp=${jsoncpp_VERSION}
  DEPENDS utility>=1.11
  JSONCPP)

set(jsoncpp_SOURCES
  as.hpp
  json.hpp
)

add_library(jsoncpp STATIC ${jsoncpp_SOURCES})
target_link_libraries(jsoncpp ${MODULE_LIBRARIES})
buildsys_target_compile_definitions(jsoncpp ${MODULE_DEFINITIONS})

set_target_properties(jsoncpp PROPERTIES LINKER_LANGUAGE CXX)
buildsys_library(jsoncpp)
