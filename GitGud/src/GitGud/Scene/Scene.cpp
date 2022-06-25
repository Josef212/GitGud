#include "ggpch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "ScriptableEntity.h"

// TMP
#include "GitGud/Renderer/Renderer2D.h"

// Box2D
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"

#define WORLD_GRAVITY { 0.0f, -9.8f }

namespace GitGud
{
	static b2BodyType GitGudRigidbody2DBodyTypeToBox2D(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case GitGud::Rigidbody2DComponent::BodyType::Static: return b2_staticBody;
		case GitGud::Rigidbody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
		case GitGud::Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		}

		GG_CORE_ASSERT(false, "Unknown body type");
		return b2_staticBody;
	}

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		delete _physicsWorld;
	}

	template<typename... TCmp>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<GUID, entt::entity>& enttMap)
	{
		([&]()
		{
			auto view = src.view<TCmp>();
			for (auto e : view)
			{
				GUID guid = src.get<GuidComponent>(e).Id;
				GG_CORE_ASSERT(enttMap.find(guid) != enttMap.end(), "");
				entt::entity dstEnttId = enttMap.at(guid);

				auto& cmp = src.get<TCmp>(e);
				dst.emplace_or_replace<TCmp>(dstEnttId, cmp);
			}
		}(), ...);
	}

	template<typename... TCmp>
	static void CopyComponent(ComponentGroup<TCmp...>, entt::registry& dst, entt::registry& src, const std::unordered_map<GUID, entt::entity>& enttMap)
	{
		CopyComponent<TCmp...>(dst, src, enttMap);
	}

	template<typename... TCmp>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
		{
			if (src.HasComponent<TCmp>())
			{
				dst.AddOrReplaceComponent<TCmp>(src.GetComponent<TCmp>());
			}
		}(), ...);
	}

	template<typename... TCmp>
	static void CopyComponentIfExists(ComponentGroup<TCmp...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<TCmp...>(dst, src);
	}

	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		auto scene = CreateRef<Scene>();

		scene->_viewportWidth = other->_viewportWidth;
		scene->_viewportHeight = other->_viewportHeight;

		std::unordered_map<GUID, entt::entity> enttMap;
		auto& srcRegistry = other->_registry;
		auto& dstRegistry = scene->_registry;

		// Create new entities
		auto idView = srcRegistry.view<GuidComponent>();
		for (auto e : idView)
		{
			GUID guid = srcRegistry.get<GuidComponent>(e).Id;
			const auto& name = srcRegistry.get<TagComponent>(e).Tag;

			Entity entity = scene->CreateEntity(guid, name);
			enttMap[guid] = (entt::entity)entity;
		}

		// Copy components but Id and Tag
		CopyComponent(AllComponents{}, dstRegistry, srcRegistry, enttMap);

		return scene;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntity(GUID(), name);
	}

	Entity Scene::CreateEntity(GUID guid, const std::string& name)
	{
		Entity e = { _registry.create(), this };
		e.AddComponent<GuidComponent>(guid);
		e.AddComponent<TransformComponent>();
		e.AddComponent<TagComponent>(name);
		return e;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		_registry.destroy(entity);
	}

	Entity Scene::DuplicateEntity(Entity entity)
	{
		auto& name = entity.GetComponent<TagComponent>().Tag;
		Entity copy = CreateEntity(name);
		CopyComponentIfExists(AllComponents{}, copy, entity);

		return copy;
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

	void Scene::OnRuntimeStart()
	{
		OnPhysics2DStart();
	}

	void Scene::OnRuntimeStop()
	{
		OnPhysics2DStop();
	}

	void Scene::OnSimulationStart()
	{
		OnPhysics2DStart();
	}

	void Scene::OnSimulationStop()
	{
		OnPhysics2DStop();
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Update scripts
		{
			_registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc) 
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
			_physicsWorld->Step(ts, velocityIterations, positionIterations);

			auto view = _registry.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& trans = entity.GetComponent<TransformComponent>();
				auto& rb = entity.GetComponent<Rigidbody2DComponent>();

				auto body = (b2Body*)rb.RuntimeBody;

				const auto& position = body->GetPosition();
				trans.Translation.x = position.x;
				trans.Translation.y = position.y;

				float rotation = body->GetAngle();
				trans.Rotation.z = rotation;
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
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)e);
			}
		}

		{
			auto view = _registry.view<TransformComponent, CircleRendererComponent>();
			for (auto e : view)
			{
				auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(e);
				Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)e);
			}
		}

		Renderer2D::EndScene();
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		RenderScene(camera);
	}

	void Scene::OnUpdateSimulation(Timestep ts, EditorCamera& camera)
	{
		// Physics
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			_physicsWorld->Step(ts, velocityIterations, positionIterations);

			auto view = _registry.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& trans = entity.GetComponent<TransformComponent>();
				auto& rb = entity.GetComponent<Rigidbody2DComponent>();

				auto body = (b2Body*)rb.RuntimeBody;

				const auto& position = body->GetPosition();
				trans.Translation.x = position.x;
				trans.Translation.y = position.y;

				float rotation = body->GetAngle();
				trans.Rotation.z = rotation;
			}
		}

		RenderScene(camera);
	}

	void Scene::OnPhysics2DStart()
	{
		_physicsWorld = new b2World(WORLD_GRAVITY);

		auto view = _registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& trans = entity.GetComponent<TransformComponent>();
			auto& rb = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = GitGudRigidbody2DBodyTypeToBox2D(rb.Type);
			bodyDef.position.Set(trans.Translation.x, trans.Translation.y);
			bodyDef.angle = trans.Rotation.z;

			auto body = _physicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb.FixedRotation);

			rb.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& collider = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape shape;
				// TODO: Use collider offset
				shape.SetAsBox(trans.Scale.x * collider.Size.x, trans.Scale.y * collider.Size.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &shape;
				fixtureDef.density = collider.Density;
				fixtureDef.friction = collider.Friction;
				fixtureDef.restitution = collider.Restitution;
				fixtureDef.restitutionThreshold = collider.RestitutionThreshold;

				auto fixture = body->CreateFixture(&fixtureDef);

				collider.RuntimeFixture = fixture;
			}

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& collider = entity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape shape;
				shape.m_p.Set(collider.Offset.x, collider.Offset.y);
				shape.m_radius = trans.Scale.x * collider.Radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &shape;
				fixtureDef.density = collider.Density;
				fixtureDef.friction = collider.Friction;
				fixtureDef.restitution = collider.Restitution;
				fixtureDef.restitutionThreshold = collider.RestitutionThreshold;

				auto fixture = body->CreateFixture(&fixtureDef);

				collider.RuntimeFixture = fixture;
			}
		}
	}

	void Scene::OnPhysics2DStop()
	{
		delete _physicsWorld;
		_physicsWorld = nullptr;
	}

	void Scene::RenderScene(EditorCamera& camera)
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

		{
			auto view = _registry.view<TransformComponent, CircleRendererComponent>();
			for (auto e : view)
			{
				auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(e);
				Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)e);
			}
		}

		Renderer2D::EndScene();
	}

	template <typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<GuidComponent>(Entity entity, GuidComponent& tag)
	{

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
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& spriteRenderer)
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

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& rb)
	{

	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& collider)
	{

	}

	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& collider)
	{

	}
}