set(LIB_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/resource_access.h
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_library(lib INTERFACE)
target_sources(lib INTERFACE ${LIB_HEADERS})
target_include_directories(lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set_property(TARGET test PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET test PROPERTY CXX_STANDARD 17)

target_compile_options(lib INTERFACE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
