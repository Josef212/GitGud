#include "ggpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GitGud
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float clipNear, float clipFar)
	{
		_orthographicSize = size;
		_orthographicNear = clipNear;
		_orthographicFar = clipFar;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		_aspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		float orthoLeft = -_orthographicSize * _aspectRatio * 0.5f;
		float orthoRight = _orthographicSize * _aspectRatio * 0.5f;
		float orthoBot = -_orthographicSize * 0.5f;
		float orthoTop = _orthographicSize * 0.5f;

		_projection = glm::ortho(orthoLeft, orthoRight, orthoBot, orthoTop, _orthographicNear, _orthographicFar);
	}

}