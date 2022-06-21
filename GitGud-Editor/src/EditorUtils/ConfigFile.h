#pragma once

#include <unordered_map>

namespace GitGud
{
	class ConfigFile
	{
	public:
		ConfigFile(const std::string& name);
		ConfigFile(const ConfigFile&) = default;

		void SerializeText(const std::string& filepath);
		void SerializeBinary(const std::string& filepath);

		bool DeserializeText(const std::string& filepath);
		bool DeserializeBinary(const std::string& filepath);

		bool HasProperty(const std::string& name) const { return _properties.find(name) != _properties.end(); }

		template<typename T>
		T Get(const std::string& name, T def);

		template<typename T>
		void Set(const std::string& name, T value);

	private:
		std::string _name;
		std::unordered_map<std::string, std::string> _properties;
	};
}