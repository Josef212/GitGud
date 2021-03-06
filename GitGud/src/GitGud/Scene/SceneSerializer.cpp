#include "ggpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace YAML
{
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
		out << YAML::BeginMap; 
		out << YAML::Key << "Entity" << YAML::Value << "123456789"; // TODO: Entity ID

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

		//SerializeComponent<NativeScriptComponent>(out, entity, [](NativeScriptComponent& nativeScriptComponent, YAML::Emitter& _out) { });

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
		uint64_t uuid = in["Entity"].as<uint64_t>(); // TODO

		std::string name;
		auto tagComponent = in[typeid(TagComponent).name()];
		if (tagComponent)
			name = tagComponent["Tag"].as<std::string>();

		GG_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

		Entity deserializedEntity = scene->CreateEntity(name);
		
		DeserializeComponent<TransformComponent>(in, deserializedEntity, [](YAML::Node& _in, TransformComponent& transformComponent)
			{
				transformComponent.Translation = _in["Translation"].as<glm::vec3>();
				transformComponent.Rotation = _in["Rotation"].as<glm::vec3>();
				transformComponent.Scale = _in["Scale"].as<glm::vec3>();
			});

		DeserializeComponent<SpriteRendererComponent>(in, deserializedEntity, [](YAML::Node& _in, SpriteRendererComponent& spriteRendererComponent)
			{
				spriteRendererComponent.Color = _in["Color"].as<glm::vec4>();
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
		
		//DeserializeComponent<NativeScriptComponent>(in, deserializedEntity, [](YAML::Node& _in, NativeScriptComponent& nativeScriptComponent) { });
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