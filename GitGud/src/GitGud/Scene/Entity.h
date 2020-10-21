#pragma once

#include "entt.hpp"
#include "Scene.h"

namespace GitGud 
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene);
		Entity(const Entity& other) = default;
		~Entity() = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			GG_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& cmp = _scene->_registry.emplace<T>(_entityHandle, std::forward<Args>(args)...);
			_scene->OnComponentAdded<T>(*this, cmp);
			return cmp;
		}

		template<typename T>
		T& GetComponent()
		{
			GG_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return _scene->_registry.get<T>(_entityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			GG_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			_scene->_registry.remove<T>(_entityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return _scene->_registry.has<T>(_entityHandle);
		}

		operator bool() const { return _entityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)_entityHandle; }
		operator entt::entity() const { return _entityHandle; }
		
		bool operator==(const Entity& other) const { return _entityHandle == other._entityHandle && _scene == other._scene; }
		bool operator!=(const Entity& other) const { return _entityHandle != other._entityHandle || _scene != other._scene; }

	private:
		entt::entity _entityHandle = entt::null;
		Scene* _scene = nullptr;
	};
}