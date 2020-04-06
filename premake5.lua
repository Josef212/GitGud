workspace "GitGud"
	startproject "Sandbox"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder 
IncludeDir = {}
IncludeDir["GLFW"] = "GitGud/vendor/glfw/include"
IncludeDir["Glad"] = "GitGud/vendor/glad/include"
IncludeDir["ImGui"] = "GitGud/vendor/imgui"
IncludeDir["Glm"] = "GitGud/vendor/glm"

group "Dependencies"
	include "GitGud/vendor/glfw"
	include "GitGud/vendor/glad"
	include "GitGud/vendor/imgui"
	
group ""

project "GitGud"
	location "GitGud"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ggpch.h"
	pchsource "GitGud/src/ggpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.Glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"GG_PLATFORM_WINDOWS",
			"GG_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "GG_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "GG_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "GG_DIST"
		runtime "Release"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"GitGud/vendor/spdlog/include",
		"GitGud/src",
		"%{IncludeDir.Glm}"
	}

	links
	{
		"GitGud"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"GG_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "GG_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "GG_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "GG_DIST"
		runtime "Release"
		optimize "On"