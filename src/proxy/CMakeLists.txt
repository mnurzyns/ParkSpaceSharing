cmake_minimum_required(VERSION ${CMAKE_REQUIRED_VERSION})

add_executable(proxy)

target_sources(
    proxy PRIVATE
    src/test.cc
    src/main.cc
)

target_include_directories(proxy PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(
    proxy PRIVATE
    database
    Boost::system
)
