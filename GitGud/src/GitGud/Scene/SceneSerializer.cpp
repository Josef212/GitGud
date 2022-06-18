#include "ggpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();

			return true;
		}
	};
}

namespace GitGud
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static std::string RigidBody2DBodyTypeToString(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case GitGud::Rigidbody2DComponent::BodyType::Static: return "Static";
			case GitGud::Rigidbody2DComponent::BodyType::Dynamic: return "Dynamic";
			case GitGud::Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";

			GG_CORE_ASSERT(false, "Unknown body type");
			return "";
		}
	}

	static Rigidbody2DComponent::BodyType RigidBody2DBodyTypeFromString(std::string bodyTypeStr)
	{
		if (bodyTypeStr == "Static") return Rigidbody2DComponent::BodyType::Static;
		if (bodyTypeStr == "Dynamic") return Rigidbody2DComponent::BodyType::Dynamic;
		if (bodyTypeStr == "Kinematic") return Rigidbody2DComponent::BodyType::Kinematic;

		GG_CORE_ASSERT(false, "Unknown body type");
		return Rigidbody2DComponent::BodyType::Static;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : _scene(scene)
	{
	}

	template<typename T, typename SC>
	static void SerializeComponent(YAML::Emitter& out, Entity entity, SC serializeCbk)
	{
		if (entity.HasComponent<T>())
		{
			out << YAML::Key << typeid(T).name() << YAML::BeginMap; // ComponentType

			serializeCbk(entity.GetComponent<T>(), out);

			out << YAML::EndMap; // ComponentType
		}
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		GG_CORE_ASSERT(entity.HasComponent<GuidComponent>(), "");

		out << YAML::BeginMap; 
		out << YAML::Key << "Entity" << YAML::Value << entity.GetGUID();

		SerializeComponent<TagComponent>(out, entity, [](TagComponent& tagComponent, YAML::Emitter& _out)
			{
				_out << YAML::Key << "Tag" << YAML::Value << tagComponent.Tag;
			});

		SerializeComponent<TransformComponent>(out, entity, [](TransformComponent& transformComponent, YAML::Emitter& _out)
			{
				_out << YAML::Key << "Translation" << YAML::Value << transformComponent.Translation;
				_out << YAML::Key << "Rotation" << YAML::Value << transformComponent.Rotation;
				_out << YAML::Key << "Scale" << YAML::Value << transformComponent.Scale;
			});

		SerializeComponent<SpriteRendererComponent>(out, entity, [](SpriteRendererComponent& spriteRendererComponent, YAML::Emitter& _out)
			{
				_out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
				// TODO: Serialize sprite path
				//_out << YAML::Key << "Sprite" << YAML::Value << spriteRendererComponent.Sprite->Path();
				_out << YAML::Key << "TilingFactor" << YAML::Value << spriteRendererComponent.TilingFactor;
			});

		SerializeComponent<CameraComponent>(out, entity, [](CameraComponent& cameraComponent, YAML::Emitter& _out)
			{
				auto& camera = cameraComponent.Camera;

				_out << YAML::Key << "Camera" << YAML::BeginMap;

				_out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
				_out << YAML::Key << "PerspectioveFOV" << YAML::Value << camera.GetPerspectiveVerticalFov();
				_out << YAML::Key << "PerspectioveNear" << YAML::Value << camera.GetPerspectiveNearClip();
				_out << YAML::Key << "PerspectioveFar" << YAML::Value << camera.GetPerspectiveFarClip();
				_out << YAML::Key << "OrthograpicSize" << YAML::Value << camera.GetOrthographicSize();
				_out << YAML::Key << "OrthograpicNear" << YAML::Value << camera.GetOrthographicNearClip();
				_out << YAML::Key << "OrthograpicFar" << YAML::Value << camera.GetOrthographicFarClip();

				_out << YAML::EndMap; // Camera

				_out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
				_out << YAML::Key << "FixedAsectRatio" << YAML::Value << cameraComponent.FixedAsectRatio;
			});

		// TODO
		//SerializeComponent<NativeScriptComponent>(out, entity, [](NativeScriptComponent& nativeScriptComponent, YAML::Emitter& _out) { });

		SerializeComponent<Rigidbody2DComponent>(out, entity, [](Rigidbody2DComponent& component, YAML::Emitter& _out)
			{
				_out << YAML::Key << "BodyType" << YAML::Value << RigidBody2DBodyTypeToString(component.Type);
				_out << YAML::Key << "FixedRotation" << YAML::Value << component.FixedRotation;
			});

		SerializeComponent<BoxCollider2DComponent>(out, entity, [](BoxCollider2DComponent& component, YAML::Emitter& _out)
			{
				_out << YAML::Key << "Offset" << YAML::Value << component.Offset;
				_out << YAML::Key << "Size" << YAML::Value << component.Size;
				_out << YAML::Key << "Density" << YAML::Value << component.Density;
				_out << YAML::Key << "Friction" << YAML::Value << component.Friction;
				_out << YAML::Key << "Restitution" << YAML::Value << component.Restitution;
				_out << YAML::Key << "RestitutionThreshold" << YAML::Value << component.RestitutionThreshold;
			});

		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::SerializeText(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		
		_scene->_registry.each([&](auto entityId)
			{
				Entity entity = { entityId, _scene.get() };
				if (!entity)
					return;

				SerializeEntity(out, entity);
			});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
		fout.close();
	}

	void SceneSerializer::SerializeBinary(const std::string& filepath)
	{
		GG_CORE_ASSERT(false, "Not implemented");
	}

	template<typename T, typename DC>
	static void DeserializeComponent(YAML::Node& in, Entity& entity, DC deserializeCbk)
	{
		auto cmp = in[typeid(T).name()];
		if (cmp)
		{
			auto& tc = entity.GetOrAddComponent<T>();
			deserializeCbk(cmp, tc);
		}
	}

	static void DeserializeEntity(const Ref<Scene>& scene, YAML::Node& in)
	{
		uint64_t guid = in["Entity"].as<uint64_t>();

		std::string name;
		auto tagComponent = in[typeid(TagComponent).name()];
		if (tagComponent)
		{
			name = tagComponent["Tag"].as<std::string>();
		}

		GG_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", guid, name);

		Entity deserializedEntity = scene->CreateEntity(guid, name);
		
		DeserializeComponent<TransformComponent>(in, deserializedEntity, [](YAML::Node& _in, TransformComponent& transformComponent)
			{
				transformComponent.Translation = _in["Translation"].as<glm::vec3>();
				transformComponent.Rotation = _in["Rotation"].as<glm::vec3>();
				transformComponent.Scale = _in["Scale"].as<glm::vec3>();
			});

		DeserializeComponent<SpriteRendererComponent>(in, deserializedEntity, [](YAML::Node& _in, SpriteRendererComponent& spriteRendererComponent)
			{
				spriteRendererComponent.Color = _in["Color"].as<glm::vec4>();
				// TODO: Deserialize sprite path and load texture
				spriteRendererComponent.TilingFactor = _in["TilingFactor"].IsDefined() ? _in["TilingFactor"].as<glm::vec2>() : glm::vec2(1.f, 1.f);
			});

		DeserializeComponent<CameraComponent>(in, deserializedEntity, [](YAML::Node& _in, CameraComponent& cameraComponent)
			{
				auto& camera = cameraComponent.Camera;
				auto cameraIn = _in["Camera"];

				if (cameraIn)
				{
					camera.SetProjectionType((SceneCamera::ProjectionType)cameraIn["ProjectionType"].as<int>());

					camera.SetPerspectiveVerticalFov(cameraIn["PerspectioveFOV"].as<float>());
					camera.SetPerspectiveNearClip(cameraIn["PerspectioveNear"].as<float>());
					camera.SetPerspectiveFarClip(cameraIn["PerspectioveFar"].as<float>());
					camera.SetOrthographicSize(cameraIn["OrthograpicSize"].as<float>());
					camera.SetOrthographicNearClip(cameraIn["OrthograpicNear"].as<float>());
					camera.SetOrthographicFarClip(cameraIn["OrthograpicFar"].as<float>());
				}

				cameraComponent.Primary = _in["Primary"].as<bool>();
				cameraComponent.FixedAsectRatio = _in["FixedAsectRatio"].as<bool>();
			});
		
		// TODO
		//DeserializeComponent<NativeScriptComponent>(in, deserializedEntity, [](YAML::Node& _in, NativeScriptComponent& nativeScriptComponent) { });

		DeserializeComponent<Rigidbody2DComponent>(in, deserializedEntity, [](YAML::Node& _in, Rigidbody2DComponent& component)
			{
				component.Type = RigidBody2DBodyTypeFromString(_in["BodyType"].as<std::string>());
				component.FixedRotation = _in["FixedRotation"].as<bool>();
			});

		DeserializeComponent<BoxCollider2DComponent>(in, deserializedEntity, [](YAML::Node& _in, BoxCollider2DComponent& component)
			{
				component.Offset = _in["Offset"].as<glm::vec2>();
				component.Size = _in["Size"].as<glm::vec2>();
				component.Density = _in["Density"].as<float>();
				component.Friction = _in["Friction"].as<float>();
				component.Restitution = _in["Restitution"].as<float>();
				component.RestitutionThreshold = _in["RestitutionThreshold"].as<float>();
			});
	}

	bool SceneSerializer::DeserializeText(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		GG_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				DeserializeEntity(_scene, entity);
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeBinary(const std::string& filepath)
	{
		GG_CORE_ASSERT(false, "Not implemented");
		return false;
	}
}