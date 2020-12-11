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
		e.AddComponent<TagComponent>(name);
		return e;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		_registry.destroy(entity);
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		{
			auto group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto e : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(e);
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}
		}

		Renderer2D::EndScene();
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Update scripts
		{
			_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) 
			{
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->_entity = Entity{ entity , this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);
			});
		}

		// Render 2D

		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;

		{
			auto view = _registry.view<TransformComponent, CameraComponent>();
			for (auto e : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(e);
				
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (!mainCamera)
		{
			return;
		}

		Renderer2D::BeginScene(*mainCamera, cameraTransform);

		{
			auto group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto e : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(e);
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}
		}

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		_viewportWidth = width;
		_viewportHeight = height;

		auto view = _registry.view<CameraComponent>();
		for (auto e : view)
		{
			auto& cameraCmp = view.get<CameraComponent>(e);
			if (!cameraCmp.FixedAsectRatio)
			{
				cameraCmp.Camera.SetViewportSize(width, height);
			}
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = _registry.view<CameraComponent>();
		for (auto e : view)
		{
			const auto& camera = view.get<CameraComponent>(e);
			if (camera.Primary)
				return Entity{ e, this };
		}

		return { };
	}

	template <typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& tag)
	{

	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& transform)
	{

	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& spriteRenderer)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& camera)
	{
		camera.Camera.SetViewportSize(_viewportWidth, _viewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& nativeScript)
	{

	}
}