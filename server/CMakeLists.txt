add_executable(server)

target_sources(
    server PRIVATE

    src/main.cc
    src/error_handler.cc
    src/service/user_service.cc
    src/service/offer_service.cc
    src/service/auth_service.cc
)

target_include_directories(
    server

    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(
    server

    PUBLIC -DPSS_OATPP_SWAGGER_RES_PATH="${oatpp_oatpp-swagger_INCLUDE_DIR}/../bin/oatpp-swagger/res"
    PUBLIC -DPSS_DATABASE_MIGRATIONS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/migrations"
    PUBLIC -DPSS_DATABASE_FILE="${CMAKE_CURRENT_BINARY_DIR}/database.sqlite3"
)

target_link_libraries(
    server

    PUBLIC oatpp::oatpp
    PUBLIC oatpp::oatpp-swagger
    PUBLIC oatpp::oatpp-sqlite
    PUBLIC oatpp::oatpp-openssl
)

