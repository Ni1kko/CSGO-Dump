#include "CheatManager.h"
// Change the path here:
/* THERE YOU GO -------------------->*/LPCSTR FilePath = "./Offsets.ini"; //<------------------------------------- HERE


using namespace std;

void CheatManager::CoutOffset(LPCSTR name, DWORD offset)
{
	cout << "[" << name << "]" << " = 0x" << uppercase << hex << offset << endl;
}
void CheatManager::Coutfloat(LPCSTR name, float option)
{
	cout << "[" << name << "]" << " = " << dec << option << endl;
}
void CheatManager::Coutbool(LPCSTR name, int boolean)
{
	if (boolean == 1)
		cout << "[" << name << "]" << " = " << "ON" << endl;
	else
		cout << "[" << name << "]" << " = " << "OFF" << endl;
}
float CheatManager::ReadConfig(LPCSTR Option, bool Float)
{
	if (Float)
	{
		char cfg[10];
		GetPrivateProfileString(TEXT("Config"), Option, TEXT("0"), cfg, 10, FilePath);
		float opt = atof(cfg);
		return opt;
	}
	else
		return GetPrivateProfileInt(TEXT("Config"), Option, NULL, FilePath);
}
DWORD CheatManager::ReadInt(LPCTSTR Offsetname)
{
	DWORD Offset = GetPrivateProfileInt(TEXT("Offsets"), Offsetname, NULL, FilePath);
	CoutOffset(Offsetname, Offset);
	return Offset;
}
void CheatManager::Options()
{
	if (GetAsyncKeyState(0x56))
	{
		CM.TriggerbotModeToggle++;
		if (CM.TriggerbotModeToggle >= 2) CM.TriggerbotModeToggle = 0;
		if (CM.TriggerbotModeToggle == 1) cout << "Triggerbot is toggled" << endl;
		if (CM.TriggerbotModeToggle == 0) cout << "Triggerbot not toggled" << endl;
		Sleep(95);
	}
	if (GetAsyncKeyState(VK_F2))
	{
		CM.WantBhop++;
		if (CM.WantBhop >= 2) CM.WantBhop = 0;
		if (CM.WantBhop == 1) cout << "Bhop ON" << endl;
		if (CM.WantBhop == 0) cout << "Bhop OFF" << endl;
		Sleep(125);
	}
	if (GetAsyncKeyState(VK_DELETE))
	{
		CM.Shutdown = true;
	}
	if (GetAsyncKeyState(VK_F6))
	{
		WantAim++;
		if (WantAim > 1) WantAim = 0;
		if (WantAim == 0) cout << "[AIMBOT] OFF" << endl;
		else cout << "[AIMBOT] ON" << endl;
		Sleep(125);
	}
	if (GetAsyncKeyState(VK_F7))
	{
		AimbotFOV += 0.2f;
		cout << "[AIMBOT] FOV = " << AimbotFOV << endl;
		Sleep(125);
	}
	if (GetAsyncKeyState(VK_F8))
	{
		AimbotFOV -= 0.2f;
		cout << "[AIMBOT] FOV = " << AimbotFOV << endl;
		Sleep(125);
	}
	if (GetAsyncKeyState(VK_F9))
	{
		CM.AimbotSMOOTH += 0.2f;
		cout << "[AIMBOT] SMOOTH = " << CM.AimbotSMOOTH << endl;
		Sleep(125);
	}
	if (GetAsyncKeyState(VK_F10))
	{
		CM.AimbotSMOOTH -= 0.2f;
		cout << "[AIMBOT] SMOOTH = " << CM.AimbotSMOOTH << endl;
		Sleep(125);
	}
	if (GetAsyncKeyState(VK_F3))
	{
		CM.Bonetype -= 2;
		if (CM.Bonetype == 6) cout << "[AIMBOT] Aim on Body" << endl;
		if (CM.Bonetype < 6)
		{
			cout << "[AIMBOT] Aim on Head" << endl;
			CM.Bonetype = 8;
		}
		Sleep(200);
	}
	if (GetAsyncKeyState(VK_F1))
	{
		CM.WantChams++;
		if (CM.WantChams >= 4) CM.WantChams = 0;
		cout << "Glowstyle = " << CM.WantChams << endl;
		Sleep(125);
	}
	if (GetAsyncKeyState(VK_F11))
	{
		CM.WantESP++;
		if (CM.WantESP >= 2) CM.WantESP = 0;
		if (WantESP == 1) cout << "[ESP] ON" << endl;
		else cout << "[ESP] OFF" << endl;
		Sleep(125);
	}
}

void CheatManager::InitializeCheat()
{
	system("Color 01");
	cout << "[=================================[OFFSETS]====================================]" << endl;
	O.dwForceJump = ReadInt(TEXT("dwForceJump"));
	O.dwLocalPlayer = ReadInt(TEXT("dwLocalPlayer"));
	O.m_fFlags = ReadInt(TEXT("m_fFlags"));
	O.oTeam = ReadInt(TEXT("oTeam"));
	O.oCrossID = ReadInt(TEXT("oCrossID"));
	O.dwEntityList = ReadInt(TEXT("dwEntityList"));
	O.m_bDormant = ReadInt(TEXT("m_bDormant"));
	O.m_iHealth = ReadInt(TEXT("m_iHealth"));
	O.dwForceAttack = ReadInt(TEXT("dwForceAttack"));
	O.m_iGlowIndex = ReadInt(TEXT("m_iGlowIndex"));
	O.dwGlowObjectManager = ReadInt(TEXT("dwGlowObjectManager"));
	O.m_bSpotted = ReadInt(TEXT("m_bSpotted"));
	O.m_vecOrigin = ReadInt(TEXT("m_vecOrigin"));
	O.m_vecViewOffset = ReadInt(TEXT("m_vecViewOffset"));
	O.dwClientState = ReadInt(TEXT("dwClientState"));
	O.dwClientState_ViewAngles = ReadInt(TEXT("dwClientState_ViewAngles"));
	O.m_dwBoneMatrix = ReadInt(TEXT("m_dwBoneMatrix"));
	O.m_iShotsFired = ReadInt(TEXT("m_iShotsFired"));
	O.m_viewPunchAngle = ReadInt(TEXT("m_viewPunchAngle"));
	O.m_aimPunchAngle = ReadInt(TEXT("m_aimPunchAngle"));
	O.m_Local = ReadInt(TEXT("m_Local"));
	O.m_iCompetitiveRanking = ReadInt(TEXT("m_iCompetitiveRanking"));
	O.m_bSpottedByMask = ReadInt(TEXT("m_bSpottedByMask"));
	O.m_flC4Blow = ReadInt(TEXT("m_flC4Blow"));
	O.m_hActiveWeapon = ReadInt(TEXT("m_hActiveWeapon"));
	O.m_iItemDefinitionIndex = ReadInt(TEXT("m_iItemDefinitionIndex"));
	O.m_iItemIDHigh = ReadInt(TEXT("m_iItemIDHigh"));
	O.m_flFallbackWear = ReadInt(TEXT("m_flFallbackWear"));
	O.m_nFallbackSeed = ReadInt(TEXT("m_nFallbackSeed"));
	O.m_nFallbackPaintKit = ReadInt(TEXT("m_nFallbackPaintKit"));
	O.m_hMyWeapons = ReadInt(TEXT("m_hMyWeapons"));
	O.m_OriginalOwnerXuidHigh = ReadInt(TEXT("m_OriginalOwnerXuidHigh"));
	O.m_OriginalOwnerXuidLow = ReadInt(TEXT("m_OriginalOwnerXuidLow"));
	O.dwPlayerResource = ReadInt(TEXT("dwPlayerResource"));
	O.m_iTeam = ReadInt(TEXT("m_iTeam"));
	O.m_iPing = ReadInt(TEXT("m_iPing"));
	O.m_iScore = ReadInt(TEXT("m_iScore"));
	O.dwModelAmbientMin = ReadInt(TEXT("dwModelAmbientMin"));
	cout << "[================================[CONFIG]======================================]" << endl;
	CM.WantAim = ReadConfig(TEXT("Aimbot"), false);
	CM.AimbotFOV = ReadConfig(TEXT("AimbotFOV"), true);
	CM.AimbotSMOOTH = ReadConfig(TEXT("AimbotSMOOTH"), true);
	CM.Bonetype = ReadConfig(TEXT("Aimbone"), false);
	CM.WantESP = ReadConfig(TEXT("ESP"), false);
	CM.WantChams = ReadConfig(TEXT("Glowstyle"), false);
	CM.WantBhop = ReadConfig(TEXT("Bhop"), false);
	CM.ChamsBrightness = ReadConfig(TEXT("ChamsBrightness "), true);
	CM.WantChamsR = ReadConfig(TEXT("Chams"), false);
	Coutfloat(TEXT("Aimbot FOV"), CM.AimbotFOV);
	Coutfloat(TEXT("Aimbot SMOOTH"), CM.AimbotSMOOTH);
	Coutbool(TEXT("Aimbot"), CM.WantAim);
	Coutfloat(TEXT("Aimbone"), CM.Bonetype);
	Coutbool(TEXT("ESP"), CM.WantESP);
	Coutfloat(TEXT("Glowstyle"), CM.WantChams);
	Coutbool(TEXT("Bhop"), CM.WantBhop);
	Coutbool(TEXT("Chams"), CM.WantChamsR);
	Coutfloat(TEXT("Chams Brightness"), CM.ChamsBrightness);

}
