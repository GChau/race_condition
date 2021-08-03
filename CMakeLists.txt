cmake_minimum_required (VERSION 3.13)

project(technical_challenge LANGUAGES CXX)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(${PROJECT_SOURCE_DIR}/src/test)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/lib)