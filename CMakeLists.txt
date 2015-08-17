cmake_minimum_required(VERSION 2.6)
cmake_policy (SET CMP0020 NEW)

SET(CMAKE_BUILD_TYPE Debug)

project ("LedProject")
message ( STATUS "LedProject" )

########################################################################################################
if(CMAKE_SIZEOF_VOID_P MATCHES "8")
	set (PLATFORM_SIZE 64)
elseif(CMAKE_SIZEOF_VOID_P MATCHES "4")
	set (PLATFORM_SIZE 32)
else()
	message (FATAL_ERROR "Unknown platform")
endif()

#if(NOT DEFINED ENV{QT_DIR})
#	if(PLATFORM_SIZE MATCHES 32)
#		if(NOT DEFINED ENV{QTDIR32})
#			message(FATAL_ERROR "QTDIR32 not defined")
#		else()
#			set (QT_DIR $ENV{QTDIR32})
#		endif()
#	elseif(PLATFORM_SIZE MATCHES 64)
#		if(NOT DEFINED ENV{QTDIR64})
#			message(FATAL_ERROR "QTDIR64 not defined")
#		else()
#			set(QT_DIR $ENV{QTDIR64})
#		endif()
#	endif()
#endif()

###########################################################################################
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
