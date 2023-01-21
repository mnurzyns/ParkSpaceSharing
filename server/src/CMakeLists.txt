cmake_minimum_required(VERSION ${CMAKE_REQUIRED_VERSION})

add_executable(Server server.cpp)

target_link_libraries(
    Server PRIVATE
    database
    CoreLib
    Threads::Threads
)

add_subdirectory(proxy)
add_subdirectory(core)
add_subdirectory(database)
