#pragma once

#include "entt.hpp"

#include "GitGud/Core/Timestep.h"
#include "GitGud/Core/GUID.h"
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

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = "Entity");
		Entity CreateEntity(GUID guid, const std::string& name = "Entity");
		void DestroyEntity(Entity entity);

		Entity DuplicateEntity(Entity entity);

		void OnViewportResize(uint32_t width, uint32_t height);

		// TODO: Might want to improve this
		Entity GetPrimaryCameraEntity();

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnSimulationStart();
		void OnSimulationStop();

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateSimulation(Timestep ts, EditorCamera& camera);

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return _registry.view<Components...>();
		}

	private:
		void OnPhysics2DStart();
		void OnPhysics2DStop();

		void RenderScene(EditorCamera& camera);

		template <typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry _registry;
		uint32_t _viewportWidth = 0, _viewportHeight = 0;
		b2World* _physicsWorld = nullptr;
	};
}