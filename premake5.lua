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

group "Dep"
	include "Saba/vendor/GLFW"
	include "Saba/vendor/GLAD"
	include "Saba/vendor/ImGui"
	include "Saba/vendor/stb_image"
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
		"%{IncludeDirs.stbi}"
	}
	
	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"stb_image",
		"opengl32.lib"
	}
	
	postbuildcommands
	{
		"{COPY} \"assets\" \"../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Game/assets\""
		"{COPY} \"assets\" \"../Game/assets\""
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
		"Saba/vendor",
		"%{IncludeDirs.spdlog}",
		"%{IncludeDirs.GLM}"
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
		defines "DEBUG"
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "on"
		runtime "Release"
		inlining "auto"
