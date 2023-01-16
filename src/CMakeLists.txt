cmake_minimum_required(VERSION ${CMAKE_REQUIRED_VERSION})

set(PROJECT_SOURCES
        main.cpp
        gui/resources/qml.qrc
)

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Threads::Threads
    ${QtLib}
)

add_subdirectory(core)
add_subdirectory(database)
add_subdirectory(proxy)

add_custom_target(${CMAKE_PROJECT_NAME}-run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}
    DEPENDS ${CMAKE_PROJECT_NAME}
    )
