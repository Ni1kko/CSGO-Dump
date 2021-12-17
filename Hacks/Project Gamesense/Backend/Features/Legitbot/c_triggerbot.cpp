#include "c_triggerbot.h"

void CTriggerBot::OnMove(CUserCmd* usercmd)
{
	std::vector<int> hitgroups;
	if (!c_config::get()->b["legit_trigger_enabled"] || !c_keyhandler::get()->auto_check("legit_trigger_key"))
	{
		is_delayed = false;
		shot_delay = false;
		goto ret;
	}

	if (c_menu::get()->is_menu_opened())
		goto ret;

	auto pWeapon = G::LocalPlayer->GetActiveWeapon();
	if (!pWeapon)
		goto ret;

	auto pWeaponData = pWeapon->get_csweapon_info();
	if (!pWeaponData)
		goto ret;

	if ((pWeapon->GetItemDefenitionIndex() == WEAPON_AWP || pWeapon->GetItemDefenitionIndex() == WEAPON_SSG08) && !G::LocalPlayer->IsScoped())
		goto ret;

	if (pWeapon->GetItemDefenitionIndex() != WEAPON_TASER && c_config::get()->b["legit_trigger_head"])
		hitgroups.push_back(HITGROUP_HEAD);
	if (c_config::get()->b["legit_trigger_chest"])
		hitgroups.push_back(HITGROUP_CHEST);
	if (c_config::get()->b["legit_trigger_stomach"])
		hitgroups.push_back(HITGROUP_STOMACH);
	Vector rem, forward, src = G::LocalPlayer->GetEyePosition();

	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip1 = G::LocalPlayer;

	Vector viewangles = G::UserCmd->viewangles + G::LocalPlayer->GetPunchAngles() * Interfaces::Var->FindVar("weapon_recoil_scale")->GetFloat();

	Cheat::Utilities->Math_AngleVectors(viewangles, forward);

	forward *= pWeaponData->range;

	rem = src + forward;

	ray.Init(src, rem);

	UINT mask = MASK_SHOT | CONTENTS_GRATE;

	Interfaces::Trace->TraceRay(ray, mask, &filter, &tr);

	if (tr.fraction == 1.f)
		goto ret;

	CBaseEntity* player;

	int hitgroup;

	auto rt = c_autowall::get()->autowall(G::LocalPlayer->GetEyePosition(), rem, G::LocalPlayer, nullptr);

	if (!c_config::get()->b["legit_trigger_burst_fire"] && c_config::get()->b["legit_trigger_autowall"])
	{
		player = rt.ent;
		hitgroup = rt.hitgroup;
	}
	else
	{
		player = tr.m_pEnt;
		hitgroup = tr.hitGroup;
	}

	if (!player || player->IsDormant() || !player->IsAlive() || !player->IsPlayer())
		goto ret;

	if (player->GetTeam() == G::LocalPlayer->GetTeam())
		goto ret;

	if (IsLineGoesThroughSmoke(G::LocalPlayer->GetEyePosition(), tr.end) && !c_config::get()->b["legit_trigger_smoke"])
		goto ret;

	if (G::LocalPlayer->IsFlashed() && !c_config::get()->b["legit_trigger_blind"])
		goto ret;

	bool hitgroup_match = false;

	for (int hitgroupsorted : hitgroups)
	{
		if (hitgroup == hitgroupsorted)
		{
			hitgroup_match = true;
			break;
		}
	}

	do_burst_fire = c_config::get()->b["legit_trigger_burst_fire"] && (((int)GetTickCount() - burst_fire_start_time) < c_config::get()->i["legit_trigger_burst_fire_time"]);

	if (do_burst_fire || (hitgroup_match && rt.damage >= c_config::get()->i["legit_trigger_mindamage"]
		&& hitchance(
			c_config::get()->i["legit_trigger_hitchance"],
			Utilities->Math_CalcAngle(G::LocalPlayer->GetEyePosition(), rem),
			player
	)))
	{
		int delay = c_config::get()->i["legit_trigger_delay"];
		if (delay == 0) delay = 1;
		if (!is_delayed && !shot_delay && delay > 0)
		{
			is_delayed = true;
			shot_delay = true;
			shot_delay_time = GetTickCount() + delay;
		}

		if (shot_delay && shot_delay_time <= GetTickCount())
			shot_delay = false;

		if (!shot_delay)
		{
			if (!do_burst_fire && !(usercmd->buttons & IN_ATTACK))
				can_burst_fire = true;
			is_attack = true;
			is_delayed = false;
			shot_delay = false;
			usercmd->buttons |= IN_ATTACK;
		}
	}
	else
	{
		if (can_burst_fire)
		{
			can_burst_fire = false;
			burst_fire_start_time = (int)GetTickCount();
		}
		shot_delay_time = 0.f;
		is_delayed = false;
		shot_delay = false;
		goto ret;
	}
	goto skip;
ret:
	is_attack = false;
skip:
	return;
}

CTriggerBot g_triggerbot;