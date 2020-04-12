#pragma once

#include <glm/glm.hpp>

namespace GitGud
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return _position; }
		void SetPosition(const glm::vec3& pos) { _position = pos; RecalculateViewMatrix(); }

		const float GetRotation() const { return _rotation; }
		void SetRotation(float rot) { _rotation = rot; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return _projection; }
		const glm::mat4& GetViewMatrix() const { return _view; }
		const glm::mat4& GetViewProjectionMatrix() const { return _viewProjection; }

		void SetProjection(float left, float right, float bottom, float top);

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 _projection;
		glm::mat4 _view;
		glm::mat4 _viewProjection;

		glm::vec3 _position;
		float _rotation = 0.0f; // Around Z axis
	};
}
