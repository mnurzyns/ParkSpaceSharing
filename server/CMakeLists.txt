add_executable(server)

set_property(TARGET server PROPERTY OUTPUT_NAME "pss-server")

target_sources(
    server PRIVATE

    src/main.cc
    src/AuthHandler.cc
    src/Config.cc
    src/ErrorHandler.cc
    src/JWT.cc

    src/database/MainDatabase.cc

    src/service/AuthService.cc
    src/service/OfferService.cc
    src/service/PlaceService.cc
    src/service/UserService.cc
)

target_include_directories(
    server

    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(
    server
    PUBLIC PROJECT_NAME="${PROJECT_NAME}"
    PUBLIC OATPP_SWAGGER_RES_PATH="${oatpp_oatpp-swagger_INCLUDE_DIR}/../bin/oatpp-swagger/res"
    PUBLIC MAIN_DATABASE_MIGRATIONS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/migrations"
)

target_link_libraries(
    server

    PUBLIC oatpp::oatpp
    PUBLIC oatpp::oatpp-swagger
    PUBLIC oatpp::oatpp-sqlite
    PUBLIC oatpp::oatpp-openssl
    PUBLIC jwt-cpp::jwt-cpp
    PUBLIC OpenSSL::SSL
    PUBLIC OpenSSL::Crypto
    PUBLIC nlohmann_json::nlohmann_json
    PUBLIC toml11::toml11
)

install(TARGETS server DESTINATION bin)

install(
    DIRECTORY "${oatpp_oatpp-swagger_INCLUDE_DIR}/../bin/oatpp-swagger/res/"
    DESTINATION "share/${PROJECT_NAME}/oatpp-swagger-res"
)
