#include "ggpch.h"
#include "OrthographicCameraController.h"

#include "GitGud/Input.h"
#include "GitGud/KeyCodes.h"

#include "imgui.h"

namespace GitGud
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) : _aspectRatio(aspectRatio), _zoomLevel(1.0f), _rotation(rotation),
		_cameraPos(0.0f), _cameraMoveSpeed(3.0f), _cameraRotation(0.0f), _cameraRotSpeed(45.0f),
		_camera(-_aspectRatio * _zoomLevel, _aspectRatio* _zoomLevel, -_zoomLevel, _zoomLevel)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKey(GG_KEY_A))
			_cameraPos.x -= _cameraMoveSpeed * ts;
		if (Input::IsKey(GG_KEY_D))
			_cameraPos.x += _cameraMoveSpeed * ts;
		if (Input::IsKey(GG_KEY_S))
			_cameraPos.y -= _cameraMoveSpeed * ts;
		if (Input::IsKey(GG_KEY_W))
			_cameraPos.y += _cameraMoveSpeed * ts;

		if (_rotation)
		{
			if (Input::IsKey(GG_KEY_Q))
				_cameraRotation += _cameraRotSpeed * ts;
			if (Input::IsKey(GG_KEY_E))
				_cameraRotation -= _cameraRotSpeed * ts;

			_camera.SetRotation(_cameraRotation);
		}

		_camera.SetPosition(_cameraPos);
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispathcer(e);
		dispathcer.Dispatch<MouseScrollEvent>(GG_BIND_EVENT_FN(OrthographicCameraController::OnMouseScroll));
		dispathcer.Dispatch<WindowResizeEvent>(GG_BIND_EVENT_FN(OrthographicCameraController::OnWindowResize));
	}

	void OrthographicCameraController::OnImGuiRender()
	{
		ImGui::Begin("Camera");
		
		ImGui::Checkbox("Rotation", &_rotation);

		ImGui::DragFloat3("Position", &_cameraPos.x, 0.1f);
		ImGui::DragFloat("Rotation", &_cameraRotation, 0.1f);
		
		if (ImGui::Button("Reset"))
		{
			_cameraPos = { 0.0f, 0.0f, 0.0f };
			_cameraRotation = 0.0f;
			_camera.SetPosition(_cameraPos);
			_camera.SetRotation(_cameraRotation);
		}
		
		ImGui::DragFloat("Camera move speed", &_cameraMoveSpeed, 0.5f);
		ImGui::DragFloat("Camera rotation speed", &_cameraRotSpeed, 0.5f);
		
		ImGui::End();
	}

	bool OrthographicCameraController::OnMouseScroll(MouseScrollEvent& e)
	{
		_zoomLevel -= e.GetYOffset() * 0.25f;
		_zoomLevel = std::max(_zoomLevel, 0.25f);
		_cameraMoveSpeed = _zoomLevel;

		_camera.SetProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& e)
	{
		_aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		_camera.SetProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);

		return false;
	}

}