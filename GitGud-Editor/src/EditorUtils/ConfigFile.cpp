#include "ConfigFile.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include "GitGud/Core/Log.h"
#include "GitGud/Core/Core.h"

namespace GitGud
{

	static std::string BoolToStr(bool val)
	{
		std::stringstream converter;
		converter << std::boolalpha << val;
		return converter.str();
	}

	static bool StrToBool(const std::string& text)
	{
		bool ret;
		std::istringstream(text) >> std::boolalpha >> ret;
		return ret;
	}

	ConfigFile::ConfigFile(const std::string& name) : _name(name)
	{
	}

	void ConfigFile::SerializeText(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Config" << YAML::Value << _name;
		out << YAML::Key << "Properties" << YAML::Value << YAML::BeginMap;

		// Serialize properties

		for (auto it : _properties)
		{
			out << YAML::Key << it.first << YAML::Value << it.second;
		}

		// --------------------

		out << YAML::EndMap << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
		fout.close();
	}

	void ConfigFile::SerializeBinary(const std::string& filepath)
	{
		GG_ASSERT(false, "Not implemented");
	}

	bool ConfigFile::DeserializeText(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		
		if (!data["Config"])
			return false;

		auto configName = data["Config"].as<std::string>();
		GG_ASSERT(configName == _name, "Mismatch config name and config file");

		auto properties = data["Properties"];
		if (properties)
		{
			for (auto prop : properties)
			{
				auto key = prop.first.as<std::string>();
				auto value = prop.second.as<std::string>();
				
				_properties[key] = value;
			}
		}

		return true;
	}

	bool ConfigFile::DeserializeBinary(const std::string& filepath)
	{
		GG_ASSERT(false, "Not implemented");
		return false;
	}

	template <typename T>
	T ConfigFile::Get(const std::string& name, T def)
	{
	}

	template <>
	int ConfigFile::Get(const std::string& name, int def)
	{
		if (!HasProperty(name))
		{
			return def;
		}

		return stoi(_properties[name]);
	}

	template <>
	float ConfigFile::Get(const std::string& name, float def)
	{
		if (!HasProperty(name))
		{
			return def;
		}

		return stof(_properties[name]);
	}

	template <>
	bool ConfigFile::Get(const std::string& name, bool def)
	{
		if (!HasProperty(name))
		{
			return def;
		}

		return StrToBool(_properties[name]);
	}

	template <>
	std::string ConfigFile::Get(const std::string& name, std::string def)
	{
		if (!HasProperty(name))
		{
			return def;
		}

		return _properties[name];
	}

	template <typename T>
	void ConfigFile::Set(const std::string& name, T value)
	{
	}

	template <>
	void ConfigFile::Set(const std::string& name, int value)
	{
		_properties[name] = std::to_string(value);
	}

	template <>
	void ConfigFile::Set(const std::string& name, float value)
	{
		_properties[name] = std::to_string(value);
	}

	template <>
	void ConfigFile::Set(const std::string& name, bool value)
	{
		_properties[name] = BoolToStr(value);
	}

	template <>
	void ConfigFile::Set(const std::string& name, std::string value)
	{
		_properties[name] = value;
	}
}