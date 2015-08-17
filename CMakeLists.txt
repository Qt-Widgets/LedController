cmake_minimum_required(VERSION 2.6)
cmake_policy (SET CMP0020 NEW)

SET(CMAKE_BUILD_TYPE Debug)

project ("LedProject")
message ( STATUS "LedProject" )

set(CMAKE_AUTOMOC ON)

set (CMAKE_FIND_ROOT_PATH ${ENV_PATH})
set (CMAKE_PREFIX_PATH ${ENV_PATH})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/BIN/${TARGET_DIR})

include_directories("./")
include_directories("./SRC/LedServer")
include_directories("./SRC/LedClient")
include_directories("./SRC/LedLib")
include_directories("${CMAKE_BINARY_DIR}")	

find_package(Qt5 COMPONENTS Core Widgets Network Test)

add_subdirectory(SRC)
