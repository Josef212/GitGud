#pragma once

#include "entt.hpp"

#include "GitGud/Core/Timestep.h"

namespace GitGud
{
	class Entity;

	class Scene
	{
	friend class Entity;
	friend class SceneHierarchyPanel;

	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "Entity");
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		template <typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry _registry;
		uint32_t _viewportWidth = 0, _viewportHeight = 0;
	};
}