#pragma once

#include "entt.hpp"

#include "GitGud/Core/Timestep.h"
#include "GitGud/Renderer/Cameras/EditorCamera.h"

class b2World;

namespace GitGud
{
	class Entity;

	class Scene
	{
	friend class Entity;
	friend class SceneSerializer;
	friend class SceneHierarchyPanel;

	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "Entity");
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		// TODO: Might want to improve this
		Entity GetPrimaryCameraEntity();

	private:
		template <typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry _registry;
		uint32_t _viewportWidth = 0, _viewportHeight = 0;
		b2World* _physicsWorld = nullptr;
	};
}