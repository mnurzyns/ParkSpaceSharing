cmake_minimum_required(VERSION ${CMAKE_REQUIRED_VERSION})

add_library(ExampleLib Example.cpp)
target_include_directories(ExampleLib PRIVATE ${INCLUDE_DIR})