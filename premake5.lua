workspace "Saba"
	startproject "Game"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["spdlog"] = "Saba/vendor/spdlog/include"
IncludeDirs["GLFW"] = "Saba/vendor/GLFW/include"
IncludeDirs["GLAD"] = "Saba/vendor/GLAD/include"
IncludeDirs["ImGui"] = "Saba/vendor/ImGui"
IncludeDirs["GLM"] = "Saba/vendor/glm"
IncludeDirs["stbi"] = "Saba/vendor/stb_image"
IncludeDirs["entt"] = "Saba/vendor/entt/include"
IncludeDirs["assimp"] = "Saba/vendor/assimp/include"
IncludeDirs["yaml_cpp"] = "Saba/vendor/yaml-cpp/include"
IncludeDirs["ImGuizmo"] = "Saba/vendor/ImGuizmo"

group "Dep"
	include "Saba/vendor/GLFW"
	include "Saba/vendor/GLAD"
	include "Saba/vendor/ImGui"
	include "Saba/vendor/stb_image"
	include "Saba/vendor/yaml-cpp"
group ""
	
project "Saba"
	location "Saba"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "pch.h"
	pchsource "%{prj.name}/src/pch.cpp"
	
	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h",
		"Saba/vendor/ImGuizmo/ImGuizmo.cpp",
		"Saba/vendor/ImGuizmo/ImGuizmo.h"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"SB_USE_GLFW"
	}
	
	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDirs.spdlog}",
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.GLAD}",
		"%{IncludeDirs.ImGui}",
		"%{IncludeDirs.GLM}",
		"%{IncludeDirs.stbi}",
		"%{IncludeDirs.entt}",
		"%{IncludeDirs.assimp}",
		"%{IncludeDirs.yaml_cpp}",
		"%{IncludeDirs.ImGuizmo}"
	}
	
	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"stb_image",
		"yaml-cpp",
		"opengl32.lib",
		"Saba/vendor/assimp/assimp.lib"
	}
	
	filter "files:Saba/vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }
	
	
	filter "configurations:Debug"
		defines "SB_DEBUG"
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "SB_RELEASE"
		optimize "on"
		runtime "Release"
		inlining "auto"
	
project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	includedirs
	{
		"Saba/src",
		"Saba/vendor",
		"%{IncludeDirs.spdlog}",
		"%{IncludeDirs.GLM}",
		"%{IncludeDirs.entt}",
		"%{IncludeDirs.ImGui}",
		"%{IncludeDirs.ImGuizmo}"
	}
	
	links
	{
		"Saba"
	}
	
	postbuildcommands
	{
		"{COPY} \"assets\" \"../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Game/assets\""
	}
	
	filter "configurations:Debug"
		defines "SB_DEBUG"
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "SB_RELEASE"
		optimize "on"
		runtime "Release"
		inlining "auto"

project "Editor"
	location "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	includedirs
	{
		"Saba/src",
		"Saba/vendor",
		"%{IncludeDirs.spdlog}",
		"%{IncludeDirs.GLM}",
		"%{IncludeDirs.entt}",
		"%{IncludeDirs.ImGui}",
		"%{IncludeDirs.ImGuizmo}"
	}
	
	links
	{
		"Saba"
	}
	
	postbuildcommands
	{
		"{COPY} \"assets\" \"../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Editor/assets\""
	}
	
	filter "configurations:Debug"
		defines "SB_DEBUG"
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "SB_RELEASE"
		optimize "on"
		runtime "Release"
		inlining "auto"
