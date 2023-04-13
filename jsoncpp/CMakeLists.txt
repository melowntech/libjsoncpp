#dump_variables()

# bump version here
set(jsoncpp_VERSION 2.5)

define_module(LIBRARY jsoncpp=${jsoncpp_VERSION}
  DEPENDS BuildSystem>=1.15
  utility>=1.41
  JSONCPP
  Boost
  # NB: this module provides target with different name
  PROVIDES jsoncpp-ext
  )

set(jsoncpp_SOURCES
  as.hpp
  json.hpp
  io.hpp io.cpp
)

add_library(jsoncpp-ext STATIC ${jsoncpp_SOURCES})
target_link_libraries(jsoncpp-ext ${MODULE_LIBRARIES})
target_compile_definitions(jsoncpp-ext PRIVATE ${MODULE_DEFINITIONS})

# set_target_properties(jsoncpp-ext PROPERTIES LINKER_LANGUAGE CXX)
buildsys_library(jsoncpp-ext)
