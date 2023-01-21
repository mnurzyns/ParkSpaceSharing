cmake_minimum_required(VERSION ${CMAKE_REQUIRED_VERSION})

add_library(CoreLib core.cpp)
target_include_directories(CoreLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(CoreLib PRIVATE database)
