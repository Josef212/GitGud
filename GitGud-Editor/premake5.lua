project "GitGud-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/GitGud/vendor/spdlog/include",
		"%{wks.location}/GitGud/src",
		"%{wks.location}/GitGud/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"GitGud"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "GG_DEBUG"
		runtime "Debug"
		symbols "on"
		
		postbuildcommands
		{
			--"{COPYDIR} \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\""
		}

	filter "configurations:Release"
		defines "GG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "GG_DIST"
		runtime "Release"
		optimize "on"