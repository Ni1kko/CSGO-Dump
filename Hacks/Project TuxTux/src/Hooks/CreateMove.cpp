#include "hooks.h"

bool CreateMove::sendPacket = true;

typedef bool (*SendNetMsgFn) (INetChannela*,INetMessage*, bool, bool );
bool SendNetMsg(INetChannela* netchan,INetMessage* &msg, bool bForceReliable = false, bool bVoice = false){
    if (strcmp("clc_Move", msg->GetName()) == 0 && inputSystem->IsButtonDown(KEY_C)) {//u can use group&&type aswell
		
        
		uiEngineVMT->GetOriginalMethod<SendNetMsgFn>(43)(netchan,msg,bForceReliable,bVoice);
		engine->GetNetChannelInfoa()->m_nOutSequenceNr = 1;
      //  return false;
    }
	else{
		engine->GetNetChannelInfoa()->m_nOutSequenceNr = 1;
		 return uiEngineVMT->GetOriginalMethod<SendNetMsgFn>(43)(netchan,msg,bForceReliable,bVoice);
	}

	
}
int Settings::Aimbot::Slowwalka = 20;
//Settings::Aimbot::Slowwalka
void VectorAngles(Vector forward, Vector angles)
{
	//Assert(s_bMathlibInitialized);
	float	tmp, yaw, pitch;
 
	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;
 
		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}
 
	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}
void slow_walk(CUserCmd *cmd)
{
    if (!inputSystem->IsButtonDown(KEY_LSHIFT))
        return;
	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
	if (!localplayer)
		return;
    auto weapon_handle = localplayer->GetActiveWeapon( );
	
	C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*) entityList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
	if (!activeWeapon)
		return;
    if (!weapon_handle)
        return;
 
    float amount = 0.0034f * Settings::Aimbot::Slowwalka; // options.misc.slow_walk_amount has 100 max value
 
    Vector velocity = localplayer->GetVelocity( );
    Vector direction;
 
   	VectorAngles(velocity, direction);
 
    float speed = velocity.Length2D( );
 
    direction.y = cmd->viewangles.y - direction.y;
 
    Vector forward;
 
    VectorAngles(direction, forward);
 
    Vector source = forward * -speed;
 
    if (speed >= (activeWeapon->GetCSWpnData()->GetMaxPlayerSpeed() * amount))
    {
        cmd->forwardmove = source.x;
        cmd->sidemove = source.y;
 
    }
/*	static int choked = 0;
    choked = choked > 7 ? 0 : choked + 1;
    cmd->forwardmove = choked < 2 || choked > 5 ? 0 : cmd->forwardmove;
    cmd->sidemove = choked < 2 || choked > 5 ? 0 : cmd->sidemove*/;
  //  CreateMove::sendPacket = choked < 1;
}


bool Hooks::CreateMove(void* thisptr, float flInputSampleTime, CUserCmd* cmd)
{
	clientModeVMT->GetOriginalMethod<CreateMoveFn>(25)(thisptr, flInputSampleTime, cmd);

	if (cmd && cmd->command_number)
	{
        // Special thanks to Gre-- I mean Heep ( https://www.unknowncheats.me/forum/counterstrike-global-offensive/290258-updating-bsendpacket-linux.html )
        uintptr_t rbp;
        asm volatile("mov %%rbp, %0" : "=r" (rbp));
        bool *sendPacket = ((*(bool **)rbp) - 0x18);
        *sendPacket = CreateMove::sendPacket;
        CreateMove::sendPacket = true;
		static bool onetime;
		if(!onetime && engine->GetNetChannelInfoa()){

		
		 /* uiEngineVMT = new VMT(engine->GetNetChannelInfoa());
 	//    uiEngineVMT->HookVM((void*)Hooks::RunScript, 110);
  	//    uiEngineVMT->HookVM((void*)Hooks::CreatePanel, 140);
  		  uiEngineVMT->HookVM((void*)SendNetMsg, 43);
  		  uiEngineVMT->ApplyVMT();*/
		  onetime = true;
		}
		/* run code that affects movement before prediction */
		Aimbot::PrePredictionCreateMove(cmd);
		BHop::CreateMove(cmd);
		AutoStrafe::CreateMove(cmd);
		ShowRanks::CreateMove(cmd);
		AutoDefuse::CreateMove(cmd);
		JumpThrow::CreateMove(cmd);
		EdgeJump::PrePredictionCreateMove(cmd);
		Autoblock::CreateMove(cmd);

		PredictionSystem::StartPrediction(cmd);
		{
			Aimbot::CreateMove(cmd);
			Triggerbot::CreateMove(cmd);
			AutoKnife::CreateMove(cmd);
			GrenadePrediction::CreateMove(cmd);
			FakeLag::CreateMove(cmd);
			if(!Settings::FakeLag::enabled)
			CreateMove::sendPacket = cmd->tick_count % 2;
			AntiAim::CreateMove(cmd);
			ESP::CreateMove(cmd);
			LogShots::CreateMove(cmd);
		}
		PredictionSystem::EndPrediction();
		slow_walk(cmd);
	/*	if (inputSystem->IsButtonDown(KEY_C)){
		engine->GetNetChannelInfoa()->RequestFile(".txt",false);
		engine->GetNetChannelInfoa()->RequestFile(".",false);
		}*/
		
//		{
		//	engine_p->SetTimescale(50);
//		}

		EdgeJump::PostPredictionCreateMove(cmd);
		ClanTagChanger::CreateMove();
	}

	return false;
}
