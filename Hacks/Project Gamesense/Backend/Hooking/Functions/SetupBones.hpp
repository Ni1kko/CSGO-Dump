#pragma once
#include "../Hooks/Hooks.h"

#include "../../Include/GlobalIncludes.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/CUserCmd.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/ISurface.h"
#include "../../Features/Visuals/Visuals.h"
#include "../../SDK/ModelRender.h"
#include "../../SDK/CMaterialHelper.h"
#include "../../DLL/Engine.h"
#include "../../SDK/ModelRender.h"

#include "../../../Frontend/Menu/Menu.h"

namespace Cheat
{
	namespace Hooked
	{
		bool __fastcall hooked_setupbones( void* ecx, void* edx, matrix3x4_t* bone_world_out, int max_bones, int bone_mask, float current_time )
		{
			//if (ecx && ((IClientRenderable*)ecx)->GetIClientUnknown()) {
			//	IClientNetworkable* pNetworkable = ((IClientRenderable*)ecx)->GetIClientUnknown()->GetClientNetworkable();
			//	if (pNetworkable && pNetworkable->GetClientClass() && pNetworkable->GetClientClass()->m_ClassID == 35) {
			//		static auto host_timescale = Interfaces::Var->FindVar(("host_timescale"));
			//		auto player = (CBaseEntity*)ecx;
			//		float OldCurTime = Interfaces::Globals->curtime;
			//		float OldRealTime = Interfaces::Globals->realtime;
			//		float OldFrameTime = Interfaces::Globals->frametime;
			//		float OldAbsFrameTime = Interfaces::Globals->absoluteframestarttimestddev;
			//		float OldAbsFrameTimeStart = Interfaces::Globals->absoluteframestarttimestddev;
			//		float OldInterpAmount = Interfaces::Globals->interpolation_amount;
			//		int OldFrameCount = Interfaces::Globals->framecount;
			//		int OldTickCount = Interfaces::Globals->tickcount;
			//		Interfaces::Globals->curtime = player->GetSimTime();
			//		Interfaces::Globals->realtime = player->GetSimTime();
			//		Interfaces::Globals->frametime = Interfaces::Globals->interval_per_tick * host_timescale->GetFloat();
			//		Interfaces::Globals->absoluteframestarttimestddev = Interfaces::Globals->interval_per_tick * host_timescale->GetFloat();
			//		Interfaces::Globals->absoluteframestarttimestddev = player->GetSimTime() - Interfaces::Globals->interval_per_tick * host_timescale->GetFloat();
			//		Interfaces::Globals->interpolation_amount = 0;
			//		Interfaces::Globals->framecount = TIME_TO_TICKS(player->GetSimTime()); // Wrong if backtracking.
			//		Interfaces::Globals->tickcount = TIME_TO_TICKS(player->GetSimTime());
			//		//*( int* ) ( ( int ) player + 224 ) |= 8;
			//		*(int*)((int)player + 236) |= 8; // IsNoInterpolationFrame
			//		bool ret_value = original_setupbones(player, bone_world_out, max_bones, bone_mask, Interfaces::Globals->curtime);
			//		*(int*)((int)player + 236) &= ~8; // (1 << 3)
			//												//*( int* ) ( ( int ) player + 224 ) &= ~8;
			//		Interfaces::Globals->curtime = OldCurTime;
			//		Interfaces::Globals->realtime = OldRealTime;
			//		Interfaces::Globals->frametime = OldFrameTime;
			//		Interfaces::Globals->absoluteframestarttimestddev = OldAbsFrameTime;
			//		Interfaces::Globals->absoluteframestarttimestddev = OldAbsFrameTimeStart;
			//		Interfaces::Globals->interpolation_amount = OldInterpAmount;
			//		Interfaces::Globals->framecount = OldFrameCount;
			//		Interfaces::Globals->tickcount = OldTickCount;
			//		return ret_value;
			//	}
			//}
			//return original_setupbones(ecx, bone_world_out, max_bones, bone_mask, current_time);
			return false;
		}
	}
}