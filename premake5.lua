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

group "Dep"
	include "Saba/vendor/GLFW"
	include "Saba/vendor/GLAD"
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
		"%{prj.name}/src/**.h"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}
	
	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDirs.spdlog}",
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.GLAD}"
	}
	
	links
	{
		"GLFW",
		"GLAD",
		"opengl32.lib"
	}
	
	filter "configurations:Debug"
		defines "DEBUG"
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "RELEASE"
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
		"%{IncludeDirs.spdlog}",
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.GLAD}"
	}
	
	links
	{
		"Saba"
	}
	
	filter "configurations:Debug"
		defines "DEBUG"
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "on"
		runtime "Release"
		inlining "auto"
