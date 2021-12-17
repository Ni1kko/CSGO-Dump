#pragma once
#include "stdafx.h"
#include <chrono>
#include "../../asd.h"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

class Misc
{
	typedef bool ( __cdecl* ServerRankRevealAllFn)( float* );

public:
	static vec_t Normalize_y( vec_t ang )
	{
		while( ang < -180.0f )
			ang += 360.0f;
		while( ang > 180.0f )
			ang -= 360.0f;
		return ang;
	}

	static void setName(const char* name)
	{
		auto namevar = Interfaces.g_ICVars->FindVar("name");
		*reinterpret_cast< int* >(reinterpret_cast< DWORD >(&namevar->fnChangeCallback) + 0xC) = 0;
		namevar->SetValue(name);
	}

	static void SetClanTag( const char* tag, const char* name )
	{
		static auto pSetClanTag = reinterpret_cast< void(__fastcall*)( const char*, const char* ) >( ( DWORD )( Utils.FindPattern( "engine.dll", "53 56 57 8B DA 8B F9 FF 15" ) ) );
		pSetClanTag( tag, name );
	}

	static int TIME_TO_TICKS( int dt )
	{
		return ( int )( 0.5f + ( float )( dt ) / Interfaces.pGlobalVars->interval_per_tick );
	}

	static float GetNetworkLatency()
	{
		INetChannelInfo* nci = Interfaces.pEngine->GetNetChannelInfo();
		if( nci )
		{
			return nci->GetAvgLatency( FLOW_OUTGOING );
		}
		return 0.0f;
	}

	static void BunnyHop()
	{
		if (Vars.Misc.Bunnyhop)
		{
			static auto bJumped = false;
			static auto bFake = false;
			if (!bJumped && bFake)
			{
				bFake = false;
				Hacks.CurrentCmd->buttons |= IN_JUMP;
			}
			else if (Hacks.CurrentCmd->buttons & IN_JUMP)
			{
				if (Hacks.LocalPlayer->GetFlags() & FL_ONGROUND)
				{
					bJumped = true;
					bFake = true;
				}
				else
				{
					Hacks.CurrentCmd->buttons &= ~IN_JUMP;
					bJumped = false;
				}
			}
			else
			{
				bJumped = false;
				bFake = false;
			}
		}
	}

	static void AutoStrafe()
	{
		if (Vars.Misc.Strafers.Enable)
		{
			bool bKeysPressed = true;
			if (GetAsyncKeyState(0x41) || GetAsyncKeyState(0x57) || GetAsyncKeyState(0x53) || GetAsyncKeyState(0x44))
				bKeysPressed = false;

			if ((GetAsyncKeyState(VK_SPACE) && !(Hacks.LocalPlayer->GetFlags() & FL_ONGROUND)) && bKeysPressed)
			{
				Hacks.CurrentCmd->forwardmove = (1550.f * 5) / Hacks.LocalPlayer->GetVecVelocity().Length2D();
				Hacks.CurrentCmd->sidemove = (Hacks.CurrentCmd->command_number % 2) == 0 ? -450.f : 450.f;
				if (Hacks.CurrentCmd->forwardmove > 450.f)
					Hacks.CurrentCmd->forwardmove = 450.f;
			}
		}
	}

	static void ClampAngles()
	{
		if (Vars.Misc.AntiUT)
		{
			Misc::Normalize(Hacks.CurrentCmd->viewangles);
			if (Hacks.CurrentCmd->forwardmove > 450)
				Hacks.CurrentCmd->forwardmove = 450;
			if (Hacks.CurrentCmd->forwardmove < -450)
				Hacks.CurrentCmd->forwardmove = -450;
			if (Hacks.CurrentCmd->sidemove > 450)
				Hacks.CurrentCmd->sidemove = 450;
			if (Hacks.CurrentCmd->sidemove < -450)
				Hacks.CurrentCmd->sidemove = -450;
		}
	}

	static void PVSFix()
	{
		for (auto i = 0; i < 65; ++i)
		{
			auto entity = Interfaces.pEntList->GetClientEntity(i);
			if (!entity
				|| !entity->IsValid(false)
				|| !Hacks.LocalPlayer)
				continue;

			//static DWORD m_nWritableBones = Netvar("DT_BaseAnimating", "m_nForceBone");

			*(int*)((uintptr_t)entity + 0xA30) = Interfaces.pGlobalVars->framecount; //we'll skip occlusion checks now
			*(int*)((uintptr_t)entity + 0xA28) = 0;//clear occlusion flags
		}

	}

	static void AnimFix()
	{
		auto entity = Hacks.LocalPlayer;

		//start
		auto OldCurtime = Interfaces.pGlobalVars->curtime;
		auto OldFrametime = Interfaces.pGlobalVars->frametime;
		auto OldFraction = entity->GetAnimState()->m_flUnknownFraction = 0;

		Interfaces.pGlobalVars->curtime = entity->GetSimulationTime();
		Interfaces.pGlobalVars->frametime =
			Interfaces.pGlobalVars->interval_per_tick *
			Interfaces.g_ICVars->FindVar("host_timescale")->GetFloat();

		CAnimationLayer Layers[15];
		std::memcpy(Layers, entity->GetAnimOverlays(), (sizeof(CAnimationLayer) * entity->GetNumAnimOverlays()));

		if (entity->GetAnimState())
			entity->GetAnimState()->m_iLastClientSideAnimationUpdateFramecount = Interfaces.pGlobalVars->framecount - 1;

		entity->GetClientSideAnimation() = true;
		entity->UpdateClientSideAnimation();
		entity->GetClientSideAnimation() = false;

		//end
		std::memcpy(entity->GetAnimOverlays(), Layers, (sizeof(CAnimationLayer) * entity->GetNumAnimOverlays()));

		Interfaces.pGlobalVars->curtime = OldCurtime;
		Interfaces.pGlobalVars->frametime = OldFrametime;

		entity->GetAnimState()->m_flUnknownFraction = OldFraction;
	}

	static double GetNumberOfTicksChoked( CBaseEntity* pEntity )
	{
		double flSimulationTime = pEntity->GetSimulationTime();
		double flSimDiff = ( ( double )Hacks.LocalPlayer->GetTickBase() * Interfaces.pGlobalVars->interval_per_tick ) - flSimulationTime;
		return TIME_TO_TICKS( max(0.0f, flSimDiff) );
	}

	static bool Shooting()
	{
		if (Hacks.LocalWeapon->IsKnife())
		{
			if (Hacks.CurrentCmd->buttons & IN_ATTACK || Hacks.CurrentCmd->buttons & IN_ATTACK2)
				return true;
		}
		else if (Hacks.LocalWeapon->IsNade())
		{
			CBaseCSGrenade* csGrenade = (CBaseCSGrenade*)Hacks.LocalWeapon;
			if (csGrenade->GetThrowTime() > 0.f)
			{
				return true;
			}
		}
		else if (Hacks.CurrentCmd->buttons & IN_ATTACK && bullettime())
		{
			if (*Hacks.LocalWeapon->GetItemDefinitionIndex() == WEAPON_REVOLVER && Vars.Ragebot.Enable)
			{
				if (Hacks.LocalWeapon->GetPostponeFireReadyTime() - GetServerTime() <= 0.05f)
				{
					return true;
				}
			}
			else
				return true;
		}
		else if (*Hacks.LocalWeapon->GetItemDefinitionIndex() == WEAPON_C4)
			return false;
		return false;
	}

	static void DrawScope()
	{
		if( Vars.Visuals.Effects.NoScope && Hacks.LocalWeapon )
			if( Hacks.LocalWeapon->isSniper() )
				if( Hacks.LocalPlayer->m_bIsScoped() )
				{
					int width = 0;
					int height = 0;
					Interfaces.pEngine->GetScreenSize( width, height );
					DrawRect( ( width / 2 ), 0, ( width / 2 ) + 1, height, CColor( 0, 0, 0, 255 ) );
					DrawRect( 0, ( height / 2 ), width, ( height / 2 ) + 1, CColor( 0, 0, 0, 255 ) );
				}
	}

	static void CircleStrafer( float& OldAngle )
	{
		static int Angle = 0;
		if( OldAngle - Angle > 360 )
			Angle -= 360;
		static bool shouldspin = false;
		static bool enabled = false;
		static bool check = false;
		if( GetAsyncKeyState(Vars.Misc.Strafers.CircleStrafeKey))
		{
			if( !check )
				enabled = !enabled;
			check = true;
		}
		else
			check = false;
		if( enabled )
		{
			shouldspin = true;
			Hacks.CurrentCmd->buttons |= IN_JUMP;
			Hacks.CurrentCmd->buttons |= IN_DUCK;
		}
		if( shouldspin )
		{
			Vector Dir;
			g_Math.angleVectors( Vector( 0, Angle, 0 ), Dir );
			Dir *= 8218;
			Ray_t ray;
			CTraceWorldOnly filter;
			trace_t trace;
			ray.Init( Hacks.LocalPlayer->GetEyePosition(), Hacks.LocalPlayer->GetVecOrigin() + Dir );
			Interfaces.pTrace->TraceRay( ray, MASK_SHOT, &filter, &trace );
			auto temp = 3.4f / ( ( trace.endpos - Hacks.LocalPlayer->GetVecOrigin() ).Length() / 100.f );
			if( temp < 3.4f )
				temp = 3.4f;
			if( enabled )
			{
				Angle += temp;
				Hacks.CurrentCmd->sidemove = -450;
			}
			else
			{
				if( OldAngle - Angle < temp )
				{
					Angle = OldAngle;
					shouldspin = false;
				}
				else
					Angle += temp;
			}
			OldAngle = Angle;
		}
		else
			Angle = OldAngle;
	}

	static Vector Normalize( Vector& angs )
	{
		while( angs.y < -180.0f )
			angs.y += 360.0f;
		while( angs.y > 180.0f )
			angs.y -= 360.0f;
		if( angs.x > 89.0f )
			angs.x = 89.0f;
		if( angs.x < -89.0f )
			angs.x = -89.0f;
		angs.z = 0;
		return angs;
	}

	static bool AimStep( Vector angSource, Vector& angDestination )
	{
		Vector angDelta = Normalize( angDestination - angSource );
		if( angDelta.Abs() > 40.f )
		{
			angDestination = Normalize( angSource + angDelta / angDelta.Abs() * 40.f );
			return false;
		}
		return true;
	}

	static float GetServerTime()
	{
		return ( float )Hacks.LocalPlayer->GetTickBase() * Interfaces.pGlobalVars->interval_per_tick;
	}

	static bool bullettime()
	{
		if( !Hacks.LocalWeapon )
			return false;
		float flNextPrimaryAttack = Hacks.LocalWeapon->NextPrimaryAttack();

		return flNextPrimaryAttack <= GetServerTime();
	}

	static void MoveFix()
	{
		Vector realvec;
		Interfaces.pEngine->GetViewAngles(realvec);

		Vector vMove(Hacks.CurrentCmd->forwardmove, Hacks.CurrentCmd->sidemove, Hacks.CurrentCmd->upmove);
		float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
		Vector vMove2;
		g_Math.vectorAnglesVec(vMove, vMove2);
		Normalize(vMove2);
		flYaw = DEG2RAD(Hacks.CurrentCmd->viewangles.y - realvec.y + vMove2.y);
		Hacks.CurrentCmd->forwardmove = cos(flYaw) * flSpeed;
		Hacks.CurrentCmd->sidemove = sin(flYaw) * flSpeed;
		if (Hacks.CurrentCmd->viewangles.x >= 180 && Hacks.CurrentCmd->viewangles.x <= 271)
			Hacks.CurrentCmd->forwardmove = -Hacks.CurrentCmd->forwardmove;

		/*cmd->buttons &= ~IN_RIGHT;
		cmd->buttons &= ~IN_MOVERIGHT;
		cmd->buttons &= ~IN_LEFT;
		cmd->buttons &= ~IN_MOVELEFT;
		cmd->buttons &= ~IN_FORWARD;
		cmd->buttons &= ~IN_BACK;

		if (cmd->forwardmove > 0.f)
			cmd->buttons |= IN_FORWARD;
		else if (cmd->forwardmove < 0.f)
			cmd->buttons |= IN_BACK;

		if (cmd->sidemove > 0.f)
		{
			cmd->buttons |= IN_RIGHT;
			cmd->buttons |= IN_MOVERIGHT;
		}
		else if (cmd->sidemove < 0.f)
		{
			cmd->buttons |= IN_LEFT;
			cmd->buttons |= IN_MOVELEFT;
		}*/
	}

	static void CalcAngle( Vector src, Vector dst, Vector& angles )
	{
		Vector delta = src - dst;
		double hyp = delta.Length2D();
		angles.y = atan( delta.y / delta.x ) * 57.295779513082f;
		angles.x = atan( delta.z / hyp ) * 57.295779513082f;
		if( delta.x >= 0.0 )
			angles.y += 180.0f;
		angles.z = 0;
	}

	static int FovTo( Vector From, Vector To )
	{
		From -= To;
		Normalize( From );
		return ( abs( From.y ) + abs( From.x ) );
	}

	static void CalcAngle2(const Vector& vecSource, const Vector& vecDestination, Vector& qAngles)
	{
		Vector delta = Vector((vecSource[0] - vecDestination[0]), (vecSource[1] - vecDestination[1]), (vecSource[2] - vecDestination[2]));
		double hyp = sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
		qAngles[0] = (float)(atan(delta[2] / hyp) * (180.0 / M_PI));
		qAngles[1] = (float)(atan(delta[1] / delta[0]) * (180.0 / M_PI));
		qAngles[2] = 0.f;
		if (delta[0] >= 0.f)
			qAngles[1] += 180.f;
	}

	static void NormalizeVector(Vector& vec)
	{
		for (int i = 0; i < 3; ++i)
		{
			while (vec[i] > 180.f)
				vec[i] -= 360.f;

			while (vec[i] < -180.f)
				vec[i] += 360.f;
		}
		vec[2] = 0.f;
	}

	static void SinCos(float a, float* s, float*c)
	{
		*s = sin(a);
		*c = cos(a);
	}

	static void AngleVectors2(const Vector& qAngles, Vector& vecForward)
	{
		float sp, sy, cp, cy;
		SinCos((float)(qAngles[1] * (M_PI / 180.f)), &sy, &cy);
		SinCos((float)(qAngles[0] * (M_PI / 180.f)), &sp, &cp);

		vecForward[0] = cp*cy;
		vecForward[1] = cp*sy;
		vecForward[2] = -sp;
	}

	static void VectorAngles2(const Vector &vecForward, Vector &vecAngles)
	{
		Vector vecView;
		if (vecForward[1] == 0.f && vecForward[0] == 0.f)
		{
			vecView[0] = 0.f;
			vecView[1] = 0.f;
		}
		else
		{
			vecView[1] = atan2(vecForward[1], vecForward[0]) * 180.f / M_PI;

			if (vecView[1] < 0.f)
				vecView[1] += 360.f;

			vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

			vecView[0] = atan2(vecForward[2], vecView[2]) * 180.f / M_PI;
		}

		vecAngles[0] = -vecView[0];
		vecAngles[1] = vecView[1];
		vecAngles[2] = 0.f;
	}

	static void AngleVectors3(const Vector& qAngles, Vector& vecForward)
	{
		float sp, sy, cp, cy;
		SinCos((float)(qAngles[1] * (M_PI / 180.f)), &sy, &cy);
		SinCos((float)(qAngles[0] * (M_PI / 180.f)), &sp, &cp);

		vecForward[0] = cp*cy;
		vecForward[1] = cp*sy;
		vecForward[2] = -sp;
	}

	static void VectorAngles3(const Vector &vecForward, Vector &vecAngles)
	{
		Vector vecView;
		if (vecForward[1] == 0.f && vecForward[0] == 0.f)
		{
			vecView[0] = 0.f;
			vecView[1] = 0.f;
		}
		else
		{
			vecView[1] = atan2(vecForward[1], vecForward[0]) * 180.f / M_PI;

			if (vecView[1] < 0.f)
				vecView[1] += 360.f;

			vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

			vecView[0] = atan2(vecForward[2], vecView[2]) * 180.f / M_PI;
		}

		vecAngles[0] = -vecView[0];
		vecAngles[1] = vecView[1];
		vecAngles[2] = 0.f;
	}
};
