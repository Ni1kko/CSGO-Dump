#include "structs.hpp"
#include <fstream>
#include <iostream>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <iomanip>

#define TIME_TO_TICKS( dt )	( ( int )( 0.5f + ( float )( dt ) / g_csgo.m_globals( )->m_interval_per_tick ) )
#define TICKS_TO_TIME( t ) ( g_csgo.m_globals( )->m_interval_per_tick *( t ) )

bool entity_t::is_player() {
	return (GetClientClass()->m_ClassID == CCSPlayer);
}

bool entity_t::is_weapon() {
	return (GetClientClass()->m_ClassID == CBaseCombatWeapon);
}

bool entity_t::is_dropped_weapon() {
	if (!call_virtual<bool(__thiscall*)(entity_t*)>(this, 160)(this))
		return false;

	auto * e = static_cast<weapon_t *>(this);

	char * strings[3] = { "CWeapon", "CAK47", "CDEagle" };

	for (int i = 0; i < 3; i++) {
		if (!strstr(e->GetClientClass()->m_pNetworkName, strings[i]))
			continue;

		if (!m_hOwnerEntity().IsValid() && m_vecOrigin() != vec3_t(0, 0, 0))
			return true;
	}

	return false;
}

bool entity_t::is_plantedc4() {
	return GetClientClass()->m_ClassID == CPlantedC4;
}

bool entity_t::is_defusekit() {
	return GetClientClass()->m_ClassID == CBaseAnimating;
}

csweaponinfo_t * weapon_t::get_csweapon_info() {
	if (!this || this == NULL)
		return NULL;

	using Fn = csweaponinfo_t * (__thiscall*)(void*);

	static auto fn = reinterpret_cast<Fn>(util::pattern_scan("client_panorama.dll", "55 8B EC 81 EC ? ? ? ? 53 8B D9 56 57 8D 8B"));
	return fn(this);
}

float weapon_t::get_innacuracy() {
	return call_virtual<float(__thiscall*)(void*)>(this, 471)(this);
}

float weapon_t::get_spread() {
	return call_virtual<float(__thiscall*)(void*)>(this, 440)(this);
}

void weapon_t::update_accuracy_penality() {
	call_virtual<void(__thiscall*)(void*)>(this, 472)(this);
}

bool weapon_t::is_empty() {
	return m_iClip1() <= 0;
}

bool weapon_t::can_fire() {
	if (is_empty()) {
		return false;
	}

	if (!g_ctx.m_local) {
		return false;
	}

	return (m_flNextPrimaryAttack() < g_csgo.m_globals()->m_curtime) && (g_ctx.m_local->m_flNextAttack() < g_csgo.m_globals()->m_curtime);
}

bool weapon_t::is_rifle() {
	switch (get_csweapon_info()->type) {
	case WEAPONTYPE_RIFLE:
		return true;
	case WEAPONTYPE_SUBMACHINEGUN:
		return true;
	case WEAPONTYPE_SHOTGUN:
		return true;
	case WEAPONTYPE_MACHINEGUN:
		return true;
	default:
		return false;
	}
}

bool weapon_t::is_smg() {
	int weapon_id = m_iItemDefinitionIndex();

	return weapon_id == WEAPON_MAC10 || weapon_id == WEAPON_MP7 || weapon_id == WEAPON_MP9 || weapon_id == WEAPON_P90 ||
		weapon_id == WEAPON_BIZON || weapon_id == WEAPON_UMP45;
}

bool weapon_t::is_shotgun() {
	int weapon_id = m_iItemDefinitionIndex();

	return weapon_id == WEAPON_XM1014 || weapon_id == WEAPON_NOVA || weapon_id == WEAPON_SAWEDOFF || weapon_id == WEAPON_MAG7;
}

bool weapon_t::is_pistol() {
	int weapon_id = m_iItemDefinitionIndex();

	return weapon_id == WEAPON_DEAGLE || weapon_id == WEAPON_ELITE || weapon_id == WEAPON_FIVESEVEN || weapon_id == WEAPON_P250 ||
		weapon_id == WEAPON_GLOCK || weapon_id == WEAPON_HKP2000 || weapon_id == WEAPON_CZ75A || weapon_id == WEAPON_USP_SILENCER || weapon_id == WEAPON_TEC9 || weapon_id == WEAPON_REVOLVER;
}

bool weapon_t::is_sniper() {
	int weapon_id = m_iItemDefinitionIndex();

	return weapon_id == WEAPON_AWP || weapon_id == WEAPON_SCAR20 || weapon_id == WEAPON_G3SG1 || weapon_id == WEAPON_SSG08;
}

bool weapon_t::is_grenade()
{
	int weapon_id = m_iItemDefinitionIndex();

	return weapon_id == WEAPON_HEGRENADE || weapon_id == WEAPON_SMOKEGRENADE || weapon_id == WEAPON_INCGRENADE || weapon_id == WEAPON_TAGRENADE || weapon_id == WEAPON_FRAG_GRENADE;

}
bool weapon_t::is_knife() {
	int idx = m_iItemDefinitionIndex();

	return (idx == WEAPON_KNIFE || idx == WEAPON_KNIFE_BAYONET || idx == WEAPON_KNIFE_BUTTERFLY || idx == WEAPON_KNIFE_FALCHION
		|| idx == WEAPON_KNIFE_FLIP || idx == WEAPON_KNIFE_GUT || idx == WEAPON_KNIFE_KARAMBIT || idx == WEAPON_KNIFE_M9_BAYONET || idx == WEAPON_KNIFE_PUSH
		|| idx == WEAPON_KNIFE_SURVIVAL_BOWIE || idx == WEAPON_KNIFE_T || idx == WEAPON_KNIFE_TACTICAL);
}
bool weapon_t::is_non_aim() {
	int idx = m_iItemDefinitionIndex();

	return (idx == WEAPON_C4 || idx == WEAPON_KNIFE || idx == WEAPON_KNIFE_BAYONET || idx == WEAPON_KNIFE_BUTTERFLY || idx == WEAPON_KNIFE_FALCHION
		|| idx == WEAPON_KNIFE_FLIP || idx == WEAPON_KNIFE_GUT || idx == WEAPON_KNIFE_KARAMBIT || idx == WEAPON_KNIFE_M9_BAYONET || idx == WEAPON_KNIFE_PUSH
		|| idx == WEAPON_KNIFE_SURVIVAL_BOWIE || idx == WEAPON_KNIFE_T || idx == WEAPON_KNIFE_TACTICAL || idx == WEAPON_FLASHBANG || idx == WEAPON_HEGRENADE
		|| idx == WEAPON_SMOKEGRENADE || idx == WEAPON_MOLOTOV || idx == WEAPON_DECOY || idx == WEAPON_INCGRENADE);
}

char * weapon_t::get_icon() {
	switch (m_iItemDefinitionIndex()) {
	case WEAPON_KNIFE_BAYONET:
		return "1";
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
		return "7";
	case WEAPON_KNIFE_BUTTERFLY:
		return "8";
	case WEAPON_KNIFE:
		return "KNIFE";
	case WEAPON_KNIFE_FALCHION:
		return "0";
	case WEAPON_KNIFE_FLIP:
		return "2";
	case WEAPON_KNIFE_GUT:
		return "3";
	case WEAPON_KNIFE_KARAMBIT:
		return "4";
	case WEAPON_KNIFE_M9_BAYONET:
		return "5";
	case WEAPON_KNIFE_T:
		return "KNIFE";
	case WEAPON_KNIFE_TACTICAL:
		return "6";
	case WEAPON_KNIFE_PUSH:
		return "]";
	case WEAPON_DEAGLE:
		return "A";
	case WEAPON_ELITE:
		return "B";
	case WEAPON_FIVESEVEN:
		return "C";
	case WEAPON_GLOCK:
		return "D";
	case WEAPON_HKP2000:
		return "E";
	case WEAPON_P250:
		return "F";
	case WEAPON_USP_SILENCER:
		return "G";
	case WEAPON_TEC9:
		return "H";
	case WEAPON_REVOLVER:
		return "J";
	case WEAPON_MAC10:
		return "K";
	case WEAPON_UMP45:
		return "L";
	case WEAPON_BIZON:
		return "M";
	case WEAPON_MP7:
		return "N";
	case WEAPON_MP9:
		return "O";
	case WEAPON_P90:
		return "P";
	case WEAPON_GALILAR:
		return "Q";
	case WEAPON_FAMAS:
		return "R";
	case WEAPON_M4A1_SILENCER:
		return "S";
	case WEAPON_M4A1:
		return "T";
	case WEAPON_AUG:
		return "U";
	case WEAPON_SG553:
		return "V";
	case WEAPON_AK47:
		return "W";
	case WEAPON_G3SG1:
		return "X";
	case WEAPON_SCAR20:
		return "Y";
	case WEAPON_AWP:
		return "Z";
	case WEAPON_SSG08:
		return "a";
	case WEAPON_XM1014:
		return "b";
	case WEAPON_SAWEDOFF:
		return "c";
	case WEAPON_MAG7:
		return "d";
	case WEAPON_NOVA:
		return "e";
	case WEAPON_NEGEV:
		return "f";
	case WEAPON_M249:
		return "g";
	case WEAPON_TASER:
		return "h";
	case WEAPON_FLASHBANG:
		return "i";
	case WEAPON_HEGRENADE:
		return "j";
	case WEAPON_SMOKEGRENADE:
		return "k";
	case WEAPON_MOLOTOV:
		return "l";
	case WEAPON_DECOY:
		return "m";
	case WEAPON_INCGRENADE:
		return "n";
	case WEAPON_C4:
		return "o";
	case WEAPON_CZ75A:
		return "I";
	default:
		return "  ";
	}
}

char * weapon_t::get_name() {
	switch (m_iItemDefinitionIndex()) {
	case WEAPON_KNIFE_BAYONET:
		return "bayonet";
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
		return "bowie";
	case WEAPON_KNIFE_BUTTERFLY:
		return "butterfly";
	case WEAPON_KNIFE:
		return "knife";
	case WEAPON_KNIFE_FALCHION:
		return "falchion";
	case WEAPON_KNIFE_FLIP:
		return "flip";
	case WEAPON_KNIFE_GUT:
		return "gut";
	case WEAPON_KNIFE_KARAMBIT:
		return "karambit";
	case WEAPON_KNIFE_M9_BAYONET:
		return "m9bayonet";
	case WEAPON_KNIFE_T:
		return "knife";
	case WEAPON_KNIFE_TACTICAL:
		return "huntsman";
	case WEAPON_KNIFE_PUSH:
		return "daggers";
	case WEAPON_DEAGLE:
		return "deserteagle";
	case WEAPON_ELITE:
		return "berretas";
	case WEAPON_FIVESEVEN:
		return "fiveseven";
	case WEAPON_GLOCK:
		return "glock";
	case WEAPON_HKP2000:
		return "p2000";
	case WEAPON_P250:
		return "p250";
	case WEAPON_USP_SILENCER:
		return "usp-s";
	case WEAPON_TEC9:
		return "tec9";
	case WEAPON_REVOLVER:
		return "revolver";
	case WEAPON_MAC10:
		return "mac10";
	case WEAPON_UMP45:
		return "ump45";
	case WEAPON_BIZON:
		return "bizon";
	case WEAPON_MP7:
		return "mp7";
	case WEAPON_MP5SD:
		return "mp5";
	case WEAPON_MP9:
		return "mp9";
	case WEAPON_P90:
		return "p90";
	case WEAPON_GALILAR:
		return "galil";
	case WEAPON_FAMAS:
		return "famas";
	case WEAPON_M4A1_SILENCER:
		return "m4a1-s";
	case WEAPON_M4A1:
		return "m4a1";
	case WEAPON_AUG:
		return "aug";
	case WEAPON_SG553:
		return "sg556";
	case WEAPON_AK47:
		return "ak47";
	case WEAPON_G3SG1:
		return "g3sg1";
	case WEAPON_SCAR20:
		return "scar20";
	case WEAPON_AWP:
		return "awp";
	case WEAPON_SSG08:
		return "scout";
	case WEAPON_XM1014:
		return "xm1014";
	case WEAPON_SAWEDOFF:
		return "sawedoff";
	case WEAPON_MAG7:
		return "mag7";
	case WEAPON_NOVA:
		return "nova";
	case WEAPON_NEGEV:
		return "negev";
	case WEAPON_M249:
		return "m249";
	case WEAPON_TASER:
		return "zeus";
	case WEAPON_FLASHBANG:
		return "flashbang";
	case WEAPON_HEGRENADE:
		return "grenade";
	case WEAPON_SMOKEGRENADE:
		return "smoke";
	case WEAPON_MOLOTOV:
		return "molotov";
	case WEAPON_DECOY:
		return "decoy";
	case WEAPON_INCGRENADE:
		return "incendiary";
	case WEAPON_C4:
		return "bomb";
	case WEAPON_CZ75A:
		return "cz75a";
	default:
		return "Unknown";
	}
}

float * player_t::m_flPoseParameter() {
	static int offset = netvars::get().get_offset("DT_BaseAnimating", "m_flPoseParameter");
	return reinterpret_cast<float *>(uintptr_t(this) + offset);
}

vec3_t player_t::get_eye_pos() {
	return m_vecOrigin() + m_vecViewOffset();
}

float player_t::get_max_desync_delta()
{
	/*enum {
		SIDE_NONE,
		SIDE_LEFT,
		SIDE_RIGHT
	};

	int side = SIDE_NONE;

	auto animstate = this->get_animation_state();
	Vector yawfeetdelta = g_ctx.m_local->GetAbsAngles();
	yawfeetdelta.y = animstate->m_flGoalFeetYaw - animstate->m_flEyeYaw;
	math::normalize_vector(yawfeetdelta);

	Vector an = g_ctx.m_local->GetAbsAngles();
	an.y = animstate->m_flCurrentFeetYaw;
	g_ctx.m_local->SetAbsAngles(an);
	float flYaw = an.y;

	float v47 = animstate->m_flCurrentFeetYaw;
	float pes = 360; float v146 = 360;
	if (v47 >= -360.0)
	{
		pes = fminf(v47, 360);
		v146 = pes;
	}

	float v155 = fmod(yawfeetdelta.y, 360.0);
	if (flYaw <= v146)
	{
		if (v155 <= -180.0)
			yawfeetdelta.y = v155 + 360, 0;
	}
	else if (v155 >= 180.0)
	{
		yawfeetdelta.y = v155 - 360, 0;
	}


	float rate = 180;
	float duckammount = *(float *)(animstate + 0xA4);
	float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));
	float speedfactor;
	if (speedfraction >= 0.0)
		speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));
	else
		speedfactor = 0.0f;

	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
	float unk2 = unk1 + 1.f;
	if (duckammount > 0) {

		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));

	}


	int maxrot = animstate->m_flAbsRotation()  * unk2;
	int invrot = animstate->m_flPoseParameter()  * unk2;

	DWORDLONG(xmmword_10BF8550) = NULL;

	if (yawfeetdelta.y <= maxrot)
	{
		if (invrot > yawfeetdelta.y)
			*(float *)(animstate + 0x80) = float(DWORD(invrot) & xmmword_10BF8550) + flYaw;
	}
	else
	{
		*(float *)(animstate + 0x80) = flYaw - float(DWORD(maxrot) & xmmword_10BF8550);
	}
	Vector gfeet2 = g_ctx.m_local->GetAbsAngles();
	gfeet2.y = fmod(*(float *)(animstate + 0x80), g_cfg.antiaim.desync_delta_swap_st);

	Vector gfeet = g_ctx.m_local->GetAbsAngles();
	gfeet.y = fmod(*(float *)(animstate + 0x80), g_cfg.antiaim.desync_delta_swap_st);

	math::clamp_angles(gfeet);

	if (gfeet2.y > g_cfg.antiaim.desync_delta_swap_mv)
	{
		gfeet.y = gfeet2.y - g_cfg.antiaim.desync_delta_swap_st;
	}

	if (gfeet.y < g_cfg.antiaim.desync_delta_swap_mv)
	{
		gfeet.y = gfeet.y + g_cfg.antiaim.desync_delta_swap_st;
	}

	float feetg = gfeet.y;

	return feetg;*/
	enum {
		SIDE_NONE,
		SIDE_LEFT,
		SIDE_RIGHT
	};

	int side = SIDE_NONE;

	auto animstate = this->get_animation_state();
	Vector yawfeetdelta = g_ctx.m_local->GetAbsAngles();
	yawfeetdelta.y = animstate->m_flGoalFeetYaw - animstate->m_flEyeYaw;
	math::normalize_vector(yawfeetdelta);

	Vector an = g_ctx.m_local->GetAbsAngles();
	an.y = animstate->m_flCurrentFeetYaw;
	g_ctx.m_local->SetAbsAngles(an);
	float flYaw = an.y;

	float v47 = animstate->m_flCurrentFeetYaw;
	float pes = 360; float v146 = 360;
	if (v47 >= -360.0)
	{
		pes = fminf(v47, 360);
		v146 = pes;
	}

	float v155 = fmod(yawfeetdelta.y, 360.0);
	if (flYaw <= v146)
	{
		if (v155 <= -180.0)
			yawfeetdelta.y = v155 + 360, 0;
	}
	else if (v155 >= 180.0)
	{
		yawfeetdelta.y = v155 - 360, 0;
	}


	float rate = 180;
	float duckammount = *(float *)(animstate + 0xA4);
	float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));
	float speedfactor;
	if (speedfraction >= 0.0)
		speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));
	else
		speedfactor = 0.0f;

	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
	float unk2 = unk1 + 1.f;
	if (duckammount > 0) {

		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));

	}


	int maxrot = animstate->m_flAbsRotation()  * unk2;
	int invrot = animstate->m_flPoseParameter()  * unk2;

	DWORDLONG(xmmword_10BF8550) = NULL;

	if (yawfeetdelta.y <= maxrot)
	{
		if (invrot > yawfeetdelta.y)
			*(float *)(animstate + 0x80) = float(DWORD(invrot) & xmmword_10BF8550) + flYaw;
	}
	else
	{
		*(float *)(animstate + 0x80) = flYaw - float(DWORD(maxrot) & xmmword_10BF8550);
	}
	Vector gfeet2 = g_ctx.m_local->GetAbsAngles();
	gfeet2.y = fmod(*(float *)(animstate + 0x80), g_cfg.antiaim.desync_delta_swap_st);

	Vector gfeet = g_ctx.m_local->GetAbsAngles();
	gfeet.y = fmod(*(float *)(animstate + 0x80), g_cfg.antiaim.desync_delta_swap_st);

	math::clamp_angles(gfeet);

	if (gfeet2.y > g_cfg.antiaim.desync_delta_swap_mv)
	{
		gfeet.y = gfeet2.y - g_cfg.antiaim.desync_delta_swap_st;
	}

	if (gfeet.y < g_cfg.antiaim.desync_delta_swap_mv)
	{
		gfeet.y = gfeet.y + g_cfg.antiaim.desync_delta_swap_st;
	}

	float feetg = gfeet.y;

	return feetg;

}

float player_t::get_max_desync_delta_resolver() 
{
	auto animstate = this->get_animation_state();

	float v49,
		v46,
		v51;

	if (animstate->m_flFeetSpeedForwardsOrSideWays >= 0.0)
		v46 = fminf(animstate->m_flFeetSpeedForwardsOrSideWays, 1.0);
	else
		v46 = 0.0;

	float v47 = (float)((animstate->m_flStopToFullRunningFraction * -0.30000001f) - 0.19999999f) * v46;
	v49 = v47 + 1.0;
	if (animstate->m_fDuckAmount > 0.0)
	{
		if (animstate->m_flFeetSpeedForwardsOrSideWays >= 0.0)
			v51 = fminf(animstate->m_flFeetSpeedForwardsOrSideWays, 1.0);
		else
			v51 = 0.0;
		float v52 = animstate->m_fDuckAmount * v51;
		v49 = v49 + (float)(v52 * (float)(0.5 - v49));
	}

	float v53 = *(float *)(animstate + 0x334) * v49;
	return v53;
}
bool player_t::is_alive()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive1()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive2()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive3()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive4()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive5()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive6()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive7()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive8()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive9()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive10()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive11()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive12()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive13()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive14()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive15()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive16()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive17()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive18()
{
	return(m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive19()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive20()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive21()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive22()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive23()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive24()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive25()
{
	return(m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive26()
{
	return (m_lifeState() = LIFE_ALIVE);
}
bool player_t::is_alive27()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive28()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive29()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive30()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive31()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive32()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive33()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive34()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive35()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive36()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive37()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive38()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive39()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive40()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive41()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive42()
{
	return (m_lifeState() == LIFE_ALIVE);
}
bool player_t::is_alive43()
{
	return (m_lifeState() == LIFE_ALIVE);
}

int	player_t::get_move_type()
{
	return *(int*)((uintptr_t)this + 0x25C);
}

vec3_t player_t::hitbox_position(int hitbox_id) {
	matrix3x4_t matrix[128];

	if (this->SetupBones(matrix, 128, 0x100, 0.f)) {
		studiohdr_t * hdr = g_csgo.m_modelinfo()->GetStudiomodel(this->GetModel());

		if (hdr) {
			mstudiobbox_t *hitbox = hdr->pHitboxSet(this->m_nHitboxSet())->pHitbox(hitbox_id);

			if (hitbox) {
				vec3_t
					min = vec3_t{ },
					max = vec3_t{ };

				math::vector_transform(hitbox->bbmin, matrix[hitbox->bone], min);
				math::vector_transform(hitbox->bbmax, matrix[hitbox->bone], max);

				return (min + max) / 2.0f;
			}
		}
	}

	return vec3_t{ };
}

vec3_t player_t::hitbox_position(int hitbox_id, matrix3x4_t matrix[128]) {
	studiohdr_t * hdr = g_csgo.m_modelinfo()->GetStudiomodel(this->GetModel());

	if (hdr) {
		mstudiobbox_t *hitbox = hdr->pHitboxSet(this->m_nHitboxSet())->pHitbox(hitbox_id);

		if (hitbox) {
			vec3_t
				min = vec3_t{ },
				max = vec3_t{ };

			math::vector_transform(hitbox->bbmin, matrix[hitbox->bone], min);
			math::vector_transform(hitbox->bbmax, matrix[hitbox->bone], max);

			return (min + max) / 2.0f;
		}
	}

	return vec3_t{ };
}

hitboxdata_t player_t::hitbox_data(int hitbox_id, matrix3x4_t matrix[128]) {
	hitboxdata_t output;

	studiohdr_t * hdr = g_csgo.m_modelinfo()->GetStudiomodel(this->GetModel());

	if (hdr) {
		mstudiobbox_t *hitbox = hdr->pHitboxSet(this->m_nHitboxSet())->pHitbox(hitbox_id);

		if (hitbox) {
			vec3_t
				min = vec3_t{ },
				max = vec3_t{ };

			math::vector_transform(hitbox->bbmin, matrix[hitbox->bone], min);
			math::vector_transform(hitbox->bbmax, matrix[hitbox->bone], max);

			output.position = (min + max) / 2.0f;
			output.radius = hitbox->m_flRadius;
		}
	}

	return output;
}

vec3_t player_t::bone_pos(int bone) {
	matrix3x4_t matrix[MAXSTUDIOBONES];

	if (SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, g_csgo.m_globals()->m_curtime)) {
		return Vector(matrix[bone][0][3], matrix[bone][1][3], matrix[bone][2][3]);
	}

	return vec3_t(0, 0, 0);
}

bool player_t::point_visible(player_t * m_player, vec3_t endpos) {
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = this;

	ray.Init(get_eye_pos(), endpos);
	g_csgo.m_trace()->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	return tr.hit_entity == m_player || tr.fraction > 0.97f;
}

bool player_t::point_visible(vec3_t endpos) {
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = this;

	ray.Init(get_eye_pos(), endpos);
	g_csgo.m_trace()->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	return tr.fraction > 0.97f;
}

CUserCmd*& player_t::m_pCurrentCommand() {
	static auto current_command = *(uint32_t*)(util::pattern_scan("client_panorama.dll", "89 BE ? ? ? ? E8 ? ? ? ? 85 FF") + 2);

	return *(CUserCmd **)((uintptr_t)this + current_command);
}

void player_t::invalidate_bone_cache() {
	static auto addr = (DWORD)util::pattern_scan("client_panorama.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");
	unsigned long g_iModelBoneCounter = **(unsigned long**)(addr + 10);
	*(unsigned int*)((DWORD)this + 0x2914) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
	*(unsigned int*)((DWORD)this + 0x2680) = (g_iModelBoneCounter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;
}

void player_t::set_abs_origin(const vec3_t & origin) {
	using Fn = void(__thiscall*)(void*, const vec3_t &);

	static auto fn = reinterpret_cast<Fn>(util::pattern_scan("client_panorama.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1"));
	return fn(this, origin);
}
void player_t::SetAbsAngles(Vector angles)
{
	using Fn = void(__thiscall*)(player_t*, const Vector& angles);
	static Fn AbsAngles = (Fn)(util::FindPattern("client_panorama.dll", (BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1\xE8", "xxxxxxxxxxxxxxx"));

	AbsAngles(this, angles);
}

void player_t::set_abs_angles(const vec3_t & angle) {
	using Fn = void(__thiscall*)(void*, const vec3_t &);

	static auto fn = reinterpret_cast<Fn>(util::pattern_scan("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8"));
	return fn(this, angle);
}

void player_t::set_render_angles(vec3_t angles) {
	*(vec3_t*)(uintptr_t(this) + 0x128) = angles;
}

bool player_t::get_bone_transform(matrix3x4_t * output, float time) {
	*(int*)(this + 0x0A28) = 0;
	*(int*)(this + 0x0A30) = g_csgo.m_globals()->m_framecount;

	return SetupBones(output, 128, 0x0100, time);
}
void player_t::SetLowerBodyYaw(float value) {
	static int m_flLowerBodyYawTarget = netvars::get().get_offset("CSPlayer", "m_flLowerBodyYawTarget");
	*reinterpret_cast<float*>(uintptr_t(this) + m_flLowerBodyYawTarget) = value;
}
void player_t::update_clientside_animation() {
	typedef void(__thiscall *Fn)(void*);
	call_virtual<Fn>(this, 219)(this);
}

VarMapping_t * player_t::var_mapping() {
	return reinterpret_cast<VarMapping_t*>((DWORD)this + 0x24);
}

vec3_t &player_t::m_angAbsRotation() {
	return *(vec3_t*)((DWORD)&m_angRotation() - 12);
}

float player_t::m_flSpawnTime() {
	return *(float*)((uintptr_t)this + 0xA290);
}

bool player_t::valid(bool check_team, bool check_dormant) {
	auto local_player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));

	if (!this) { return false; }
	if (IsDormant() && check_dormant) { return false; }
	if (!is_alive()) { return false; }
	if (!is_player()) { return false; }
	if (!g_ctx.m_local) { return false; }

	g_ctx.m_local = local_player;

	if (check_team) {
		if (local_player->m_iTeamNum() == m_iTeamNum()) {
			return false;
		}
	}
	return true;
}

int player_t::animlayer_count() {
	return *(int*)((DWORD)this + 0x298C);
}

AnimationLayer * player_t::get_animlayers() {
	return *(AnimationLayer**)((DWORD)this + 0x2980);//changed from 10608 to 0x2980
}
AnimationLayer* player_t::GetAnimOverlays()
{
	return *(AnimationLayer**)((DWORD)this + 0x2980);//changed from 10608 to 0x2980
}
int  player_t::NumOverlays()
{
	return 15;
}
bool player_t::IsKnifeorNade()
{
	if (!this)
		return false;
	if (!this->is_alive1())
		return false;

	weapon_t *pWeapon = (weapon_t*)this->m_hActiveWeapon();

	if (!pWeapon)
		return false;

	std::string WeaponName = pWeapon->get_name();

	if (WeaponName == "weapon_knife")
		return true;
	else if (WeaponName == "weapon_incgrenade")
		return true;
	else if (WeaponName == "weapon_decoy")
		return true;
	else if (WeaponName == "weapon_flashbang")
		return true;
	else if (WeaponName == "weapon_hegrenade")
		return true;
	else if (WeaponName == "weapon_smokegrenade")
		return true;
	else if (WeaponName == "weapon_molotov")
		return true;

	return false;
}

AnimationLayer & player_t::get_animlayer(int id) {//from 0x2970 to 0x2980
	return (*(AnimationLayer**)(uintptr_t(this) + 0x2980))[id];
}

float player_t::get_animtime() {
	return *reinterpret_cast<float*>((DWORD)this + 0x3C);
}

void player_t::set_animlayer(int id, AnimationLayer layer) {//from 0x2970 to 0x2980
	(*(AnimationLayer**)((DWORD)this + 0x2980))[id] = layer;
}

int player_t::sequence_activity(int sequence)
{
	auto hdr = g_csgo.m_modelinfo()->GetStudiomodel(this->GetModel());

	if (!hdr)
		return -1;

	static auto sequence_activity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(util::pattern_scan("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83"));

	return sequence_activity(this, hdr, sequence);
}

float network_latency() {
	INetChannelInfo *nci = g_csgo.m_engine()->GetNetChannelInfo();
	if (nci)
		return nci->GetAvgLatency(FLOW_INCOMING);

	return 0.0f;
}

int player_t::get_choked_ticks() {
	float flSimulationTime = this->m_flSimulationTime();
	float flSimDiff = g_csgo.m_globals()->m_curtime - flSimulationTime;
	float latency = network_latency();
	return TIME_TO_TICKS(max(0.0f, flSimDiff - latency));
}

c_baseplayeranimationstate * player_t::get_animation_state()
{
	return *reinterpret_cast<c_baseplayeranimationstate **>(uintptr_t(this) + 0x3900);
}

c_baseplayeranimationstate* player_t::AnimationState()
{
	return *(c_baseplayeranimationstate**)((DWORD)this + 0x3900);
}

void player_t::set_animation_state(c_baseplayeranimationstate * state) 
{
	*reinterpret_cast<c_baseplayeranimationstate **>(uintptr_t(this) + 0x3900) = state;
}

float player_t::get_animation_time() {
	return *(float *)((uintptr_t)this + 0x3C);
}

float player_t::m_flLerpTime() {
	return *(float *)((uintptr_t)this + 0x0BF4);
}

CBoneAccessor * player_t::m_BoneAccessor() {
	return reinterpret_cast<CBoneAccessor*>(uintptr_t(this) + 0x2694);
}

CThreadFastMutex * player_t::m_BoneSetupLock() {
	return reinterpret_cast<CThreadFastMutex *>(uintptr_t(this) + 0x26A4);
}
matrix3x4_t* player_t::dw_boneMatrix()
{
	return (matrix3x4_t*)((DWORD)this + 0x2698);
}
void player_t::SetFlags(int flag)
{
	*reinterpret_cast<int*>(uintptr_t(this) + 0x00000100) = flag;
}

void  player_t::setAbsOriginal(Vector origin) // 55 8B EC 83 E4 F8 51 53 56 57 8B F1
{
	using SetAbsAnglesFn = void(__thiscall*)(void*, const Vector &origin);
	static SetAbsAnglesFn SetAbsAngles;

	if (!SetAbsAngles)
		SetAbsAngles = (SetAbsAnglesFn)(util::FindPattern("client_panorama.dll", (BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1\xE8", "xxxxxxxxxxxxxxx"));

	SetAbsAngles(this, origin);
}
void player_t::setAbsThereIsAFlyInMyRoom(Vector aechse)
{
	using SetAbsAnglesFn = void(__thiscall*)(player_t*, const Vector &angles);
	static SetAbsAnglesFn SetAbsAngles;
	if (!SetAbsAngles)
		SetAbsAngles = (SetAbsAnglesFn)(util::FindPattern("client_panorama.dll", (BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1\xE8", "xxxxxxxxxxxxxxx"));

	SetAbsAngles(this, aechse);
}
vec3_t & player_t::abs_origin() {
	using Fn = vec3_t & (__thiscall *)(void*);
	return call_virtual< Fn >(this, 10)(this);
}

vec3_t & player_t::abs_angles() {
	using Fn = vec3_t & (__thiscall *)(void *);
	return call_virtual< Fn >(this, 11)(this);
}

float & player_t::m_surfaceFriction() {
	static unsigned int _m_surfaceFriction = util::find_in_datamap(GetPredDescMap(), "m_surfaceFriction");
	return *(float *)(uintptr_t(this) + _m_surfaceFriction);
}

vec3_t & player_t::m_vecBaseVelocity() {
	static unsigned int _m_vecBaseVelocity = util::find_in_datamap(GetPredDescMap(), "m_vecBaseVelocity");
	return *(vec3_t*)(uintptr_t(this) + _m_vecBaseVelocity);
}

float & player_t::m_flMaxspeed() {
	static unsigned int _m_flMaxspeed = util::find_in_datamap(GetPredDescMap(), "m_flMaxspeed");
	return *(float *)(uintptr_t(this) + _m_flMaxspeed);
}