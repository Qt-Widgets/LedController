cmake_policy (SET CMP0015 NEW)

function (add_project NAME TYPE ALLSOURCES)		

    set (ALLSOURCES ${ALLSOURCES} ${ARGN})    
        add_definitions("-std=c++14")

    foreach (CUR_SRC_FILE ${ALLSOURCES})
        if (CUR_SRC_FILE MATCHES "\\.h$")
            set (HEADERS ${HEADERS} ${CUR_SRC_FILE})            
        elseif(CUR_SRC_FILE MATCHES "\\.ui$")
            set(UI_FORMS ${UI_FORMS} ${CUR_SRC_FILE})
        elseif(CUR_SRC_FILE MATCHES "\\.cpp$")
            set(CPP_FILES ${CPP_FILES} ${CUR_SRC_FILE})
            endif()
    endforeach()					       

    link_directories ( ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE} )

    if( TYPE STREQUAL "SHARED_LIB")
        add_library(${NAME}
                    SHARED
                    ${ALLSOURCES})
					
        target_link_libraries(${NAME} Qt5::Core)				   	

    elseif(TYPE STREQUAL "BIN")
		qt5_wrap_ui(GEN_UI_FORMS ${UI_FORMS})       
		set (GENERATED_FILES ${RESOURCE_FILES} ${GEN_UI_FORMS})
		
        add_executable(${NAME}
                       ${ALLSOURCES}
                       ${GENERATED_FILES})

        target_link_libraries(${NAME}
                              Qt5::Widgets
                              Qt5::Network
                              LedLib)

        link_directories (${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
		
	elseif(TYPE STREQUAL "TEST")
		add_executable(${NAME}
                       ${ALLSOURCES})
					   
		target_link_libraries(${NAME}
                              Qt5::Core
                              Qt5::Test                              
                              LedLib)
		link_directories (${CMAKE_RUNTIME_OUTPUT_DIRECTORY})							  
		
    else()
        message(FATAL_ERROR "Unknown project type '${TYPE}'")
    endif()

    source_group ("Header Files" FILES ${HEADERS})
    source_group ("Source Files" FILES ${CPP_FILES})
    source_group ("Generated Files" FILES ${GENERATED_FILES})

endfunction(add_project)

add_subdirectory (LedServer)
add_subdirectory (LedClient)
add_subdirectory (LedLib)
add_subdirectory (LedTests)
