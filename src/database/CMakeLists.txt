add_library(database)

target_include_directories(database PRIVATE detail)
target_include_directories(database PUBLIC  include)

target_sources(
    database PRIVATE
    src/context.cc
)

target_link_libraries(
    database PRIVATE
    SQLite::SQLite3
)

add_subdirectory(config)
