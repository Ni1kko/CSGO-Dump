#pragma once

#include <string>

class Profile
{
public:
	std::string Changer(void* pubDest, uint32_t* pcubMsgSize);
};

inline Profile* g_Profile;