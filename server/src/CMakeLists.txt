cmake_minimum_required(VERSION ${CMAKE_REQUIRED_VERSION})

add_executable(Server server.cpp)

target_link_libraries(
    Server PRIVATE
    ProxyLib
    database
    CoreLib
    Threads::Threads
)

add_subdirectory(core)
add_subdirectory(proxy)
add_subdirectory(database)
