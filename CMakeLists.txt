set(CMAKE_REQUIRED_VERSION 3.14)
cmake_minimum_required(VERSION ${CMAKE_REQUIRED_VERSION})

option(CONAN "Use conan package manager" ON)

#set(CMAKE_BUILD_TYPE Release)
project(ParkingSpaceSharing LANGUAGES CXX VERSION 0.0.1)

if(NOT MSVC)
    add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wconversion -Wshadow)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options (-fsanitize=address,leak,undefined)
        add_link_options    (-fsanitize=address,leak,undefined)
    endif()
else()
    add_compile_options(/W4 /WX)
endif()

if(CONAN)
    include(cmake/conan.cmake)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(oatpp         1.3.0 REQUIRED)
find_package(oatpp-swagger 1.3.0 REQUIRED)
find_package(oatpp-sqlite  1.3.0 REQUIRED)
find_package(oatpp-openssl 1.3.0 REQUIRED)

find_package(OpenSSL 3.0.8 REQUIRED)
find_package(jwt-cpp 0.6.0 REQUIRED)
find_package(nlohmann_json 3.9.0 REQUIRED)

find_package(toml11 3.7 REQUIRED)

#find_package(QT NAMES Qt5 COMPONENTS Core Quick REQUIRED)
#find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_subdirectory(server)

#include(CTest)
#enable_testing()

