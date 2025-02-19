file(GLOB_RECURSE AppSrc "*.cpp" "*.h")
add_executable(App ${AppSrc})
target_link_libraries(App PRIVATE Saba SabaOptions SabaWarnings)

if (WIN32)
    add_dependencies(App CopySabaDLL)
endif()
