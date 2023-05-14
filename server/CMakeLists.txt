add_library(server)

target_sources(
    server PRIVATE

    lib/AuthHandler.cc
    lib/Config.cc
    lib/ErrorHandler.cc
    lib/TokenUtils.cc
    lib/Validator.cc

    lib/database/MainDatabase.cc

    lib/service/AuthService.cc
    lib/service/OfferService.cc
    lib/service/PlaceService.cc
    lib/service/UserService.cc
)

target_include_directories(
    server

    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib
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
    PUBLIC botan::botan
    PUBLIC re2::re2
)

install(TARGETS server DESTINATION bin)

install(
    DIRECTORY "${oatpp_oatpp-swagger_INCLUDE_DIR}/../bin/oatpp-swagger/res/"
    DESTINATION "share/${PROJECT_NAME}/oatpp-swagger-res"
)

add_executable(server-exe)

set_property(TARGET server-exe PROPERTY OUTPUT_NAME "pss-server")

target_sources(
    server-exe PRIVATE

    exe/main.cc
)

target_link_libraries(
    server-exe

    PRIVATE server
)

# TODO if tests enabled
add_executable(server-tests)

target_sources(
  server-tests PRIVATE

  tests/main.cc
  tests/Assert.cc

  tests/FullTest.cc
  tests/AuthController.cc
  tests/PlaceController.cc
  tests/OfferController.cc
)

target_include_directories(server-tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

target_link_libraries(
    server-tests

    PRIVATE server
    PRIVATE $<$<NOT:$<BOOL:${SOURCE_LOCATION_SUPPORTED}>>:source_location::source_location>
)

add_test(NAME server COMMAND server-tests)
