#pragma once
#include "../../helpers/math.hpp"
#include "../../helpers/utils.hpp"
#include "../menu/menu.hpp"
#include "../../helpers/input.hpp"

template< typename T >
class ConfigItem {
	std::string category, name;
	T* value;
public:
	ConfigItem(std::string category, std::string name, T* value)
	{
		this->category = category;
		this->name = name;
		this->value = value;
	}
};

template< typename T >
class ConfigValue {
public:
	ConfigValue(std::string category_, std::string name_, T* value_, const T& def)
	{
		category = category_;
		name = name_;
		value = value_;
		default_value = def; 
	}

	std::string category, name;
	T* value;
	T default_value;
};

class CConfig : public Singleton<CConfig> {
	friend class Singleton<CConfig>;

protected:
	std::vector< ConfigValue< int >* > ints;
	std::vector< ConfigValue< char >* > chars;
	std::vector< ConfigValue< bool >* > bools;
	std::vector< ConfigValue< float >* > floats;
private:
	void SetupValue(int&, int, std::string, std::string);
	void SetupValue(char* value, char* def, std::string category, std::string name);
	void SetupValue(bool&, bool, std::string, std::string);
	void SetupValue(float&, float, std::string, std::string);
	void SetupColor(float value[4], std::string name);
	void SetupLegit();
	void SetupVisuals();
	void SetupMisc();
	void SetupSkins();
	void SetupColors();
public:
	void Setup();
	void Load(const std::string& name);
	void Save(const std::string& name);
};
