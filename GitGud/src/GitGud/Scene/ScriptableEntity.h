#pragma once

#include "Entity.h"

namespace GitGud
{
	class ScriptableEntity
	{
	public:
		ScriptableEntity() = default;
		virtual ~ScriptableEntity() = default;

		template<typename T>
		T& GetComponent()
		{
			return _entity.GetComponent<T>();
		}

	private:
		Entity _entity;
		friend class Scene;
	};
}