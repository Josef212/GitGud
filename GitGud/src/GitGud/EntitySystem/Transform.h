#pragma once

#include "GitGud/Core/Core.h"
#include <glm/glm.hpp>

namespace GitGud
{
	class GG_API Transform
	{
	public:
		Transform();
		~Transform() = default;

		const glm::vec3& GetLocalPosition() const { return _localPosition; }
		const glm::quat& GetLocalRotation() const { return _localRotation; }
		const glm::vec3& GetLocalScale() const { return _localScale; }

		const glm::vec3& GetEulerRotation() const { return _eulerLocalRotation; }
		const glm::mat4& GetLocalTransformMatrix() const { return _localTransformMatrix; }

		void SetLocalPosition(const glm::vec3& pos);
		void SetLocalRotation(const glm::quat& rot);
		void SetLocalScale(const glm::vec3& scl);
		void SetEulerLocalPosition(const glm::vec3& rot);

	private:
		void UpdateTransform();

	public:

	private:
		glm::vec3 _localPosition;
		glm::quat _localRotation;
		glm::vec3 _localScale;
		glm::vec3 _eulerLocalRotation;

		glm::mat4 _localTransformMatrix;
	};
}