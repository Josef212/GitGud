#pragma once

#include <GitGud.h>
#include "GitGud/Renderer/Cameras/EditorCamera.h"
#include "Panels/SceneHierarchyPanel.h"

namespace GitGud
{
	class EditorLayer : public Layer
	{
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

		void NewScene();
		void OpenScene();
		void SaveSceneAs();

		void Gizmos();

	private:
		OrthographicCameraController _cameraController;
		EditorCamera _editorCamera;

		Ref<Texture2D> _checkerTexture;

		Ref<Framebuffer> _frambuffer;

		glm::vec2 _viewportSize;
		bool _viewportFocused = false, _viewportHovered = false;

		int _selectedOperation = -1;

		Ref<Scene> _activeScene;
		Entity _cameraEntity;

		// Panels
		SceneHierarchyPanel _sceneHiararchyPanel;
	};
}