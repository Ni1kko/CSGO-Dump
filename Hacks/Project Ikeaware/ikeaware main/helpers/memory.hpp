#pragma once

#include "utils.hpp"

class Memory
{
public:
	Memory()
	{
		IsLineGoesThroughSmoke = Utils::PatternScan("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
		ListLeavesInBox = Utils::PatternScan("client.dll", "52 8B 55 0C 8B 01 56 52 FF 50 18 89 44 24 14") + 0xB;
		LoadNamedSky = Utils::PatternScan("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45");
		SetClantag = Utils::PatternScan("engine.dll", "53 56 57 8B DA 8B F9 FF 15");
		CamThink = Utils::PatternScan("client.dll", "85 C0 75 30 38 86");
		AcceptMatch = Utils::PatternScan("client.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12");
		GetSequenceActivity = Utils::PatternScan("client.dll", "55 8B EC 83 7D 08 FF 56 8B F1 74 3D");
		HasC4 = Utils::PatternScan("client.dll", "56 8B F1 85 F6 74 31");
		InvalidateBoneCache = Utils::PatternScan("client.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");
		InReload = Utils::PatternScan("client.dll", "C6 87 ? ? ? ? ? 8B 06 8B CE FF 90") + 0x2;
		SetAbsOrigin = Utils::PatternScan("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1");
		PredictionRandomSeed = Utils::PatternScan("client.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 0x2;
		
		FrameNetUpdateStart = Utils::PatternScan("client.dll", "FF 50 40 5F 5E 5D C2 04 00 80") + 0x3;
		FrameNetUpdateEnd = Utils::PatternScan("client.dll", "FF 50 40 5F 5E 5D C2 04 00 83") + 0x3;
		FrameRenderStart = Utils::PatternScan("client.dll", "FF 50 40 8B 1D") + 0x3;
		FrameRenderEnd = Utils::PatternScan("client.dll", "FF 50 40 E8 ? ? ? ? 8B 0D") + 0x3;
	}

	std::uint8_t* IsLineGoesThroughSmoke;
	std::uint8_t* ListLeavesInBox;
	std::uint8_t* LoadNamedSky;
	std::uint8_t* SetClantag;
	std::uint8_t* CamThink;
	std::uint8_t* AcceptMatch;
	std::uint8_t* GetSequenceActivity;
	std::uint8_t* HasC4;
	std::uint8_t* InvalidateBoneCache;
	std::uint8_t* InReload;
	std::uint8_t* SetAbsOrigin;
	std::uint8_t* PredictionRandomSeed;

	std::uint8_t* FrameRenderStart;
	std::uint8_t* FrameRenderEnd;
	std::uint8_t* FrameNetUpdateStart;
	std::uint8_t* FrameNetUpdateEnd;
};

inline Memory* g_Memory;