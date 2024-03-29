if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(
        DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
        "${CMAKE_BINARY_DIR}/conan.cmake"
        TLS_VERIFY ON
    )
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)

set(CONAN_EXTRA_REQUIRES "")
if(NOT SOURCE_LOCATION_SUPPORTED)
  set(CONAN_EXTRA_REQUIRES "source_location/0.2.1")
endif()

conan_cmake_configure(
    REQUIRES
    oatpp/1.3.0
    oatpp-swagger/1.3.0
    oatpp-sqlite/1.3.0
    oatpp-openssl/1.3.0
    openssl/3.0.8
    jwt-cpp/0.6.0
    nlohmann_json/3.11.2
    toml11/3.7.1
    botan/2.19.3
    re2/20230301
    ${CONAN_EXTRA_REQUIRES}

    GENERATORS
    cmake_paths
    cmake_find_package

    OPTIONS
    sqlite3:enable_fts5=True
)

conan_cmake_autodetect(settings)

conan_cmake_install(
    PATH_OR_REFERENCE .
    BUILD missing
    REMOTE conancenter
    SETTINGS ${settings}
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
