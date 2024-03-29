#pragma once

#include <GitGud.h>
#include "GitGud/Renderer/Cameras/EditorCamera.h"

#include "Panels/EditorPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/EntityInspectorPanel.h"
#include "Panels/ContentPanel.h"
#include "Panels/StatsPanel.h"

namespace GitGud
{
	class EditorLayer : public Layer
	{
		enum class SceneState
		{
			Edit = 0,
			Play = 1,
			Simulate = 2,
		};

	public:
		EditorLayer();
		virtual ~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

	private:
		bool OnKeyPressed(KeyPressEvent e);
		bool OnMouseButtonPressed(MouseButtonPressEvent e);

		void MainMenuBar();
		void DrawPanels();
		void DrawViewport();
		void DrawToolbar();
		void DrawSettings();

		void RenderOverlay();

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveSceneAs();
		void SaveScene();

		void SerializeScene(Ref<Scene> scene, std::filesystem::path path);

		void OnScenePlay();
		void OnSceneSimulate();
		void OnSceneStop();

		void OnDuplicateEntity();

		void Gizmos();

		void LoadConfigs();
		void SaveConfigs();

	private:
		EditorCamera _editorCamera;

		Ref<Texture2D> _checkerTexture;
		Ref<Framebuffer> _frambuffer;

		glm::vec2 _viewportSize;
		glm::vec2 _viewportBounds[2];
		bool _viewportFocused = false, _viewportHovered = false;

		int _selectedOperation = -1;

		Ref<Scene> _activeScene;
		Ref<Scene> _editorScene;
		std::filesystem::path _editorScenePath;

		Entity _cameraEntity;

		Entity _hoveredEntity;

		SceneState _sceneState = SceneState::Edit;

		// TODO: Manage editor resources properly
		Ref<Texture2D> _playIcon;
		Ref<Texture2D> _stopIcon;
		Ref<Texture2D> _simulateIcon;

		// Panels
		std::map<std::type_index, EditorPanel*> _editorPanels;
		SceneHierarchyPanel _sceneHiararchyPanel;
		EntityInspectorPanel _entityInspectorPanel;
		ContentPanel _contentPanel;
		StatsPanel _statsPanel;

		// Settings
		bool _showPhysicsColliders = false;
	};
}