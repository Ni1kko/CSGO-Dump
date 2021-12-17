#pragma once
#include "Includes.h"
#include "HackProcess.h"
class Offsets
{
public:
	DWORD dwForceJump = 0xC648AC;
	DWORD dwLocalPlayer = 0xCBD6B4;
	DWORD m_fFlags = 0x100;
	DWORD m_flFlashMaxAlpha = 0xA328;
	DWORD oTeam = 0xF0;
	DWORD oCrossID = 0xB390d;
	DWORD dwEntityList = 0x4C3E624;
	DWORD m_bDormant = 0xE9;
	DWORD m_iHealth = 0xFC;
	DWORD dwForceAttack = 0x307FCF4;
	DWORD m_flFlashDuration = 0xA32C;
	DWORD m_iGlowIndex = 0xA344;
	DWORD dwGlowObjectManager = 0x517D8A0;
	DWORD m_bSpotted = 0x939;
	DWORD m_vecOrigin = 0x134;
	DWORD m_vecViewOffset = 0x104;
	DWORD dwClientState = 0x588B34;
	DWORD dwClientState_ViewAngles = 0x4D10;
	DWORD m_dwBoneMatrix = 0x2698;
	DWORD m_clrRender = 0x70;
	DWORD m_iShotsFired = 0xA2E0;
	DWORD m_viewPunchAngle = 0x3010;
	DWORD m_aimPunchAngle = 0x301C;
	DWORD m_Local = 0x2FAC;
	DWORD m_iCompetitiveRanking = 0x1A84;
	DWORD m_bSpottedByMask = 0x97C;
	DWORD m_flC4Blow = 0x2980;
	DWORD dwGlobalVars = 0x588838;
	DWORD m_hActiveWeapon = 0x2EE8;
	DWORD m_iItemDefinitionIndex = 0x2F9A;
	DWORD m_iItemIDHigh = 0x2FB0;
	DWORD m_flFallbackWear = 0x31B0;
	DWORD m_nFallbackSeed = 0x31AC;
	DWORD m_nFallbackPaintKit = 0x31A8;
	DWORD m_nFallbackStatTrak = 0x31B4;
	DWORD m_hMyWeapons = 0x2DE8;
	DWORD m_OriginalOwnerXuidHigh = 0x31A4;
	DWORD m_OriginalOwnerXuidLow = 0x31A0;
	DWORD ShotHits = 0x99EA28; //server.dll
	DWORD m_iTeam = 0xF38;
	DWORD m_iPing = 0xB28;
	DWORD m_iScore = 0x1980;
	DWORD dwPlayerResource = 0x30800DC;
	DWORD dwModelAmbientMin = 5815212;
	DWORD dwSetClanTag = 0x894F0;

	DWORD clientstate_choked_commands = 0x4CB0;
	DWORD clientstate_delta_ticks = 0x174;
	DWORD clientstate_last_outgoing_command = 0x4CAC;
	DWORD clientstate_net_channel = 0x9C;
	DWORD dwbSendPackets = 0xD210A;
	DWORD dwInput = 0x5118990;
};
extern Offsets O;
class MemoryRead
{
public:
	DWORD BasePointer;
	int Team;
	int HP;
	void GetAll();
	int GetLocalHealth();
	DWORD GetLocalPlayer();
	int GetLocalTeam();
	DWORD GetEntity(int EntNum);
	int GetEntityHealth(DWORD Entity);
	int GetEntityTeam(DWORD Entity);
	void SetSendPackets(bool set);
}; extern MemoryRead Read;
template <class T>
T RPM(DWORD address)
{
	T read;
	ReadProcessMemory(Init.Hdl, (DWORD*)address, &read, sizeof(T), 0);
	return read;
}
template <class T>
void WPM(DWORD address, T value)
{
	WriteProcessMemory(Init.Hdl, (DWORD*)address, &value, sizeof(T), 0);
}