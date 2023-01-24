add_executable(DatabaseTest)

target_sources(
    DatabaseTest PRIVATE
    src/context.cc
)

# DATABASE_DIR points to the directory
# with databases needed for unit tests
target_compile_definitions(
    DatabaseTest PRIVATE
    DATABASE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/databases"
)

target_link_libraries(
    DatabaseTest PRIVATE
    database
    CoreLib
    GTest::GTest
    GTest::Main
    gmock
)

gtest_discover_tests(DatabaseTest)
