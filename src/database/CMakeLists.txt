add_library(database)

target_include_directories(database PRIVATE detail)
target_include_directories(database PUBLIC  include)

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
