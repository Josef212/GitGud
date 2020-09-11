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

	void Scene::OnUpdate(Timestep ts)
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
		glm::mat4* cameraTransform = nullptr;

		{
			auto view = _registry.view<TransformComponent, CameraComponent>();
			for (auto e : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(e);
				
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (!mainCamera || !cameraTransform)
		{
			return;
		}

		Renderer2D::BeginScene(*mainCamera, *cameraTransform);

		{
			auto group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto e : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(e);
				Renderer2D::DrawQuad(transform, sprite.Color);
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
}