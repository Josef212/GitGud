workspace "GitGud"
	startproject "GitGud-Editor"
	architecture "x86_64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "GitGud/vendor/glfw"
	include "GitGud/vendor/glad"
	include "GitGud/vendor/imgui"
	
group ""
	include "GitGud"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"GitGud/vendor",
		"GitGud/vendor/glm"
	}

	links
	{
		"GitGud"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			--"GG_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "GG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "GG_DIST"
		runtime "Release"
		optimize "on"

project "GitGud-Editor"
	location "GitGud-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"GitGud/vendor",
		"GitGud/vendor/glm"
	}

	links
	{
		"GitGud"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			--"GG_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "GG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "GG_DIST"
		runtime "Release"
		optimize "on"