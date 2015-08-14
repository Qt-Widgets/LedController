cmake_policy (SET CMP0015 NEW)

function (add_project NAME TYPE ALLSOURCES)
        set (ALLSOURCES ${ALLSOURCES} ${ARGN})

        find_package (Qt4 REQUIRED QtCore QtGui QtNetwork)
        ADD_DEFINITIONS(${QT_DEFINITIONS})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        include(${QT_USE_FILE})

        foreach (CUR_SRC_FILE ${ALLSOURCES})
                if (CUR_SRC_FILE MATCHES "\\.h$")
                        set (HEADERS ${HEADERS} ${CUR_SRC_FILE})            
                elseif(CUR_SRC_FILE MATCHES "\\.ui$")
                        set(UI_FORMS ${UI_FORMS} ${CUR_SRC_FILE})
                elseif(CUR_SRC_FILE MATCHES "\\.cpp$")
                        set(CPP_FILES ${CPP_FILES} ${CUR_SRC_FILE})
                endif()
        endforeach()


        qt4_wrap_ui(GEN_UI_FORMS ${UI_FORMS})       
        set (GENERATED_FILES ${RESOURCE_FILES} ${GEN_UI_FORMS})

        link_directories ( ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE} )


        if( TYPE STREQUAL "DLL")
                add_library(          ${NAME}
                                      SHARED
                                      ${ALLSOURCES}
                                      ${GENERATED_FILES})

                target_link_libraries(${NAME} ${QT_LIBRARIES})

        elseif(TYPE STREQUAL "EXE")
                add_executable(       ${NAME}
                                      ${ALLSOURCES}
                                      ${GENERATED_FILES})

                target_link_libraries(${NAME}
                                      ${QT_QTMAIN_LIBRARY}
                                      ${QT_QTNETWORK_LIBRARY}
                                      ${QT_LIBRARIES}
                                      LedLib)

                link_directories (../../BIN/)
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
