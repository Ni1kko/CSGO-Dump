#include "AntiAim.h"
#include "..\Aimbot\Autowall.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\Utils\Math.h"
#include "..\..\Menu\Menu.h"
#include "..\..\Menu\config.h"
#include "..\..\Features\Aimbot\Aimbot.h"
AntiAim g_AntiAim;

bool Swtich = false;

enum MovingAAType {
	Standing = 0,
	Moving,
	Air
};


int ReturnMovementType() {
	if (Globals::LocalPlayer->GetFlags() & FL_ONGROUND)
	{
		if (Globals::LocalPlayer->GetVelocity().Length2D() < 32)
		{
			return MovingAAType::Standing;
		}
		else if (Globals::LocalPlayer->GetVelocity().Length2D() > 32)
		{
			return MovingAAType::Moving;
		}
	}
	else
	{
		return MovingAAType::Air;
	}
}

void FreeStanding()
{
	static float FinalAngle;
	bool bside1 = false;
	bool bside2 = false;
	bool autowalld = false;
	for (int i = 0; i <= g_pEngine->GetMaxClients(); ++i) 
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant()
			|| pPlayerEntity == Globals::LocalPlayer
			|| pPlayerEntity->GetTeam() == Globals::LocalPlayer->GetTeam())
			continue;

		float angToLocal = g_Math.CalcAngle(Globals::LocalPlayer->GetOrigin(), pPlayerEntity->GetOrigin()).y;
		Vector ViewPoint = pPlayerEntity->GetOrigin() + Vector(0, 0, 90);

		Vector2D Side1 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal))),(45 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side2 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(45 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector2D Side3 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal))),(50 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side4 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(50 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector Origin = Globals::LocalPlayer->GetOrigin();

		Vector2D OriginLeftRight[] = { Vector2D(Side1.x, Side1.y), Vector2D(Side2.x, Side2.y) };

		Vector2D OriginLeftRightLocal[] = { Vector2D(Side3.x, Side3.y), Vector2D(Side4.x, Side4.y) };

		for (int side = 0; side < 2; side++)
		{
			Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 80 };
			Vector OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };

			if (g_Autowall.CanHitFloatingPoint(OriginAutowall, ViewPoint))
			{
				if (side == 0)
				{
					bside1 = true;
					FinalAngle = angToLocal + 90;
				}
				else if (side == 1)
				{
					bside2 = true;
					FinalAngle = angToLocal - 90;
				}
				autowalld = true;
			}
			else
			{
				for (int side222 = 0; side222 < 2; side222++)
				{
					Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side222].x,  Origin.y - OriginLeftRight[side222].y , Origin.z + 80 };

					if (g_Autowall.CanHitFloatingPoint(OriginAutowall222, OriginAutowall2))
					{
						if (side222 == 0)
						{
							bside1 = true;
							FinalAngle = angToLocal + 90;
						}
						else if (side222 == 1)
						{
							bside2 = true;
							FinalAngle = angToLocal - 90;
						}
						autowalld = true;
					}
				}
			}
		}
	}

	if (!autowalld || (bside1 && bside2)) {
		switch (ReturnMovementType()) {
		case MovingAAType::Standing: {
			switch (c_config::get().yaw_standing) {
			case 1: Globals::pCmd->viewangles.y += 180; break;
			case 2: Globals::pCmd->viewangles.y = 0; break;
			case 3: Globals::pCmd->viewangles.y += g_pGlobalVars->curtime * 3000; break;
			case 4: Globals::pCmd->viewangles.y += Globals::Manual_Side ? 90 : -90; break;
			}
		}break;
		case MovingAAType::Moving: {
			switch (c_config::get().yaw_moving) {
			case 1: Globals::pCmd->viewangles.y += 180; break;
			case 2: Globals::pCmd->viewangles.y = 0; break;
			case 3: Globals::pCmd->viewangles.y += g_pGlobalVars->curtime * 3000; break;
			case 4: Globals::pCmd->viewangles.y += Globals::Manual_Side ? 90 : -90; break;
			}
		}break;
		case MovingAAType::Air: {
			switch (c_config::get().yaw_air) {
			case 1: Globals::pCmd->viewangles.y += 180; break;
			case 2: Globals::pCmd->viewangles.y = 0; break;
			case 3: Globals::pCmd->viewangles.y += g_pGlobalVars->curtime * 3000; break;
			case 4: Globals::pCmd->viewangles.y += Globals::Manual_Side ? 90 : -90; break;
			}
		}break;
		}
	}
	else
		Globals::pCmd->viewangles.y = FinalAngle;

	if (c_config::get().aa_jitter != 0)
	{
		float Offset = c_config::get().aa_jitter;


			Swtich ? Globals::pCmd->viewangles.y -= Offset : Globals::pCmd->viewangles.y += Offset;
	}
}

#include "..\..\SDK\IVModelInfo.h"
mstudiobbox_t* get_hitboxv2(C_BaseEntity* entity, int hitbox_index)
{
	if (entity->IsDormant() || entity->GetHealth() <= 0)
		return NULL;

	const auto pModel = entity->GetModel();
	if (!pModel)
		return NULL;

	auto pStudioHdr = g_pModelInfo->GetStudiomodel(pModel);
	if (!pStudioHdr)
		return NULL;

	auto pSet = pStudioHdr->GetHitboxSet(0);
	if (!pSet)
		return NULL;

	if (hitbox_index >= pSet->numhitboxes || hitbox_index < 0)
		return NULL;

	return pSet->GetHitbox(hitbox_index);
}
Vector GetHitboxPositionv2(C_BaseEntity* entity, int hitbox_id)
{
	auto hitbox = get_hitboxv2(entity, hitbox_id);
	if (!hitbox)
		return Vector(0, 0, 0);

	auto bone_matrix = entity->GetBoneMatrix(hitbox->bone);

	Vector bbmin, bbmax;
	g_Math.VectorTransform(hitbox->min, bone_matrix, bbmin);
	g_Math.VectorTransform(hitbox->max, bone_matrix, bbmax);

	return (bbmin + bbmax) * 0.5f;
}
float m_bestthreat = 0.f;
void NormalizeNum(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}
void VectorSubtractv2(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}
float fov_player(Vector ViewOffSet, Vector View, C_BaseEntity* entity, int hitbox)
{
	// Anything past 180 degrees is just going to wrap around
	CONST FLOAT MaxDegrees = 180.0f;

	// Get local angles
	Vector Angles = View;

	// Get local view / eye position
	Vector Origin = ViewOffSet;

	// Create and intiialize vectors for calculations below
	Vector Delta(0, 0, 0);
	//Vector Origin(0, 0, 0);
	Vector Forward(0, 0, 0);

	// Convert angles to normalized directional forward vector
	g_Math.AngleVectors(Angles, &Forward);

	Vector AimPos = GetHitboxPositionv2(entity, hitbox); //pvs fix disabled

	VectorSubtract(AimPos, Origin, Delta);
	//Delta = AimPos - Origin;

	// Normalize our delta vector
	NormalizeNum(Delta, Delta);

	// Get dot product between delta position and directional forward vectors
	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (MaxDegrees / M_PI));
}


int closest_to_crosshair()
{
	int index = -1;
	float lowest_fov = INT_MAX;

	auto local_player = Globals::LocalPlayer;

	if (!local_player)
		return -1;

	Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

	Vector angles;
	g_pEngine->GetViewAngles(angles);

	for (int i = 1; i <= g_pGlobalVars->maxClients; i++)
	{
		auto entity = g_pEntityList->GetClientEntity(i);

		if (!entity || !entity->IsAlive() || entity->GetTeam() == local_player->GetTeam() || entity->IsDormant() || entity == local_player)
			continue;

		float fov = fov_player(local_position, angles, entity, 0);

		if (fov < lowest_fov)
		{
			lowest_fov = fov;
			index = i;
		}
	}

	return index;
}

int closest_to_local_player()
{
	int index = -1;
	float lowest_distance = INT_MAX;

	auto local_player = Globals::LocalPlayer;

	if (!local_player)
		return -1;

	Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

	Vector angles;
	g_pEngine->GetViewAngles(angles);

	for (int i = 1; i <= g_pGlobalVars->maxClients; i++)
	{
		auto entity = g_pEntityList->GetClientEntity(i);

		if (!entity || !entity->IsAlive() || entity->GetTeam() == local_player->GetTeam() || entity->IsDormant() || entity == local_player)
			continue;

		float distance = g_Math.Distance(Vector2D(local_player->GetOrigin().x, local_player->GetOrigin().y), Vector2D(entity->GetOrigin().x, entity->GetOrigin().y));

		if (distance < lowest_distance)
		{
			lowest_distance = distance;
			index = i;
		}
	}

	return index;
}


int BestTargetByThreat() {

	for (int i = 0; i <= g_pGlobalVars->maxClients; i++)
	{
		auto Entity = g_pEntityList->GetClientEntity(i);
		auto LocalPlayer = Globals::LocalPlayer;

		if (!Entity || !LocalPlayer || !Entity->IsAlive() || Entity->IsDormant())
			continue;

		m_bestthreat = 0.f;

		float dist = g_Math.Distance(Vector2D(LocalPlayer->GetOrigin().x, LocalPlayer->GetOrigin().y), Vector2D(Entity->GetOrigin().x, Entity->GetOrigin().y));

		float health = (float)Entity->GetHealth();

		float threat = health / dist;

		if (threat > m_bestthreat)
		{
			return Entity->EntIndex();
		}

		return 0;
	}
}
void freestand(CUserCmd* cmd)
{
	auto local_player = Globals::LocalPlayer;

	if (!local_player)
		return;

	static float last_real;
	bool no_active = true;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	Vector besthead;

	m_bestthreat = 0.f;

	auto leyepos = local_player->GetVecOrigin() + local_player->GetViewOffset();
	auto headpos = GetHitboxPositionv2(local_player, 0);
	auto origin = local_player->GetAbsOrigin();


	auto checkWallThickness = [&](C_BaseEntity* pPlayer, Vector newhead) -> float
	{
		Vector endpos1, endpos2;
		Vector eyepos = pPlayer->GetVecOrigin() + pPlayer->GetViewOffset();


		CTraceFilterSkipTwoEntities filter(pPlayer, local_player);

		C_Trace trace1, trace2;
		g_pTrace->TraceRay(C_Ray(newhead, eyepos), mask_shot_brushonly, &filter, &trace1);

		if (trace1.DidHit())
			endpos1 = trace1.end;
		else
			return 0.f;


		g_pTrace->TraceRay(C_Ray(eyepos, newhead), mask_shot_brushonly, &filter, &trace2);

		if (trace2.DidHit())
			endpos2 = trace2.end;

		float add = newhead.DistTo(eyepos) - leyepos.DistTo(eyepos) + 3.f;
		return endpos1.DistTo(endpos2) + add / 3;
	};

	int index = -1;

	switch (c_config::get().antiaim_freestanding_mode) {
	case 1: { index = closest_to_crosshair();  } break;
	case 2: { index = closest_to_local_player(); } break;
	}
	
	static C_BaseEntity* entity;

	if (index != -1)
		entity = g_pEntityList->GetClientEntity(index);

	float step = (2 * M_PI) / 18.f; // One PI = half a circle ( for stacker cause low iq :sunglasses: ), 28

	float radius = fabs(Vector(headpos - origin).Length2D());

	if (index == -1)
	{
		no_active = true;
	}
	else
	{
		for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
		{
			Vector newhead(radius * cos(rotation) + leyepos.x, radius * sin(rotation) + leyepos.y, leyepos.z);

			float totalthickness = 0.f;

			no_active = false;

			totalthickness += checkWallThickness(entity, newhead);

			if (totalthickness > highestthickness)
			{
				highestthickness = totalthickness;
				bestrotation = rotation;
				besthead = newhead;
			}
		}
	}


	if (no_active)
		switch (ReturnMovementType()) {
		case MovingAAType::Standing: {
			switch (c_config::get().yaw_standing) {
			case 1: Globals::pCmd->viewangles.y += 180; break;
			case 2: Globals::pCmd->viewangles.y = 0; break;
			case 3: Globals::pCmd->viewangles.y += g_pGlobalVars->curtime * 3000; break;
			case 4: Globals::pCmd->viewangles.y += Globals::Manual_Side ? 90 : -90; break;
			}
		}break;
		case MovingAAType::Moving: {
			switch (c_config::get().yaw_moving) {
			case 1: Globals::pCmd->viewangles.y += 180; break;
			case 2: Globals::pCmd->viewangles.y = 0; break;
			case 3: Globals::pCmd->viewangles.y += g_pGlobalVars->curtime * 3000; break;
			case 4: Globals::pCmd->viewangles.y += Globals::Manual_Side ? 90 : -90; break;
			}
		}break;
		case MovingAAType::Air: {
			switch (c_config::get().yaw_air) {
			case 1: Globals::pCmd->viewangles.y += 180; break;
			case 2: Globals::pCmd->viewangles.y = 0; break;
			case 3: Globals::pCmd->viewangles.y += g_pGlobalVars->curtime * 3000; break;
			case 4: Globals::pCmd->viewangles.y += Globals::Manual_Side ? 90 : -90; break;
			}
		}break;
		}
	else
		cmd->viewangles.y = RAD2DEG(bestrotation);

	last_real = cmd->viewangles.y;
}

bool Left_AA = false;
bool Right_AA = false;
bool Back_AA = false;

static float next_lby_update_time = 0;
float get_curtime(CUserCmd* ucmd) {
	auto local_player = Globals::LocalPlayer;

	if (!local_player)
		return 0;

	int g_tick = 0;
	CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = (float)local_player->GetTickBase();
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * g_pGlobalVars->intervalPerTick;
	return curtime;
}
bool next_lby_update(const float yaw_to_break, CUserCmd* cmd)
{
	auto local_player = Globals::LocalPlayer;

	if (!local_player)
		return false;

	static float next_lby_update_time = 0;
	float curtime = get_curtime(cmd);

	auto animstate = local_player->AnimState();
	if (!animstate)
		return false;

	if (!(local_player->GetFlags() & FL_ONGROUND))
		return false;

	if (animstate->speed_2d > 0.1)
		next_lby_update_time = curtime + 0.22f;

	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1f;
		return true;
	}

	return false;
}
bool m_bAutomaticDir = false;
int m_iAutoDirection = 0;

int m_iJitter = 0;
int m_iJitterIteration = 0;

float m_flCurrentFeetYaw = 0.0f;
float m_flPreviousFeetYaw = 0.0f;

void Real()
{
	//if (!g::bSendPacket && g::pLocalEntity->AnimState() && g_Menu.Config.DesyncAngle)
		//g::pCmd->viewangles.y = g_Math.NormalizeYaw(g::pLocalEntity->AnimState()->m_flGoalFeetYaw + 180.f);
	//else {
		// freestanding soon
	//}

	static bool Swtich2 = false;
	Swtich2 = !Swtich2;

	static float test = 0.f;
	if (Swtich2)
		test += 90.f;

	test = g_Math.NormalizeYaw(test);

	if (c_config::get().desync > 0) {
		Globals::DesyncEnabledSpecialChecks = true;
	}
	else {
		Globals::DesyncEnabledSpecialChecks = false;
	}
	if (c_config::get().desync == 3 && Globals::LocalPlayer->AnimState()) {
		float desync = Globals::LocalPlayer->DesyncValue();
		float balance = 1.0f;

		int type = 2;
		if (type == 2)
			balance = -1.0f;

		if (g_pGlobalVars->curtime <= next_lby_update(Globals::pCmd->viewangles.y, Globals::pCmd)) {

			auto net_channel = g_pEngine->GetNetChannel();

			if (!net_channel)
				return;

			if (net_channel->m_nChokedPackets >= 2) {
				Globals::pCmd->viewangles.y = g_Math.NormalizeYaw(Globals::pCmd->viewangles.y);
				return;
			}

			if (type == 1)
				Globals::pCmd->viewangles.y -= 100.0f;
			else
				Globals::pCmd->viewangles.y += (balance * 120.0f);
		}
		else if (type != 1) {
			Globals::pCmd->viewangles.y -= (desync + 30.0f) * balance;
		}
	}

	if (c_config::get().desync == 4) {
		int jitter_side = 1;

		Globals::pCmd->viewangles.y += 180.0f;

		float desync = Globals::LocalPlayer->DesyncValue();
		float lby_delta = 180.0f - desync + 10.0f;
		float desync_length = 180.0f - lby_delta * 0.5f;
		float jitter = 90.0f * jitter_side;


		if (jitter_side == 1)
			Globals::pCmd->viewangles.y += desync_length;
		else if (jitter_side == -1)
			Globals::pCmd->viewangles.y -= desync_length;


		int v19 = 0;
		if (g_pGlobalVars->curtime < g_pGlobalVars->curtime <= next_lby_update(Globals::pCmd->viewangles.y, Globals::pCmd)) {
			v19 = m_iJitter;
		}
		else {
			m_iJitter = 0;
		}

		int v20 = v19 - 1;
		if (v20) {
			if (v20 == 1) {
				if (jitter_side == 1)
					Globals::pCmd->viewangles.y += lby_delta;
				else
					Globals::pCmd->viewangles.y += desync - 190.0f;
			}
		}
		else {
			if (jitter_side == 1)
				Globals::pCmd->viewangles.y += desync - 190.0;
			else
				Globals::pCmd->viewangles.y += lby_delta;
			Globals::bSendPacket = true;
		}

		if (++m_iJitter >= 3)
			m_iJitter = 0;

		Globals::pCmd->viewangles.y = g_Math.NormalizeYaw(Globals::pCmd->viewangles.y);
	}

	if (!Globals::bSendPacket && Globals::LocalPlayer->AnimState() && c_config::get().desync > 0) {
		if (c_config::get().desync == 1) {
			Globals::pCmd->viewangles.y = g_Math.NormalizeYaw(Globals::RealAngle.y + 90 + test);
		}
		else if (c_config::get().desync == 2) {
			Globals::pCmd->viewangles.y += Globals::LocalPlayer->DesyncValue();
		}
	}
	else {
		if (!c_config::get().antiaim_freestanding) {

			switch (ReturnMovementType()) {
			case MovingAAType::Standing: {
				switch (c_config::get().yaw_standing) {
				case 1: Globals::pCmd->viewangles.y += 180; break;
				case 2: Globals::pCmd->viewangles.y = 0; break;
				case 3: Globals::pCmd->viewangles.y += g_pGlobalVars->curtime * 3000; break;
				case 4: Globals::pCmd->viewangles.y += Globals::Manual_Side ? 90 : -90; break;
				}
			}break;
			case MovingAAType::Moving: {
				switch (c_config::get().yaw_moving) {
				case 1: Globals::pCmd->viewangles.y += 180; break;
				case 2: Globals::pCmd->viewangles.y = 0; break;
				case 3: Globals::pCmd->viewangles.y += g_pGlobalVars->curtime * 3000; break;
				case 4: Globals::pCmd->viewangles.y += Globals::Manual_Side ? 90 : -90; break;
				}
			}break;
			case MovingAAType::Air: {
				switch (c_config::get().yaw_air) {
				case 1: Globals::pCmd->viewangles.y += 180; break;
				case 2: Globals::pCmd->viewangles.y = 0; break;
				case 3: Globals::pCmd->viewangles.y += g_pGlobalVars->curtime * 3000; break;
				case 4: Globals::pCmd->viewangles.y += Globals::Manual_Side ? 90 : -90; break;
				}
			}break;
			}

		}
		else {
			if (c_config::get().antiaim_freestanding_mode == 0) {
				FreeStanding();
			}
			else {
				freestand(Globals::pCmd);
			}
		}
	}
}

void AntiAim::OnCreateMove()
{
	if (!Globals::LocalPlayer || !g_pEngine->IsInGame() || !Globals::LocalPlayer->GetActiveWeapon() || !c_config::get().antiaim_enabled || g_Menu.Config.LegitBacktrack) {
		Globals::DesyncEnabledSpecialChecks = false;
		return;
	}
	

	if (!Globals::LocalPlayer->IsAlive() || Globals::LocalPlayer->IsNade()) {
		Globals::DesyncEnabledSpecialChecks = false;
		return;
	}
		

	if (Globals::LocalPlayer->GetMoveType() == MoveType_t::MOVETYPE_LADDER) {
		Globals::DesyncEnabledSpecialChecks = false;
		return;
	}

	if (Globals::pCmd->buttons & IN_USE)
	{
		Globals::DesyncEnabledSpecialChecks = false;
		return;
	}
		

	float flServerTime = Globals::LocalPlayer->GetTickBase() * g_pGlobalVars->intervalPerTick;
	bool canShoot = (Globals::LocalPlayer->GetActiveWeapon()->GetNextPrimaryAttack() <= flServerTime);

	if ((canShoot && (Globals::pCmd->buttons & IN_ATTACK)) && Globals::LocalPlayer->GetActiveWeapon()->GetItemDefinitionIndex() != ItemDefinitionIndex::WEAPON_REVOLVER) {
		Globals::DesyncEnabledSpecialChecks = false;

		return;
	}

	if (Globals::bSendPacket)
		Swtich = !Swtich;

	switch (ReturnMovementType()) {
	case MovingAAType::Standing: {
		switch (c_config::get().pitch_standing) {
		case 1: Globals::pCmd->viewangles.x = 89.9f; break;
		case 2: Globals::pCmd->viewangles.x = -89.9f; break;
		case 3: Globals::pCmd->viewangles.x = 0; break;
		case 4: Globals::pCmd->viewangles.x = 45.9f; break;
		}
	}break;
	case MovingAAType::Moving: {
		switch (c_config::get().pitch_moving) {
		case 1: Globals::pCmd->viewangles.x = 89.9f; break;
		case 2: Globals::pCmd->viewangles.x = -89.9f; break;
		case 3: Globals::pCmd->viewangles.x = 0; break;
		case 4: Globals::pCmd->viewangles.x = 45.9f; break;
		}
	}break;
	case MovingAAType::Air: {
		switch (c_config::get().pitch_air) {
		case 1: Globals::pCmd->viewangles.x = 89.9f; break;
		case 2: Globals::pCmd->viewangles.x = -89.9f; break;
		case 3: Globals::pCmd->viewangles.x = 0; break;
		case 4: Globals::pCmd->viewangles.x = 45.9f; break;
		}
	}break;
	}


	Real();
}