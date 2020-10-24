include "./vendor/premake/premake_customization/solution_items.lua"

workspace "GitGud"
	startproject "GitGud-Editor"
	architecture "x86_64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder 
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/GitGud/vendor/glfw/include"
IncludeDir["Glad"] = "%{wks.location}/GitGud/vendor/glad/include"
IncludeDir["ImGui"] = "%{wks.location}/GitGud/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/GitGud/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/GitGud/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/GitGud/vendor/entt"
IncludeDir["yaml_cpp"] = "%{wks.location}/GitGud/vendor/yaml-cpp/include"

group "Dependencies"
	include "vendor/premake"
	include "GitGud/vendor/glfw"
	include "GitGud/vendor/glad"
	include "GitGud/vendor/imgui"
	include "GitGud/vendor/yaml-cpp"
	
group ""
	include "GitGud"
	include "GitGud-Editor"
	include "Sandbox"
