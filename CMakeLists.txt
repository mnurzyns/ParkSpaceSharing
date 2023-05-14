cmake_minimum_required(VERSION 3.14)

option(CONAN "Use conan package manager" ON)
option(SANITIZERS "Enable sanitizers" ON)

project(ParkingSpaceSharing LANGUAGES CXX VERSION 0.0.1)

# If on clang, enable opaque pointers.
# They are enabled by default on versions => 15,
# and necessary for lto
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_link_options(-opaque-pointers)
endif()

# Try to use lld linker when using clang
# Dosen't work with MacOS
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    find_program(LD_LLD_FOUND ld.lld ld64.lld lld-link)
    if(LD_LLD_FOUND)
        message(STATUS "Using lld linker")
        add_link_options(-fuse-ld=lld)
    else()
        message(WARNING "Could not find lld linker")
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    include(cmake/ipo.cmake)
endif()

if(NOT MSVC)
    add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wconversion -Wshadow)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND SANITIZERS EQUAL ON)
        add_compile_options(-fsanitize=address,leak,undefined)
        add_link_options(-fsanitize=address,leak,undefined)
    endif()
else()
    # 4996 - getenv unsafe
    add_compile_options(/W4 /wd4996)

    # This variable is defined before including Config.hh file,
    # but MSVC gives linker errors if Config.cc was not compiled with it.
    # Works fine without it on GCC and Clang.
    add_compile_definitions(CONFIG_ALLOW_MUTATION)
endif()

# If our standard library does not support source_location
# conan will include a drop-in replacement package.
include(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX("source_location" SOURCE_LOCATION_SUPPORTED)

if(CONAN)
    include(cmake/conan.cmake)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(oatpp 1.3.0 REQUIRED)
find_package(oatpp-swagger 1.3.0 REQUIRED)
find_package(oatpp-sqlite 1.3.0 REQUIRED)
find_package(oatpp-openssl 1.3.0 REQUIRED)

find_package(OpenSSL 3.0.8 REQUIRED)
find_package(jwt-cpp 0.6.0 REQUIRED)
find_package(nlohmann_json 3.9.0 REQUIRED)
find_package(botan 2.19.3 REQUIRED)
find_package(toml11 3.7 REQUIRED)
find_package(re2 REQUIRED)

if(NOT SOURCE_LOCATION_SUPPORTED)
  find_package(source_location REQUIRED)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# TODO if tests enabled
enable_testing()

add_subdirectory(server)

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_BUILD_TYPE}")
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR "TGZ")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CPACK_GENERATOR "ZIP")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CPACK_GENERATOR "TGZ")
endif()
include(CPack)
