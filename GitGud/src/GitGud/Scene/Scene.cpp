#include "ggpch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"

// TMP
#include "GitGud/Renderer/Renderer2D.h"

// Box2D
//#include "box2d/b2_world.h"
//#include "box2d/b2_body.h"
//#include "box2d/b2_fixture.h"
//#include "box2d/b2_polygon_shape.h"

#define WORLD_GRAVITY { 0.0f, -9.8f }

namespace GitGud
{
	//static b2BodyType GitGudRigidbody2DBodyTypeToBox2D(Rigidbody2DComponent::BodyType bodyType)
	//{
	//	switch (bodyType)
	//	{
	//	case GitGud::Rigidbody2DComponent::BodyType::Static: return b2_staticBody;
	//	case GitGud::Rigidbody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
	//	case GitGud::Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
	//	}

	//	GG_CORE_ASSERT(false, "Unknown body type");
	//	return b2_staticBody;
	//}

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

	void Scene::OnRuntimeStart()
	{
		//_physicsWorld = new b2World(WORLD_GRAVITY);

		auto view = _registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& trans = entity.GetComponent<TransformComponent>();
			auto& rb = entity.GetComponent<Rigidbody2DComponent>();

			//b2BodyDef bodyDef;
			//bodyDef.type = GitGudRigidbody2DBodyTypeToBox2D(rb.Type);
			//bodyDef.position.Set(trans.Translation.x, trans.Translation.y);
			//bodyDef.angle = trans.Rotation.z;

			//auto body = _physicsWorld->CreateBody(&bodyDef);
			//body->SetFixedRotation(rb.FixedRotation);

			//rb.RuntimeBody = body;

			//if (entity.HasComponent<BoxCollider2DComponent>())
			//{
			//	auto& collider = entity.GetComponent<BoxCollider2DComponent>();

			//	b2PolygonShape shape;
			//	shape.SetAsBox(trans.Scale.x * collider.Size.x, trans.Scale.y * collider.Size.y);

			//	b2FixtureDef fixtureDef;
			//	fixtureDef.shape = &shape;
			//	fixtureDef.density = collider.Density;
			//	fixtureDef.friction = collider.Friction;
			//	fixtureDef.restitution = collider.Restitution;
			//	fixtureDef.restitutionThreshold = collider.RestitutionThreshold;

			//	auto fixture = body->CreateFixture(&fixtureDef);

			//	collider.RuntimeFixture = fixture;
			//}
		}
	}

	void Scene::OnRuntimeStop()
	{
		//delete _physicsWorld;
		//_physicsWorld = nullptr;
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		{
			auto group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto e : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(e);
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)e);
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

		// Physics
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			//_physicsWorld->Step(ts, velocityIterations, positionIterations);

			auto view = _registry.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& trans = entity.GetComponent<TransformComponent>();
				auto& rb = entity.GetComponent<Rigidbody2DComponent>();

				//auto body = (b2Body*)rb.RuntimeBody;

				//const auto& position = body->GetPosition();
				//trans.Translation.x = position.x;
				//trans.Translation.y = position.y;

				//float rotation = body->GetAngle();
				//trans.Rotation.z = rotation;
			}
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