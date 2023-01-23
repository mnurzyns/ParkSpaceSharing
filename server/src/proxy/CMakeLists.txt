cmake_minimum_required(VERSION ${CMAKE_REQUIRED_VERSION})


add_library(ProxyLib proxy.cpp)
target_include_directories(ProxyLib PRIVATE ${INCLUDE_DIR})
target_link_libraries(ProxyLib PRIVATE CoreLib)