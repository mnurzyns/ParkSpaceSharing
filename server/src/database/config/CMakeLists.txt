set(SQLITE3_COMMAND sqlite3)

find_program(
    SQLITE3_COMMAND sqlite3
    PATHS ${SQLite_INCLUDE_DIR}/../bin
)

if(SQLITE3_COMMAND STREQUAL "SQLITE3_COMMAND-NOTFOUND")
    message(WARNING "No sqlite3 executable found")
else()
    add_custom_target(
        sqlite-configure
        ALL
        COMMENT "Generate sqlite3 database from template"
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/config.sql
        BYPRODUCTS ${DATABASE_PATH}
        COMMAND ${SQLITE3_COMMAND} ${DATABASE_PATH} < ${CMAKE_CURRENT_SOURCE_DIR}/config.sql
    )
endif()

