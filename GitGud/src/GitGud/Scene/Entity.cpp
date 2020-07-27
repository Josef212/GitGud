#include "ggpch.h"
#include "Entity.h"

namespace GitGud
{
	Entity::Entity(entt::entity entity, Scene* scene) : _entityHandle(entity), _scene(scene)
	{
	}
}