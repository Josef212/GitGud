#include "ggpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GitGud
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFov, float nearClip, float farClip)
	{
		_projectionType = ProjectionType::Perspective;

		_perspectiveFar = verticalFov;
		_perspectiveNear = nearClip;
		_perspectiveFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		_projectionType = ProjectionType::Orthographic;

		_orthographicSize = size;
		_orthographicNear = nearClip;
		_orthographicFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		_aspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (_projectionType == ProjectionType::Perspective)
		{
			_projection = glm::perspective(_perspectiveFov, _aspectRatio, _perspectiveNear, _perspectiveFar);
		}
		else
		{
			float orthoLeft = -_orthographicSize * _aspectRatio * 0.5f;
			float orthoRight = _orthographicSize * _aspectRatio * 0.5f;
			float orthoBot = -_orthographicSize * 0.5f;
			float orthoTop = _orthographicSize * 0.5f;

			_projection = glm::ortho(orthoLeft, orthoRight, orthoBot, orthoTop, _orthographicNear, _orthographicFar);
		}
	}

}