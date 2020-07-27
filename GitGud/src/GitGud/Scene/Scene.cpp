#include "ggpch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"

// TMP
#include "GitGud/Renderer/Renderer2D.h"

namespace GitGud
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity e = { _registry.create(), this };
		e.AddComponent<TransformComponent>();
		auto& t = e.AddComponent<TagComponent>(name);
		return e;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto e : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(e);
			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
}