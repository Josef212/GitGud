#pragma once

#include <glm/glm.hpp>

namespace GitGud::Maths
{
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& euler, glm::vec3& scale);
}
