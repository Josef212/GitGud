#include "ggpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GitGud
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) : _projection(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), 
		_view(glm::mat4(1.0f)), _viewProjection(glm::mat4(1.0f)), _position(glm::vec3(0.0f)), _rotation(0.0f)
	{
		_viewProjection = _projection * _view;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		_viewProjection = _projection * _view;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position);
		transform = glm::rotate(transform, glm::radians(_rotation), glm::vec3(0, 0, 1));

		_view = glm::inverse(transform);
		_viewProjection = _projection * _view;
	}
}
