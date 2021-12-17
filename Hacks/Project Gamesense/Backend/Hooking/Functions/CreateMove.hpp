#pragma once
#include "../../../Frontend/Menu/cmenu.h"

#include "../../Include/GlobalIncludes.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/CUserCmd.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../Features/Ragebot/c_ragebot.h"
#include "../../SDK/CBaseEntity.h"
#include "../../Features/Misc/Misc.h"
#include "../../Features/Legitbot/c_legitbot.h"
#include "../../Features/Legitbot/c_triggerbot.h"
#include "../../Features/Legitbot/c_legit_backtrack.h"
#include "../../Features/Misc/LagCompensation.h"
#include "../../Features/Ragebot/AntiAims.h"
#include "../../Features/Ragebot/Resolver.h"
#include "../Hooks/Hooks.h"
#include "..//..//Features/Ragebot/c_backtrack.h"
#include "SendDataGram.hpp"
#include "../../Features/SkinChanger/skinchanger2.h"

#include "../../Lua/CLua.h"
#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / Interfaces::Globals->interval_per_tick ) )

typedef bool(__thiscall* fnCreateMove)(IClientMode*, float, CUserCmd*);
fnCreateMove oCreateMove = nullptr;
bool netchannel_hook1 = false;
bool tagreset = false;

namespace Cheat
{
	namespace Hooked
	{

		bool __stdcall CreateMoveHook(float flInputSampleTime, CUserCmd* UserCmd)
		{
			if (Cheat::Settings->UnloadCheat)
			{
				Cheat::Settings->UnloadReadyTable.CreateMove = 0;
				return oCreateMove(Interfaces::ClientMode, flInputSampleTime, UserCmd);
			}
			Cheat::Settings->UnloadReadyTable.CreateMove = (int)GetTickCount();

			oCreateMove(Interfaces::ClientMode, flInputSampleTime, UserCmd);
			/*if (!UserCmd || !Interfaces::Engine->IsInGame() || !Interfaces::Engine->IsConnected() || !UserCmd->tick_count)
				return false;*/
			if (!UserCmd || !UserCmd->command_number)
				return true;

			G::LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayerIndex());

			if (!G::LocalPlayer)
				return true;

			static Hooking::VMTHook* playerHooks;
			static bool hooked = false;
			auto ent = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayerIndex());
			auto unhook = false;


			if (c_menu::get()->is_menu_opened())
				UserCmd->buttons &= ~(IN_ATTACK | IN_ATTACK2);

			if (c_config::get()->b["misc_clantagspammer"]) {
				static int oldcurtime = 0;
				if (oldcurtime != (int)(Interfaces::Globals->curtime * 3.3f) % 28) {
					Features::Misc->ClanTagChanger();
					oldcurtime = (int)(Interfaces::Globals->curtime * 3.3f) % 28;
				}


				tagreset = false;
			}
			else {
				if (!tagreset)
					Features::Misc->SetClanTag("", "");

				tagreset = true;
			}

			if (c_config::get()->b["misc_pingspike"]) {
				if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame())
					g_BacktrackHelper->ClearIncomingSequences();

				g_BacktrackHelper->AddLatencyToNetchan(Interfaces::ClientState->pNetChannel, c_config::get()->i["misc_pingspike_val"]);
			}

			if (c_config::get()->b["rage_doubletap"] && UserCmd->buttons & IN_ATTACK)
			{
				int doubletapTickcountDelta = Interfaces::Globals->tickcount - G::lastDoubleTapInTickcount;

				if (doubletapTickcountDelta >= 64) {

					G::lastDoubleTapInTickcount = Interfaces::Globals->tickcount;

					G::TickbaseShift = 16;
				}

			}
			else if (c_config::get()->i["rage_exploit"] == 1 && UserCmd->buttons & IN_ATTACK)
			{
				int doubletapTickcountDelta = Interfaces::Globals->tickcount - G::lastDoubleTapInTickcount;

				if (doubletapTickcountDelta >= 32) {

					G::lastDoubleTapInTickcount = Interfaces::Globals->tickcount;

					G::TickbaseShift = 12;
				}

			}

			if (c_config::get()->b["vis_viewmodel"]) {
				int viewX = c_config::get()->f["vis_veiwx"];

				ConVar* viewXcvar = Interfaces::Var->FindVar("viewmodel_offset_x");
				*(int*)((DWORD)&viewXcvar->fnChangeCallback + 0xC) = NULL;

				viewXcvar->SetValue(viewX);

				int viewY = c_config::get()->f["vis_veiwy"];

				ConVar* viewYcvar = Interfaces::Var->FindVar("viewmodel_offset_y");
				*(int*)((DWORD)&viewYcvar->fnChangeCallback + 0xC) = NULL;
				viewYcvar->SetValue(viewY);

				int viewZ = c_config::get()->f["vis_veiwz"];

				ConVar* viewZcvar = Interfaces::Var->FindVar("viewmodel_offset_z");
				*(int*)((DWORD)&viewZcvar->fnChangeCallback + 0xC) = NULL;

				viewZcvar->SetValue(viewZ);
			}
			G::UnpredictedVelocity = G::LocalPlayer->GetVelocity();

			if (G::LocalPlayer->IsAlive()) {

				if (G::LocalPlayer->GetActiveWeapon()) {
					auto w = G::LocalPlayer->GetActiveWeapon();
					G::VisualLastCone = w->GetSpreadCone();
					G::VisualLastInaccuracy = w->GetInaccuracy();
					G::VisualLastKnife = w->is_knife();
					G::VisualLastNade = w->is_grenade();
					G::VisualLastRange = w->get_full_info()->range;
				}

				Vector wish_angle = UserCmd->viewangles;
				G::UserCmd = UserCmd;

				Features::Misc->Run();
				if (c_config::get()->b["rage_fakeduck"])
					c_antiaims::get()->fake_duck();

				engineprediction::get()->start();
				if (c_config::get()->b["rage_autofire"] && c_config::get()->b["rage_aimbot"]) {
					if (G::LocalPlayer->GetActiveWeapon()->GetItemDefenitionIndex() == WEAPON_REVOLVER) {
						UserCmd->buttons |= IN_ATTACK;
						float flPostponeFireReady = G::LocalPlayer->GetActiveWeapon()->GetPostponeFireReadyTime();
						if (flPostponeFireReady > 0 && flPostponeFireReady < Interfaces::Globals->curtime) {
							UserCmd->buttons &= ~IN_ATTACK;
							if (flPostponeFireReady + Interfaces::Globals->interval_per_tick * 15 > Interfaces::Globals->curtime) {
								UserCmd->buttons |= IN_ATTACK2;
							}
						}
					}
				}
				//c_resolver::get()->OnCreateMove();
				bool canShoot = (G::LocalPlayer->GetActiveWeapon()->GetNextPrimaryAttack() <= Interfaces::Globals->curtime && G::LocalPlayer->GetActiveWeapon()->GetAmmoInClip() > 0);
				if (UserCmd->buttons & IN_ATTACK && canShoot)
					G::LastShoot = Interfaces::Globals->tickcount;

				c_ragebot::get()->create_move();
				if (!c_config::get()->b["rage_aimbot"])
				{
					auto pWeapon = G::LocalPlayer->GetActiveWeapon();
					if (pWeapon != nullptr && pWeapon->GetAmmoInClip() > 0)
					{
						g_triggerbot.OnMove(UserCmd);
						g_legitbot.OnMove(UserCmd);
						g_legit_backtrack.OnMove(UserCmd);
					}
				}

				if (c_config::get()->i["rage_doubletap_mode"] == 0 && c_keyhandler::get()->auto_check("rage_exploit_key") && !(UserCmd->buttons & IN_ATTACK) && G::IsDoubleTapShooting && G::LastShoot + 64 > Interfaces::Globals->curtime) {
					G::Recharge = 17;
					G::IsDoubleTapShooting = false;
				}
				if (c_config::get()->b["rage_quickpeek"])
					c_antiaims::get()->quickpeek(UserCmd);
				if (c_config::get()->b["misc_faststop"]) {
					c_antiaims::get()->fastStop(UserCmd);
				}
				if (c_config::get()->b["aa_enabled"]) {
					c_antiaims::get()->update_lowerbody_breaker();
					c_antiaims::get()->create_move();
					if (!(UserCmd->buttons & IN_USE) && !(G::LocalPlayer->GetMoveType() == MOVETYPE_LADDER) && !(G::LocalPlayer->GetActiveWeapon()->is_grenade())) {
						if (c_config::get()->i["rage_antiaim_lby"] == 0 || (c_config::get()->i["rage_antiaim_lby"] == 3 && c_config::get()->b["rage_doubletap"])) {
							if (G::LocalPlayer->GetFlags() & FL_ONGROUND && UserCmd->sidemove < 3 && UserCmd->sidemove > -3) {
								static bool switch_ = false;
								if (switch_)
									UserCmd->sidemove = 1.01;
								else
									UserCmd->sidemove = -1.01;
								switch_ = !switch_;
							}
						}
					}
				}


				g_BacktrackHelper->UpdateIncomingSequences();

				if (G::GrenadeDamage && c_config::get()->b["misc_autogrenade"] && G::LocalPlayer->GetActiveWeapon()->is_grenade())
					UserCmd->buttons &= ~IN_ATTACK;

				if (c_config::get()->b["misc_automaticwpns"]) {
					if (G::LocalPlayer && G::LocalPlayer->IsAlive()) {
						auto activeWeapon = G::LocalPlayer->GetActiveWeapon();
						if (activeWeapon && activeWeapon->is_pistol() && (activeWeapon->GetNextPrimaryAttack() + (c_config::get()->i["misc_automaticwpns_time"] / 1000.f)) > Interfaces::Globals->curtime) {
							UserCmd->buttons &= ~IN_ATTACK;
						}
					}
				}

				if (c_config::get()->i["misc_airstrafe"] == 1)
					c_antiaims::get()->wasdstrafe(UserCmd);

				engineprediction::get()->end();

				for (auto hk : c_lua::get()->hooks->getHooks("on_create_move"))
					hk.func(UserCmd, &G::SendPacket);

				static auto angle_vectors = [](const Vector& angles, Vector* forward, Vector* right, Vector* up) {
					static auto sincos = [](float radians, float* sine, float* cosine) {
						*sine = sin(radians);
						*cosine = cos(radians);
					};

					float sr, sp, sy, cr, cp, cy;

					sincos(DEG2RAD(angles[1]), &sy, &cy);
					sincos(DEG2RAD(angles[0]), &sp, &cp);
					sincos(DEG2RAD(angles[2]), &sr, &cr);

					if (forward) {
						forward->x = cp * cy;
						forward->y = cp * sy;
						forward->z = -sp;
					}

					if (right) {
						right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
						right->y = (-1 * sr * sp * sy + -1 * cr * cy);
						right->z = -1 * sr * cp;
					}

					if (up) {
						up->x = (cr * sp * cy + -sr * -sy);
						up->y = (cr * sp * sy + -sr * cy);
						up->z = cr * cp;
					}
				};

				Vector view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
				auto viewangles = UserCmd->viewangles;
				viewangles.Normalized();

				angle_vectors(wish_angle, &view_fwd, &view_right, &view_up);
				angle_vectors(viewangles, &cmd_fwd, &cmd_right, &cmd_up);

				float v8 = sqrtf((view_fwd.x * view_fwd.x) + (view_fwd.y * view_fwd.y));
				float v10 = sqrtf((view_right.x * view_right.x) + (view_right.y * view_right.y));
				float v12 = sqrtf(view_up.z * view_up.z);

				Vector norm_view_fwd((1.f / v8) * view_fwd.x, (1.f / v8) * view_fwd.y, 0.f);
				Vector norm_view_right((1.f / v10) * view_right.x, (1.f / v10) * view_right.y, 0.f);
				Vector norm_view_up(0.f, 0.f, (1.f / v12) * view_up.z);

				float v14 = sqrtf((cmd_fwd.x * cmd_fwd.x) + (cmd_fwd.y * cmd_fwd.y));
				float v16 = sqrtf((cmd_right.x * cmd_right.x) + (cmd_right.y * cmd_right.y));
				float v18 = sqrtf(cmd_up.z * cmd_up.z);

				Vector norm_cmd_fwd((1.f / v14) * cmd_fwd.x, (1.f / v14) * cmd_fwd.y, 0.f);
				Vector norm_cmd_right((1.f / v16) * cmd_right.x, (1.f / v16) * cmd_right.y, 0.f);
				Vector norm_cmd_up(0.f, 0.f, (1.f / v18) * cmd_up.z);

				float v22 = norm_view_fwd.x * UserCmd->forwardmove;
				float v26 = norm_view_fwd.y * UserCmd->forwardmove;
				float v28 = norm_view_fwd.z * UserCmd->forwardmove;
				float v24 = norm_view_right.x * UserCmd->sidemove;
				float v23 = norm_view_right.y * UserCmd->sidemove;
				float v25 = norm_view_right.z * UserCmd->sidemove;
				float v30 = norm_view_up.x * UserCmd->upmove;
				float v27 = norm_view_up.z * UserCmd->upmove;
				float v29 = norm_view_up.y * UserCmd->upmove;

				UserCmd->forwardmove = ((((norm_cmd_fwd.x * v24) + (norm_cmd_fwd.y * v23)) + (norm_cmd_fwd.z * v25))
					+ (((norm_cmd_fwd.x * v22) + (norm_cmd_fwd.y * v26)) + (norm_cmd_fwd.z * v28)))
					+ (((norm_cmd_fwd.y * v30) + (norm_cmd_fwd.x * v29)) + (norm_cmd_fwd.z * v27));
				UserCmd->sidemove = ((((norm_cmd_right.x * v24) + (norm_cmd_right.y * v23)) + (norm_cmd_right.z * v25))
					+ (((norm_cmd_right.x * v22) + (norm_cmd_right.y * v26)) + (norm_cmd_right.z * v28)))
					+ (((norm_cmd_right.x * v29) + (norm_cmd_right.y * v30)) + (norm_cmd_right.z * v27));
				UserCmd->upmove = ((((norm_cmd_up.x * v23) + (norm_cmd_up.y * v24)) + (norm_cmd_up.z * v25))
					+ (((norm_cmd_up.x * v26) + (norm_cmd_up.y * v22)) + (norm_cmd_up.z * v28)))
					+ (((norm_cmd_up.x * v30) + (norm_cmd_up.y * v29)) + (norm_cmd_up.z * v27));

				UserCmd->forwardmove = std::clamp<float>(UserCmd->forwardmove, -450.f, 450.f);
				UserCmd->sidemove = std::clamp<float>(UserCmd->sidemove, -450.f, 450.f);
				UserCmd->upmove = std::clamp<float>(UserCmd->upmove, -320.f, 320.f);


				Cheat::Utilities->Math_Normalize(UserCmd->viewangles);

				if (UserCmd->forwardmove > 450.0f)
					UserCmd->forwardmove = 450.0f;
				else if (UserCmd->forwardmove < -450.0f)
					UserCmd->forwardmove = -450.0f;

				if (UserCmd->sidemove > 450.0f)
					UserCmd->sidemove = 450.0f;
				else if (UserCmd->sidemove < -450.0f)
					UserCmd->sidemove = -450.0f;



				if (c_config::get()->i["rage_fixleg"] == 1)
					Features::Misc->SlideWalk();

				if (c_config::get()->i["rage_fixleg"] == 2) {
					UserCmd->buttons &= ~IN_MOVERIGHT;
					UserCmd->buttons &= ~IN_MOVELEFT;
					UserCmd->buttons &= ~IN_FORWARD;
					UserCmd->buttons &= ~IN_BACK;

					if (UserCmd->forwardmove > 0.f)
						UserCmd->buttons |= IN_FORWARD;
					else if (UserCmd->forwardmove < 0.f)
						UserCmd->buttons |= IN_BACK;
					if (UserCmd->sidemove > 0.f)
					{
						UserCmd->buttons |= IN_MOVERIGHT;
					}
					else if (UserCmd->sidemove < 0.f)
					{
						UserCmd->buttons |= IN_MOVELEFT;
					}
				}

				if (!G::SendPacket) {
					//G::RealAngle = UserCmd->viewangles;
				}

				if (G::SendPacket) {
					G::DisplayFakeAngle = UserCmd->viewangles;
					G::RealAngle = UserCmd->viewangles;
					G::FakeAngle = G::LocalPlayer->GetAbsAngles();
					G::FakePosition = G::LocalPlayer->GetAbsOrigin();
				}


				Features::grenade_prediction::get()->Tick(UserCmd->buttons);

				uintptr_t* frame_ptr;
				__asm mov frame_ptr, ebp;
				*(bool*)(*frame_ptr - 0x1C) = G::SendPacket;



				return false;
			}

		}
	}

}