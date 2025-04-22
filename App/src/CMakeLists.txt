file(GLOB_RECURSE AppSrc "*.cpp" "*.h")

add_executable(App ${AppSrc})
target_link_libraries(App PRIVATE Saba SabaOptions SabaWarnings )
add_dependencies(App CopyAssets CopyImguiIni)

if (WIN32)
    add_dependencies(App CopySabaDLL SabaShaders)
endif()
