#include "vars.h"
#include "json.h"

Var g_Var;
Skins g_Skins;

nlohmann::json preset;
nlohmann::json presetskin;

void assign(int &value, char* sz)
{
	if (preset[sz].empty())
		return;

	value = preset[sz].get<int>();
}

void assign(float &value, char* sz)
{
	if (preset[sz].empty())
		return;

	value = preset[sz].get<float>();
}

void assignskin(int &value, char* sz)
{
	if (presetskin[sz].empty())
		return;

	value = presetskin[sz].get<int>();
}

void assign_array(int value[3], char* sz)
{
	if (preset[sz]["r"].empty() || preset[sz]["g"].empty() || preset[sz]["b"].empty())
		return;

	value[0] = preset[sz]["r"].get<int>();
	value[1] = preset[sz]["g"].get<int>();
	value[2] = preset[sz]["b"].get<int>();
}

void assign_skinarray(int value[517], char* sz)
{
	if (presetskin[sz]["Deagle"].empty() || presetskin[sz]["Duals"].empty()
		|| presetskin[sz]["Five7"].empty() || presetskin[sz]["Glock"].empty()
		|| presetskin[sz]["Ak47"].empty() || presetskin[sz]["Aug"].empty()
		|| presetskin[sz]["Awp"].empty() || presetskin[sz]["Famas"].empty()
		|| presetskin[sz]["G3sg1"].empty() || presetskin[sz]["Galil"].empty()
		|| presetskin[sz]["M249"].empty() || presetskin[sz]["M4a4"].empty()
		|| presetskin[sz]["Mac10"].empty() || presetskin[sz]["P90"].empty()
		|| presetskin[sz]["Ump45"].empty() || presetskin[sz]["Xm1014"].empty()
		|| presetskin[sz]["Bizon"].empty() || presetskin[sz]["Mag7"].empty()
		|| presetskin[sz]["Negev"].empty() || presetskin[sz]["SawedOff"].empty()
		|| presetskin[sz]["Tec9"].empty() || presetskin[sz]["P2000"].empty()
		|| presetskin[sz]["Mp7"].empty() || presetskin[sz]["Mp9"].empty()
		|| presetskin[sz]["Nova"].empty() || presetskin[sz]["P250"].empty()
		|| presetskin[sz]["Scar20"].empty() || presetskin[sz]["Sg553"].empty()
		|| presetskin[sz]["Scout"].empty() || presetskin[sz]["M4a1s"].empty()
		|| presetskin[sz]["Usps"].empty() || presetskin[sz]["Cz75"].empty()
		|| presetskin[sz]["Revolver"].empty() || presetskin[sz]["Bayonet"].empty()
		|| presetskin[sz]["Flip"].empty() || presetskin[sz]["Gut"].empty()
		|| presetskin[sz]["Karambit"].empty() || presetskin[sz]["M9"].empty()
		|| presetskin[sz]["Huntsman"].empty() || presetskin[sz]["Falchion"].empty()
		|| presetskin[sz]["Bowie"].empty() || presetskin[sz]["Butterfly"].empty()
		|| presetskin[sz]["Daggers"].empty())
		return;

	value[1] = presetskin[sz]["Deagle"].get<float>();
	value[2] = presetskin[sz]["Duals"].get<float>();
	value[3] = presetskin[sz]["Five7"].get<float>();
	value[4] = presetskin[sz]["Glock"].get<float>();
	value[7] = presetskin[sz]["Ak47"].get<float>();
	value[8] = presetskin[sz]["Aug"].get<float>();
	value[9] = presetskin[sz]["Awp"].get<float>();
	value[10] = presetskin[sz]["Famas"].get<float>();
	value[11] = presetskin[sz]["G3sg1"].get<float>();
	value[13] = presetskin[sz]["Galil"].get<float>();
	value[14] = presetskin[sz]["M249"].get<float>();
	value[16] = presetskin[sz]["M4a4"].get<float>();
	value[17] = presetskin[sz]["Mac10"].get<float>();
	value[19] = presetskin[sz]["P90"].get<float>();
	value[24] = presetskin[sz]["Ump45"].get<float>();
	value[25] = presetskin[sz]["Xm1014"].get<float>();
	value[26] = presetskin[sz]["Bizon"].get<float>();
	value[27] = presetskin[sz]["Mag7"].get<float>();
	value[28] = presetskin[sz]["Negev"].get<float>();
	value[29] = presetskin[sz]["SawedOff"].get<float>();
	value[30] = presetskin[sz]["Tec9"].get<float>();
	value[32] = presetskin[sz]["P2000"].get<float>();
	value[33] = presetskin[sz]["Mp7"].get<float>();
	value[34] = presetskin[sz]["Mp9"].get<float>();
	value[35] = presetskin[sz]["Nova"].get<float>();
	value[36] = presetskin[sz]["P250"].get<float>();
	value[38] = presetskin[sz]["Scar20"].get<float>();
	value[39] = presetskin[sz]["Sg553"].get<float>();
	value[40] = presetskin[sz]["Scout"].get<float>();
	value[60] = presetskin[sz]["M4a1s"].get<float>();
	value[61] = presetskin[sz]["Usps"].get<float>();
	value[63] = presetskin[sz]["Cz75"].get<float>();
	value[64] = presetskin[sz]["Revolver"].get<float>();
	value[500] = presetskin[sz]["Bayonet"].get<float>();
	value[505] = presetskin[sz]["Flip"].get<float>();
	value[506] = presetskin[sz]["Gut"].get<float>();
	value[507] = presetskin[sz]["Karambit"].get<float>();
	value[508] = presetskin[sz]["M9"].get<float>();
	value[509] = presetskin[sz]["Huntsman"].get<float>();
	value[512] = presetskin[sz]["Falchion"].get<float>();
	value[514] = presetskin[sz]["Bowie"].get<float>();
	value[515] = presetskin[sz]["Butterfly"].get<float>();
	value[516] = presetskin[sz]["Daggers"].get<float>();
}

template<typename T>
void save(T value, char* sz)
{
	preset[sz] = value;
}

template<typename Tp>
void saveskin(Tp value, char* sz)
{
	presetskin[sz] = value;
}

void save_array(int value[3], char* sz)
{
	preset[sz]["r"] = value[0];
	preset[sz]["g"] = value[1];
	preset[sz]["b"] = value[2];
}

void save_skinarray(int value[517], char* sz)
{
	presetskin[sz]["Deagle"] = value[1];
	presetskin[sz]["Duals"] = value[2];
	presetskin[sz]["Five7"] = value[3];
	presetskin[sz]["Glock"] = value[4];
	presetskin[sz]["Ak47"] = value[7];
	presetskin[sz]["Aug"] = value[8];
	presetskin[sz]["Awp"] = value[9];
	presetskin[sz]["Famas"] = value[10];
	presetskin[sz]["G3sg1"] = value[11];
	presetskin[sz]["Galil"] = value[13];
	presetskin[sz]["M249"] = value[14];
	presetskin[sz]["M4a4"] = value[16];
	presetskin[sz]["Mac10"] = value[17];
	presetskin[sz]["P90"] = value[19];
	presetskin[sz]["Ump45"] = value[24];
	presetskin[sz]["Xm1014"] = value[25];
	presetskin[sz]["Bizon"] = value[26];
	presetskin[sz]["Mag7"] = value[27];
	presetskin[sz]["Negev"] = value[28];
	presetskin[sz]["SawedOff"] = value[29];
	presetskin[sz]["Tec9"] = value[30];
	presetskin[sz]["P2000"] = value[32];
	presetskin[sz]["Mp7"] = value[33];
	presetskin[sz]["Mp9"] = value[34];
	presetskin[sz]["Nova"] = value[35];
	presetskin[sz]["P250"] = value[36];
	presetskin[sz]["Scar20"] = value[38];
	presetskin[sz]["Sg553"] = value[39];
	presetskin[sz]["Scout"] = value[40];
	presetskin[sz]["M4a1s"] = value[60];
	presetskin[sz]["Usps"] = value[61];
	presetskin[sz]["Cz75"] = value[63];
	presetskin[sz]["Revolver"] = value[64];
	presetskin[sz]["Bayonet"] = value[500];
	presetskin[sz]["Flip"] = value[505];
	presetskin[sz]["Gut"] = value[506];
	presetskin[sz]["Karambit"] = value[507];
	presetskin[sz]["M9"] = value[508];
	presetskin[sz]["Huntsman"] = value[509];
	presetskin[sz]["Falchion"] = value[512];
	presetskin[sz]["Bowie"] = value[514];
	presetskin[sz]["Butterfly"] = value[515];
	presetskin[sz]["Daggers"] = value[516];
}

CConfigFile g_ConfigFile;

const bool CConfigFile::LoadConfig()
{
	std::ifstream input_file = std::ifstream("c:/kidua/rage.json");

	switch (g_ConfigFile.m_iID)
	{
	case 0:
		input_file = std::ifstream("c:/kidua/rage.json");
		break;
	case 1:
		input_file = std::ifstream("c:/kidua/rage1.json");
		break;
	case 2:
		input_file = std::ifstream("c:/kidua/rage2.json");
		break;
	}

	if (!input_file.good())
		return false;

	try
	{
		preset << input_file;
	}
	catch (...)
	{
		input_file.close();
		return false;
	}

	assign(g_Var.Ragebot[0].Enable, "[ Aimbot Light ] Enable");
	assign(g_Var.Ragebot[0].Autoshoot, "[ Aimbot Light ] Auto Fire");
	assign(g_Var.Ragebot[0].Multipoint, "[ Aimbot Light ] Multipoints");
	assign(g_Var.Ragebot[0].Multipointbox[0], "[ Aimbot Light ] Multipoints Head");
	assign(g_Var.Ragebot[0].Multipointbox[1], "[ Aimbot Light ] Multipoints Body");
	assign(g_Var.Ragebot[0].Multipointbox[2], "[ Aimbot Light ] Multipoints Arms");
	assign(g_Var.Ragebot[0].Multipointbox[3], "[ Aimbot Light ] Multipoints Legs");
	assign(g_Var.Ragebot[0].Multipointbox[4], "[ Aimbot Light ] Multipoints Feet");
	assign(g_Var.Ragebot[0].HeadPointScale, "[ Aimbot Light ] Multipoints Scale Head");
	assign(g_Var.Ragebot[0].PointScale, "[ Aimbot Light ] Multipoints Scale");
	assign(g_Var.Ragebot[0].AutoPointScale, "[ Aimbot Light ] Multipoints Scale Auto");
	assign(g_Var.Ragebot[0].BodyAfter, "[ Aimbot Light ] Target Body After");
	assign(g_Var.Ragebot[0].Team, "[ Aimbot Light ] Team");
	assign(g_Var.Ragebot[0].Spot, "[ Aimbot Light ] Prior Spot");
	assign(g_Var.Ragebot[0].Selection, "[ Aimbot Light ] Target Selection");
	assign(g_Var.Ragebot[0].Multibox, "[ Aimbot Light ] Multibox");
	assign(g_Var.Ragebot[0].Multiboxes[0], "[ Aimbot Light ] Multibox Head");
	assign(g_Var.Ragebot[0].Multiboxes[1], "[ Aimbot Light ] Multibox Body");
	assign(g_Var.Ragebot[0].Multiboxes[2], "[ Aimbot Light ] Multibox Arms");
	assign(g_Var.Ragebot[0].Multiboxes[3], "[ Aimbot Light ] Multibox Legs");
	assign(g_Var.Ragebot[0].Multiboxes[4], "[ Aimbot Light ] Multibox Feet");
	assign(g_Var.Ragebot[0].Fov, "[ Aimbot Light ] Field of View");
	assign(g_Var.Ragebot[0].Hitchance, "[ Aimbot Light ] Hitchance");
	assign(g_Var.Ragebot[0].Mindamage, "[ Aimbot Light ] Minimum Damage");
	assign(g_Var.Ragebot[0].Walldamage, "[ Aimbot Light ] Wall Minimum Damage");
	assign(g_Var.Ragebot[0].AutoMindDamage, "[ Aimbot Light ] Minimum Damage Auto");
	assign(g_Var.Ragebot[0].Silent, "[ Aimbot Light ] Silent");
	assign(g_Var.Ragebot[0].Autowall, "[ Aimbot Light ] Autowall");
	assign(g_Var.Ragebot[0].PosAdjust, "[ Aimbot Light ] Position Adjustment");
	assign(g_Var.Ragebot[0].Recoil, "[ Aimbot Light ] Remove Recoil");
	assign(g_Var.Ragebot[0].Spread, "[ Aimbot Light ] Remove Spread");
	assign(g_Var.Ragebot[0].Autozoom, "[ Aimbot Light ] Auto Scope");
	assign(g_Var.Ragebot[0].Autostop, "[ Aimbot Light ] Auto Stop");
	assign(g_Var.Ragebot[0].Limbs[0], "[ Aimbot Light ] Ignore Limbs In Move");
	assign(g_Var.Ragebot[0].Limbs[1], "[ Aimbot Light ] Ignore Limbs In Air");
	assign(g_Var.Ragebot[0].IgnoreHead[0], "[ Aimbot Light ] Ignore Head In Air");
	assign(g_Var.Ragebot[0].IgnoreHead[1], "[ Aimbot Light ] Ignore Head No Update");
	assign(g_Var.Ragebot[0].IgnoreHead[2], "[ Aimbot Light ] Ignore Head High Fake");
	assign(g_Var.Ragebot[0].PriorityRecord[0], "[ Aimbot Light ] History On Shot");
	assign(g_Var.Ragebot[0].PriorityRecord[1], "[ Aimbot Light ] History No Weapon");
	assign(g_Var.Ragebot[0].PriorityRecord[2], "[ Aimbot Light ] History In Air");
	assign(g_Var.Ragebot[0].PriorityRecord[3], "[ Aimbot Light ] History Running");
	assign(g_Var.Ragebot[0].BodyAimHp, "[ Aimbot Light ] Prefer Body if Lower Than");
	assign(g_Var.Ragebot[0].ForceBaim, "[ Aimbot Light ] Force BodyAim");

	assign(g_Var.Ragebot[1].Enable, "[ Aimbot Heavy ] Enable");
	assign(g_Var.Ragebot[1].Autoshoot, "[ Aimbot Heavy ] Auto Fire");
	assign(g_Var.Ragebot[1].Multipoint, "[ Aimbot Heavy ] Multipoints");
	assign(g_Var.Ragebot[1].Multipointbox[0], "[ Aimbot Heavy ] Multipoints Head");
	assign(g_Var.Ragebot[1].Multipointbox[1], "[ Aimbot Heavy ] Multipoints Body");
	assign(g_Var.Ragebot[1].Multipointbox[2], "[ Aimbot Heavy ] Multipoints Arms");
	assign(g_Var.Ragebot[1].Multipointbox[3], "[ Aimbot Heavy ] Multipoints Legs");
	assign(g_Var.Ragebot[1].Multipointbox[4], "[ Aimbot Heavy ] Multipoints Feet");
	assign(g_Var.Ragebot[1].HeadPointScale, "[ Aimbot Heavy ] Multipoints Scale Head");
	assign(g_Var.Ragebot[1].PointScale, "[ Aimbot Heavy ] Multipoints Scale");
	assign(g_Var.Ragebot[1].AutoPointScale, "[ Aimbot Heavy ] Multipoints Scale Auto");
	assign(g_Var.Ragebot[1].BodyAfter, "[ Aimbot Heavy ] Target Body After");
	assign(g_Var.Ragebot[1].Team, "[ Aimbot Heavy ] Team");
	assign(g_Var.Ragebot[1].Spot, "[ Aimbot Heavy ] Prior Spot");
	assign(g_Var.Ragebot[1].Selection, "[ Aimbot Heavy ] Target Selection");
	assign(g_Var.Ragebot[1].Multibox, "[ Aimbot Heavy ] Multibox");
	assign(g_Var.Ragebot[1].Multiboxes[0], "[ Aimbot Heavy ] Multibox Head");
	assign(g_Var.Ragebot[1].Multiboxes[1], "[ Aimbot Heavy ] Multibox Body");
	assign(g_Var.Ragebot[1].Multiboxes[2], "[ Aimbot Heavy ] Multibox Arms");
	assign(g_Var.Ragebot[1].Multiboxes[3], "[ Aimbot Heavy ] Multibox Legs");
	assign(g_Var.Ragebot[1].Multiboxes[4], "[ Aimbot Heavy ] Multibox Feet");
	assign(g_Var.Ragebot[1].Fov, "[ Aimbot Heavy ] Field of View");
	assign(g_Var.Ragebot[1].Hitchance, "[ Aimbot Heavy ] Hitchance");
	assign(g_Var.Ragebot[1].Mindamage, "[ Aimbot Heavy ] Minimum Damage");
	assign(g_Var.Ragebot[1].Walldamage, "[ Aimbot Heavy ] Wall Minimum Damage");
	assign(g_Var.Ragebot[1].AutoMindDamage, "[ Aimbot Heavy ] Minimum Damage Auto");
	assign(g_Var.Ragebot[1].Silent, "[ Aimbot Heavy ] Silent");
	assign(g_Var.Ragebot[1].Autowall, "[ Aimbot Heavy ] Autowall");
	assign(g_Var.Ragebot[1].PosAdjust, "[ Aimbot Heavy ] Position Adjustment");
	assign(g_Var.Ragebot[1].Recoil, "[ Aimbot Heavy ] Remove Recoil");
	assign(g_Var.Ragebot[1].Spread, "[ Aimbot Heavy ] Remove Spread");
	assign(g_Var.Ragebot[1].Autozoom, "[ Aimbot Heavy ] Auto Scope");
	assign(g_Var.Ragebot[1].Autostop, "[ Aimbot Heavy ] Auto Stop");
	assign(g_Var.Ragebot[1].Limbs[0], "[ Aimbot Heavy ] Ignore Limbs In Move");
	assign(g_Var.Ragebot[1].Limbs[1], "[ Aimbot Heavy ] Ignore Limbs In Air");
	assign(g_Var.Ragebot[1].IgnoreHead[0], "[ Aimbot Heavy ] Ignore Head In Air");
	assign(g_Var.Ragebot[1].IgnoreHead[1], "[ Aimbot Heavy ] Ignore Head No Update");
	assign(g_Var.Ragebot[1].IgnoreHead[2], "[ Aimbot Heavy ] Ignore Head High Fake");
	assign(g_Var.Ragebot[1].PriorityRecord[0], "[ Aimbot Heavy ] History On Shot");
	assign(g_Var.Ragebot[1].PriorityRecord[1], "[ Aimbot Heavy ] History No Weapon");
	assign(g_Var.Ragebot[1].PriorityRecord[2], "[ Aimbot Heavy ] History In Air");
	assign(g_Var.Ragebot[1].PriorityRecord[3], "[ Aimbot Heavy ] History Running");
	assign(g_Var.Ragebot[1].BodyAimHp, "[ Aimbot Heavy ] Prefer Body if Lower Than");
	assign(g_Var.Ragebot[1].ForceBaim, "[ Aimbot Heavy ] Force BodyAim");

	assign(g_Var.Ragebot[2].Enable, "[ Aimbot Machinegun ] Enable");
	assign(g_Var.Ragebot[2].Autoshoot, "[ Aimbot Machinegun ] Auto Fire");
	assign(g_Var.Ragebot[2].Multipoint, "[ Aimbot Machinegun ] Multipoints");
	assign(g_Var.Ragebot[2].Multipointbox[0], "[ Aimbot Machinegun ] Multipoints Head");
	assign(g_Var.Ragebot[2].Multipointbox[1], "[ Aimbot Machinegun ] Multipoints Body");
	assign(g_Var.Ragebot[2].Multipointbox[2], "[ Aimbot Machinegun ] Multipoints Arms");
	assign(g_Var.Ragebot[2].Multipointbox[3], "[ Aimbot Machinegun ] Multipoints Legs");
	assign(g_Var.Ragebot[2].Multipointbox[4], "[ Aimbot Machinegun ] Multipoints Feet");
	assign(g_Var.Ragebot[2].HeadPointScale, "[ Aimbot Machinegun ] Multipoints Scale Head");
	assign(g_Var.Ragebot[2].PointScale, "[ Aimbot Machinegun ] Multipoints Scale");
	assign(g_Var.Ragebot[2].AutoPointScale, "[ Aimbot Machinegun ] Multipoints Scale Auto");
	assign(g_Var.Ragebot[2].BodyAfter, "[ Aimbot Machinegun ] Target Body After");
	assign(g_Var.Ragebot[2].Team, "[ Aimbot Machinegun ] Team");
	assign(g_Var.Ragebot[2].Spot, "[ Aimbot Machinegun ] Prior Spot");
	assign(g_Var.Ragebot[2].Selection, "[ Aimbot Machinegun ] Target Selection");
	assign(g_Var.Ragebot[2].Multibox, "[ Aimbot Machinegun ] Multibox");
	assign(g_Var.Ragebot[2].Multiboxes[0], "[ Aimbot Machinegun ] Multibox Head");
	assign(g_Var.Ragebot[2].Multiboxes[1], "[ Aimbot Machinegun ] Multibox Body");
	assign(g_Var.Ragebot[2].Multiboxes[2], "[ Aimbot Machinegun ] Multibox Arms");
	assign(g_Var.Ragebot[2].Multiboxes[3], "[ Aimbot Machinegun ] Multibox Legs");
	assign(g_Var.Ragebot[2].Multiboxes[4], "[ Aimbot Machinegun ] Multibox Feet");
	assign(g_Var.Ragebot[2].Fov, "[ Aimbot Machinegun ] Field of View");
	assign(g_Var.Ragebot[2].Hitchance, "[ Aimbot Machinegun ] Hitchance");
	assign(g_Var.Ragebot[2].Mindamage, "[ Aimbot Machinegun ] Minimum Damage");
	assign(g_Var.Ragebot[2].Walldamage, "[ Aimbot Machinegun ] Wall Minimum Damage");
	assign(g_Var.Ragebot[2].AutoMindDamage, "[ Aimbot Machinegun ] Minimum Damage Auto");
	assign(g_Var.Ragebot[2].Silent, "[ Aimbot Machinegun ] Silent");
	assign(g_Var.Ragebot[2].Autowall, "[ Aimbot Machinegun ] Autowall");
	assign(g_Var.Ragebot[2].PosAdjust, "[ Aimbot Machinegun ] Position Adjustment");
	assign(g_Var.Ragebot[2].Recoil, "[ Aimbot Machinegun ] Remove Recoil");
	assign(g_Var.Ragebot[2].Spread, "[ Aimbot Machinegun ] Remove Spread");
	assign(g_Var.Ragebot[2].Autozoom, "[ Aimbot Machinegun ] Auto Scope");
	assign(g_Var.Ragebot[2].Autostop, "[ Aimbot Machinegun ] Auto Stop");
	assign(g_Var.Ragebot[2].Limbs[0], "[ Aimbot Machinegun ] Ignore Limbs In Move");
	assign(g_Var.Ragebot[2].Limbs[1], "[ Aimbot Machinegun ] Ignore Limbs In Air");
	assign(g_Var.Ragebot[2].IgnoreHead[0], "[ Aimbot Machinegun ] Ignore Head In Air");
	assign(g_Var.Ragebot[2].IgnoreHead[1], "[ Aimbot Machinegun ] Ignore Head No Update");
	assign(g_Var.Ragebot[2].IgnoreHead[2], "[ Aimbot Machinegun ] Ignore Head High Fake");
	assign(g_Var.Ragebot[2].PriorityRecord[0], "[ Aimbot Machinegun ] History On Shot");
	assign(g_Var.Ragebot[2].PriorityRecord[1], "[ Aimbot Machinegun ] History No Weapon");
	assign(g_Var.Ragebot[2].PriorityRecord[2], "[ Aimbot Machinegun ] History In Air");
	assign(g_Var.Ragebot[2].PriorityRecord[3], "[ Aimbot Machinegun ] History Running");
	assign(g_Var.Ragebot[2].BodyAimHp, "[ Aimbot Machinegun ] Prefer Body if Lower Than");
	assign(g_Var.Ragebot[2].ForceBaim, "[ Aimbot Machinegun ] Force BodyAim");

	assign(g_Var.Ragebot[3].Enable, "[ Aimbot Rifle ] Enable");
	assign(g_Var.Ragebot[3].Autoshoot, "[ Aimbot Rifle ] Auto Fire");
	assign(g_Var.Ragebot[3].Multipoint, "[ Aimbot Rifle ] Multipoints");
	assign(g_Var.Ragebot[3].Multipointbox[0], "[ Aimbot Rifle ] Multipoints Head");
	assign(g_Var.Ragebot[3].Multipointbox[1], "[ Aimbot Rifle ] Multipoints Body");
	assign(g_Var.Ragebot[3].Multipointbox[2], "[ Aimbot Rifle ] Multipoints Arms");
	assign(g_Var.Ragebot[3].Multipointbox[3], "[ Aimbot Rifle ] Multipoints Legs");
	assign(g_Var.Ragebot[3].Multipointbox[4], "[ Aimbot Rifle ] Multipoints Feet");
	assign(g_Var.Ragebot[3].HeadPointScale, "[ Aimbot Rifle ] Multipoints Scale Head");
	assign(g_Var.Ragebot[3].PointScale, "[ Aimbot Rifle ] Multipoints Scale");
	assign(g_Var.Ragebot[3].AutoPointScale, "[ Aimbot Rifle ] Multipoints Scale Auto");
	assign(g_Var.Ragebot[3].BodyAfter, "[ Aimbot Rifle ] Target Body After");
	assign(g_Var.Ragebot[3].Team, "[ Aimbot Rifle ] Team");
	assign(g_Var.Ragebot[3].Spot, "[ Aimbot Rifle ] Prior Spot");
	assign(g_Var.Ragebot[3].Selection, "[ Aimbot Rifle ] Target Selection");
	assign(g_Var.Ragebot[3].Multibox, "[ Aimbot Rifle ] Multibox");
	assign(g_Var.Ragebot[3].Multiboxes[0], "[ Aimbot Rifle ] Multibox Head");
	assign(g_Var.Ragebot[3].Multiboxes[1], "[ Aimbot Rifle ] Multibox Body");
	assign(g_Var.Ragebot[3].Multiboxes[2], "[ Aimbot Rifle ] Multibox Arms");
	assign(g_Var.Ragebot[3].Multiboxes[3], "[ Aimbot Rifle ] Multibox Legs");
	assign(g_Var.Ragebot[3].Multiboxes[4], "[ Aimbot Rifle ] Multibox Feet");
	assign(g_Var.Ragebot[3].Fov, "[ Aimbot Rifle ] Field of View");
	assign(g_Var.Ragebot[3].Hitchance, "[ Aimbot Rifle ] Hitchance");
	assign(g_Var.Ragebot[3].Mindamage, "[ Aimbot Rifle ] Minimum Damage");
	assign(g_Var.Ragebot[3].Walldamage, "[ Aimbot Rifle ] Wall Minimum Damage");
	assign(g_Var.Ragebot[3].AutoMindDamage, "[ Aimbot Rifle ] Minimum Damage Auto");
	assign(g_Var.Ragebot[3].Silent, "[ Aimbot Rifle ] Silent");
	assign(g_Var.Ragebot[3].Autowall, "[ Aimbot Rifle ] Autowall");
	assign(g_Var.Ragebot[3].PosAdjust, "[ Aimbot Rifle ] Position Adjustment");
	assign(g_Var.Ragebot[3].Recoil, "[ Aimbot Rifle ] Remove Recoil");
	assign(g_Var.Ragebot[3].Spread, "[ Aimbot Rifle ] Remove Spread");
	assign(g_Var.Ragebot[3].Autozoom, "[ Aimbot Rifle ] Auto Scope");
	assign(g_Var.Ragebot[3].Autostop, "[ Aimbot Rifle ] Auto Stop");
	assign(g_Var.Ragebot[3].Limbs[0], "[ Aimbot Rifle ] Ignore Limbs In Move");
	assign(g_Var.Ragebot[3].Limbs[1], "[ Aimbot Rifle ] Ignore Limbs In Air");
	assign(g_Var.Ragebot[3].IgnoreHead[0], "[ Aimbot Rifle ] Ignore Head In Air");
	assign(g_Var.Ragebot[3].IgnoreHead[1], "[ Aimbot Rifle ] Ignore Head No Update");
	assign(g_Var.Ragebot[3].IgnoreHead[2], "[ Aimbot Rifle ] Ignore Head High Fake");
	assign(g_Var.Ragebot[3].PriorityRecord[0], "[ Aimbot Rifle ] History On Shot");
	assign(g_Var.Ragebot[3].PriorityRecord[1], "[ Aimbot Rifle ] History No Weapon");
	assign(g_Var.Ragebot[3].PriorityRecord[2], "[ Aimbot Rifle ] History In Air");
	assign(g_Var.Ragebot[3].PriorityRecord[3], "[ Aimbot Rifle ] History Running");
	assign(g_Var.Ragebot[3].BodyAimHp, "[ Aimbot Rifle ] Prefer Body if Lower Than");
	assign(g_Var.Ragebot[3].ForceBaim, "[ Aimbot Rifle ] Force BodyAim");

	assign(g_Var.Ragebot[4].Enable, "[ Aimbot Shotgun ] Enable");
	assign(g_Var.Ragebot[4].Autoshoot, "[ Aimbot Shotgun ] Auto Fire");
	assign(g_Var.Ragebot[4].Multipoint, "[ Aimbot Shotgun ] Multipoints");
	assign(g_Var.Ragebot[4].Multipointbox[0], "[ Aimbot Shotgun ] Multipoints Head");
	assign(g_Var.Ragebot[4].Multipointbox[1], "[ Aimbot Shotgun ] Multipoints Body");
	assign(g_Var.Ragebot[4].Multipointbox[2], "[ Aimbot Shotgun ] Multipoints Arms");
	assign(g_Var.Ragebot[4].Multipointbox[3], "[ Aimbot Shotgun ] Multipoints Legs");
	assign(g_Var.Ragebot[4].Multipointbox[4], "[ Aimbot Shotgun ] Multipoints Feet");
	assign(g_Var.Ragebot[4].HeadPointScale, "[ Aimbot Shotgun ] Multipoints Scale Head");
	assign(g_Var.Ragebot[4].PointScale, "[ Aimbot Shotgun ] Multipoints Scale");
	assign(g_Var.Ragebot[4].AutoPointScale, "[ Aimbot Shotgun ] Multipoints Scale Auto");
	assign(g_Var.Ragebot[4].BodyAfter, "[ Aimbot Shotgun ] Target Body After");
	assign(g_Var.Ragebot[4].Team, "[ Aimbot Shotgun ] Team");
	assign(g_Var.Ragebot[4].Spot, "[ Aimbot Shotgun ] Prior Spot");
	assign(g_Var.Ragebot[4].Selection, "[ Aimbot Shotgun ] Target Selection");
	assign(g_Var.Ragebot[4].Multibox, "[ Aimbot Shotgun ] Multibox");
	assign(g_Var.Ragebot[4].Multiboxes[0], "[ Aimbot Shotgun ] Multibox Head");
	assign(g_Var.Ragebot[4].Multiboxes[1], "[ Aimbot Shotgun ] Multibox Body");
	assign(g_Var.Ragebot[4].Multiboxes[2], "[ Aimbot Shotgun ] Multibox Arms");
	assign(g_Var.Ragebot[4].Multiboxes[3], "[ Aimbot Shotgun ] Multibox Legs");
	assign(g_Var.Ragebot[4].Multiboxes[4], "[ Aimbot Shotgun ] Multibox Feet");
	assign(g_Var.Ragebot[4].Fov, "[ Aimbot Shotgun ] Field of View");
	assign(g_Var.Ragebot[4].Hitchance, "[ Aimbot Shotgun ] Hitchance");
	assign(g_Var.Ragebot[4].Mindamage, "[ Aimbot Shotgun ] Minimum Damage");
	assign(g_Var.Ragebot[4].Walldamage, "[ Aimbot Shotgun ] Wall Minimum Damage");
	assign(g_Var.Ragebot[4].AutoMindDamage, "[ Aimbot Shotgun ] Minimum Damage Auto");
	assign(g_Var.Ragebot[4].Silent, "[ Aimbot Shotgun ] Silent");
	assign(g_Var.Ragebot[4].Autowall, "[ Aimbot Shotgun ] Autowall");
	assign(g_Var.Ragebot[4].PosAdjust, "[ Aimbot Shotgun ] Position Adjustment");
	assign(g_Var.Ragebot[4].Recoil, "[ Aimbot Shotgun ] Remove Recoil");
	assign(g_Var.Ragebot[4].Spread, "[ Aimbot Shotgun ] Remove Spread");
	assign(g_Var.Ragebot[4].Autozoom, "[ Aimbot Shotgun ] Auto Scope");
	assign(g_Var.Ragebot[4].Autostop, "[ Aimbot Shotgun ] Auto Stop");
	assign(g_Var.Ragebot[4].Limbs[0], "[ Aimbot Shotgun ] Ignore Limbs In Move");
	assign(g_Var.Ragebot[4].Limbs[1], "[ Aimbot Shotgun ] Ignore Limbs In Air");
	assign(g_Var.Ragebot[4].IgnoreHead[0], "[ Aimbot Shotgun ] Ignore Head In Air");
	assign(g_Var.Ragebot[4].IgnoreHead[1], "[ Aimbot Shotgun ] Ignore Head No Update");
	assign(g_Var.Ragebot[4].IgnoreHead[2], "[ Aimbot Shotgun ] Ignore Head High Fake");
	assign(g_Var.Ragebot[4].PriorityRecord[0], "[ Aimbot Shotgun ] History On Shot");
	assign(g_Var.Ragebot[4].PriorityRecord[1], "[ Aimbot Shotgun ] History No Weapon");
	assign(g_Var.Ragebot[4].PriorityRecord[2], "[ Aimbot Shotgun ] History In Air");
	assign(g_Var.Ragebot[4].PriorityRecord[3], "[ Aimbot Shotgun ] History Running");
	assign(g_Var.Ragebot[4].BodyAimHp, "[ Aimbot Shotgun ] Prefer Body if Lower Than");
	assign(g_Var.Ragebot[4].ForceBaim, "[ Aimbot Shotgun ] Force BodyAim");

	assign(g_Var.Ragebot[5].Enable, "[ Aimbot Light Sniper ] Enable");
	assign(g_Var.Ragebot[5].Autoshoot, "[ Aimbot Light Sniper ] Auto Fire");
	assign(g_Var.Ragebot[5].Multipoint, "[ Aimbot Light Sniper ] Multipoints");
	assign(g_Var.Ragebot[5].Multipointbox[0], "[ Aimbot Light Sniper ] Multipoints Head");
	assign(g_Var.Ragebot[5].Multipointbox[1], "[ Aimbot Light Sniper ] Multipoints Body");
	assign(g_Var.Ragebot[5].Multipointbox[2], "[ Aimbot Light Sniper ] Multipoints Arms");
	assign(g_Var.Ragebot[5].Multipointbox[3], "[ Aimbot Light Sniper ] Multipoints Legs");
	assign(g_Var.Ragebot[5].Multipointbox[4], "[ Aimbot Light Sniper ] Multipoints Feet");
	assign(g_Var.Ragebot[5].HeadPointScale, "[ Aimbot Light Sniper ] Multipoints Scale Head");
	assign(g_Var.Ragebot[5].PointScale, "[ Aimbot Light Sniper ] Multipoints Scale");
	assign(g_Var.Ragebot[5].AutoPointScale, "[ Aimbot Light Sniper ] Multipoints Scale Auto");
	assign(g_Var.Ragebot[5].BodyAfter, "[ Aimbot Light Sniper ] Target Body After");
	assign(g_Var.Ragebot[5].Team, "[ Aimbot Light Sniper ] Team");
	assign(g_Var.Ragebot[5].Spot, "[ Aimbot Light Sniper ] Prior Spot");
	assign(g_Var.Ragebot[5].Selection, "[ Aimbot Light Sniper ] Target Selection");
	assign(g_Var.Ragebot[5].Multibox, "[ Aimbot Light Sniper ] Multibox");
	assign(g_Var.Ragebot[5].Multiboxes[0], "[ Aimbot Light Sniper ] Multibox Head");
	assign(g_Var.Ragebot[5].Multiboxes[1], "[ Aimbot Light Sniper ] Multibox Body");
	assign(g_Var.Ragebot[5].Multiboxes[2], "[ Aimbot Light Sniper ] Multibox Arms");
	assign(g_Var.Ragebot[5].Multiboxes[3], "[ Aimbot Light Sniper ] Multibox Legs");
	assign(g_Var.Ragebot[5].Multiboxes[4], "[ Aimbot Light Sniper ] Multibox Feet");
	assign(g_Var.Ragebot[5].Fov, "[ Aimbot Light Sniper ] Field of View");
	assign(g_Var.Ragebot[5].Hitchance, "[ Aimbot Light Sniper ] Hitchance");
	assign(g_Var.Ragebot[5].Mindamage, "[ Aimbot Light Sniper ] Minimum Damage");
	assign(g_Var.Ragebot[5].Walldamage, "[ Aimbot Light Sniper ] Wall Minimum Damage");
	assign(g_Var.Ragebot[5].AutoMindDamage, "[ Aimbot Light Sniper ] Minimum Damage Auto");
	assign(g_Var.Ragebot[5].Silent, "[ Aimbot Light Sniper ] Silent");
	assign(g_Var.Ragebot[5].Autowall, "[ Aimbot Light Sniper ] Autowall");
	assign(g_Var.Ragebot[5].PosAdjust, "[ Aimbot Light Sniper ] Position Adjustment");
	assign(g_Var.Ragebot[5].Recoil, "[ Aimbot Light Sniper ] Remove Recoil");
	assign(g_Var.Ragebot[5].Spread, "[ Aimbot Light Sniper ] Remove Spread");
	assign(g_Var.Ragebot[5].Autozoom, "[ Aimbot Light Sniper ] Auto Scope");
	assign(g_Var.Ragebot[5].Autostop, "[ Aimbot Light Sniper ] Auto Stop");
	assign(g_Var.Ragebot[5].Limbs[0], "[ Aimbot Light Sniper ] Ignore Limbs In Move");
	assign(g_Var.Ragebot[5].Limbs[1], "[ Aimbot Light Sniper ] Ignore Limbs In Air");
	assign(g_Var.Ragebot[5].IgnoreHead[0], "[ Aimbot Light Sniper ] Ignore Head In Air");
	assign(g_Var.Ragebot[5].IgnoreHead[1], "[ Aimbot Light Sniper ] Ignore Head No Update");
	assign(g_Var.Ragebot[5].IgnoreHead[2], "[ Aimbot Light Sniper ] Ignore Head High Fake");
	assign(g_Var.Ragebot[5].PriorityRecord[0], "[ Aimbot Light Sniper ] History On Shot");
	assign(g_Var.Ragebot[5].PriorityRecord[1], "[ Aimbot Light Sniper ] History No Weapon");
	assign(g_Var.Ragebot[5].PriorityRecord[2], "[ Aimbot Light Sniper ] History In Air");
	assign(g_Var.Ragebot[5].PriorityRecord[3], "[ Aimbot Light Sniper ] History Running");
	assign(g_Var.Ragebot[5].BodyAimHp, "[ Aimbot Light Sniper ] Prefer Body if Lower Than");
	assign(g_Var.Ragebot[5].ForceBaim, "[ Aimbot Light Sniper ] Force BodyAim");

	assign(g_Var.Ragebot[6].Enable, "[ Aimbot Auto Sniper ] Enable");
	assign(g_Var.Ragebot[6].Autoshoot, "[ Aimbot Auto Sniper ] Auto Fire");
	assign(g_Var.Ragebot[6].Multipoint, "[ Aimbot Auto Sniper ] Multipoints");
	assign(g_Var.Ragebot[6].Multipointbox[0], "[ Aimbot Auto Sniper ] Multipoints Head");
	assign(g_Var.Ragebot[6].Multipointbox[1], "[ Aimbot Auto Sniper ] Multipoints Body");
	assign(g_Var.Ragebot[6].Multipointbox[2], "[ Aimbot Auto Sniper ] Multipoints Arms");
	assign(g_Var.Ragebot[6].Multipointbox[3], "[ Aimbot Auto Sniper ] Multipoints Legs");
	assign(g_Var.Ragebot[6].Multipointbox[4], "[ Aimbot Auto Sniper ] Multipoints Feet");
	assign(g_Var.Ragebot[6].HeadPointScale, "[ Aimbot Auto Sniper ] Multipoints Scale Head");
	assign(g_Var.Ragebot[6].PointScale, "[ Aimbot Auto Sniper ] Multipoints Scale");
	assign(g_Var.Ragebot[6].AutoPointScale, "[ Aimbot Auto Sniper ] Multipoints Scale Auto");
	assign(g_Var.Ragebot[6].BodyAfter, "[ Aimbot Auto Sniper ] Target Body After");
	assign(g_Var.Ragebot[6].Team, "[ Aimbot Auto Sniper ] Team");
	assign(g_Var.Ragebot[6].Spot, "[ Aimbot Auto Sniper ] Prior Spot");
	assign(g_Var.Ragebot[6].Selection, "[ Aimbot Auto Sniper ] Target Selection");
	assign(g_Var.Ragebot[6].Multibox, "[ Aimbot Auto Sniper ] Multibox");
	assign(g_Var.Ragebot[6].Multiboxes[0], "[ Aimbot Auto Sniper ] Multibox Head");
	assign(g_Var.Ragebot[6].Multiboxes[1], "[ Aimbot Auto Sniper ] Multibox Body");
	assign(g_Var.Ragebot[6].Multiboxes[2], "[ Aimbot Auto Sniper ] Multibox Arms");
	assign(g_Var.Ragebot[6].Multiboxes[3], "[ Aimbot Auto Sniper ] Multibox Legs");
	assign(g_Var.Ragebot[6].Multiboxes[4], "[ Aimbot Auto Sniper ] Multibox Feet");
	assign(g_Var.Ragebot[6].Fov, "[ Aimbot Auto Sniper ] Field of View");
	assign(g_Var.Ragebot[6].Hitchance, "[ Aimbot Auto Sniper ] Hitchance");
	assign(g_Var.Ragebot[6].Mindamage, "[ Aimbot Auto Sniper ] Minimum Damage");
	assign(g_Var.Ragebot[6].Walldamage, "[ Aimbot Auto Sniper ] Wall Minimum Damage");
	assign(g_Var.Ragebot[6].AutoMindDamage, "[ Aimbot Auto Sniper ] Minimum Damage Auto");
	assign(g_Var.Ragebot[6].Silent, "[ Aimbot Auto Sniper ] Silent");
	assign(g_Var.Ragebot[6].Autowall, "[ Aimbot Auto Sniper ] Autowall");
	assign(g_Var.Ragebot[6].PosAdjust, "[ Aimbot Auto Sniper ] Position Adjustment");
	assign(g_Var.Ragebot[6].Recoil, "[ Aimbot Auto Sniper ] Remove Recoil");
	assign(g_Var.Ragebot[6].Spread, "[ Aimbot Auto Sniper ] Remove Spread");
	assign(g_Var.Ragebot[6].Autozoom, "[ Aimbot Auto Sniper ] Auto Scope");
	assign(g_Var.Ragebot[6].Autostop, "[ Aimbot Auto Sniper ] Auto Stop");
	assign(g_Var.Ragebot[6].Limbs[0], "[ Aimbot Auto Sniper ] Ignore Limbs In Move");
	assign(g_Var.Ragebot[6].Limbs[1], "[ Aimbot Auto Sniper ] Ignore Limbs In Air");
	assign(g_Var.Ragebot[6].IgnoreHead[0], "[ Aimbot Auto Sniper ] Ignore Head In Air");
	assign(g_Var.Ragebot[6].IgnoreHead[1], "[ Aimbot Auto Sniper ] Ignore Head No Update");
	assign(g_Var.Ragebot[6].IgnoreHead[2], "[ Aimbot Auto Sniper ] Ignore Head High Fake");
	assign(g_Var.Ragebot[6].PriorityRecord[0], "[ Aimbot Auto Sniper ] History On Shot");
	assign(g_Var.Ragebot[6].PriorityRecord[1], "[ Aimbot Auto Sniper ] History No Weapon");
	assign(g_Var.Ragebot[6].PriorityRecord[2], "[ Aimbot Auto Sniper ] History In Air");
	assign(g_Var.Ragebot[6].PriorityRecord[3], "[ Aimbot Auto Sniper ] History Running");
	assign(g_Var.Ragebot[6].BodyAimHp, "[ Aimbot Auto Sniper ] Prefer Body if Lower Than");
	assign(g_Var.Ragebot[6].ForceBaim, "[ Aimbot Auto Sniper ] Force BodyAim");

	assign(g_Var.Ragebot[7].Enable, "[ Aimbot Heavy Sniper ] Enable");
	assign(g_Var.Ragebot[7].Autoshoot, "[ Aimbot Heavy Sniper ] Auto Fire");
	assign(g_Var.Ragebot[7].Multipoint, "[ Aimbot Heavy Sniper ] Multipoints");
	assign(g_Var.Ragebot[7].Multipointbox[0], "[ Aimbot Heavy Sniper ] Multipoints Head");
	assign(g_Var.Ragebot[7].Multipointbox[1], "[ Aimbot Heavy Sniper ] Multipoints Body");
	assign(g_Var.Ragebot[7].Multipointbox[2], "[ Aimbot Heavy Sniper ] Multipoints Arms");
	assign(g_Var.Ragebot[7].Multipointbox[3], "[ Aimbot Heavy Sniper ] Multipoints Legs");
	assign(g_Var.Ragebot[7].Multipointbox[4], "[ Aimbot Heavy Sniper ] Multipoints Feet");
	assign(g_Var.Ragebot[7].HeadPointScale, "[ Aimbot Heavy Sniper ] Multipoints Scale Head");
	assign(g_Var.Ragebot[7].PointScale, "[ Aimbot Heavy Sniper ] Multipoints Scale");
	assign(g_Var.Ragebot[7].AutoPointScale, "[ Aimbot Heavy Sniper ] Multipoints Scale Auto");
	assign(g_Var.Ragebot[7].BodyAfter, "[ Aimbot Heavy Sniper ] Target Body After");
	assign(g_Var.Ragebot[7].Team, "[ Aimbot Heavy Sniper ] Team");
	assign(g_Var.Ragebot[7].Spot, "[ Aimbot Heavy Sniper ] Prior Spot");
	assign(g_Var.Ragebot[7].Selection, "[ Aimbot Heavy Sniper ] Target Selection");
	assign(g_Var.Ragebot[7].Multibox, "[ Aimbot Heavy Sniper ] Multibox");
	assign(g_Var.Ragebot[7].Multiboxes[0], "[ Aimbot Heavy Sniper ] Multibox Head");
	assign(g_Var.Ragebot[7].Multiboxes[1], "[ Aimbot Heavy Sniper ] Multibox Body");
	assign(g_Var.Ragebot[7].Multiboxes[2], "[ Aimbot Heavy Sniper ] Multibox Arms");
	assign(g_Var.Ragebot[7].Multiboxes[3], "[ Aimbot Heavy Sniper ] Multibox Legs");
	assign(g_Var.Ragebot[7].Multiboxes[4], "[ Aimbot Heavy Sniper ] Multibox Feet");
	assign(g_Var.Ragebot[7].Fov, "[ Aimbot Heavy Sniper ] Field of View");
	assign(g_Var.Ragebot[7].Hitchance, "[ Aimbot Heavy Sniper ] Hitchance");
	assign(g_Var.Ragebot[7].Mindamage, "[ Aimbot Heavy Sniper ] Minimum Damage");
	assign(g_Var.Ragebot[7].Walldamage, "[ Aimbot Heavy Sniper ] Wall Minimum Damage");
	assign(g_Var.Ragebot[7].AutoMindDamage, "[ Aimbot Heavy Sniper ] Minimum Damage Auto");
	assign(g_Var.Ragebot[7].Silent, "[ Aimbot Heavy Sniper ] Silent");
	assign(g_Var.Ragebot[7].Autowall, "[ Aimbot Heavy Sniper ] Autowall");
	assign(g_Var.Ragebot[7].PosAdjust, "[ Aimbot Heavy Sniper ] Position Adjustment");
	assign(g_Var.Ragebot[7].Recoil, "[ Aimbot Heavy Sniper ] Remove Recoil");
	assign(g_Var.Ragebot[7].Spread, "[ Aimbot Heavy Sniper ] Remove Spread");
	assign(g_Var.Ragebot[7].Autozoom, "[ Aimbot Heavy Sniper ] Auto Scope");
	assign(g_Var.Ragebot[7].Autostop, "[ Aimbot Heavy Sniper ] Auto Stop");
	assign(g_Var.Ragebot[7].Limbs[0], "[ Aimbot Heavy Sniper ] Ignore Limbs In Move");
	assign(g_Var.Ragebot[7].Limbs[1], "[ Aimbot Heavy Sniper ] Ignore Limbs In Air");
	assign(g_Var.Ragebot[7].IgnoreHead[0], "[ Aimbot Heavy Sniper ] Ignore Head In Air");
	assign(g_Var.Ragebot[7].IgnoreHead[1], "[ Aimbot Heavy Sniper ] Ignore Head No Update");
	assign(g_Var.Ragebot[7].IgnoreHead[2], "[ Aimbot Heavy Sniper ] Ignore Head High Fake");
	assign(g_Var.Ragebot[7].PriorityRecord[0], "[ Aimbot Heavy Sniper ] History On Shot");
	assign(g_Var.Ragebot[7].PriorityRecord[1], "[ Aimbot Heavy Sniper ] History No Weapon");
	assign(g_Var.Ragebot[7].PriorityRecord[2], "[ Aimbot Heavy Sniper ] History In Air");
	assign(g_Var.Ragebot[7].PriorityRecord[3], "[ Aimbot Heavy Sniper ] History Running");
	assign(g_Var.Ragebot[7].BodyAimHp, "[ Aimbot Heavy Sniper ] Prefer Body if Lower Than");
	assign(g_Var.Ragebot[7].ForceBaim, "[ Aimbot Heavy Sniper ] Force BodyAim");

	assign(g_Var.AntiAim.Enable, "[ AntiAim ] Enable");
	assign(g_Var.AntiAim.Pitch, "[ AntiAim ] Pitch");

	assign(g_Var.AntiAim.Move, "[ AntiAim ] Move Yaw");
	assign(g_Var.AntiAim.MoveEnemyBased, "[ AntiAim ] Move Enemy Based");
	assign(g_Var.AntiAim.MoveStyle, "[ AntiAim ] Move Yaw Style");
	assign(g_Var.AntiAim.MoveValue, "[ AntiAim ] Move Yaw Value");
	assign(g_Var.AntiAim.MoveSpeed, "[ AntiAim ] Move Yaw Speed");

	assign(g_Var.AntiAim.Air, "[ AntiAim ] Air Yaw");
	assign(g_Var.AntiAim.AirEnemyBased, "[ AntiAim ] Air Enemy Based");
	assign(g_Var.AntiAim.AirStyle, "[ AntiAim ] Air Yaw Style");
	assign(g_Var.AntiAim.AirValue, "[ AntiAim ] Air Yaw Value");
	assign(g_Var.AntiAim.AirSpeed, "[ AntiAim ] Air Yaw Speed");

	assign(g_Var.AntiAim.Stand, "[ AntiAim ] Stand Yaw");
	assign(g_Var.AntiAim.StandEnemyBased, "[ AntiAim ] Stand Enemy Based");
	assign(g_Var.AntiAim.StandStyle, "[ AntiAim ] Stand Yaw Style");
	assign(g_Var.AntiAim.StandValue, "[ AntiAim ] Stand Yaw Value");
	assign(g_Var.AntiAim.StandSpeed, "[ AntiAim ] Stand Yaw Speed");

	assign(g_Var.AntiAim.Break, "[ AntiAim ] LBY Breaker");
	assign(g_Var.AntiAim.BreakToggle, "[ AntiAim ] LBY Breaker Key");
	assign(g_Var.AntiAim.BreakRad, "[ AntiAim ] LBY Breaker Distance");

	assign(g_Var.AntiAim.LeftToggle, "[ AntiAim ] Direction Left");
	assign(g_Var.AntiAim.RightToggle, "[ AntiAim ] Direction Right");

	assign(g_Var.AntiAim.SideWaysTolerance, "[ AntiAim ] Direction Tolerance");

	assign(g_Var.AntiAim.BackToggle, "[ AntiAim ] Direction Back");
	assign(g_Var.AntiAim.FrontToggle, "[ AntiAim ] Direction Forward");

	assign(g_Var.AntiAim.LockToggle, "[ AntiAim ] Direction Lock");

	assign(g_Var.AntiAim.Points, "[ AntiAim ] Dynamic Points");

	assign(g_Var.AntiAim.Tolerance, "[ AntiAim ] Yaw Tolerance");

	assign(g_Var.AntiAim.Override, "[ AntiAim ] Override");

	assign(g_Var.AntiAim.Resolve, "[ AntiAim ] Resolver");

	assign(g_Var.AntiAim.Fakewalk, "[ AntiAim ] Fakewalk");

	assign(g_Var.Fakelag.Enable, "[ Fakelag ] Enable");
	assign(g_Var.Fakelag.Factor, "[ Fakelag ] Limit");
	assign(g_Var.Fakelag.Style, "[ Fakelag ] Style");
	assign(g_Var.Fakelag.Flags[0], "[ Fakelag ] In Stand");
	assign(g_Var.Fakelag.Flags[1], "[ Fakelag ] In Air");
	assign(g_Var.Fakelag.Flags[2], "[ Fakelag ] In Move");
	assign(g_Var.Fakelag.Flags[3], "[ Fakelag ] On Land");
	assign(g_Var.Fakelag.Flags[4], "[ Fakelag ] On Sight");

	/*
	MORE TO COME
	*/

	assign(g_Var.Visuals.Player.Enable, "[ Visuals Player ] Enable");
	assign(g_Var.Visuals.Player.Team, "[ Visuals Player ] Team");
	assign(g_Var.Visuals.Player.Box, "[ Visuals Player ] Box");
	assign(g_Var.Visuals.Player.Name, "[ Visuals Player ] Name");
	assign(g_Var.Visuals.Player.Health, "[ Visuals Player ] Health");
	assign(g_Var.Visuals.Player.Weapon, "[ Visuals Player ] Weapon");
	assign(g_Var.Visuals.Player.KevHelm, "[ Visuals Player ] Armor");
	assign(g_Var.Visuals.Player.AmmoBar, "[ Visuals Player ] Ammo");
	assign(g_Var.Visuals.Player.Surrounding, "[ Visuals Player ] Surrounding");
	assign(g_Var.Visuals.Player.Radar, "[ Visuals Player ] Radar");
	assign(g_Var.Visuals.Player.Spectators, "[ Visuals Player ] Spectators");
	assign(g_Var.Visuals.Player.LowerBody, "[ Visuals Player ] LowerBody Bar");
	assign(g_Var.Visuals.Player.Bone, "[ Visuals Player ] Bone");
	assign(g_Var.Visuals.Glow.Player, "[ Visuals Player ] Glow");

	assign(g_Var.Visuals.Chams.Enable, "[ Visuals Chams ] Enable");
	assign(g_Var.Visuals.Chams.Players, "[ Visuals Chams ] Player");
	assign(g_Var.Visuals.Chams.Team, "[ Visuals Chams ] Player Team");
	assign(g_Var.Visuals.Chams.TeamTransparency, "[ Visuals Chams ] Player Team Transparency");
	assign(g_Var.Visuals.Chams.DrawOriginal, "[ Visuals Chams ] Player Original");
	assign(g_Var.Visuals.Chams.Transparency, "[ Visuals Chams ] Player Transparency");
	assign(g_Var.Visuals.Chams.Weapons, "[ Visuals Chams ] Weapons");
	assign(g_Var.Visuals.Chams.Hands, "[ Visuals Chams ] Hands");
	assign(g_Var.Visuals.Chams.HandsTransparency, "[ Visuals Chams ] Hands Transparency");
	assign(g_Var.Visuals.Chams.View, "[ Visuals Chams ] View Weapons");
	assign(g_Var.Visuals.Chams.BacktrackMaterial, "[ Visuals Chams ] Backtrack Chams Material");
	assign(g_Var.Visuals.Chams.BacktrackAlpha, "[ Visuals Chams ] Backtrack Chams Alpha");

	assign(g_Var.Visuals.Local.Enable, "[ Visuals Local ] Enable");
	assign(g_Var.Visuals.Local.Chams, "[ Visuals Local ] Chams");
	assign(g_Var.Visuals.Local.Glow, "[ Visuals Local ] Glow");
	assign(g_Var.Visuals.Local.Transparency, "[ Visuals Local ] Transparency");
	assign(g_Var.Visuals.Local.FakeChams, "[ Visuals Local ] Fake Chams");

	assign(g_Var.Visuals.Local.Draw[0], "[ Visuals Local ] Flag Name");
	assign(g_Var.Visuals.Local.Draw[1], "[ Visuals Local ] Flag Health");
	assign(g_Var.Visuals.Local.Draw[2], "[ Visuals Local ] Flag Border");
	assign(g_Var.Visuals.Local.Draw[3], "[ Visuals Local ] Flag Weapon");
	assign(g_Var.Visuals.Local.Draw[4], "[ Visuals Local ] Flag Ammo");

	assign(g_Var.Visuals.World.Crosshair[0], "[ Visuals World ] Crosshair Spread");
	assign(g_Var.Visuals.World.Crosshair[1], "[ Visuals World ] Crosshair Recoil");
	assign(g_Var.Visuals.World.Crosshair[2], "[ Visuals World ] Crosshair Autowall");
	assign(g_Var.Visuals.World.Crosshair[3], "[ Visuals World ] Crosshair Sniper");

	assign(g_Var.Visuals.World.Removals[0], "[ Visuals World ] Removals Flash");
	assign(g_Var.Visuals.World.Removals[1], "[ Visuals World ] Removals Smoke");
	assign(g_Var.Visuals.World.Removals[2], "[ Visuals World ] Removals Scope");
	assign(g_Var.Visuals.World.Removals[3], "[ Visuals World ] Removals Recoil");
	assign(g_Var.Visuals.World.Removals[4], "[ Visuals World ] Removals First Zoom");
	assign(g_Var.Visuals.World.Removals[5], "[ Visuals World ] Removals PostProcessing");

	assign(g_Var.Visuals.World.Grenade[0], "[ Visuals World ] Grenade Text");
	assign(g_Var.Visuals.World.Grenade[1], "[ Visuals World ] Grenade Icon");
	assign(g_Var.Visuals.World.Grenade[2], "[ Visuals World ] Grenade Prediction");
	assign(g_Var.Visuals.World.ViewModel, "[ Visuals World ] View Model");
	assign(g_Var.Visuals.World.ViewModelOffset[0], "[ Visuals World ] View Model Offset X");
	assign(g_Var.Visuals.World.ViewModelOffset[1], "[ Visuals World ] View Model Offset Y");
	assign(g_Var.Visuals.World.ViewModelOffset[2], "[ Visuals World ] View Model Offset Z");
	assign(g_Var.Visuals.World.Fieldofview, "[ Visuals World ] Field of View");
	assign(g_Var.Visuals.World.Hitmarker, "[ Visuals World ] Hitmarker");
	assign(g_Var.Visuals.World.BulletTracer[0], "[ Visuals World ] Bullet Tracer");
	assign(g_Var.Visuals.World.BulletTracer[1], "[ Visuals World ] Bullet Impacts");
	assign(g_Var.Visuals.World.BulletTracer[2], "[ Visuals World ] Bullet Hits");
	assign(g_Var.Visuals.World.BulletTracer[3], "[ Visuals World ] Bullet Hitbox");
	assign(g_Var.Visuals.World.PropsAlpha, "[ Visuals World ] Proph Alpha");
	assign(g_Var.Visuals.World.Nightmode, "[ Visuals World ] Wall Dimness");
	assign(g_Var.Visuals.World.Fullbright, "[ Visuals World ] Fullbright");
	assign(g_Var.Visuals.World.Ragdoll, "[ Visuals World ] Ragdoll");
	/*
	MORE TO COME
	*/

	assign(g_Var.Misc.AutoJump, "[ Movement ] Auto Jump");
	assign(g_Var.Misc.AutoStrafe, "[ Movement ] Auto Strafe");
	assign(g_Var.Misc.Slide, "[ Movement ] Slide");
	assign(g_Var.Misc.CrouchJump, "[ Movement ] Crouch Jump");
	assign(g_Var.Misc.FakeDuck, "[ Movement ] Fake Duck");

	assign(g_Var.Misc.Thirdperson, "[ Misc ] Thirdperson");
	assign(g_Var.Misc.ThirdpersonDist, "[ Misc ] Thirdperson Distance");
	assign(g_Var.Misc.ThirdpersonToggleKey, "[ Misc ] Thirdperson Key");

	assign(g_Var.Misc.ZeusBot[0], "[ Misc ] Auto Zeus");
	assign(g_Var.Misc.ZeusBot[1], "[ Misc ] Auto Zeus Range");
	assign(g_Var.Misc.KnifeBot[0], "[ Misc ] Knife Bot");
	assign(g_Var.Misc.KnifeBot[1], "[ Misc ] Knife Bot Range");

	assign(g_Var.Misc.DebugResolver, "[ Misc ] Debug Resolver");
	assign(g_Var.Misc.DebugMultiPoint, "[ Misc ] Debug Multipoints");

	assign(g_Var.Misc.ExtendKillfeed, "[ Misc ] Extended Killfeed");

	assign(g_Var.Misc.Insults, "[ Misc ] Flame Enemy");
	assign(g_Var.Misc.Clantag, "[ Misc ] ClanTag");
	//	assign(g_Var.Misc.AnimFix, "[ Misc ] AnimFix");

	assign(g_Var.Misc.EventLog[0], "[ Misc ] Damage Log");
	assign(g_Var.Misc.EventLog[1], "[ Misc ] Buy Log");
	assign(g_Var.Misc.EventLog[2], "[ Misc ] Bomb Log");

	assign(g_Var.Misc.Indicator[0], "[ Misc ] AA Indicator");
	assign(g_Var.Misc.Indicator[1], "[ Misc ] LBY Indicator");
	assign(g_Var.Misc.Indicator[2], "[ Misc ] Lag Indicator");
	assign(g_Var.Misc.Indicator[3], "[ Misc ] Bomb Indicator");
	assign(g_Var.Misc.Indicator[4], "[ Misc ] Angle Lines");
	assign(g_Var.Misc.Indicator[5], "[ Misc ] LC Indicator");

	assign(g_Var.Misc.BuyBot.Primary, "[ BuyBot ] Primary Weapon");
	assign(g_Var.Misc.BuyBot.Secondary, "[ BuyBot ] Secondary Weapon");
	assign(g_Var.Misc.BuyBot.Gear[0], "[ BuyBot ] Kevlar / Helm");
	assign(g_Var.Misc.BuyBot.Gear[1], "[ BuyBot ] Defuser");
	assign(g_Var.Misc.BuyBot.Gear[2], "[ BuyBot ] Taser");
	assign(g_Var.Misc.BuyBot.Grenades[0], "[ BuyBot ] Grenade");
	assign(g_Var.Misc.BuyBot.Grenades[1], "[ BuyBot ] Smoke");
	assign(g_Var.Misc.BuyBot.Grenades[2], "[ BuyBot ] Flash");
	assign(g_Var.Misc.BuyBot.Grenades[3], "[ BuyBot ] Molotov");
	assign(g_Var.Misc.BuyBot.Grenades[4], "[ BuyBot ] Decoy");

	assign_array(g_Var.Color.Menu, "[ Color ] Menu");

	assign_array(g_Var.Color.Name, "[ Color ] Visuals Name");

	assign_array(g_Var.Color.Player, "[ Color ] Visuals Box");
	assign_array(g_Var.Color.PlayerVisible, "[ Color ] Visuals Box Visible");

	assign_array(g_Var.Color.Team, "[ Color ] Visuals Team Box");
	assign_array(g_Var.Color.TeamVisible, "[ Color ] Visuals Team Box Visible");

	assign_array(g_Var.Color.Weapon, "[ Color ] Visuals Weapon");
	assign_array(g_Var.Color.Health, "[ Color ] Visuals Health");
	assign_array(g_Var.Color.Kevlar, "[ Color ] Visuals Armor");
	assign_array(g_Var.Color.AmmoBar, "[ Color ] Visuals Ammo");
	assign_array(g_Var.Color.Surrounding, "[ Color ] Visuals Surrounding");
	assign_array(g_Var.Color.LowerBodyBar, "[ Color ] Visuals LowerBody");

	assign_array(g_Var.Color.Chams, "[ Color ] Chams");
	assign_array(g_Var.Color.BacktrackChams, "[ Color ] BacktrackChams");
	assign_array(g_Var.Color.LocalChams, "[ Color ] Local Chams");
	assign_array(g_Var.Color.LocalFakeChams, "[ Color ] Fake Chams");
	assign_array(g_Var.Color.LocalGlow, "[ Color ] Local Glow");
	assign_array(g_Var.Color.ChamsVisible, "[ Color ] Chams Visible");
	assign_array(g_Var.Color.TeamChams, "[ Color ] Chams Team");
	assign_array(g_Var.Color.TeamChamsVisible, "[ Color ] Chams Team Visible");
	assign_array(g_Var.Color.HandChams, "[ Color ] Chams Hand");
	assign_array(g_Var.Color.HandChamsVisible, "[ Color ] Chams Hand Visible");
	assign_array(g_Var.Color.WeaponChams, "[ Color ] Chams Weapon");
	assign_array(g_Var.Color.WeaponChamsVisible, "[ Color ] Chams Weapon Visible");
	assign_array(g_Var.Color.ViewChams, "[ Color ] Chams View Weapon");

	assign_array(g_Var.Color.Glow, "[ Color ] Glow");
	assign_array(g_Var.Color.GlowVisible, "[ Color ] Glow Visible");

	assign_array(g_Var.Color.BulletTracer, "[ Color ] Bullet Tracer");

	assign(g_Skins.Knife, "[ Skin ] Knife Model");
	assign(g_Skins.Glove, "[ Skin ] Glove Model");
	assign(g_Skins.GlovePaint, "[ Skin ] Glove PaintKit");

	input_file.close();

	return true;
}

const bool CConfigFile::SaveConfig()
{
	std::ofstream output_file;

	switch (g_ConfigFile.m_iID)
	{
	case 0:
		output_file = std::ofstream("c:/kidua/rage.json");
		break;
	case 1:
		output_file = std::ofstream("c:/kidua/rage1.json");
		break;
	case 2:
		output_file = std::ofstream("c:/kidua/rage2.json");
		break;
	}

	if (!output_file.good())
		return false;

	save(g_Var.Ragebot[0].Enable, "[ Aimbot Light ] Enable");
	save(g_Var.Ragebot[0].Autoshoot, "[ Aimbot Light ] Auto Fire");
	save(g_Var.Ragebot[0].Multipoint, "[ Aimbot Light ] Multipoints");
	save(g_Var.Ragebot[0].Multipointbox[0], "[ Aimbot Light ] Multipoints Head");
	save(g_Var.Ragebot[0].Multipointbox[1], "[ Aimbot Light ] Multipoints Body");
	save(g_Var.Ragebot[0].Multipointbox[2], "[ Aimbot Light ] Multipoints Arms");
	save(g_Var.Ragebot[0].Multipointbox[3], "[ Aimbot Light ] Multipoints Legs");
	save(g_Var.Ragebot[0].Multipointbox[4], "[ Aimbot Light ] Multipoints Feet");
	save(g_Var.Ragebot[0].HeadPointScale, "[ Aimbot Light ] Multipoints Scale Head");
	save(g_Var.Ragebot[0].PointScale, "[ Aimbot Light ] Multipoints Scale");
	save(g_Var.Ragebot[0].AutoPointScale, "[ Aimbot Light ] Multipoints Scale Auto");
	save(g_Var.Ragebot[0].BodyAfter, "[ Aimbot Light ] Target Body After");
	save(g_Var.Ragebot[0].Team, "[ Aimbot Light ] Team");
	save(g_Var.Ragebot[0].Spot, "[ Aimbot Light ] Prior Spot");
	save(g_Var.Ragebot[0].Selection, "[ Aimbot Light ] Target Selection");
	save(g_Var.Ragebot[0].Multibox, "[ Aimbot Light ] Multibox");
	save(g_Var.Ragebot[0].Multiboxes[0], "[ Aimbot Light ] Multibox Head");
	save(g_Var.Ragebot[0].Multiboxes[1], "[ Aimbot Light ] Multibox Body");
	save(g_Var.Ragebot[0].Multiboxes[2], "[ Aimbot Light ] Multibox Arms");
	save(g_Var.Ragebot[0].Multiboxes[3], "[ Aimbot Light ] Multibox Legs");
	save(g_Var.Ragebot[0].Multiboxes[4], "[ Aimbot Light ] Multibox Feet");
	save(g_Var.Ragebot[0].Fov, "[ Aimbot Light ] Field of View");
	save(g_Var.Ragebot[0].Hitchance, "[ Aimbot Light ] Hitchance");
	save(g_Var.Ragebot[0].Mindamage, "[ Aimbot Light ] Minimum Damage");
	save(g_Var.Ragebot[0].Walldamage, "[ Aimbot Light ] Wall Minimum Damage");
	save(g_Var.Ragebot[0].AutoMindDamage, "[ Aimbot Light ] Minimum Damage Auto");
	save(g_Var.Ragebot[0].Silent, "[ Aimbot Light ] Silent");
	save(g_Var.Ragebot[0].Autowall, "[ Aimbot Light ] Autowall");
	save(g_Var.Ragebot[0].PosAdjust, "[ Aimbot Light ] Position Adjustment");
	save(g_Var.Ragebot[0].Recoil, "[ Aimbot Light ] Remove Recoil");
	save(g_Var.Ragebot[0].Spread, "[ Aimbot Light ] Remove Spread");
	save(g_Var.Ragebot[0].Autozoom, "[ Aimbot Light ] Auto Scope");
	save(g_Var.Ragebot[0].Autostop, "[ Aimbot Light ] Auto Stop");
	save(g_Var.Ragebot[0].Limbs[0], "[ Aimbot Light ] Ignore Limbs In Move");
	save(g_Var.Ragebot[0].Limbs[1], "[ Aimbot Light ] Ignore Limbs In Air");
	save(g_Var.Ragebot[0].IgnoreHead[0], "[ Aimbot Light ] Ignore Head In Air");
	save(g_Var.Ragebot[0].IgnoreHead[1], "[ Aimbot Light ] Ignore Head No Update");
	save(g_Var.Ragebot[0].IgnoreHead[2], "[ Aimbot Light ] Ignore Head High Fake");
	save(g_Var.Ragebot[0].PriorityRecord[0], "[ Aimbot Light ] History On Shot");
	save(g_Var.Ragebot[0].PriorityRecord[1], "[ Aimbot Light ] History No Weapon");
	save(g_Var.Ragebot[0].PriorityRecord[2], "[ Aimbot Light ] History In Air");
	save(g_Var.Ragebot[0].PriorityRecord[3], "[ Aimbot Light ] History Running");
	save(g_Var.Ragebot[0].BodyAimHp, "[ Aimbot Light ] Prefer Body if Lower Than");
	save(g_Var.Ragebot[0].ForceBaim, "[ Aimbot Light ] Force BodyAim");

	save(g_Var.Ragebot[1].Enable, "[ Aimbot Heavy ] Enable");
	save(g_Var.Ragebot[1].Autoshoot, "[ Aimbot Heavy ] Auto Fire");
	save(g_Var.Ragebot[1].Multipoint, "[ Aimbot Heavy ] Multipoints");
	save(g_Var.Ragebot[1].Multipointbox[0], "[ Aimbot Heavy ] Multipoints Head");
	save(g_Var.Ragebot[1].Multipointbox[1], "[ Aimbot Heavy ] Multipoints Body");
	save(g_Var.Ragebot[1].Multipointbox[2], "[ Aimbot Heavy ] Multipoints Arms");
	save(g_Var.Ragebot[1].Multipointbox[3], "[ Aimbot Heavy ] Multipoints Legs");
	save(g_Var.Ragebot[1].Multipointbox[4], "[ Aimbot Heavy ] Multipoints Feet");
	save(g_Var.Ragebot[1].HeadPointScale, "[ Aimbot Heavy ] Multipoints Scale Head");
	save(g_Var.Ragebot[1].PointScale, "[ Aimbot Heavy ] Multipoints Scale");
	save(g_Var.Ragebot[1].AutoPointScale, "[ Aimbot Heavy ] Multipoints Scale Auto");
	save(g_Var.Ragebot[1].BodyAfter, "[ Aimbot Heavy ] Target Body After");
	save(g_Var.Ragebot[1].Team, "[ Aimbot Heavy ] Team");
	save(g_Var.Ragebot[1].Spot, "[ Aimbot Heavy ] Prior Spot");
	save(g_Var.Ragebot[1].Selection, "[ Aimbot Heavy ] Target Selection");
	save(g_Var.Ragebot[1].Multibox, "[ Aimbot Heavy ] Multibox");
	save(g_Var.Ragebot[1].Multiboxes[0], "[ Aimbot Heavy ] Multibox Head");
	save(g_Var.Ragebot[1].Multiboxes[1], "[ Aimbot Heavy ] Multibox Body");
	save(g_Var.Ragebot[1].Multiboxes[2], "[ Aimbot Heavy ] Multibox Arms");
	save(g_Var.Ragebot[1].Multiboxes[3], "[ Aimbot Heavy ] Multibox Legs");
	save(g_Var.Ragebot[1].Multiboxes[4], "[ Aimbot Heavy ] Multibox Feet");
	save(g_Var.Ragebot[1].Fov, "[ Aimbot Heavy ] Field of View");
	save(g_Var.Ragebot[1].Hitchance, "[ Aimbot Heavy ] Hitchance");
	save(g_Var.Ragebot[1].Mindamage, "[ Aimbot Heavy ] Minimum Damage");
	save(g_Var.Ragebot[1].Walldamage, "[ Aimbot Heavy ] Wall Minimum Damage");
	save(g_Var.Ragebot[1].AutoMindDamage, "[ Aimbot Heavy ] Minimum Damage Auto");
	save(g_Var.Ragebot[1].Silent, "[ Aimbot Heavy ] Silent");
	save(g_Var.Ragebot[1].Autowall, "[ Aimbot Heavy ] Autowall");
	save(g_Var.Ragebot[1].PosAdjust, "[ Aimbot Heavy ] Position Adjustment");
	save(g_Var.Ragebot[1].Recoil, "[ Aimbot Heavy ] Remove Recoil");
	save(g_Var.Ragebot[1].Spread, "[ Aimbot Heavy ] Remove Spread");
	save(g_Var.Ragebot[1].Autozoom, "[ Aimbot Heavy ] Auto Scope");
	save(g_Var.Ragebot[1].Autostop, "[ Aimbot Heavy ] Auto Stop");
	save(g_Var.Ragebot[1].Limbs[0], "[ Aimbot Heavy ] Ignore Limbs In Move");
	save(g_Var.Ragebot[1].Limbs[1], "[ Aimbot Heavy ] Ignore Limbs In Air");
	save(g_Var.Ragebot[1].IgnoreHead[0], "[ Aimbot Heavy ] Ignore Head In Air");
	save(g_Var.Ragebot[1].IgnoreHead[1], "[ Aimbot Heavy ] Ignore Head No Update");
	save(g_Var.Ragebot[1].IgnoreHead[2], "[ Aimbot Heavy ] Ignore Head High Fake");
	save(g_Var.Ragebot[1].PriorityRecord[0], "[ Aimbot Heavy ] History On Shot");
	save(g_Var.Ragebot[1].PriorityRecord[1], "[ Aimbot Heavy ] History No Weapon");
	save(g_Var.Ragebot[1].PriorityRecord[2], "[ Aimbot Heavy ] History In Air");
	save(g_Var.Ragebot[1].PriorityRecord[3], "[ Aimbot Heavy ] History Running");
	save(g_Var.Ragebot[1].BodyAimHp, "[ Aimbot Heavy ] Prefer Body if Lower Than");
	save(g_Var.Ragebot[1].ForceBaim, "[ Aimbot Heavy ] Force BodyAim");

	save(g_Var.Ragebot[2].Enable, "[ Aimbot Machinegun ] Enable");
	save(g_Var.Ragebot[2].Autoshoot, "[ Aimbot Machinegun ] Auto Fire");
	save(g_Var.Ragebot[2].Multipoint, "[ Aimbot Machinegun ] Multipoints");
	save(g_Var.Ragebot[2].Multipointbox[0], "[ Aimbot Machinegun ] Multipoints Head");
	save(g_Var.Ragebot[2].Multipointbox[1], "[ Aimbot Machinegun ] Multipoints Body");
	save(g_Var.Ragebot[2].Multipointbox[2], "[ Aimbot Machinegun ] Multipoints Arms");
	save(g_Var.Ragebot[2].Multipointbox[3], "[ Aimbot Machinegun ] Multipoints Legs");
	save(g_Var.Ragebot[2].Multipointbox[4], "[ Aimbot Machinegun ] Multipoints Feet");
	save(g_Var.Ragebot[2].HeadPointScale, "[ Aimbot Machinegun ] Multipoints Scale Head");
	save(g_Var.Ragebot[2].PointScale, "[ Aimbot Machinegun ] Multipoints Scale");
	save(g_Var.Ragebot[2].AutoPointScale, "[ Aimbot Machinegun ] Multipoints Scale Auto");
	save(g_Var.Ragebot[2].BodyAfter, "[ Aimbot Machinegun ] Target Body After");
	save(g_Var.Ragebot[2].Team, "[ Aimbot Machinegun ] Team");
	save(g_Var.Ragebot[2].Spot, "[ Aimbot Machinegun ] Prior Spot");
	save(g_Var.Ragebot[2].Selection, "[ Aimbot Machinegun ] Target Selection");
	save(g_Var.Ragebot[2].Multibox, "[ Aimbot Machinegun ] Multibox");
	save(g_Var.Ragebot[2].Multiboxes[0], "[ Aimbot Machinegun ] Multibox Head");
	save(g_Var.Ragebot[2].Multiboxes[1], "[ Aimbot Machinegun ] Multibox Body");
	save(g_Var.Ragebot[2].Multiboxes[2], "[ Aimbot Machinegun ] Multibox Arms");
	save(g_Var.Ragebot[2].Multiboxes[3], "[ Aimbot Machinegun ] Multibox Legs");
	save(g_Var.Ragebot[2].Multiboxes[4], "[ Aimbot Machinegun ] Multibox Feet");
	save(g_Var.Ragebot[2].Fov, "[ Aimbot Machinegun ] Field of View");
	save(g_Var.Ragebot[2].Hitchance, "[ Aimbot Machinegun ] Hitchance");
	save(g_Var.Ragebot[2].Mindamage, "[ Aimbot Machinegun ] Minimum Damage");
	save(g_Var.Ragebot[2].Walldamage, "[ Aimbot Machinegun ] Wall Minimum Damage");
	save(g_Var.Ragebot[2].AutoMindDamage, "[ Aimbot Machinegun ] Minimum Damage Auto");
	save(g_Var.Ragebot[2].Silent, "[ Aimbot Machinegun ] Silent");
	save(g_Var.Ragebot[2].Autowall, "[ Aimbot Machinegun ] Autowall");
	save(g_Var.Ragebot[2].PosAdjust, "[ Aimbot Machinegun ] Position Adjustment");
	save(g_Var.Ragebot[2].Recoil, "[ Aimbot Machinegun ] Remove Recoil");
	save(g_Var.Ragebot[2].Spread, "[ Aimbot Machinegun ] Remove Spread");
	save(g_Var.Ragebot[2].Autozoom, "[ Aimbot Machinegun ] Auto Scope");
	save(g_Var.Ragebot[2].Autostop, "[ Aimbot Machinegun ] Auto Stop");
	save(g_Var.Ragebot[2].Limbs[0], "[ Aimbot Machinegun ] Ignore Limbs In Move");
	save(g_Var.Ragebot[2].Limbs[1], "[ Aimbot Machinegun ] Ignore Limbs In Air");
	save(g_Var.Ragebot[2].IgnoreHead[0], "[ Aimbot Machinegun ] Ignore Head In Air");
	save(g_Var.Ragebot[2].IgnoreHead[1], "[ Aimbot Machinegun ] Ignore Head No Update");
	save(g_Var.Ragebot[2].IgnoreHead[2], "[ Aimbot Machinegun ] Ignore Head High Fake");
	save(g_Var.Ragebot[2].PriorityRecord[0], "[ Aimbot Machinegun ] History On Shot");
	save(g_Var.Ragebot[2].PriorityRecord[1], "[ Aimbot Machinegun ] History No Weapon");
	save(g_Var.Ragebot[2].PriorityRecord[2], "[ Aimbot Machinegun ] History In Air");
	save(g_Var.Ragebot[2].PriorityRecord[3], "[ Aimbot Machinegun ] History Running");
	save(g_Var.Ragebot[2].BodyAimHp, "[ Aimbot Machinegun ] Prefer Body if Lower Than");
	save(g_Var.Ragebot[2].ForceBaim, "[ Aimbot Machinegun ] Force BodyAim");

	save(g_Var.Ragebot[3].Enable, "[ Aimbot Rifle ] Enable");
	save(g_Var.Ragebot[3].Autoshoot, "[ Aimbot Rifle ] Auto Fire");
	save(g_Var.Ragebot[3].Multipoint, "[ Aimbot Rifle ] Multipoints");
	save(g_Var.Ragebot[3].Multipointbox[0], "[ Aimbot Rifle ] Multipoints Head");
	save(g_Var.Ragebot[3].Multipointbox[1], "[ Aimbot Rifle ] Multipoints Body");
	save(g_Var.Ragebot[3].Multipointbox[2], "[ Aimbot Rifle ] Multipoints Arms");
	save(g_Var.Ragebot[3].Multipointbox[3], "[ Aimbot Rifle ] Multipoints Legs");
	save(g_Var.Ragebot[3].Multipointbox[4], "[ Aimbot Rifle ] Multipoints Feet");
	save(g_Var.Ragebot[3].HeadPointScale, "[ Aimbot Rifle ] Multipoints Scale Head");
	save(g_Var.Ragebot[3].PointScale, "[ Aimbot Rifle ] Multipoints Scale");
	save(g_Var.Ragebot[3].AutoPointScale, "[ Aimbot Rifle ] Multipoints Scale Auto");
	save(g_Var.Ragebot[3].BodyAfter, "[ Aimbot Rifle ] Target Body After");
	save(g_Var.Ragebot[3].Team, "[ Aimbot Rifle ] Team");
	save(g_Var.Ragebot[3].Spot, "[ Aimbot Rifle ] Prior Spot");
	save(g_Var.Ragebot[3].Selection, "[ Aimbot Rifle ] Target Selection");
	save(g_Var.Ragebot[3].Multibox, "[ Aimbot Rifle ] Multibox");
	save(g_Var.Ragebot[3].Multiboxes[0], "[ Aimbot Rifle ] Multibox Head");
	save(g_Var.Ragebot[3].Multiboxes[1], "[ Aimbot Rifle ] Multibox Body");
	save(g_Var.Ragebot[3].Multiboxes[2], "[ Aimbot Rifle ] Multibox Arms");
	save(g_Var.Ragebot[3].Multiboxes[3], "[ Aimbot Rifle ] Multibox Legs");
	save(g_Var.Ragebot[3].Multiboxes[4], "[ Aimbot Rifle ] Multibox Feet");
	save(g_Var.Ragebot[3].Fov, "[ Aimbot Rifle ] Field of View");
	save(g_Var.Ragebot[3].Hitchance, "[ Aimbot Rifle ] Hitchance");
	save(g_Var.Ragebot[3].Mindamage, "[ Aimbot Rifle ] Minimum Damage");
	save(g_Var.Ragebot[3].Walldamage, "[ Aimbot Rifle ] Wall Minimum Damage");
	save(g_Var.Ragebot[3].AutoMindDamage, "[ Aimbot Rifle ] Minimum Damage Auto");
	save(g_Var.Ragebot[3].Silent, "[ Aimbot Rifle ] Silent");
	save(g_Var.Ragebot[3].Autowall, "[ Aimbot Rifle ] Autowall");
	save(g_Var.Ragebot[3].PosAdjust, "[ Aimbot Rifle ] Position Adjustment");
	save(g_Var.Ragebot[3].Recoil, "[ Aimbot Rifle ] Remove Recoil");
	save(g_Var.Ragebot[3].Spread, "[ Aimbot Rifle ] Remove Spread");
	save(g_Var.Ragebot[3].Autozoom, "[ Aimbot Rifle ] Auto Scope");
	save(g_Var.Ragebot[3].Autostop, "[ Aimbot Rifle ] Auto Stop");
	save(g_Var.Ragebot[3].Limbs[0], "[ Aimbot Rifle ] Ignore Limbs In Move");
	save(g_Var.Ragebot[3].Limbs[1], "[ Aimbot Rifle ] Ignore Limbs In Air");
	save(g_Var.Ragebot[3].IgnoreHead[0], "[ Aimbot Rifle ] Ignore Head In Air");
	save(g_Var.Ragebot[3].IgnoreHead[1], "[ Aimbot Rifle ] Ignore Head No Update");
	save(g_Var.Ragebot[3].IgnoreHead[2], "[ Aimbot Rifle ] Ignore Head High Fake");
	save(g_Var.Ragebot[3].PriorityRecord[0], "[ Aimbot Rifle ] History On Shot");
	save(g_Var.Ragebot[3].PriorityRecord[1], "[ Aimbot Rifle ] History No Weapon");
	save(g_Var.Ragebot[3].PriorityRecord[2], "[ Aimbot Rifle ] History In Air");
	save(g_Var.Ragebot[3].PriorityRecord[3], "[ Aimbot Rifle ] History Running");
	save(g_Var.Ragebot[3].BodyAimHp, "[ Aimbot Rifle ] Prefer Body if Lower Than");
	save(g_Var.Ragebot[3].ForceBaim, "[ Aimbot Rifle ] Force BodyAim");

	save(g_Var.Ragebot[4].Enable, "[ Aimbot Shotgun ] Enable");
	save(g_Var.Ragebot[4].Autoshoot, "[ Aimbot Shotgun ] Auto Fire");
	save(g_Var.Ragebot[4].Multipoint, "[ Aimbot Shotgun ] Multipoints");
	save(g_Var.Ragebot[4].Multipointbox[0], "[ Aimbot Shotgun ] Multipoints Head");
	save(g_Var.Ragebot[4].Multipointbox[1], "[ Aimbot Shotgun ] Multipoints Body");
	save(g_Var.Ragebot[4].Multipointbox[2], "[ Aimbot Shotgun ] Multipoints Arms");
	save(g_Var.Ragebot[4].Multipointbox[3], "[ Aimbot Shotgun ] Multipoints Legs");
	save(g_Var.Ragebot[4].Multipointbox[4], "[ Aimbot Shotgun ] Multipoints Feet");
	save(g_Var.Ragebot[4].HeadPointScale, "[ Aimbot Shotgun ] Multipoints Scale Head");
	save(g_Var.Ragebot[4].PointScale, "[ Aimbot Shotgun ] Multipoints Scale");
	save(g_Var.Ragebot[4].AutoPointScale, "[ Aimbot Shotgun ] Multipoints Scale Auto");
	save(g_Var.Ragebot[4].BodyAfter, "[ Aimbot Shotgun ] Target Body After");
	save(g_Var.Ragebot[4].Team, "[ Aimbot Shotgun ] Team");
	save(g_Var.Ragebot[4].Spot, "[ Aimbot Shotgun ] Prior Spot");
	save(g_Var.Ragebot[4].Selection, "[ Aimbot Shotgun ] Target Selection");
	save(g_Var.Ragebot[4].Multibox, "[ Aimbot Shotgun ] Multibox");
	save(g_Var.Ragebot[4].Multiboxes[0], "[ Aimbot Shotgun ] Multibox Head");
	save(g_Var.Ragebot[4].Multiboxes[1], "[ Aimbot Shotgun ] Multibox Body");
	save(g_Var.Ragebot[4].Multiboxes[2], "[ Aimbot Shotgun ] Multibox Arms");
	save(g_Var.Ragebot[4].Multiboxes[3], "[ Aimbot Shotgun ] Multibox Legs");
	save(g_Var.Ragebot[4].Multiboxes[4], "[ Aimbot Shotgun ] Multibox Feet");
	save(g_Var.Ragebot[4].Fov, "[ Aimbot Shotgun ] Field of View");
	save(g_Var.Ragebot[4].Hitchance, "[ Aimbot Shotgun ] Hitchance");
	save(g_Var.Ragebot[4].Mindamage, "[ Aimbot Shotgun ] Minimum Damage");
	save(g_Var.Ragebot[4].Walldamage, "[ Aimbot Shotgun ] Wall Minimum Damage");
	save(g_Var.Ragebot[4].AutoMindDamage, "[ Aimbot Shotgun ] Minimum Damage Auto");
	save(g_Var.Ragebot[4].Silent, "[ Aimbot Shotgun ] Silent");
	save(g_Var.Ragebot[4].Autowall, "[ Aimbot Shotgun ] Autowall");
	save(g_Var.Ragebot[4].PosAdjust, "[ Aimbot Shotgun ] Position Adjustment");
	save(g_Var.Ragebot[4].Recoil, "[ Aimbot Shotgun ] Remove Recoil");
	save(g_Var.Ragebot[4].Spread, "[ Aimbot Shotgun ] Remove Spread");
	save(g_Var.Ragebot[4].Autozoom, "[ Aimbot Shotgun ] Auto Scope");
	save(g_Var.Ragebot[4].Autostop, "[ Aimbot Shotgun ] Auto Stop");
	save(g_Var.Ragebot[4].Limbs[0], "[ Aimbot Shotgun ] Ignore Limbs In Move");
	save(g_Var.Ragebot[4].Limbs[1], "[ Aimbot Shotgun ] Ignore Limbs In Air");
	save(g_Var.Ragebot[4].IgnoreHead[0], "[ Aimbot Shotgun ] Ignore Head In Air");
	save(g_Var.Ragebot[4].IgnoreHead[1], "[ Aimbot Shotgun ] Ignore Head No Update");
	save(g_Var.Ragebot[4].IgnoreHead[2], "[ Aimbot Shotgun ] Ignore Head High Fake");
	save(g_Var.Ragebot[4].PriorityRecord[0], "[ Aimbot Shotgun ] History On Shot");
	save(g_Var.Ragebot[4].PriorityRecord[1], "[ Aimbot Shotgun ] History No Weapon");
	save(g_Var.Ragebot[4].PriorityRecord[2], "[ Aimbot Shotgun ] History In Air");
	save(g_Var.Ragebot[4].PriorityRecord[3], "[ Aimbot Shotgun ] History Running");
	save(g_Var.Ragebot[4].BodyAimHp, "[ Aimbot Shotgun ] Prefer Body if Lower Than");
	save(g_Var.Ragebot[4].ForceBaim, "[ Aimbot Shotgun ] Force BodyAim");

	save(g_Var.Ragebot[5].Enable, "[ Aimbot Light Sniper ] Enable");
	save(g_Var.Ragebot[5].Autoshoot, "[ Aimbot Light Sniper ] Auto Fire");
	save(g_Var.Ragebot[5].Multipoint, "[ Aimbot Light Sniper ] Multipoints");
	save(g_Var.Ragebot[5].Multipointbox[0], "[ Aimbot Light Sniper ] Multipoints Head");
	save(g_Var.Ragebot[5].Multipointbox[1], "[ Aimbot Light Sniper ] Multipoints Body");
	save(g_Var.Ragebot[5].Multipointbox[2], "[ Aimbot Light Sniper ] Multipoints Arms");
	save(g_Var.Ragebot[5].Multipointbox[3], "[ Aimbot Light Sniper ] Multipoints Legs");
	save(g_Var.Ragebot[5].Multipointbox[4], "[ Aimbot Light Sniper ] Multipoints Feet");
	save(g_Var.Ragebot[5].HeadPointScale, "[ Aimbot Light Sniper ] Multipoints Scale Head");
	save(g_Var.Ragebot[5].PointScale, "[ Aimbot Light Sniper ] Multipoints Scale");
	save(g_Var.Ragebot[5].AutoPointScale, "[ Aimbot Light Sniper ] Multipoints Scale Auto");
	save(g_Var.Ragebot[5].BodyAfter, "[ Aimbot Light Sniper ] Target Body After");
	save(g_Var.Ragebot[5].Team, "[ Aimbot Light Sniper ] Team");
	save(g_Var.Ragebot[5].Spot, "[ Aimbot Light Sniper ] Prior Spot");
	save(g_Var.Ragebot[5].Selection, "[ Aimbot Light Sniper ] Target Selection");
	save(g_Var.Ragebot[5].Multibox, "[ Aimbot Light Sniper ] Multibox");
	save(g_Var.Ragebot[5].Multiboxes[0], "[ Aimbot Light Sniper ] Multibox Head");
	save(g_Var.Ragebot[5].Multiboxes[1], "[ Aimbot Light Sniper ] Multibox Body");
	save(g_Var.Ragebot[5].Multiboxes[2], "[ Aimbot Light Sniper ] Multibox Arms");
	save(g_Var.Ragebot[5].Multiboxes[3], "[ Aimbot Light Sniper ] Multibox Legs");
	save(g_Var.Ragebot[5].Multiboxes[4], "[ Aimbot Light Sniper ] Multibox Feet");
	save(g_Var.Ragebot[5].Fov, "[ Aimbot Light Sniper ] Field of View");
	save(g_Var.Ragebot[5].Hitchance, "[ Aimbot Light Sniper ] Hitchance");
	save(g_Var.Ragebot[5].Mindamage, "[ Aimbot Light Sniper ] Minimum Damage");
	save(g_Var.Ragebot[5].Walldamage, "[ Aimbot Light Sniper ] Wall Minimum Damage");
	save(g_Var.Ragebot[5].AutoMindDamage, "[ Aimbot Light Sniper ] Minimum Damage Auto");
	save(g_Var.Ragebot[5].Silent, "[ Aimbot Light Sniper ] Silent");
	save(g_Var.Ragebot[5].Autowall, "[ Aimbot Light Sniper ] Autowall");
	save(g_Var.Ragebot[5].PosAdjust, "[ Aimbot Light Sniper ] Position Adjustment");
	save(g_Var.Ragebot[5].Recoil, "[ Aimbot Light Sniper ] Remove Recoil");
	save(g_Var.Ragebot[5].Spread, "[ Aimbot Light Sniper ] Remove Spread");
	save(g_Var.Ragebot[5].Autozoom, "[ Aimbot Light Sniper ] Auto Scope");
	save(g_Var.Ragebot[5].Autostop, "[ Aimbot Light Sniper ] Auto Stop");
	save(g_Var.Ragebot[5].Limbs[0], "[ Aimbot Light Sniper ] Ignore Limbs In Move");
	save(g_Var.Ragebot[5].Limbs[1], "[ Aimbot Light Sniper ] Ignore Limbs In Air");
	save(g_Var.Ragebot[5].IgnoreHead[0], "[ Aimbot Light Sniper ] Ignore Head In Air");
	save(g_Var.Ragebot[5].IgnoreHead[1], "[ Aimbot Light Sniper ] Ignore Head No Update");
	save(g_Var.Ragebot[5].IgnoreHead[2], "[ Aimbot Light Sniper ] Ignore Head High Fake");
	save(g_Var.Ragebot[5].PriorityRecord[0], "[ Aimbot Light Sniper ] History On Shot");
	save(g_Var.Ragebot[5].PriorityRecord[1], "[ Aimbot Light Sniper ] History No Weapon");
	save(g_Var.Ragebot[5].PriorityRecord[2], "[ Aimbot Light Sniper ] History In Air");
	save(g_Var.Ragebot[5].PriorityRecord[3], "[ Aimbot Light Sniper ] History Running");
	save(g_Var.Ragebot[5].BodyAimHp, "[ Aimbot Light Sniper ] Prefer Body if Lower Than");
	save(g_Var.Ragebot[5].ForceBaim, "[ Aimbot Light Sniper ] Force BodyAim");

	save(g_Var.Ragebot[6].Enable, "[ Aimbot Auto Sniper ] Enable");
	save(g_Var.Ragebot[6].Autoshoot, "[ Aimbot Auto Sniper ] Auto Fire");
	save(g_Var.Ragebot[6].Multipoint, "[ Aimbot Auto Sniper ] Multipoints");
	save(g_Var.Ragebot[6].Multipointbox[0], "[ Aimbot Auto Sniper ] Multipoints Head");
	save(g_Var.Ragebot[6].Multipointbox[1], "[ Aimbot Auto Sniper ] Multipoints Body");
	save(g_Var.Ragebot[6].Multipointbox[2], "[ Aimbot Auto Sniper ] Multipoints Arms");
	save(g_Var.Ragebot[6].Multipointbox[3], "[ Aimbot Auto Sniper ] Multipoints Legs");
	save(g_Var.Ragebot[6].Multipointbox[4], "[ Aimbot Auto Sniper ] Multipoints Feet");
	save(g_Var.Ragebot[6].HeadPointScale, "[ Aimbot Auto Sniper ] Multipoints Scale Head");
	save(g_Var.Ragebot[6].PointScale, "[ Aimbot Auto Sniper ] Multipoints Scale");
	save(g_Var.Ragebot[6].AutoPointScale, "[ Aimbot Auto Sniper ] Multipoints Scale Auto");
	save(g_Var.Ragebot[6].BodyAfter, "[ Aimbot Auto Sniper ] Target Body After");
	save(g_Var.Ragebot[6].Team, "[ Aimbot Auto Sniper ] Team");
	save(g_Var.Ragebot[6].Spot, "[ Aimbot Auto Sniper ] Prior Spot");
	save(g_Var.Ragebot[6].Selection, "[ Aimbot Auto Sniper ] Target Selection");
	save(g_Var.Ragebot[6].Multibox, "[ Aimbot Auto Sniper ] Multibox");
	save(g_Var.Ragebot[6].Multiboxes[0], "[ Aimbot Auto Sniper ] Multibox Head");
	save(g_Var.Ragebot[6].Multiboxes[1], "[ Aimbot Auto Sniper ] Multibox Body");
	save(g_Var.Ragebot[6].Multiboxes[2], "[ Aimbot Auto Sniper ] Multibox Arms");
	save(g_Var.Ragebot[6].Multiboxes[3], "[ Aimbot Auto Sniper ] Multibox Legs");
	save(g_Var.Ragebot[6].Multiboxes[4], "[ Aimbot Auto Sniper ] Multibox Feet");
	save(g_Var.Ragebot[6].Fov, "[ Aimbot Auto Sniper ] Field of View");
	save(g_Var.Ragebot[6].Hitchance, "[ Aimbot Auto Sniper ] Hitchance");
	save(g_Var.Ragebot[6].Mindamage, "[ Aimbot Auto Sniper ] Minimum Damage");
	save(g_Var.Ragebot[6].Walldamage, "[ Aimbot Auto Sniper ] Wall Minimum Damage");
	save(g_Var.Ragebot[6].AutoMindDamage, "[ Aimbot Auto Sniper ] Minimum Damage Auto");
	save(g_Var.Ragebot[6].Silent, "[ Aimbot Auto Sniper ] Silent");
	save(g_Var.Ragebot[6].Autowall, "[ Aimbot Auto Sniper ] Autowall");
	save(g_Var.Ragebot[6].PosAdjust, "[ Aimbot Auto Sniper ] Position Adjustment");
	save(g_Var.Ragebot[6].Recoil, "[ Aimbot Auto Sniper ] Remove Recoil");
	save(g_Var.Ragebot[6].Spread, "[ Aimbot Auto Sniper ] Remove Spread");
	save(g_Var.Ragebot[6].Autozoom, "[ Aimbot Auto Sniper ] Auto Scope");
	save(g_Var.Ragebot[6].Autostop, "[ Aimbot Auto Sniper ] Auto Stop");
	save(g_Var.Ragebot[6].Limbs[0], "[ Aimbot Auto Sniper ] Ignore Limbs In Move");
	save(g_Var.Ragebot[6].Limbs[1], "[ Aimbot Auto Sniper ] Ignore Limbs In Air");
	save(g_Var.Ragebot[6].IgnoreHead[0], "[ Aimbot Auto Sniper ] Ignore Head In Air");
	save(g_Var.Ragebot[6].IgnoreHead[1], "[ Aimbot Auto Sniper ] Ignore Head No Update");
	save(g_Var.Ragebot[6].IgnoreHead[2], "[ Aimbot Auto Sniper ] Ignore Head High Fake");
	save(g_Var.Ragebot[6].PriorityRecord[0], "[ Aimbot Auto Sniper ] History On Shot");
	save(g_Var.Ragebot[6].PriorityRecord[1], "[ Aimbot Auto Sniper ] History No Weapon");
	save(g_Var.Ragebot[6].PriorityRecord[2], "[ Aimbot Auto Sniper ] History In Air");
	save(g_Var.Ragebot[6].PriorityRecord[3], "[ Aimbot Auto Sniper ] History Running");
	save(g_Var.Ragebot[6].BodyAimHp, "[ Aimbot Auto Sniper ] Prefer Body if Lower Than");
	save(g_Var.Ragebot[6].ForceBaim, "[ Aimbot Auto Sniper ] Force BodyAim");

	save(g_Var.Ragebot[7].Enable, "[ Aimbot Heavy Sniper ] Enable");
	save(g_Var.Ragebot[7].Autoshoot, "[ Aimbot Heavy Sniper ] Auto Fire");
	save(g_Var.Ragebot[7].Multipoint, "[ Aimbot Heavy Sniper ] Multipoints");
	save(g_Var.Ragebot[7].Multipointbox[0], "[ Aimbot Heavy Sniper ] Multipoints Head");
	save(g_Var.Ragebot[7].Multipointbox[1], "[ Aimbot Heavy Sniper ] Multipoints Body");
	save(g_Var.Ragebot[7].Multipointbox[2], "[ Aimbot Heavy Sniper ] Multipoints Arms");
	save(g_Var.Ragebot[7].Multipointbox[3], "[ Aimbot Heavy Sniper ] Multipoints Legs");
	save(g_Var.Ragebot[7].Multipointbox[4], "[ Aimbot Heavy Sniper ] Multipoints Feet");
	save(g_Var.Ragebot[7].HeadPointScale, "[ Aimbot Heavy Sniper ] Multipoints Scale Head");
	save(g_Var.Ragebot[7].PointScale, "[ Aimbot Heavy Sniper ] Multipoints Scale");
	save(g_Var.Ragebot[7].AutoPointScale, "[ Aimbot Heavy Sniper ] Multipoints Scale Auto");
	save(g_Var.Ragebot[7].BodyAfter, "[ Aimbot Heavy Sniper ] Target Body After");
	save(g_Var.Ragebot[7].Team, "[ Aimbot Heavy Sniper ] Team");
	save(g_Var.Ragebot[7].Spot, "[ Aimbot Heavy Sniper ] Prior Spot");
	save(g_Var.Ragebot[7].Selection, "[ Aimbot Heavy Sniper ] Target Selection");
	save(g_Var.Ragebot[7].Multibox, "[ Aimbot Heavy Sniper ] Multibox");
	save(g_Var.Ragebot[7].Multiboxes[0], "[ Aimbot Heavy Sniper ] Multibox Head");
	save(g_Var.Ragebot[7].Multiboxes[1], "[ Aimbot Heavy Sniper ] Multibox Body");
	save(g_Var.Ragebot[7].Multiboxes[2], "[ Aimbot Heavy Sniper ] Multibox Arms");
	save(g_Var.Ragebot[7].Multiboxes[3], "[ Aimbot Heavy Sniper ] Multibox Legs");
	save(g_Var.Ragebot[7].Multiboxes[4], "[ Aimbot Heavy Sniper ] Multibox Feet");
	save(g_Var.Ragebot[7].Fov, "[ Aimbot Heavy Sniper ] Field of View");
	save(g_Var.Ragebot[7].Hitchance, "[ Aimbot Heavy Sniper ] Hitchance");
	save(g_Var.Ragebot[7].Mindamage, "[ Aimbot Heavy Sniper ] Minimum Damage");
	save(g_Var.Ragebot[7].Walldamage, "[ Aimbot Heavy Sniper ] Wall Minimum Damage");
	save(g_Var.Ragebot[7].AutoMindDamage, "[ Aimbot Heavy Sniper ] Minimum Damage Auto");
	save(g_Var.Ragebot[7].Silent, "[ Aimbot Heavy Sniper ] Silent");
	save(g_Var.Ragebot[7].Autowall, "[ Aimbot Heavy Sniper ] Autowall");
	save(g_Var.Ragebot[7].PosAdjust, "[ Aimbot Heavy Sniper ] Position Adjustment");
	save(g_Var.Ragebot[7].Recoil, "[ Aimbot Heavy Sniper ] Remove Recoil");
	save(g_Var.Ragebot[7].Spread, "[ Aimbot Heavy Sniper ] Remove Spread");
	save(g_Var.Ragebot[7].Autozoom, "[ Aimbot Heavy Sniper ] Auto Scope");
	save(g_Var.Ragebot[7].Autostop, "[ Aimbot Heavy Sniper ] Auto Stop");
	save(g_Var.Ragebot[7].Limbs[0], "[ Aimbot Heavy Sniper ] Ignore Limbs In Move");
	save(g_Var.Ragebot[7].Limbs[1], "[ Aimbot Heavy Sniper ] Ignore Limbs In Air");
	save(g_Var.Ragebot[7].IgnoreHead[0], "[ Aimbot Heavy Sniper ] Ignore Head In Air");
	save(g_Var.Ragebot[7].IgnoreHead[1], "[ Aimbot Heavy Sniper ] Ignore Head No Update");
	save(g_Var.Ragebot[7].IgnoreHead[2], "[ Aimbot Heavy Sniper ] Ignore Head High Fake");
	save(g_Var.Ragebot[7].PriorityRecord[0], "[ Aimbot Heavy Sniper ] History On Shot");
	save(g_Var.Ragebot[7].PriorityRecord[1], "[ Aimbot Heavy Sniper ] History No Weapon");
	save(g_Var.Ragebot[7].PriorityRecord[2], "[ Aimbot Heavy Sniper ] History In Air");
	save(g_Var.Ragebot[7].PriorityRecord[3], "[ Aimbot Heavy Sniper ] History Running");
	save(g_Var.Ragebot[7].BodyAimHp, "[ Aimbot Heavy Sniper ] Prefer Body if Lower Than");
	save(g_Var.Ragebot[7].ForceBaim, "[ Aimbot Heavy Sniper ] Force BodyAim");

	save(g_Var.AntiAim.Enable, "[ AntiAim ] Enable");
	save(g_Var.AntiAim.Pitch, "[ AntiAim ] Pitch");

	save(g_Var.AntiAim.Move, "[ AntiAim ] Move Yaw");
	save(g_Var.AntiAim.MoveEnemyBased, "[ AntiAim ] Move Enemy Based");
	save(g_Var.AntiAim.MoveStyle, "[ AntiAim ] Move Yaw Style");
	save(g_Var.AntiAim.MoveValue, "[ AntiAim ] Move Yaw Value");
	save(g_Var.AntiAim.MoveSpeed, "[ AntiAim ] Move Yaw Speed");

	save(g_Var.AntiAim.Air, "[ AntiAim ] Air Yaw");
	save(g_Var.AntiAim.AirEnemyBased, "[ AntiAim ] Air Enemy Based");
	save(g_Var.AntiAim.AirStyle, "[ AntiAim ] Air Yaw Style");
	save(g_Var.AntiAim.AirValue, "[ AntiAim ] Air Yaw Value");
	save(g_Var.AntiAim.AirSpeed, "[ AntiAim ] Air Yaw Speed");

	save(g_Var.AntiAim.Stand, "[ AntiAim ] Stand Yaw");
	save(g_Var.AntiAim.StandEnemyBased, "[ AntiAim ] Stand Enemy Based");
	save(g_Var.AntiAim.StandStyle, "[ AntiAim ] Stand Yaw Style");
	save(g_Var.AntiAim.StandValue, "[ AntiAim ] Stand Yaw Value");
	save(g_Var.AntiAim.StandSpeed, "[ AntiAim ] Stand Yaw Speed");

	save(g_Var.AntiAim.Break, "[ AntiAim ] LBY Breaker");
	save(g_Var.AntiAim.BreakToggle, "[ AntiAim ] LBY Breaker Key");
	save(g_Var.AntiAim.BreakRad, "[ AntiAim ] LBY Breaker Distance");

	save(g_Var.AntiAim.LeftToggle, "[ AntiAim ] Direction Left");
	save(g_Var.AntiAim.RightToggle, "[ AntiAim ] Direction Right");

	save(g_Var.AntiAim.SideWaysTolerance, "[ AntiAim ] Direction Tolerance");

	save(g_Var.AntiAim.BackToggle, "[ AntiAim ] Direction Back");
	save(g_Var.AntiAim.FrontToggle, "[ AntiAim ] Direction Forward");

	save(g_Var.AntiAim.LockToggle, "[ AntiAim ] Direction Lock");

	save(g_Var.AntiAim.Points, "[ AntiAim ] Dynamic Points");

	save(g_Var.AntiAim.Tolerance, "[ AntiAim ] Yaw Tolerance");

	save(g_Var.AntiAim.Override, "[ AntiAim ] Override");

	save(g_Var.AntiAim.Resolve, "[ AntiAim ] Resolver");

	save(g_Var.AntiAim.Fakewalk, "[ AntiAim ] Fakewalk");

	save(g_Var.Fakelag.Enable, "[ Fakelag ] Enable");
	save(g_Var.Fakelag.Factor, "[ Fakelag ] Limit");
	save(g_Var.Fakelag.Style, "[ Fakelag ] Style");
	save(g_Var.Fakelag.Flags[0], "[ Fakelag ] In Stand");
	save(g_Var.Fakelag.Flags[1], "[ Fakelag ] In Air");
	save(g_Var.Fakelag.Flags[2], "[ Fakelag ] In Move");
	save(g_Var.Fakelag.Flags[3], "[ Fakelag ] On Land");
	save(g_Var.Fakelag.Flags[4], "[ Fakelag ] On Sight");

	/*
	MORE TO COME
	*/

	save(g_Var.Visuals.Player.Enable, "[ Visuals Player ] Enable");
	save(g_Var.Visuals.Player.Team, "[ Visuals Player ] Team");
	save(g_Var.Visuals.Player.Box, "[ Visuals Player ] Box");
	save(g_Var.Visuals.Player.Name, "[ Visuals Player ] Name");
	save(g_Var.Visuals.Player.Health, "[ Visuals Player ] Health");
	save(g_Var.Visuals.Player.Weapon, "[ Visuals Player ] Weapon");
	save(g_Var.Visuals.Player.KevHelm, "[ Visuals Player ] Armor");
	save(g_Var.Visuals.Player.AmmoBar, "[ Visuals Player ] Ammo");
	save(g_Var.Visuals.Player.Surrounding, "[ Visuals Player ] Surrounding");
	save(g_Var.Visuals.Player.Radar, "[ Visuals Player ] Radar");
	save(g_Var.Visuals.Player.Spectators, "[ Visuals Player ] Spectators");
	save(g_Var.Visuals.Player.LowerBody, "[ Visuals Player ] LowerBody Bar");
	save(g_Var.Visuals.Player.Bone, "[ Visuals Player ] Bone");
	save(g_Var.Visuals.Glow.Player, "[ Visuals Player ] Glow");

	save(g_Var.Visuals.Local.Enable, "[ Visuals Local ] Enable");
	save(g_Var.Visuals.Local.Chams, "[ Visuals Local ] Chams");
	save(g_Var.Visuals.Local.Glow, "[ Visuals Local ] Glow");
	save(g_Var.Visuals.Local.Transparency, "[ Visuals Local ] Transparency");
	save(g_Var.Visuals.Local.FakeChams, "[ Visuals Local ] Fake Chams");

	save(g_Var.Visuals.Local.Draw[0], "[ Visuals Local ] Flag Name");
	save(g_Var.Visuals.Local.Draw[1], "[ Visuals Local ] Flag Health");
	save(g_Var.Visuals.Local.Draw[2], "[ Visuals Local ] Flag Border");
	save(g_Var.Visuals.Local.Draw[3], "[ Visuals Local ] Flag Weapon");
	save(g_Var.Visuals.Local.Draw[4], "[ Visuals Local ] Flag Ammo");

	save(g_Var.Visuals.Chams.Enable, "[ Visuals Chams ] Enable");
	save(g_Var.Visuals.Chams.Players, "[ Visuals Chams ] Player");
	save(g_Var.Visuals.Chams.Team, "[ Visuals Chams ] Player Team");
	save(g_Var.Visuals.Chams.TeamTransparency, "[ Visuals Chams ] Player Team Transparency");
	save(g_Var.Visuals.Chams.DrawOriginal, "[ Visuals Chams ] Player Original");
	save(g_Var.Visuals.Chams.Transparency, "[ Visuals Chams ] Player Transparency");
	save(g_Var.Visuals.Chams.Weapons, "[ Visuals Chams ] Weapons");
	save(g_Var.Visuals.Chams.Hands, "[ Visuals Chams ] Hands");
	save(g_Var.Visuals.Chams.HandsTransparency, "[ Visuals Chams ] Hands Transparency");
	save(g_Var.Visuals.Chams.View, "[ Visuals Chams ] View Weapons");
	save(g_Var.Visuals.Chams.BacktrackMaterial, "[ Visuals Chams ] Backtrack Chams Material");
	save(g_Var.Visuals.Chams.BacktrackAlpha, "[ Visuals Chams ] Backtrack Chams Alpha");

	save(g_Var.Visuals.World.Crosshair[0], "[ Visuals World ] Crosshair Spread");
	save(g_Var.Visuals.World.Crosshair[1], "[ Visuals World ] Crosshair Recoil");
	save(g_Var.Visuals.World.Crosshair[2], "[ Visuals World ] Crosshair Autowall");
	save(g_Var.Visuals.World.Crosshair[3], "[ Visuals World ] Crosshair Sniper");

	save(g_Var.Visuals.World.Removals[0], "[ Visuals World ] Removals Flash");
	save(g_Var.Visuals.World.Removals[1], "[ Visuals World ] Removals Smoke");
	save(g_Var.Visuals.World.Removals[2], "[ Visuals World ] Removals Scope");
	save(g_Var.Visuals.World.Removals[3], "[ Visuals World ] Removals Recoil");
	save(g_Var.Visuals.World.Removals[4], "[ Visuals World ] Removals First Zoom");
	save(g_Var.Visuals.World.Removals[5], "[ Visuals World ] Removals PostProcessing");

	save(g_Var.Visuals.World.Grenade[0], "[ Visuals World ] Grenade Text");
	save(g_Var.Visuals.World.Grenade[1], "[ Visuals World ] Grenade Icon");
	save(g_Var.Visuals.World.Grenade[2], "[ Visuals World ] Grenade Prediction");
	save(g_Var.Visuals.World.ViewModel, "[ Visuals World ] View Model");
	save(g_Var.Visuals.World.ViewModelOffset[0], "[ Visuals World ] View Model Offset X");
	save(g_Var.Visuals.World.ViewModelOffset[1], "[ Visuals World ] View Model Offset Y");
	save(g_Var.Visuals.World.ViewModelOffset[2], "[ Visuals World ] View Model Offset Z");
	save(g_Var.Visuals.World.Fieldofview, "[ Visuals World ] Field of View");
	save(g_Var.Visuals.World.Hitmarker, "[ Visuals World ] Hitmarker");
	save(g_Var.Visuals.World.BulletTracer[0], "[ Visuals World ] Bullet Tracer");
	save(g_Var.Visuals.World.BulletTracer[1], "[ Visuals World ] Bullet Impacts");
	save(g_Var.Visuals.World.BulletTracer[2], "[ Visuals World ] Bullet Hits");
	save(g_Var.Visuals.World.BulletTracer[3], "[ Visuals World ] Bullet Hitbox");
	save(g_Var.Visuals.World.PropsAlpha, "[ Visuals World ] Proph Alpha");
	save(g_Var.Visuals.World.Nightmode, "[ Visuals World ] Wall Dimness");
	save(g_Var.Visuals.World.Fullbright, "[ Visuals World ] Fullbright");
	save(g_Var.Visuals.World.Ragdoll, "[ Visuals World ] Ragdoll");
	/*
	MORE TO COME
	*/

	save(g_Var.Misc.AutoJump, "[ Movement ] Auto Jump");
	save(g_Var.Misc.AutoStrafe, "[ Movement ] Auto Strafe");
	save(g_Var.Misc.Slide, "[ Movement ] Slide");
	save(g_Var.Misc.CrouchJump, "[ Movement ] Crouch Jump");
	save(g_Var.Misc.FakeDuck, "[ Movement ] Fake Duck");

	save(g_Var.Misc.Thirdperson, "[ Misc ] Thirdperson");
	save(g_Var.Misc.ThirdpersonDist, "[ Misc ] Thirdperson Distance");
	save(g_Var.Misc.ThirdpersonToggleKey, "[ Misc ] Thirdperson Key");

	save(g_Var.Misc.ZeusBot[0], "[ Misc ] Auto Zeus");
	save(g_Var.Misc.ZeusBot[1], "[ Misc ] Auto Zeus Range");
	save(g_Var.Misc.KnifeBot[0], "[ Misc ] Knife Bot");
	save(g_Var.Misc.KnifeBot[1], "[ Misc ] Knife Bot Range");

	save(g_Var.Misc.DebugResolver, "[ Misc ] Debug Resolver");
	save(g_Var.Misc.DebugMultiPoint, "[ Misc ] Debug Multipoints");

	save(g_Var.Misc.ExtendKillfeed, "[ Misc ] Extended Killfeed");

	save(g_Var.Misc.Insults, "[ Misc ] Flame Enemy");
	save(g_Var.Misc.Clantag, "[ Misc ] ClanTag");
	//	save( g_Var.Misc.AnimFix, "[ Misc ] AnimFix");

	save(g_Var.Misc.EventLog[0], "[ Misc ] Damage Log");
	save(g_Var.Misc.EventLog[1], "[ Misc ] Buy Log");
	save(g_Var.Misc.EventLog[2], "[ Misc ] Bomb Log");

	save(g_Var.Misc.Indicator[0], "[ Misc ] AA Indicator");
	save(g_Var.Misc.Indicator[1], "[ Misc ] LBY Indicator");
	save(g_Var.Misc.Indicator[2], "[ Misc ] Lag Indicator");
	save(g_Var.Misc.Indicator[3], "[ Misc ] Bomb Indicator");
	save(g_Var.Misc.Indicator[4], "[ Misc ] Angle Lines");
	save(g_Var.Misc.Indicator[5], "[ Misc ] LC Indicator");

	save(g_Var.Misc.BuyBot.Primary, "[ BuyBot ] Primary Weapon");
	save(g_Var.Misc.BuyBot.Secondary, "[ BuyBot ] Secondary Weapon");
	save(g_Var.Misc.BuyBot.Gear[0], "[ BuyBot ] Kevlar / Helm");
	save(g_Var.Misc.BuyBot.Gear[1], "[ BuyBot ] Defuser");
	save(g_Var.Misc.BuyBot.Gear[2], "[ BuyBot ] Taser");
	save(g_Var.Misc.BuyBot.Grenades[0], "[ BuyBot ] Grenade");
	save(g_Var.Misc.BuyBot.Grenades[1], "[ BuyBot ] Smoke");
	save(g_Var.Misc.BuyBot.Grenades[2], "[ BuyBot ] Flash");
	save(g_Var.Misc.BuyBot.Grenades[3], "[ BuyBot ] Molotov");
	save(g_Var.Misc.BuyBot.Grenades[4], "[ BuyBot ] Decoy");

	save_array(g_Var.Color.Menu, "[ Color ] Menu");

	save_array(g_Var.Color.Name, "[ Color ] Visuals Name");

	save_array(g_Var.Color.Player, "[ Color ] Visuals Box");
	save_array(g_Var.Color.PlayerVisible, "[ Color ] Visuals Box Visible");

	save_array(g_Var.Color.Team, "[ Color ] Visuals Team Box");
	save_array(g_Var.Color.TeamVisible, "[ Color ] Visuals Team Box Visible");

	save_array(g_Var.Color.Weapon, "[ Color ] Visuals Weapon");
	save_array(g_Var.Color.Health, "[ Color ] Visuals Health");
	save_array(g_Var.Color.Kevlar, "[ Color ] Visuals Armor");
	save_array(g_Var.Color.AmmoBar, "[ Color ] Visuals Ammo");
	save_array(g_Var.Color.Surrounding, "[ Color ] Visuals Surrounding");
	save_array(g_Var.Color.LowerBodyBar, "[ Color ] Visuals LowerBody");

	save_array(g_Var.Color.Chams, "[ Color ] Chams");
	save_array(g_Var.Color.BacktrackChams, "[ Color ] BacktrackChams");
	save_array(g_Var.Color.LocalChams, "[ Color ] Local Chams");
	save_array(g_Var.Color.LocalFakeChams, "[ Color ] Fake Chams");
	save_array(g_Var.Color.LocalGlow, "[ Color ] Local Glow");
	save_array(g_Var.Color.ChamsVisible, "[ Color ] Chams Visible");
	save_array(g_Var.Color.TeamChams, "[ Color ] Chams Team");
	save_array(g_Var.Color.TeamChamsVisible, "[ Color ] Chams Team Visible");
	save_array(g_Var.Color.HandChams, "[ Color ] Chams Hand");
	save_array(g_Var.Color.HandChamsVisible, "[ Color ] Chams Hand Visible");
	save_array(g_Var.Color.WeaponChams, "[ Color ] Chams Weapon");
	save_array(g_Var.Color.WeaponChamsVisible, "[ Color ] Chams Weapon Visible");
	save_array(g_Var.Color.ViewChams, "[ Color ] Chams View Weapon");

	save_array(g_Var.Color.Glow, "[ Color ] Glow");
	save_array(g_Var.Color.GlowVisible, "[ Color ] Glow Visible");

	save_array(g_Var.Color.BulletTracer, "[ Color ] Bullet Tracer");

	save(g_Skins.Knife, "[ Skin ] Knife Model");
	save(g_Skins.Glove, "[ Skin ] Glove Model");
	save(g_Skins.GlovePaint, "[ Skin ] Glove PaintKit");

	output_file << std::setw(4) << preset << std::endl;
	output_file.close();

	return true;
}

const bool CConfigFile::LoadSkinConfig()
{
	std::ifstream input_file = std::ifstream("c:/kidua/skins.json");

	if (!input_file.good())
		return false;

	try
	{
		presetskin << input_file;
	}
	catch (...)
	{
		input_file.close();
		return false;
	}

	assign_skinarray(g_Skins.Enable, "[ Skin ] Change PaintKit");
	assign_skinarray(g_Skins.PaintKit, "[ Skin ] PaintKit");

	input_file.close();

	return true;
}

const bool CConfigFile::SaveSkinConfig()
{
	std::ofstream output_file = std::ofstream("c:/kidua/skins.json");

	if (!output_file.good())
		return false;

	save_skinarray(g_Skins.Enable, "[ Skin ] Change PaintKit");
	save_skinarray(g_Skins.PaintKit, "[ Skin ] PaintKit");

	output_file << std::setw(4) << presetskin << std::endl;
	output_file.close();

	return true;
}