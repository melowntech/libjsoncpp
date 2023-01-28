#dump_variables()

# bump version here
set(jsoncppwrapper_VERSION 2.4)

define_module(LIBRARY jsoncppwrapper=${jsoncppwrapper_VERSION}
  DEPENDS utility>=1.41
  JSONCPP
  Boost)

set(jsoncppwrapper_SOURCES
  as.hpp
  json.hpp
  io.hpp io.cpp
)

add_library(jsoncppwrapper STATIC ${jsoncppwrapper_SOURCES})
target_link_libraries(jsoncppwrapper ${MODULE_LIBRARIES})
target_compile_definitions(jsoncppwrapper PRIVATE ${MODULE_DEFINITIONS})

set_target_properties(jsoncppwrapper PROPERTIES LINKER_LANGUAGE CXX)
buildsys_library(jsoncppwrapper)
