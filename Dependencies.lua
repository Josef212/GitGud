--VULKAN_SDK = os.getenv("VULKAN_SDK")
VULKAN_SDK = "C:/VulkanSDK/1.3.216.0" -- TODO: Issue with env variable

IncludeDir = {}
IncludeDir["stb_image"] = "%{wks.location}/GitGud/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/GitGud/vendor/yaml-cpp/include"
IncludeDir["Box2D"] = "%{wks.location}/GitGud/vendor/Box2D/include"
IncludeDir["GLFW"] = "%{wks.location}/GitGud/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/GitGud/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/GitGud/vendor/ImGui"
IncludeDir["ImGuizmo"] = "%{wks.location}/GitGud/vendor/ImGuizmo"
IncludeDir["glm"] = "%{wks.location}/GitGud/vendor/glm"
IncludeDir["entt"] = "%{wks.location}/GitGud/vendor/entt"
IncludeDir["shaderc"] = "%{wks.location}/GitGud/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/GitGud/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_DebugDLL"] = "%{VULKAN_SDK}/Bin"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"