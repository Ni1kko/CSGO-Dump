#include "antiaim.h"
#include "autowall.h"

bool Settings::AntiAim::Yaw::enabled = false;
bool Settings::AntiAim::FreeStanding::enabled = false;
bool Settings::AntiAim::Pitch::enabled = false;
bool Settings::AntiAim::Fake::enabled = false;
AntiAimType_Y Settings::AntiAim::Yaw::type = AntiAimType_Y::SPIN_FAST;
AntiAimType_X Settings::AntiAim::Pitch::type = AntiAimType_X::STATIC_DOWN;
AntiAimType_Fake Settings::AntiAim::Fake::type = AntiAimType_Fake::STATIC_LEFT;
bool Settings::AntiAim::AutoDisable::noEnemy = false;
bool Settings::AntiAim::AutoDisable::knifeHeld = false;

QAngle AntiAim::real;
QAngle AntiAim::fake;

static bool GetBestHeadAngle(QAngle& angle)
{
	float b, r, l;

	Vector src3D, dst3D, forward, right, up, src, dst;

	trace_t tr;
	Ray_t ray, ray2, ray3, ray4, ray5;
	CTraceFilter filter;

	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
	if (!localplayer)
		return false;

	QAngle viewAngles;
	engine->GetViewAngles(viewAngles);

	viewAngles.x = 0;

	Math::AngleVectors(viewAngles, forward, right, up);

	auto GetTargetEntity = [ & ] ( void )
	{
		float bestFov = FLT_MAX;
		C_BasePlayer* bestTarget = NULL;

		for( int i = 0; i < engine->GetMaxClients(); ++i )
		{
			C_BasePlayer* player = (C_BasePlayer*) entityList->GetClientEntity(i);

			if (!player
				|| player == localplayer
				|| player->GetDormant()
				|| !player->GetAlive()
				|| player->GetImmune()
				|| player->GetTeam() == localplayer->GetTeam())
				continue;

			float fov = Math::GetFov(viewAngles, Math::CalcAngle(localplayer->GetEyePosition(), player->GetEyePosition()));

			if( fov < bestFov )
			{
				bestFov = fov;
				bestTarget = player;
			}
		}

		return bestTarget;
	};

	auto target = GetTargetEntity();
	filter.pSkip = localplayer;
	src3D = localplayer->GetEyePosition();
	dst3D = src3D + (forward * 384);

	if (!target)
		return false;

	ray.Init(src3D, dst3D);
	trace->TraceRay(ray, MASK_SHOT, &filter, &tr);
	b = (tr.endpos - tr.startpos).Length();

	ray2.Init(src3D + right * 35, dst3D + right * 35);
	trace->TraceRay(ray2, MASK_SHOT, &filter, &tr);
	r = (tr.endpos - tr.startpos).Length();

	ray3.Init(src3D - right * 35, dst3D - right * 35);
	trace->TraceRay(ray3, MASK_SHOT, &filter, &tr);
	l = (tr.endpos - tr.startpos).Length();

	if (b < r && b < l && l == r)
		return true; //if left and right are equal and better than back

	if (b > r && b > l)
		angle.y -= 180; //if back is the best angle
	else if (r > l && r > b)
		angle.y += 90; //if right is the best angle
	else if (r > l && r == b)
		angle.y += 135; //if right is equal to back
	else if (l > r && l > b)
		angle.y -= 90; //if left is the best angle
	else if (l > r && l == b)
		angle.y -= 135; //if left is equal to back
	else
		return false;
	angle.y *= 360;
	return true;
}

float AntiAim::GetMaxDelta( CCSGOAnimState *animState ) {

    float speedFraction = std::max(0.0f, std::min(animState->feetShuffleSpeed, 1.0f));

    float speedFactor = std::max(0.0f, std::min(1.0f, animState->feetShuffleSpeed2));

    float unk1 = ((animState->runningAccelProgress * -0.30000001) - 0.19999999) * speedFraction;
    float unk2 = unk1 + 1.0f;
    float delta;

    if (animState->duckProgress > 0)
    {
        unk2 += ((animState->duckProgress * speedFactor) * (0.5f - unk2));// - 1.f
    }

    delta = *(float*)((uintptr_t)animState + 0x3A4) * unk2;

    return delta - 0.5f;
}


static bool HasViableEnemy()
{
	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());

	for (int i = 1; i < engine->GetMaxClients(); ++i)
	{
		C_BasePlayer* entity = (C_BasePlayer*) entityList->GetClientEntity(i);

		if (!entity
			|| entity == localplayer
			|| entity->GetDormant()
			|| !entity->GetAlive()
			|| entity->GetImmune())
			continue;

		if( !Aimbot::friends.empty() ) // check for friends, if any
		{
			IEngineClient::player_info_t entityInformation;
			engine->GetPlayerInfo(i, &entityInformation);

			if (std::find(Aimbot::friends.begin(), Aimbot::friends.end(), entityInformation.xuid) != Aimbot::friends.end())
				continue;
		}

		if (Settings::Aimbot::friendly || entity->GetTeam() != localplayer->GetTeam())
			return true;
	}

	return false;
}
float polak(int type,float maxdesync,float aa) {
 
    int m_iJitter = 0;
    float desync = maxdesync;
    float balance = 1.0f;
 
    if (type == 2)
        balance = -1.0f;
 
    /*if (next_lby_update(aa)) {
 
        auto net_channel = engine->GetNetChannelInfo();
 
        if (!net_channel)
            return;
 
      // if (net_channel->m_nChokedPackets >= 5) {
            aa = NormalizeYaw(aa);
            return;
      //  }
        if (type == 1)
            aa -= 180.0f;
        else
            aa += (balance * 60.0f);
    }*/
 
    else if (type != 1) {
        int jitter_side = 1;
        aa += 88.0f;
 
        float desync = maxdesync;
        float lby_delta = 30.0f - desync + 28.0f;
        float desync_length = 30.0f - lby_delta * 30.0f;
        float jitter = 30.0f * jitter_side;
        auto net_channel = engine->GetNetChannelInfo();
 
        if (!net_channel)
            return aa;
        if (net_channel->m_nChokedPackets >= 5) {
            aa = aa;
            return aa;
        }
        if (jitter_side == 1)
            aa += desync - 80.0f;
 
        else if (jitter_side == -1)
            aa += desync + 80.0f;
       // g::bSendPacket = false;
 
 
        int v19 = 0;
        /*if (globalVars->curtime <= next_lby_update(aa)) {
            v19 = m_iJitter;
        }
        else {*/
            m_iJitter = 0;
      //  }
 
        int v20 = v19 - 1;
        if (v20) {
            if (v20 == 1) {
                if (jitter_side == 1)
                    aa += lby_delta;
                else
                    aa += desync - 51.0f;
       //         g::bSendPacket = false;
            }
        }
        else {
            if (jitter_side == 1)
                aa += desync - 51.0f;
            else
                aa += lby_delta;
          //  g::bSendPacket = false;
        }
 
        if (++m_iJitter >= 3)
            m_iJitter = 0;
        aa += desync - 51.0f;
    }
	return aa;
}
static void DoAntiAimY(QAngle& angle, bool& clamp)
{
	AntiAimType_Y aa_type = Settings::AntiAim::Yaw::type;

	static bool yFlip;
	yFlip = !yFlip;
	float temp;
	double factor;
	static float trigger;
	QAngle temp_qangle;
	int random;
	int maxJitter;
	static bool yFlip1;

	switch (aa_type)
	{
		case AntiAimType_Y::SPIN_FAST:
			factor =  360.0 / M_PHI;
			factor *= 15;
			angle.y = fmodf(globalVars->curtime * factor, 360.0);
			if(yFlip1)
			angle.y -= 180;
			break;
		case AntiAimType_Y::SPIN_SLOW:
			factor =  360.0 / M_PHI;
			angle.y = fmodf(globalVars->curtime * factor, 360.0);
			break;
		case AntiAimType_Y::JITTER:
			yFlip1 ? angle.y -= 90.0f : angle.y -= 270.0f;
			break;
		case AntiAimType_Y::BACKJITTER:
			angle.y -= 180;
			random = rand() % 100;
			maxJitter = rand() % (85 - 70 + 1) + 70;
			temp = maxJitter - (rand() % maxJitter);
			if (random < 35 + (rand() % 15))
				angle.y -= temp;
			else if (random < 85 + (rand() % 15 ))
				angle.y += temp;
			break;
		case AntiAimType_Y::SIDE:
			yFlip1 ? angle.y += 90.f : angle.y -= 90.0f;
			break;
		case AntiAimType_Y::BACKWARDS:
			angle.y -= 180.0f;
			break;
		case AntiAimType_Y::FORWARDS:
			angle.y -= 0.0f;
			break;
		case AntiAimType_Y::LEFT:
			angle.y += 90.0f;
			break;
		case AntiAimType_Y::RIGHT:
			angle.y -= 90.0f;
			break;
		case AntiAimType_Y::STATICAA:
			angle.y = 0.0f;
			break;
		case AntiAimType_Y::STATICJITTER:
			trigger += 15.0f;
			angle.y = trigger > 50.0f ? 150.0f : -150.0f;

			if (trigger > 100.0f)
				trigger = 0.0f;
			break;
		case AntiAimType_Y::STATICSMALLJITTER:
			trigger += 15.0f;
			angle.y = trigger > 50.0f ? -30.0f : 30.0f;

			if (trigger > 100.0f)
				trigger = 0.0f;
			break;
		case AntiAimType_Y::LISP:
			clamp = false;
			yFlip1 ? angle.y += 323210.0f : angle.y -= 323210.0f;
			break;
		case AntiAimType_Y::LISP_SIDE:
			clamp = false;
			temp = angle.y + 90.0f;
			temp_qangle = QAngle(0.0f, temp, 0.0f);
			Math::NormalizeAngles(temp_qangle);
			temp = temp_qangle.y;

			if (temp > -45.0f)
				temp < 0.0f ? temp = -90.0f : temp < 45.0f ? temp = 90.0f : temp = temp;

			temp += 2984194.0f;
			angle.y = temp;
			break;
		case AntiAimType_Y::LISP_JITTER:
			clamp = false;
			temp = angle.y - 155.0f;

			if (globalVars->tickcount % 2)
			{
				temp = angle.y + 58.0f;
				temp *= 360;//6012.4f;
			}

			angle.y = temp;
			break;
		case AntiAimType_Y::ANGEL_BACKWARD:
			clamp = false;
			angle.y += 36180.0f;
			break;
		case AntiAimType_Y::ANGEL_INVERSE:
			clamp = false;
			angle.y = 36180.0f;
			break;
		case AntiAimType_Y::LOWERBODY:
			angle.y = *((C_BasePlayer*)entityList->GetClientEntity(engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + rand()%35 + 165;
			break;
		case AntiAimType_Y::ANGEL_SPIN:
			clamp = false;
			factor = (globalVars->curtime * 5000.0f);
			angle.y = factor + 1491109.0f;
			break;
		case AntiAimType_Y::CASUAL:
			yFlip1 ? angle.y -= 35.0f : angle.y += 35.0f;
			break;
		case AntiAimType_Y::LBYONGROUND:
			static C_BasePlayer* player = ((C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer()));
			if (player->GetFlags() & FL_ONGROUND)
				angle.y = *((C_BasePlayer*)entityList->GetClientEntity(engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + rand()%35 + 165;
			else
			{
				static int aimType = rand() % 4;
				switch (aimType)
				{
					case 1:
						yFlip ? angle.y += 90.f : angle.y -= 90.0f;
						break;
					case 2:
						yFlip ? angle.y -= 120.0f : angle.y -= 210.0f;
						break;
					case 3:
						factor =  360.0 / M_PHI;
						factor *= 25;
						angle.y = fmodf(globalVars->curtime * factor, 360.0);
						break;
					default:
						angle.y -= 180.0f;
				}
			}
			break;
		case AntiAimType_Y::spin_side_JITTER:
			factor =  360.0 / M_PHI;
			angle.y = fmodf(globalVars->curtime * factor, 360.0);
			if(yFlip1)
			angle.y += 180;	
			break;
		default:
			angle.y -= 0.0f;
			break;
	}
	yFlip1 = !yFlip1;
}

static void DoAntiAimX(QAngle& angle, bool& clamp)
{
	static float pDance = 0.0f;
	AntiAimType_X aa_type = Settings::AntiAim::Pitch::type;

	switch (aa_type)
	{
		case AntiAimType_X::STATIC_UP:
			angle.x = -89.0f;
			break;
		case AntiAimType_X::STATIC_DOWN:
			angle.x = 89.0f;
			break;
		case AntiAimType_X::DANCE:
			pDance += 45.0f;
			if (pDance > 100)
				pDance = 0.0f;
			else if (pDance > 75.f)
				angle.x = -89.f;
			else if (pDance < 75.f)
				angle.x = 89.f;
			break;
		case AntiAimType_X::FRONT:
			angle.x = 0.0f;
			break;
		case AntiAimType_X::LISP_DOWN:
			clamp = false;
			angle.x = 1981;
			break;
		case AntiAimType_X::ANGEL_DOWN:
			clamp = false;
			angle.x = -991;
			break;
		case AntiAimType_X::ANGEL_UP:
			clamp = false;
			angle.x = 180.01011;
			break;
		default:
			angle.x -= 0.0f;
			break;
	}
}

static void DoAntiAimFake(QAngle &angle, CCSGOAnimState* animState)
{
	if (!animState)
		return;
 	auto net_channel = engine->GetNetChannelInfo();
	float maxDelta = AntiAim::GetMaxDelta(animState);
	static bool yFlip = false;
	float factor;
	switch (Settings::AntiAim::Fake::type)
	{
		case AntiAimType_Fake::STATIC_LEFT:
			angle.y += maxDelta;
			break;

		case AntiAimType_Fake::STATIC_RIGHT:
			angle.y -= maxDelta;
			break;

		case AntiAimType_Fake::JITTER:
			if (net_channel->m_nChokedPackets >= 5) {
				angle.y += maxDelta;
			}
			else{
				angle.y -= (maxDelta + 30);
			}
			break;
		case AntiAimType_Fake::spin:
			angle.y = polak(2, maxDelta,angle.y);
			break;
		case AntiAimType_Fake::test:
			if (net_channel->m_nChokedPackets >= 5) {
				angle.y += 291441;
			}
			else{
				angle.y -= 294141;
			}
			break;
	}
	yFlip = !yFlip;
}

void AntiAim::CreateMove(CUserCmd* cmd)
{
	if (!Settings::AntiAim::Yaw::enabled && !Settings::AntiAim::Pitch::enabled)
		return;

	if (Settings::Aimbot::AimStep::enabled && Aimbot::aimStepInProgress)
		return;

	QAngle oldAngle = cmd->viewangles;
	float oldForward = cmd->forwardmove;
	float oldSideMove = cmd->sidemove;

	QAngle angle = cmd->viewangles;

	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
	if (!localplayer)
		return;

	C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*) entityList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
	if (!activeWeapon)
		return;

	CCSGOAnimState* animState = localplayer->GetAnimState();
	if (!animState)
		return;

	if (activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_GRENADE)
	{
		C_BaseCSGrenade* csGrenade = (C_BaseCSGrenade*) activeWeapon;

		if (csGrenade->GetThrowTime() > 0.f)
			return;
	}

	if (cmd->buttons & IN_USE || cmd->buttons & IN_ATTACK || (cmd->buttons & IN_ATTACK2 && (*activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_REVOLVER || activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_KNIFE)))
		return;

	if (localplayer->GetMoveType() == MOVETYPE_LADDER || localplayer->GetMoveType() == MOVETYPE_NOCLIP)
		return;

	// AutoDisable checks

	// Knife
	if (Settings::AntiAim::AutoDisable::knifeHeld && localplayer->GetAlive() && activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_KNIFE)
		return;

	if (Settings::AntiAim::AutoDisable::noEnemy && localplayer->GetAlive() && !HasViableEnemy())
		return;

	QAngle edge_angle = angle;
	bool freestanding = Settings::AntiAim::FreeStanding::enabled && GetBestHeadAngle(edge_angle);
	static bool bSend1 = true;
	bSend1 = !bSend1;
	static bool bSend = true;
	bSend = CreateMove::sendPacket;//!bSend;

	bool should_clamp = true;

	if (!ValveDSCheck::forceUT && (*csGameRules) && (*csGameRules)->IsValveDS())
	{
		if (Settings::AntiAim::Yaw::type >= AntiAimType_Y::LISP)
			Settings::AntiAim::Yaw::type = AntiAimType_Y::SPIN_SLOW;

		if (Settings::AntiAim::Pitch::type >= AntiAimType_X::LISP_DOWN)
			Settings::AntiAim::Pitch::type = AntiAimType_X::STATIC_UP;
	}

	if (Settings::AntiAim::Yaw::enabled)
	{
		DoAntiAimY(angle, should_clamp);

		if (freestanding)
			angle.y = edge_angle.y;

      //  Math::NormalizeAngles(angle);
	}

	if (Settings::AntiAim::Fake::enabled && !bSend)
    {
	    DoAntiAimFake(angle, animState);
    //    Math::NormalizeAngles(angle);
    }

	if (Settings::AntiAim::Pitch::enabled)
		DoAntiAimX(angle, should_clamp);

	if (should_clamp)
	{
		//Math::NormalizeAngles(angle);
		Math::clamp_angles_skeet(angle);
	}
	//	Math::clamp_angles_skeet(angle);
///	CreateMove::sendPacket = bSend;
	if (bSend)
	    real = angle;
    else
        fake = angle;

	

	cmd->viewangles = angle;
	
	Math::CorrectMovement(oldAngle, cmd, oldForward, oldSideMove);
}

