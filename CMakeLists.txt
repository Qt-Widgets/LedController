cmake_minimum_required(VERSION 2.6)

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

if(UNIX)
	set (PLATFORM_TYPE unix)
elseif(WIN32)
	set (PLATFORM_TYPE win)
else()
	message(FATAL_ERROR "Unknown platfrom type")
endif()

if(NOT DEFINED QT_DIR)
	if(PLATFORM_SIZE MATCHES 32)
		if(NOT DEFINED ENV{QTDIR32})
			message(FATAL_ERROR "QTDIR32 not defined")
		else()
			set (QT_DIR $ENV{QTDIR32})
		endif()
	elseif(PLATFORM_SIZE MATCHES 64)
		if(NOT DEFINED ENV{QTDIR64})
			message(FATAL_ERROR "QTDIR64 not defined")
		else()
			set(QT_DIR $ENV{QTDIR64})
		endif()
	endif()
endif()

###########################################################################################
set(CMAKE_AUTOMOC ON)

set (CMAKE_FIND_ROOT_PATH ${ENV_PATH})
set (CMAKE_PREFIX_PATH ${ENV_PATH})

include_directories("./")
include_directories("./SRC/LedServer")
include_directories("./SRC/LedClient")
include_directories("./SRC/LedLib")
include_directories("${CMAKE_BINARY_DIR}")	

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/BIN/${TARGET_DIR})

find_package (Qt4 4.8.4 REQUIRED QtCore QtGui QtNetwork)
include (${QT_USE_FILE})

add_subdirectory(SRC)
