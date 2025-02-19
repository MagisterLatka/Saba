project(App)

if (WIN32)
    set (DLLFilePath "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Saba.dll")
endif()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputdir}/App")

add_subdirectory(src)

if (WIN32)
    add_custom_target(CopySabaDLL ALL
        COMMAND ${CMAKE_COMMAND} -E copy ${DLLFilePath} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Copy dll file to executable"
    )
    add_dependencies(CopySabaDLL Saba)
endif()
