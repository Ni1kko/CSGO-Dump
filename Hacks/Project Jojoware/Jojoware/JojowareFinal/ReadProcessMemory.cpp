#include "ReadProcessMemory.h"
void MemoryRead::GetAll()
{
	HP = GetLocalHealth();
	Team = GetLocalTeam();
	BasePointer = GetLocalPlayer();
}

int MemoryRead::GetLocalHealth()
{
	DWORD hp = RPM<int>(GetLocalPlayer() + O.m_iHealth);
	return hp;
}
DWORD MemoryRead::GetLocalPlayer()
{
	DWORD localp = RPM<DWORD>((Init.ClientDLL + O.dwLocalPlayer));
	return localp;
}

int MemoryRead::GetLocalTeam()
{
	DWORD localt = RPM<DWORD>((GetLocalPlayer() + O.oTeam));
	return localt;
}

DWORD MemoryRead::GetEntity(int EntNum)
{
	DWORD entity = RPM<DWORD>((Init.ClientDLL + O.dwEntityList + (EntNum - 1) * 0x10));
	return entity;
}

int MemoryRead::GetEntityHealth(DWORD Entity)
{
	int health = RPM<int>((Entity + O.m_iHealth));
	return health;
}

int MemoryRead::GetEntityTeam(DWORD Entity)
{
	int team = RPM<int>((Entity + O.oTeam));
	return team;
}

void MemoryRead::SetSendPackets(bool set)
{
	WPM<bool>(Init.EngineDLL + O.dwbSendPackets, set);
}