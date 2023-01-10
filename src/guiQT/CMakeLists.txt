cmake_minimum_required(VERSION ${CMAKE_REQUIRED_VERSION})

add_library(guiQTLib qt.cpp)
target_include_directories(guiQTLib PRIVATE ${INCLUDE_DIR})
