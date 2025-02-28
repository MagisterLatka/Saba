#include <pch.h>

#define IMGUI_IMPLEMENTATION
#include <misc/single_file/imgui_single_file.h>

#if defined(SB_PLATFORM_WINDOWS)
#   include <backends/imgui_impl_win32.cpp>
#   include <backends/imgui_impl_dx11.cpp>
//#   include <backends/imgui_impl_dx12.cpp>
#else
#   include <backends/imgui_impl_glfw.cpp>
#endif

#include <backends/imgui_impl_opengl3.cpp>
//#include <backends/imgui_impl_vulkan.cpp>
