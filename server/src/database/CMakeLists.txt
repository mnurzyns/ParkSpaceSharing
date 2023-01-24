add_library(database)

set(DATABASE_PATH ${CMAKE_CURRENT_BINARY_DIR}/database.sqlite)

target_include_directories(database PRIVATE detail)
target_include_directories(database PUBLIC  include)

target_compile_definitions(
    database PUBLIC
    Database_PATH="${DATABASE_PATH}"
)

target_sources(
    database PRIVATE
    src/context.cc
    src/statement.cc
)

target_link_libraries(
    database PUBLIC
    SQLite::SQLite3
    nlohmann_json::nlohmann_json
)

add_subdirectory(config)
