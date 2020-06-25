#include "EditorLayer.h"
#include <imgui/imgui.h>

namespace GitGud
{
	EditorLayer::EditorLayer() : Layer("GitGud-Editor"), _cameraController(1280.0f / 720.0f), _viewportSize({0, 0})
	{
		GG_PROFILE_FUNCTION();
		_transform = new Transform();
	}

	EditorLayer::~EditorLayer()
	{
		GG_PROFILE_FUNCTION();
		delete _transform;
	}

	void EditorLayer::OnAttach()
	{
		GG_PROFILE_FUNCTION();

		_checkerTexture = Texture2D::Create("assets/textures/Checkerboard.png");
		_logoTexture = Texture2D::Create("assets/textures/GitGudIconLogo.png");
		_spriteSheet = Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");

		_stairsSprite = SubTexture2D::CreateFromCoords(_spriteSheet, { 7, 6 }, { 128, 128 });
		_barrelSprite = SubTexture2D::CreateFromCoords(_spriteSheet, { 8, 2 }, { 128, 128 });
		_orangeTree = SubTexture2D::CreateFromCoords(_spriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });

		FramebufferSpecification specs;
		specs.Width = 1280;
		specs.Height = 720;
		_frambuffer = Framebuffer::Create(specs);
	}

	void EditorLayer::OnDetach()
	{
		GG_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		GG_PROFILE_FUNCTION();

		_cameraController.OnUpdate(ts);

		Renderer2D::ResetStats();

		_frambuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

#if 1
		Renderer2D::BeginScene(_cameraController.GetCamera());

		static float rot = 0.0f;
		rot += ts * 20.0f;

		//Renderer2D::DrawQuad({ _pos.x, _pos.y, 0.0f }, _size, _angle, _color);
		auto scl = _transform->GetLocalScale();
		float zAngle = _transform->GetEulerRotation().z;
		Renderer2D::DrawQuad(_transform->GetLocalPosition(), {scl.x, scl.y}, glm::radians(zAngle), _color);
		Renderer2D::DrawQuad({ -1.0f, 2.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rot), { 0.2f, 0.3f, 0.8f, 1.0f });
		Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, 0.0f, _checkerTexture, glm::vec2(10.0f, 10.0f));
		Renderer2D::DrawQuad({ 4.0f, -4.0f, 0.0f }, { 1.0f, 1.0f }, 0.0f, _logoTexture);
		Renderer2D::DrawQuad({ 1.2f, 1.4f, 0.0f }, { 1.0f, 1.0f }, 45.0f, { 0.2f, 0.8f, 0.4f, 1.0f }, _checkerTexture, glm::vec2(1.0f));

		Renderer2D::EndScene();

		Renderer2D::BeginScene(_cameraController.GetCamera());

		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Renderer2D::DrawQuad({ x, y, 0.0f }, { 0.45f, 0.45f }, color);
			}
		}

		Renderer2D::EndScene();
#endif

		Renderer2D::BeginScene(_cameraController.GetCamera());

		Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, _stairsSprite);
		Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, _barrelSprite);
		Renderer2D::DrawQuad({ 1.0f, 0.0f, 0.0f }, { 1.0f, 2.0f }, _orangeTree);

		Renderer2D::EndScene();

		_frambuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		GG_PROFILE_FUNCTION();

		_cameraController.OnEvent(e);
	}

	void EditorLayer::OnImGuiRender()
	{
		GG_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen) ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit", "")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		{
			_cameraController.OnImGuiRender();

			ImGui::Begin("Settings");

			auto stats = Renderer2D::GetStatistics();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indicies: %d", stats.GetTotalIndexCount());

			ImGui::ColorEdit4("Color", &_color.r);
			ImGui::DragFloat2("Pos", &_pos.x, 0.1f);
			ImGui::DragFloat2("Size", &_size.x, 0.1f);
			ImGui::DragFloat("Angle", &_angle);

			if (ImGui::Button("Begin session"))
			{
				GG_PROFILE_BEGIN_SESSION("TestSession", "testSession.json");
			}

			ImGui::SameLine();

			if (ImGui::Button("End session"))
			{
				GG_PROFILE_END_SESSION();
			}

			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
				ImGui::Begin("Viewport");

				ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
				if (_viewportSize != *((glm::vec2*) & viewportPanelSize))
				{
					_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
					_frambuffer->Resize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);

					_cameraController.OnEvent(WindowResizeEvent((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y));
				}

				ImGui::Image((void*)_frambuffer->GetColorAttachmentRendererId(), ImVec2(_viewportSize.x, _viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::End();
				ImGui::PopStyleVar();
			}
			
			ImGui::End();
		}

		{
			ImGui::Begin("Test transform");

			glm::vec3 pos = _transform->GetLocalPosition();
			if (ImGui::DragFloat3("Local position", &pos.x, 0.1f))
			{
				_transform->SetLocalPosition(pos);
			}

			glm::vec3 euler = _transform->GetEulerRotation();
			if (ImGui::DragFloat3("Local rotation", &euler.x, 0.1f))
			{
				_transform->SetEulerLocalPosition(euler);
			}

			glm::vec3 scl = _transform->GetLocalScale();
			if (ImGui::DragFloat3("Local scale", &scl.x, 0.1f))
			{
				_transform->SetLocalScale(scl);
			}

			ImGui::End();
		}

		ImGui::End();
	}

}