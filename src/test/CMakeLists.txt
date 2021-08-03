set(TEST_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/catch.cpp
    ${CMAKE_CURRENT_LIST_DIR}/fixed_exclusive_access_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/spec_exclusive_access_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test_main.cpp
)

set(TEST_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/catch.hpp
    ${CMAKE_CURRENT_LIST_DIR}/stdafx.h
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(test ${TEST_SRCS} ${TEST_HEADERS})

set_property(TARGET test PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET test PROPERTY CXX_STANDARD 17)

target_compile_options(test PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  target_link_libraries(test PRIVATE Threads::Threads)
endif()

target_link_libraries(test PUBLIC lib)