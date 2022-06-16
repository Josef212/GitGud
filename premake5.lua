include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "GitGud"
	architecture "x86_64"
	startproject "GitGud-Editor"

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

group "Dependencies"
	include "vendor/premake"
	include "GitGud/vendor/Box2D"
	include "GitGud/vendor/GLFW"
	include "GitGud/vendor/Glad"
	include "GitGud/vendor/imgui"
	include "GitGud/vendor/yaml-cpp"
group ""

include "GitGud"
include "Sandbox"
include "GitGud-Editor"
