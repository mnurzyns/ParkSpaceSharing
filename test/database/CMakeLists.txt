add_executable(DatabaseTest)

target_sources(
    DatabaseTest PRIVATE
    src/context.cc
)

target_compile_definitions(
    DatabaseTest PRIVATE
    DATABASE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/databases"
)

target_link_libraries(
    DatabaseTest PRIVATE
    database
    GTest::GTest
    GTest::Main
    gmock
)

gtest_discover_tests(DatabaseTest)
