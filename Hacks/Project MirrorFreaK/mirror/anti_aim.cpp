#include "antiaim.h"
#include "Hooks.h"
#include "MathFunctions.h"
anti_aim * c_antiaim = new anti_aim();
antiaim_helper * c_helper = new antiaim_helper();

static bool dir = false;
static bool back = false;
static bool up = false;
static bool jitter = false;
static bool jitter2 = false;
inline float RandomFloat(float min, float max)
{
	static auto fn = (decltype(&RandomFloat))(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
	return fn(min, max);
}

float anti_aim::get_feet_yaw() 
{
	auto local_player = static_cast<IClientEntity*>(Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer()));

	if (!local_player)
		return 0.f;

	auto state = local_player->GetBasePlayerAnimState();

	float current_feet_yaw = state->m_flGoalFeetYaw;

	if (current_feet_yaw >= -360)
		current_feet_yaw = min(current_feet_yaw, 360.f);

	return current_feet_yaw;
}

#define MASK_SHOT_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)

//--------------------------------------------------------------------------------
void anti_aim::Base_AntiAim(CUserCmd* pCmd, IClientEntity* pLocal, bool &bSendPacket)
{

	if (GetAsyncKeyState(Options::Menu.MiscTab.manualleft.GetKey())) // right
	{
		dir = true;
		back = false;
		up = false;
		bigboi::indicator = 1;
	}

	if (GetAsyncKeyState(Options::Menu.MiscTab.manualright.GetKey())) // left
	{
		dir = false;
		back = false;
		up = false;
		bigboi::indicator = 2;
	}

	if (GetAsyncKeyState(Options::Menu.MiscTab.manualback.GetKey()))
	{
		dir = false;
		back = true;
		up = false;
		bigboi::indicator = 3;
	}

	if (GetAsyncKeyState(Options::Menu.MiscTab.manualfront.GetKey()))
	{
		dir = false;
		back = false;
		up = true;
		bigboi::indicator = 4;
	}

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
//	float speed = localp->GetVelocity().Length2D();


	if (!pLocal->IsMoving() && (pLocal->GetFlags() & FL_ONGROUND))
	{
		if ((Options::Menu.MiscTab.desync_aa_stand.GetState() && !bSendPacket) || !Options::Menu.MiscTab.desync_aa_stand.GetState())
		{
			switch (Options::Menu.MiscTab.AntiAimYaw.GetIndex())
			{

			case 1:
			{
				backwards(pCmd);
			}
			break;

			case 2:
			{
				backwards_jitter(pCmd);
			}
			break;

			case 3:
			{
				rand_lowerbody(pCmd);
			}
			break;

			case 4:
			{
				manual(pCmd);
			}
			break;

			case 5:
			{
				jitter_crooked(pCmd);
			}
			break;

			case 6:
			{
				freestanding_jitter(pCmd);
			}
			break;

			}
		}
		if (Options::Menu.MiscTab.desync_aa_stand.GetState() && bSendPacket)
		{
			pCmd->viewangles.y += jitter2 ? get_feet_yaw() + 50 : get_feet_yaw() - 50;
		}
	}

	if (pLocal->IsMoving() && (pLocal->GetFlags() & FL_ONGROUND))
	{
		if ((Options::Menu.MiscTab.desync_aa_move.GetState() && !bSendPacket) || !Options::Menu.MiscTab.desync_aa_move.GetState())
		{
			switch (Options::Menu.MiscTab.AntiAimYawrun.GetIndex())
			{

			case 1:
			{
				backwards(pCmd);
			}
			break;

			case 2:
			{
				backwards_jitter(pCmd);
			}
			break;

			case 3:
			{
				rand_lowerbody(pCmd);
			}
			break;

			case 4:
			{
				manual(pCmd);
			}
			break;

			case 5:
			{
				jitter_crooked(pCmd);
			}
			break;

			case 6:
			{
				freestanding_jitter(pCmd);
			}
			break;

			}
		}
		if (Options::Menu.MiscTab.desync_aa_move.GetState() && bSendPacket)
		{
			pCmd->viewangles.y += jitter2 ? get_feet_yaw() + 50 : get_feet_yaw() - 50;
		}
	}

	if (!(pLocal->GetFlags() & FL_ONGROUND))
	{
		switch (Options::Menu.MiscTab.AntiAimYaw3.GetIndex())
		{

		case 1:
		{
			backwards(pCmd);
		}
		break;

		case 2:
		{
			backwards_jitter(pCmd);
		}
		break;

		case 3:
		{
			rand_lowerbody(pCmd);
		}
		break;

		case 4:
		{
			manual(pCmd);
		}
		break;

		case 5:
		{
			jitter_crooked(pCmd);
		}
		break;

		case 6:
		{
			freestanding_jitter(pCmd);
		}
		break;

		}
	}
	
}

void anti_aim::DoPitch(CUserCmd * pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (pCmd->buttons & IN_ATTACK)
		return;

	if (Interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::MOUSE_LEFT))
		return;

	switch (Options::Menu.MiscTab.AntiAimPitch.GetIndex())
	{
	case 0:
		break;
	case 1:
		pitchdown(pCmd);
		break;
	case 2:
		pitchup(pCmd);
		break;
	case 3:
	{
		pitchjitter(pCmd);
	}
	break;
	case 4:
	{
		pitchrandom(pCmd);
	}
	break;
	case 5:
	{
		zero(pCmd);
	}
	case 6:
	{
		fakedown(pCmd);
	}
	break;
	case 7:
	{
		fakeup(pCmd);
	}
	break;

	}
}

void anti_aim::DoAntiAim(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if ((pCmd->buttons & IN_USE) || pLocal->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (Interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::MOUSE_LEFT))
		return;

	if (GameUtils::IsGrenade(pWeapon) || pWeapon->IsGrenade())
		return;

	if (!Options::Menu.MiscTab.AntiAimEnable.GetState())
		return;

	if (Options::Menu.MiscTab.disable_on_dormant.GetState() && c_helper->closest() == -1)
		return;

	DoPitch(pCmd);
	Base_AntiAim(pCmd, hackManager.pLocal(), bSendPacket);

	if (Options::Menu.MiscTab.antilby.GetIndex() > 0 && (pLocal->GetFlags() & FL_ONGROUND))
		c_helper->anti_lby(pCmd, bSendPacket);
}

void anti_aim::backwards(CUserCmd * pcmd)
{
	pcmd->viewangles.y -= 180;
}

void anti_aim::backwards_jitter(CUserCmd * pcmd)
{
	jitter = !jitter;
	if (jitter)
		pcmd->viewangles.y -= 150;
	else
		pcmd->viewangles.y += 150;
}

void anti_aim::lowerbody(CUserCmd * pcmd)
{
	pcmd->viewangles.y -= hackManager.pLocal()->GetLowerBodyYaw() + Options::Menu.MiscTab.lby1.GetValue();
}

void anti_aim::rand_lowerbody(CUserCmd * pcmd)
{
	pcmd->viewangles.y -= hackManager.pLocal()->GetLowerBodyYaw() + RandomFloat(Options::Menu.MiscTab.randlbyr.GetValue(), -Options::Menu.MiscTab.randlbyr.GetValue());
}

void anti_aim::manual(CUserCmd * pCmd)
{
	if (dir && !back && !up)
		pCmd->viewangles.y -= 90.f;
	else if (!dir && !back && !up)
		pCmd->viewangles.y += 90.f;
	else if (!dir && back && !up)
		pCmd->viewangles.y -= 180.f;
	else if (!dir && !back && up)
		pCmd->viewangles.y;
}


void anti_aim::manual_jitter(CUserCmd * pCmd)
{

	if (dir && !back && !up)
	{
		if (jitter)
			pCmd->viewangles.y -= 75.f;
		else
			pCmd->viewangles.y -= 160.f;
		jitter = !jitter;
	}
	else if (!dir && !back && !up)
	{
		if (jitter)
			pCmd->viewangles.y += 75.f;
		else
			pCmd->viewangles.y += 160.f;
		jitter = !jitter;
	}
	else if (!dir && back && !up)
	{
		if (jitter)
			pCmd->viewangles.y += 135.f + RandomFloat(20, -20);
		else
			pCmd->viewangles.y -= 135.f + RandomFloat(20, -20);
		jitter = !jitter;
	}
	else if (!dir && !back && up)
	{
		if (jitter)
			pCmd->viewangles.y += 29.f + RandomFloat(20, -20);
		else
			pCmd->viewangles.y -= 29.f + RandomFloat(20, -20);
		jitter = !jitter;
	}
}

void anti_aim::pitchdown(CUserCmd * pcmd)
{
	pcmd->viewangles.x = 89.f;
}

void anti_aim::pitchup(CUserCmd * pcmd)
{
	pcmd->viewangles.x = -89.f;
}

void anti_aim::zero(CUserCmd * pcmd)
{
	pcmd->viewangles.x = 0.f;
}

void anti_aim::pitchjitter(CUserCmd * pcmd)
{
	if (jitter)
		pcmd->viewangles.x = 89.f;
	else
		pcmd->viewangles.x = -89.f;
	jitter = !jitter;
}

void anti_aim::pitchrandom(CUserCmd * pcmd)
{
	pcmd->viewangles.x = 0.f + RandomFloat(-89.f, 89.f);
}

void anti_aim::fakedown(CUserCmd * pcmd)
{
	pcmd->viewangles.x = 540;
}

void anti_aim::fakeup(CUserCmd * pcmd)
{
	pcmd->viewangles.x = -540;
}


#define RandomInt(min, max) (rand() % (max - min + 1) + min)
#define	MASK_ALL				(0xFFFFFFFF)
#define	MASK_SOLID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE) 			/**< everything that is normally solid */
#define	MASK_PLAYERSOLID		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE) 	/**< everything that blocks player movement */
#define	MASK_NPCSOLID			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE) /**< blocks npc movement */
#define	MASK_WATER				(CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME) 							/**< water physics in these contents */
#define	MASK_OPAQUE				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE) 							/**< everything that blocks line of sight for AI, lighting, etc */
#define MASK_OPAQUE_AND_NPCS	(MASK_OPAQUE|CONTENTS_MONSTER)										/**< everything that blocks line of sight for AI, lighting, etc, but with monsters added. */
#define	MASK_VISIBLE			(MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE) 								/**< everything that blocks line of sight for players */
#define MASK_VISIBLE_AND_NPCS	(MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE) 							/**< everything that blocks line of sight for players, but with monsters added. */
#define	MASK_SHOT				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX) 	/**< bullets see these as solid */
#define MASK_SHOT_HULL			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE) 	/**< non-raycasted weapons see this as solid (includes grates) */
#define MASK_SHOT_PORTAL		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW) 							/**< hits solids (not grates) and passes through everything else */
#define MASK_SHOT_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS) // non-raycasted weapons see this as solid (includes grates)
#define MASK_SOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE) 					/**< everything normally solid, except monsters (world+brush only) */
#define MASK_PLAYERSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE) 			/**< everything normally solid for player movement, except monsters (world+brush only) */
#define MASK_NPCSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE) 			/**< everything normally solid for npc movement, except monsters (world+brush only) */
#define MASK_NPCWORLDSTATIC		(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE) 					/**< just the world, used for route rebuilding */
#define MASK_SPLITAREAPORTAL	(CONTENTS_WATER|CONTENTS_SLIME) 		

void anti_aim::freestanding_jitter(CUserCmd* pCmd) {


	IClientEntity* local_player = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	float range = Options::Menu.MiscTab.freerange.GetValue() / 2;
	static int Ticks = 0;
	if (Interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::MOUSE_LEFT))
		return;

	bool no_active = true;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	static float hold = 0.f;
	Vector besthead;
	float opposite = 0.f;

	auto leyepos = hackManager.pLocal()->GetOrigin_likeajew() + hackManager.pLocal()->GetViewOffset();
	auto headpos = hitbox_location(local_player, 0);
	auto origin = hackManager.pLocal()->GetOrigin_likeajew();

	auto checkWallThickness = [&](IClientEntity* pPlayer, Vector newhead) -> float
	{

		Vector endpos1, endpos2;

		Vector eyepos = pPlayer->GetOrigin_likeajew() + pPlayer->GetViewOffset();
		Ray_t ray;
		ray.Init(newhead, eyepos);
		CTraceFilterSkipTwoEntities filter(pPlayer, hackManager.pLocal());

		trace_t trace1, trace2;
		Interfaces::Trace->TraceRay(ray, MASK_SHOT | MASK_SHOT_BRUSHONLY | MASK_OPAQUE_AND_NPCS, &filter, &trace1);

		if (trace1.DidHit())
			endpos1 = trace1.endpos;
		else
			return 0.f;

		ray.Init(eyepos, newhead);
		Interfaces::Trace->TraceRay(ray, MASK_SHOT | MASK_SHOT_BRUSHONLY | MASK_OPAQUE_AND_NPCS, &filter, &trace2);

		if (trace2.DidHit())
			endpos2 = trace2.endpos;

		float add = newhead.Dist(eyepos) - leyepos.Dist(eyepos) + 3.f;
		return endpos1.Dist(endpos2) + add / 3;

	};

	int index = c_helper->closest();
	static IClientEntity* entity;

	if (index != -1)
		entity = Interfaces::EntList->GetClientEntity(index); // maybe?

	if (!entity->isValidPlayer() || entity == nullptr)
	{
		pCmd->viewangles.y -= 180.f;
		return;
	}

	float radius = Vector(headpos - origin).Length2D();

	if (index == -1)
	{
		no_active = true;
	}
	else
	{
		for (float besthead = 0; besthead < 7; besthead += 0.1)
		{
			Vector newhead(radius * cos(besthead) + leyepos.x, radius * sin(besthead) + leyepos.y, leyepos.z);
			float totalthickness = 0.f;
			no_active = false;
			totalthickness += checkWallThickness(entity, newhead);
			if (totalthickness > highestthickness)
			{
				hold = besthead;
				highestthickness = totalthickness;
				bestrotation = besthead;
			}
		}
	}
	if (no_active)
	{
		pCmd->viewangles.y = RAD2DEG(hold);
		return;
	}
	
	else
	{
		jitter = !jitter;
		if (jitter)
			pCmd->viewangles.y = RAD2DEG(bestrotation) + range;
		else
			pCmd->viewangles.y = RAD2DEG(bestrotation) - range;
	}
}

void AngleVectors_3(const QAngle &angles, Vector& forward, Vector& right, Vector& up)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[0]), &sp, &cp);
	SinCos(DEG2RAD(angles[2]), &sr, &cr);

	forward.x = (cp * cy);
	forward.y = (cp * sy);
	forward.z = (-sp);
	right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
	right.y = (-1 * sr * sp * sy + -1 * cr *  cy);
	right.z = (-1 * sr * cp);
	up.x = (cr * sp * cy + -sr * -sy);
	up.y = (cr * sp * sy + -sr * cy);
	up.z = (cr * cp);
}

void anti_aim::jitter_crooked(CUserCmd * pcmd) //by faxzee
{
	jitter2 = !jitter2;
	float fljit = 78;
	float flCrookedoffset = -20;
	
	if (jitter2) {
		pcmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + flCrookedoffset;
		pcmd->viewangles.y += fljit;
	}
	else {
		pcmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + flCrookedoffset;
		pcmd->viewangles.y -= fljit;
	}
}

