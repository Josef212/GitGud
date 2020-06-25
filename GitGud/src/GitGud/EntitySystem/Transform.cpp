#include "ggpch.h"
#include "Transform.h"

namespace GitGud
{
	Transform::Transform() : _localPosition({ 0.0f }), _localRotation(glm::vec3(0.0f)), _localScale({ 1.f }),
		_eulerLocalRotation({0.0f}), _localTransformMatrix({1.0f})
	{
	}

	void Transform::SetLocalPosition(const glm::vec3& pos)
	{
		_localPosition = pos;
		UpdateTransform();
	}

	void Transform::SetLocalRotation(const glm::quat& rot)
	{
		_localRotation = rot;
		_eulerLocalRotation = glm::eulerAngles(_localRotation);
		UpdateTransform();
	}

	void Transform::SetLocalScale(const glm::vec3& scl)
	{
		_localScale = scl;
		UpdateTransform();
	}

	void Transform::SetEulerLocalPosition(const glm::vec3& rot)
	{
		_eulerLocalRotation = rot;
		_localRotation = glm::quat(_eulerLocalRotation);
		UpdateTransform();
	}

	void Transform::UpdateTransform()
	{
		_localTransformMatrix = glm::translate(glm::mat4(1.0f), _localPosition);
		_localTransformMatrix = glm::rotate(_localTransformMatrix, _eulerLocalRotation.x, glm::vec3(1.f, 0.f, 0.f));
		_localTransformMatrix = glm::rotate(_localTransformMatrix, _eulerLocalRotation.y, glm::vec3(0.f, 1.f, 0.f));
		_localTransformMatrix = glm::rotate(_localTransformMatrix, _eulerLocalRotation.z, glm::vec3(0.f, 0.f, 1.f));
		_localTransformMatrix = glm::scale(_localTransformMatrix, _localScale);
	}
}
