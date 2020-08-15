#pragma once


#include "GitGud/Renderer/Cameras/Camera.h"

namespace GitGud
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float clipNear, float clipFar);
		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthographicSize() const { return _orthographicSize; }
		void GetOrthographicSize(float size) { _orthographicSize = size; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		float _orthographicSize = 10.0f;
		float _orthographicNear = -1.0f, _orthographicFar = 1.0f;

		float _aspectRatio = 0.0f;
	};
}