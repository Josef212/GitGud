#pragma once

#include "entt.hpp"

namespace GitGud
{
	class Scene
	{
	public:
		Scene();
		~Scene();

	private:
		entt::registry _registry;
	};
}