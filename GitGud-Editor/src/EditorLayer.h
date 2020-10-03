#pragma once

#include <GitGud.h>
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
		OrthographicCameraController _cameraController;
		Ref<Texture2D> _checkerTexture;

		Ref<Framebuffer> _frambuffer;

		glm::vec2 _viewportSize;
		bool _viewportFocused = false, _viewportHovered = false;
				
		Ref<Scene> _activeScene;
		Entity _cameraEntity;

		// Panels
		SceneHierarchyPanel _sceneHiararchyPanel;
	};
}