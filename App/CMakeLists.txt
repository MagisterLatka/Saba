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

    add_custom_target(SabaShaders)

    file(GLOB VertexShaders "assets/shaders/*.vert.hlsl")
    set_source_files_properties(${VertexShaders} PROPERTIES ShaderType "vs")
    set_source_files_properties(${VertexShaders} PROPERTIES ShaderModel "4_0")
    foreach (FILE ${VertexShaders})
        get_filename_component(FILE_WE ${FILE} NAME_WE)
        get_source_file_property(shaderType ${FILE} ShaderType)
        get_source_file_property(shaderModel ${FILE} ShaderModel)
        add_custom_command(TARGET SabaShaders POST_BUILD
            COMMAND fxc.exe /nologo /Emain /T${shaderType}_${shaderModel} $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo
                ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/${FILE_WE}.vert.cso /Fd ${CMAKE_BINARY_DIR}/${FILE_WE}.vert.pbd ${FILE}
            MAIN_DEPENDENCY ${FILE}
            COMMENT "HLSL VERTEX ${FILE}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM)
    endforeach()

    file(GLOB FragmentShaders "assets/shaders/*.frag.hlsl")
    set_source_files_properties(${FragmentShaders} PROPERTIES ShaderType "ps")
    set_source_files_properties(${FragmentShaders} PROPERTIES ShaderModel "4_0")
    foreach (FILE ${FragmentShaders})
        get_filename_component(FILE_WE ${FILE} NAME_WE)
        get_source_file_property(shaderType ${FILE} ShaderType)
        get_source_file_property(shaderModel ${FILE} ShaderModel)
        add_custom_command(TARGET SabaShaders POST_BUILD
            COMMAND fxc.exe /nologo /Emain /T${shaderType}_${shaderModel} $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo
                ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/${FILE_WE}.frag.cso /Fd ${CMAKE_BINARY_DIR}/${FILE_WE}.frag.pbd ${FILE}
            MAIN_DEPENDENCY ${FILE}
            COMMENT "HLSL FRAGMENT ${FILE}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM)
    endforeach()
endif()

add_custom_target(CopyAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
    COMMENT "Copy assets folder to executable"
)

