#pragma once
#include <Windows.h>
#include <string>
#include <xstring>

class tool
{
public:
	void check_internet();
	std::string httpRequest(std::string site, std::string param);

	tool();
	~tool();
};