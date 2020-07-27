#pragma once

#include "entt.hpp"

#include "GitGud/Core/Timestep.h"

namespace GitGud
{
	class Entity;

	class Scene
	{
	friend class Entity;

	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "Entity");

		void OnUpdate(Timestep ts);

	private:
		entt::registry _registry;
	};
}