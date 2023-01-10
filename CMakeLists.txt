set(CMAKE_REQUIRED_VERSION 3.14)
cmake_minimum_required(VERSION ${CMAKE_REQUIRED_VERSION})

project(ParkingSpaceSharing LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# In case you have QtLib installed and cmake is not able to find it
# set(CMAKE_PREFIX_PATH "{PATH-TO-YOUR-LIB}/Qt/5.15.2/gcc_64")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(QT NAMES Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)
find_package(Threads REQUIRED)
find_package(SQLite3 REQUIRED)

set(INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(TEST_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/test)
set(CMAKE_BASE_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})
set(QtLib Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_subdirectory(src)
include(CTest)
enable_testing()
# add_subdirectory(test)
