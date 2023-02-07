cmake_minimum_required(VERSION ${CMAKE_REQUIRED_VERSION})

add_library(proxy)

target_sources(
    proxy PRIVATE
    src/test.cc
)

target_include_directories(proxy PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
