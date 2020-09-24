#pragma once

#include "GitGud/Renderer/Cameras/Camera.h"

namespace GitGud
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		ProjectionType GetProjectionType() const { return _projectionType; }
		void SetProjectionType(ProjectionType type) { _projectionType = type; RecalculateProjection(); }

		void SetPerspective(float verticalFov, float nearClip, float farClip);
		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetPerspectiveVerticalFov() const { return _perspectiveFov; }
		void SetPerspectiveVerticalFov(float verticalFov) { _perspectiveFov = verticalFov; RecalculateProjection(); }
		float GetPerspectiveNearClip() const { return _perspectiveNear; }
		void SetPerspectiveNearClip(float nearClip) { _perspectiveNear = nearClip; RecalculateProjection(); }
		float GetPerspectiveFarClip() const { return _perspectiveFar; }
		void SetPerspectiveFarClip(float farClip) { _perspectiveFar = farClip; RecalculateProjection(); }

		float GetOrthographicSize() const { return _orthographicSize; }
		void SetOrthographicSize(float size) { _orthographicSize = size; RecalculateProjection(); }
		float GetOrthographicNearClip() const { return _orthographicNear; }
		void SetOrthographicNearClip(float nearClip) { _orthographicNear = nearClip; RecalculateProjection(); }
		float GetOrthographicFarClip() const { return _orthographicFar; }
		void SetOrthographicFarClip(float farClip) { _orthographicFar = farClip; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		ProjectionType _projectionType = ProjectionType::Orthographic;

		float _perspectiveFov = glm::radians(45.0f);
		float _perspectiveNear = 0.01f, _perspectiveFar = 1000.0f;

		float _orthographicSize = 10.0f;
		float _orthographicNear = -1.0f, _orthographicFar = 1.0f;

		float _aspectRatio = 0.0f;
	};
}