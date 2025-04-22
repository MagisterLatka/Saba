file(GLOB_RECURSE EditorSrc "*.cpp" "*.h")

add_executable(Editor ${EditorSrc})
target_link_libraries(Editor PRIVATE Saba SabaOptions SabaWarnings)
add_dependencies(Editor CopyEditorAssets CopyEditorImguiIni)

if (WIN32)
    add_dependencies(Editor CopyEditorSabaDLL SabaEditorShaders)
endif()
