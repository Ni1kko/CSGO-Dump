#include <iostream>
#include <vector>
#include <Windows.h>
#include <winerror.h>
#include <ShlObj.h>
#include <string>
#include "valve_sdk\misc\Color.hpp"
#pragma once
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
	void SetupValue(float&, float, std::string, std::string);
	void SetupValueC(Color& color, Color def, std::string category, std::string name);
public:
	CConfig()
	{
		Setup();
	}
	void Setup();
	void Save(std::string cfg);
	void Load(std::string cfg);
	void Clean(std::string cfg);
	void Show(std::string cfg);
};
extern CConfig* cConfigManager;
