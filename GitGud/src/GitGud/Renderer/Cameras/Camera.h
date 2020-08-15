#pragma once

#include <glm/glm.hpp>

namespace GitGud
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection) : _projection(projection) {}
		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return _projection; }

	protected:
		glm::mat4 _projection = glm::mat4(1.0f);
	};
}
