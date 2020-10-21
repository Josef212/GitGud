#include "EditorLayer.h"
#include <imgui/imgui.h>

namespace GitGud
{
	EditorLayer::EditorLayer() : Layer("GitGud-Editor"), _cameraController(1280.0f / 720.0f), _viewportSize({0, 0}), _viewportFocused(false), _viewportHovered(false)
	{
		GG_PROFILE_FUNCTION();
	}

	EditorLayer::~EditorLayer()
	{
		GG_PROFILE_FUNCTION();
	}

	void EditorLayer::OnAttach()
	{
		GG_PROFILE_FUNCTION();

		_checkerTexture = Texture2D::Create("assets/textures/Checkerboard.png");

		FramebufferSpecification specs;
		specs.Width = 1280;
		specs.Height = 720;
		_frambuffer = Framebuffer::Create(specs);

		_activeScene = CreateRef<Scene>();

		// Entity tests
		_activeScene->CreateEntity("Square A").AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
		_activeScene->CreateEntity("Square B").AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		_cameraEntity = _activeScene->CreateEntity("CameraEntity");
		_cameraEntity.AddComponent<CameraComponent>();

		_activeScene->CreateEntity("TestCamera").AddComponent<CameraComponent>().Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnUpdate(Timestep ts) override
			{
				auto& transform = GetComponent<TransformComponent>();

				float speed = 5.0f;

				if (Input::IsKey(GG_KEY_A))
					transform.Translation.x -= speed * ts;
				if (Input::IsKey(GG_KEY_D))
					transform.Translation.x += speed * ts;
				if (Input::IsKey(GG_KEY_W))
					transform.Translation.y += speed * ts;
				if (Input::IsKey(GG_KEY_S))
					transform.Translation.y -= speed * ts;
			}
		};

		_cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		_sceneHiararchyPanel.SetContext(_activeScene);
	}

	void EditorLayer::OnDetach()
	{
		GG_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		GG_PROFILE_FUNCTION();

		// Resize
		if (FramebufferSpecification spec = _frambuffer->GetSpecification();
			_viewportSize.x > 0.0f && _viewportSize.y > 0.0f && (spec.Width != _viewportSize.x || spec.Height != _viewportSize.y))
		{
			_frambuffer->Resize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
			//_cameraController.OnResize(_viewportSize.x, _viewportSize.y); // TODO
			_activeScene->OnViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
		}

		if (_viewportFocused)
		{
			_cameraController.OnUpdate(ts);
		}

		Renderer2D::ResetStats();
		_frambuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();
		_activeScene->OnUpdate(ts);
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

		{
			static bool dockspaceOpen = true;
			bool optFullscreen = true;
			static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

			ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (optFullscreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			{
				windowFlags |= ImGuiWindowFlags_NoBackground;
			}

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", &dockspaceOpen, windowFlags);
			ImGui::PopStyleVar();

			if (optFullscreen)
			{
				ImGui::PopStyleVar(2);
			}

			// DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
			}
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

		// Panels
		{
			_sceneHiararchyPanel.OnImGui();
		}

		
		{
				ImGui::Begin("Stats");

				auto stats = Renderer2D::GetStatistics();
				ImGui::Text("Renderer2D Stats:");
				ImGui::Text("Draw calls: %d", stats.DrawCalls);
				ImGui::Text("Quads: %d", stats.QuadCount);
				ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
				ImGui::Text("Indicies: %d", stats.GetTotalIndexCount());

				if (ImGui::Button("Begin session"))
				{
					GG_PROFILE_BEGIN_SESSION("TestSession", "testSession.json");
				}

				ImGui::SameLine();

				if (ImGui::Button("End session"))
				{
					GG_PROFILE_END_SESSION();
				}

				ImGui::End();
			}

		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Viewport");

			_viewportFocused = ImGui::IsWindowFocused();
			_viewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->SetBlockEvents(!_viewportFocused || !_viewportHovered);

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			ImGui::Image((void*)_frambuffer->GetColorAttachmentRendererId(), ImVec2(_viewportSize.x, _viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();
			ImGui::PopStyleVar();
		}

		ImGui::End();
	}
}