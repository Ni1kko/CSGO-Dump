#pragma once
#include <iostream>
#include <vector>
template< typename T >
class ConfigItem
{
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
class ConfigValue
{
public:
	ConfigValue(std::string category_, std::string name_, T* value_)
	{
		category = category_;
		name = name_;
		value = value_;
	}

	std::string category, name;
	T* value;
};

class CConfig
{
protected:
	std::vector< ConfigValue< int >* > ints;
	std::vector< ConfigValue< bool >* > bools;
	std::vector< ConfigValue< float >* > floats;
private:
	
	void SetupValue(int&, int, std::string, std::string);
	void SetupValue(bool&, bool, std::string, std::string);
	void SetupValue(bool * value, bool def, int size, std::string category, std::string name);
	void SetupValue(float&, float, std::string, std::string);
public:
	CConfig()
	{
		Setup();
	}

	void Setup();

	void export_to_clipboard(std::string file);
	bool import_from_clipboard(std::string file);
	void Save(std::string ConfigName);
	void Load(std::string ConfigName);
};

extern CConfig* Config2;