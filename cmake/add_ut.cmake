include(CMakeParseArguments)

find_package(GTest REQUIRED)

function(add_ut)
set(options)
set(oneValueArgs NAME)
set(multiValueArgs SOURCES DEPENDS)
cmake_parse_arguments(ADD_UT "${options}" "${oneValueArgs}"
                             "${multiValueArgs}" ${ARGN} )

add_executable(${ADD_UT_NAME}Test ${ADD_UT_SOURCES})
target_link_libraries(${ADD_UT_NAME}Test PRIVATE
    Threads::Threads
    ${ADD_UT_NAME}Lib
    ${ADD_UT_DEPENDS}
    GTest::GTest
    GTest::gtest_main
    )

target_include_directories(${ADD_UT_NAME}Test PRIVATE ${INCLUDE_DIR} ${TEST_INCLUDE_DIR})
add_test(NAME ${ADD_UT_NAME}Test COMMAND ${ADD_UT_NAME}Test)

get_property(tmp GLOBAL PROPERTY ALL_TESTS_LIST)
set_property(GLOBAL PROPERTY ALL_TESTS_LIST "${tmp};${ADD_UT_NAME}Test")

add_custom_target(${ADD_UT_NAME}Test-run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${ADD_UT_NAME}Test
    DEPENDS ${ADD_UT_NAME}Test
    )

endfunction()
