#pragma once

#include "GitGud/Core/Core.h"
#include "GitGud/Core/Application.h"
#include "GitGud/Core/Layer.h"
#include "GitGud/Core/Log.h"

#include "GitGud/Debug/Instrumentor.h"

#include "GitGud/Core/TimeStep.h"

// ---- Input ---------------------------------
#include "GitGud/Core/Input.h"
#include "GitGud/Core/KeyCodes.h"
#include "GitGud/Core/MouseButtonCodes.h"
// --------------------------------------------

// ---- ECS -----------------------------------
#include "GitGud/Scene/Scene.h"
#include "GitGud/Scene/Entity.h"
#include "GitGud/Scene/ScriptableEntity.h"
#include "GitGud/Scene/Components.h"

#include "GitGud/Scene/SceneSerializer.h"
// --------------------------------------------

// ---- Renderer ------------------------------
#include "GitGud/Renderer/Renderer.h"
#include "GitGud/Renderer/Renderer2D.h"
#include "GitGud/Renderer/RenderCommand.h"

#include "GitGud/Renderer/Buffer.h"
#include "GitGud/Renderer/VertexArray.h"
#include "GitGud/Renderer/Shader.h"
#include "GitGud/Renderer/Texture.h"
#include "GitGud/Renderer/SubTexture2D.h"
#include "GitGud/Renderer/Framebuffer.h"

#include "GitGud/Renderer/Cameras/OrthographicCamera.h"
#include "GitGud/Renderer/Cameras/OrthographicCameraController.h"
// --------------------------------------------

#include "GitGud/ImGui/ImGuiLayer.h"