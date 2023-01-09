set(SQLITE3_COMMAND sqlite3)
set(DATABASE_PATH ${CMAKE_CURRENT_BINARY_DIR}/database.sqlite3)

add_custom_target(
    sqlite-configure
    ALL
    COMMENT "Generate sqlite3 database from template"
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/config.sql
    BYPRODUCTS ${DATABASE_PATH}
    COMMAND ${SQLITE3_COMMAND} ${DATABASE_PATH} < ${CMAKE_CURRENT_SOURCE_DIR}/config.sql
)
