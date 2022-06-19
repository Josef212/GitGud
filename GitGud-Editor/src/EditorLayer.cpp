#include "EditorLayer.h"

#include "GitGud/Maths/Maths.h"

#include "EditorSelection.h"

#include <imgui/imgui.h>
#include <ImGuizmo.h>

namespace GitGud
{
	// TODO: Projects
	extern const std::filesystem::path g_assetsPath;

	EditorLayer::EditorLayer() : Layer("GitGud-Editor"), _viewportSize({0, 0}), _viewportFocused(false), _viewportHovered(false)
	{
		GG_PROFILE_FUNCTION();

		_editorPanels[typeid(StatsPanel)] = &_statsPanel;
		_editorPanels[typeid(SceneHierarchyPanel)] = &_sceneHiararchyPanel;
		_editorPanels[typeid(EntityInspectorPanel)] = &_entityInspectorPanel;
		_editorPanels[typeid(ContentPanel)] = &_contentPanel;
	}

	EditorLayer::~EditorLayer()
	{
		GG_PROFILE_FUNCTION();
	}

	void EditorLayer::OnAttach()
	{
		GG_PROFILE_FUNCTION();

		_hoveredEntity = Entity::Null();

		_checkerTexture = Texture2D::Create("assets/textures/Checkerboard.png");

		// Icons
		_playIcon = Texture2D::Create("EditorAssets/Icons/PlayButton.png");
		_stopIcon = Texture2D::Create("EditorAssets/Icons/StopButton.png");

		FramebufferSpecification specs;
		specs.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INT, FramebufferTextureFormat::DEPTH };
		specs.Width = 1280;
		specs.Height = 720;
		_frambuffer = Framebuffer::Create(specs);

		_activeScene = CreateRef<Scene>();
		_editorCamera = EditorCamera(30.f, 1.778f, 0.1f, 1000.0f);

#if 0
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
#else
		// TODO: Load default scene
#endif
		_sceneHiararchyPanel.SetContext(_activeScene);

		for (auto it : _editorPanels)
		{
			it.second->OnAttach();
		}
	}

	void EditorLayer::OnDetach()
	{
		GG_PROFILE_FUNCTION();

		for (auto it = _editorPanels.rbegin(); it != _editorPanels.rend(); ++it)
		{
			(*it).second->OnDettach();
		}
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		GG_PROFILE_FUNCTION();

		// Resize
		FramebufferSpecification spec = _frambuffer->GetSpecification();
		if (
				_viewportSize.x > 0.0f && _viewportSize.y > 0.0f 
				&& (spec.Width != _viewportSize.x || spec.Height != _viewportSize.y)
			)
		{
			_frambuffer->Resize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
			_editorCamera.SetViewportSize(_viewportSize.x, _viewportSize.y);
			_activeScene->OnViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
		}

		Renderer2D::ResetStats();
		_frambuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		// Clear entity ID frambebuffer attachment
		_frambuffer->ClearColorAttachment(1, -1);

		switch (_sceneState)
		{
			case GitGud::EditorLayer::SceneState::Edit:
			{
				if (_viewportFocused)
				{
					_editorCamera.OnUpdate(ts);
				}

				_activeScene->OnUpdateEditor(ts, _editorCamera);
				break;
			}
			case GitGud::EditorLayer::SceneState::Play:
			{
				_activeScene->OnUpdateRuntime(ts);
				break;
			}
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= _viewportBounds[0].x;
		my -= _viewportBounds[0].y;
		
		glm::vec2 viewportSize = _viewportBounds[1] - _viewportBounds[0];
		my = viewportSize.y - my;

		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int id = _frambuffer->ReadPixel(1, mouseX, mouseY);
			_hoveredEntity = id == -1 ? Entity::Null() : Entity((entt::entity)id, _activeScene.get());
		}

		_frambuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		GG_PROFILE_FUNCTION();

		_editorCamera.OnEven(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressEvent>(GG_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressEvent>(GG_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
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
			ImGuiStyle& style = ImGui::GetStyle();
			float minWinSizeX = style.WindowMinSize.x;
			style.WindowMinSize.x = 370.0f;

			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
			}

			style.WindowMinSize.x = minWinSizeX;
		}

		MainMenuBar();
		DrawPanels();
		DrawViewport();
		DrawToolbar();

		ImGui::Begin("Tmp");

		std::string name = _hoveredEntity ? _hoveredEntity.GetComponent<TagComponent>().Tag : "None";
		ImGui::Text("Hovered entity: %s", name.c_str());
		
		ImGui::End();

		ImGui::End();
	}

	bool EditorLayer::OnKeyPressed(KeyPressEvent e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		bool controlPressed = Input::IsKey(GG_KEY_LEFT_CONTROL) || Input::IsKey(GG_KEY_RIGHT_CONTROL);
		bool shiftPressed = Input::IsKey(GG_KEY_LEFT_SHIFT) || Input::IsKey(GG_KEY_RIGHT_SHIFT);

		switch (e.GetKeyCode())
		{
		case GG_KEY_N:
		{
			if (controlPressed)
				NewScene();
			break;
		}

		case GG_KEY_O:
		{
			if (controlPressed)
				OpenScene();
			break;
		}

		case GG_KEY_S:
		{
			if (controlPressed && shiftPressed)
				SaveSceneAs();
			else if (controlPressed)
				SaveScene();

			break;
		}

		case GG_KEY_D:
		{
			if (controlPressed)
				OnDuplicateEntity();
			break;
		}

		case GG_KEY_Q: _selectedOperation = -1; break;
		case GG_KEY_W: _selectedOperation = ImGuizmo::OPERATION::TRANSLATE; break;
		case GG_KEY_E: _selectedOperation = ImGuizmo::OPERATION::ROTATE; break;
		case GG_KEY_R: _selectedOperation = ImGuizmo::OPERATION::SCALE; break;
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressEvent e)
	{
		if (e.GetMouseButton() == GG_MOUSE_BUTTON_LEFT)
		{
			if (_viewportHovered && !ImGuizmo::IsOver() && !Input::IsKey(GG_KEY_LEFT_ALT))
			{
				EditorSelection::Select(_hoveredEntity);
			}
		}

		return false;
	}

	void EditorLayer::MainMenuBar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save...", "Ctrl+S"))
					SaveScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Exit", ""))
					Application::Get().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void EditorLayer::DrawPanels()
	{
		GG_PROFILE_FUNCTION();

		for (auto it : _editorPanels)
		{
			it.second->OnImGui();
		}
	}

	void EditorLayer::DrawViewport()
	{
		GG_PROFILE_FUNCTION();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		_viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		_viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		_viewportFocused = ImGui::IsWindowFocused();
		_viewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockEvents(!_viewportFocused && !_viewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureId = _frambuffer->GetColorAttachmentRendererId();
		ImGui::Image(reinterpret_cast<void*>(textureId), ImVec2(_viewportSize.x, _viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginDragDropTarget())
		{
			if (auto payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				auto relativePath = (const wchar_t*)payload->Data;
				OpenScene(g_assetsPath / relativePath);
			}

			ImGui::EndDragDropTarget();
		}

		Gizmos();

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorLayer::DrawToolbar()
	{
		auto& colors = ImGui::GetStyle().Colors;
		const auto& hovered = colors[ImGuiCol_ButtonHovered];
		const auto& active = colors[ImGuiCol_ButtonActive];

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(hovered.x, hovered.y, hovered.z, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(active.x, active.y, active.z, 0.5f));

		auto flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::Begin("##toolbar", nullptr, flags);

		float size = ImGui::GetWindowHeight() - 4.0f;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		auto icon = _sceneState == SceneState::Edit ? _playIcon : _stopIcon;
		if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (_sceneState == SceneState::Edit)
			{
				OnScenePlay();
			}
			else if (_sceneState == SceneState::Play)
			{
				OnSceneStop();
			}
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);

		ImGui::End();
	}

	void EditorLayer::NewScene()
	{
		_hoveredEntity = Entity::Null();
		EditorSelection::Select(Entity::Null());

		_activeScene = CreateRef<Scene>();
		_activeScene->OnViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
		_sceneHiararchyPanel.SetContext(_activeScene);
		
		_editorScenePath = std::filesystem::path();
	}

	void EditorLayer::OpenScene()
	{
		auto filepath = FileDialogs::OpenFile("GitGud Scene (*.gg)\0*.gg\0");
		if (filepath.has_value() && !filepath.value().empty())
		{
			OpenScene(filepath.value());
		}
	}
	
	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (path.extension().string() != ".gg")
		{
			GG_WARN("Could not load {0} - not scene file", path.filename().string());
			return;
		}

		if (_sceneState != SceneState::Edit)
		{
			OnSceneStop();
		}

		auto scene = CreateRef<Scene>();
		SceneSerializer serializer(scene);
		if (serializer.DeserializeText(path.string()))
		{
			_hoveredEntity = Entity::Null();
			EditorSelection::Select(Entity::Null());

			_editorScene = scene;
			_editorScene->OnViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
			_sceneHiararchyPanel.SetContext(_editorScene);

			_activeScene = _editorScene;
			_editorScenePath = path;
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		auto filepath = FileDialogs::SaveFile("GitGud Scene (*.gg)\0*.gg\0");
		if (filepath)
		{
			SerializeScene(_activeScene, *filepath);
			_editorScenePath = *filepath;
		}
	}

	void EditorLayer::SaveScene()
	{
		if (_editorScenePath.empty())
		{
			return;
		}

		SerializeScene(_activeScene, _editorScenePath);
	}

	void EditorLayer::SerializeScene(Ref<Scene> scene, std::filesystem::path path)
	{
		SceneSerializer serializer(scene);
		serializer.SerializeText(path.string());
	}
	
	void EditorLayer::OnScenePlay()
	{
		_sceneState = SceneState::Play;

		_activeScene = Scene::Copy(_editorScene);
		_activeScene->OnRuntimeStart();

		_sceneHiararchyPanel.SetContext(_activeScene);
	}

	void EditorLayer::OnSceneStop()
	{
		_sceneState = SceneState::Edit;

		_activeScene->OnRuntimeStop();
		_activeScene = _editorScene;

		_sceneHiararchyPanel.SetContext(_activeScene);
	}

	void EditorLayer::OnDuplicateEntity()
	{
		if (_sceneState != SceneState::Edit)
		{
			return;
		}

		Entity selected = EditorSelection::GetSelection();
		if (selected)
		{
			auto copy = _editorScene->DuplicateEntity(selected);
			EditorSelection::Select(copy);
		}
	}

	void EditorLayer::Gizmos()
	{
		GG_PROFILE_FUNCTION();

		if (_selectedOperation == -1)
			return;

		Entity selectedEntity = EditorSelection::GetSelection();
		if (!selectedEntity)
			return;

		//auto cameraEntity = _activeScene->GetPrimaryCameraEntity();
		//if (!cameraEntity)
		//	return;
		
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(_viewportBounds[0].x, _viewportBounds[0].y, _viewportBounds[1].x - _viewportBounds[0].x, _viewportBounds[1].y - _viewportBounds[0].y);

		// Camera
		//const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
		//const glm::mat4 cameraProj = camera.GetProjection();
		//const glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

		// EditorCamera
		const glm::mat4 cameraProj = _editorCamera.GetProjection();
		const glm::mat4 cameraView = _editorCamera.GetViewMatrix();

		auto& entityTransformCmp = selectedEntity.GetComponent<TransformComponent>();
		glm::mat4 entityTransform = entityTransformCmp.GetTransform();

		bool controlPressed = Input::IsKey(GG_KEY_LEFT_CONTROL) || Input::IsKey(GG_KEY_RIGHT_CONTROL);
		float snap = _selectedOperation == ImGuizmo::OPERATION::ROTATE ? 45.0f : 0.5f;
		float snaps[3] = { snap, snap, snap };

		ImGuizmo::Manipulate(
			glm::value_ptr(cameraView),
			glm::value_ptr(cameraProj),
			(ImGuizmo::OPERATION)_selectedOperation,
			ImGuizmo::LOCAL,
			glm::value_ptr(entityTransform),
			nullptr,
			controlPressed ? snaps : nullptr
		);

		if (ImGuizmo::IsUsing())
		{
			glm::vec3 translation, rotation, scale;
			if (Maths::DecomposeTransform(entityTransform, translation, rotation, scale))
			{
				entityTransformCmp.Translation = translation;
				entityTransformCmp.Rotation += (rotation - entityTransformCmp.Rotation);
				entityTransformCmp.Scale = scale;
			}
		}
	}
}