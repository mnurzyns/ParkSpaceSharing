cmake_minimum_required(VERSION ${CMAKE_REQUIRED_VERSION})

include(${CMAKE_BASE_LIST_DIR}/cmake/add_ut.cmake)

set_property(GLOBAL PROPERTY ALL_TESTS_LIST)
get_property(ALL_TESTS_LIST GLOBAL PROPERTY ALL_TESTS_LIST)

add_subdirectory(core)
add_subdirectory(example)
add_subdirectory(proxy)
add_subdirectory(database)

add_custom_target(all_tests
    DEPENDS
    ${ALL_TESTS_LIST}
    COMMAND ctest)
