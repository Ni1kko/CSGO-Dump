// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Memory.h"

template <typename T>
static constexpr auto relativeToAbsolute(int* address) noexcept
{
	return reinterpret_cast<T>(reinterpret_cast<char*>(address + 1) + *address);
}

void Memory::initialize() noexcept
{
	auto temp = (uintptr_t)((uintptr_t)Cheat::Utilities->Memory_PatternScan(xorstr_("client.dll"), xorstr_("B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08")));
	for (;;)
	{
		if (temp != NULL)
		{
			temp += 1;
			hud = *(uintptr_t*)temp;
			findHudElement = relativeToAbsolute<decltype(findHudElement)>(reinterpret_cast<int*>(reinterpret_cast<char*>(temp) + 5));
			clearHudWeapon = (decltype(clearHudWeapon))Cheat::Utilities->Memory_PatternScan(xorstr_("client.dll"), xorstr_("55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C"));
			itemSchema = relativeToAbsolute<decltype(itemSchema)>((int*)((uintptr_t)Cheat::Utilities->Memory_PatternScan(xorstr_("client.dll"), xorstr_("E8 ? ? ? ? 0F B7 0F")) + 1));
			equipWearable = (decltype(equipWearable))Cheat::Utilities->Memory_PatternScan(xorstr_("client.dll"), xorstr_("55 8B EC 83 EC 10 53 8B 5D 08 57 8B F9"));
			return;
		}
		Sleep(100);
	}
}