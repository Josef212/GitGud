#pragma once

#include "OrthographicCamera.h"
#include "GitGud/Core/Timestep.h"

#include "GitGud/Events/ApplicationEvent.h"
#include "GitGud/Events/KeyEvent.h"
#include "GitGud/Events/MouseEvent.h"

namespace GitGud
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);
		~OrthographicCameraController() = default;

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		void OnImGuiRender();

		inline OrthographicCamera& GetCamera() { return _camera; }
		inline const OrthographicCamera& GetCamera() const { return _camera; }

	private:
		bool OnMouseScroll(MouseScrollEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		float _aspectRatio;
		float _zoomLevel;
		bool _rotation;

		glm::vec3 _cameraPos;
		float _cameraMoveSpeed;

		float _cameraRotation;
		float _cameraRotSpeed;

		OrthographicCamera _camera;
	};
}