#include "..\includes.h"
#include "hooks.h"
#include "../UTILS/interfaces.h"
#include "../UTILS/offsets.h"
#include "../UTILS/NetvarHookManager.h"
#include "../UTILS/render.h"
#include "../FEATURES/NewEventLog.h"
#include "../SDK/CInput.h"
#include "../SDK/IClient.h"
#include "../SDK/CPanel.h"
#include "../SDK/ConVar.h"
#include "../SDK/CGlowObjectManager.h"
#include "../SDK/IEngine.h"
#include "../SDK/CTrace.h"
#include "../DesyncCorrect.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/RecvData.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/ModelInfo.h"
#include "../SDK/ModelRender.h"
#include "../SDK/RenderView.h"
#include "../SDK/CTrace.h"
#include "../NewBacktracking.h"
#include "../SDK/CViewSetup.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/CPrediction.h"
#include "../SDK/IVDebugOverlay.h"
#include "../extrapolation.h"
#include "../LagCompensation.h"
#include "../NewResolverh.h"

#include "../FEATURES/Movement.h"
#include "../FEATURES/Visuals.h"
#include "../FEATURES/Chams.h"
#include "../FEATURES/AntiAim.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/Resolver.h"
#include "../FEATURES/Backtracking.h"
#include "../FEATURES/FakeWalk.h"
#include "../FEATURES/FakeLag.h"
#include "../FEATURES/EnginePred.h"
#include "../FEATURES/EventListener.h"
#include "../FEATURES/GrenadePrediction.h"
#include "../FEATURES/Legitbot.h"
#include "../FEATURES/Flashlight.h"
#include "../FEATURES/GloveChanger.h"
#include "../FEATURES/SkinChanger.h"
#include "../NewAimbot.h"
#include "../FEATURES/D9Visuals.h"
#include "../FEATURES/dh.h"
#include "../shit.h"
#include <intrin.h>
#include "../SDK/NetChannel.h"
#include "../MENU/menu_framework.h"
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
namespace INIT
{
	HMODULE Dll;
	HWND Window;
	WNDPROC OldWindow;
}

#define XorStr( s ) ( s )
static bool menu_open = false;
static bool d3d_init = false;
bool PressedKeys[256] = {};
const char* merixids[] =
{
	"1","2","3","4","5","6", "7", "8", "9",
	"Q","W","E","R","T","Y","U","I","O","P",
	"A","S","D","F","G","H","J","K","L",
	"Z","X","C","V","B","N","M",".","\\","|", "/","}","{","[","]",
	"<",">","?","'"
};
static char ConfigNamexd[64] = { 0 };

//--- Other Globally Used Variables ---///
static bool tick = false;
static int ground_tick;
Vector vecAimPunch, vecViewPunch;
Vector* pAimPunch = nullptr;
Vector* pViewPunch = nullptr;

//--- Declare Signatures and Patterns Here ---///
static auto CAM_THINK = UTILS::FindSignature("client_panorama.dll", "85 C0 75 30 38 86");
static auto linegoesthrusmoke = UTILS::FindPattern("client_panorama.dll", (PBYTE)"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx");

//--- Tick Counting ---//
void ground_ticks()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (local_player->GetFlags() & FL_ONGROUND)
		ground_tick++;
	else
		ground_tick = 0;
}


LinearExtrapolations linear_extraps;


void draw_hitbox_bt(SDK::CBaseEntity* pEntity, int r, int g, int b, int a, float duration, float diameter) {
	matrix3x4_t matrix[128];
	if (!pEntity->SetupBones(matrix, 128, 0x00000100, pEntity->GetSimTime()))
		return;
	SDK::studiohdr_t* hdr = INTERFACES::ModelInfo->GetStudioModel(pEntity->GetModel());
	SDK::mstudiohitboxset_t* set = hdr->pHitboxSet(0);

	for (int i = 0; i < set->numhitboxes; i++) {
		SDK::mstudiobbox_t* hitbox = set->GetHitbox(i);
		if (!hitbox)
			continue;
		Vector vMin, vMax;
		auto VectorTransform_Wrapperx = [](const Vector& in1, const matrix3x4_t &in2, Vector &out)
		{
			auto VectorTransform = [](const float *in1, const matrix3x4_t& in2, float *out)
			{
				auto DotProducts = [](const float *v1, const float *v2)
				{
					return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
				};
				out[0] = DotProducts(in1, in2[0]) + in2[0][3];
				out[1] = DotProducts(in1, in2[1]) + in2[1][3];
				out[2] = DotProducts(in1, in2[2]) + in2[2][3];
			};
			VectorTransform(&in1.x, in2, &out.x);
		};
		VectorTransform_Wrapperx(hitbox->bbmin, matrix[hitbox->bone], vMin);
		VectorTransform_Wrapperx(hitbox->bbmax, matrix[hitbox->bone], vMax);
		INTERFACES::DebugOverlay->DrawPill(vMin, vMax, diameter, r, g, b, a, duration);
	}
}

namespace HOOKS
{
	static bool slowwalk;
	CreateMoveFn original_create_move;
	PaintTraverseFn original_paint_traverse;
	//SetChokedFn set_choked;
	SendDatagramFn original_send_datagram;
	PaintFn original_paint;
	FrameStageNotifyFn original_frame_stage_notify;
	DrawModelExecuteFn original_draw_model_execute;
	SceneEndFn original_scene_end;
	TraceRayFn original_trace_ray;
	SetupBonesFn original_setup_bones;
	OverrideViewFn original_override_view;
	//DrawSetClr orig_drawsetcolor;
	RenderViewFn original_render_view;
	SvCheatsGetBoolFn original_get_bool;
	GetViewmodelFOVFn original_viewmodel_fov;
	//DrawSetColorFn orig_drawsetcolor;
	ExtraBoneProcess_t Do_ExtraBonesProcessing;

	vfunc_hook fireevent;
	vfunc_hook directz;

	VMT::VMTHookManager iclient_hook_manager;
	VMT::VMTHookManager panel_hook_manager;
	VMT::VMTHookManager paint_hook_manager;
	VMT::VMTHookManager model_render_hook_manager;
	VMT::VMTHookManager scene_end_hook_manager;
	VMT::VMTHookManager render_view_hook_manager;
	VMT::VMTHookManager trace_hook_manager;
	VMT::VMTHookManager net_channel_hook_manager;
	VMT::VMTHookManager override_view_hook_manager;
	//VMT::VMTHookManager Do_ExtraBonesProcessing;
	VMT::VMTHookManager input_table_manager;
	VMT::VMTHookManager get_bool_manager;
	//VMT::VMTHookManager Do_ExtraBonesProcessing;
	std::string sPanel = ("FocusOverlayPanel");
	VMT::VMTHookManager setup_bones_hook_manager;

	inline float FastSqrt(float x)
	{
		unsigned int i = *(unsigned int*)&x;
		i += 127 << 23;
		i >>= 1;
		return *(float*)&i;
	}
#define square( x ) ( x * x )
	void slowmogang(SDK::CUserCmd* cmd, float fMaxSpeed) {
		if (fMaxSpeed <= 0.f)
			return;
		float fSpeed = (float)(FastSqrt(square(GLOBAL::originalCMD.move.x) + square(GLOBAL::originalCMD.move.y) + square(GLOBAL::originalCMD.move.z)));
		if (fSpeed <= 0.f)
			return;
		if (cmd->buttons & IN_DUCK)
			fMaxSpeed *= 2.94117647f;
		if (fSpeed <= fMaxSpeed)
			return;
		float fRatio = fMaxSpeed / fSpeed;
		GLOBAL::originalCMD.move.x *= fRatio;
		GLOBAL::originalCMD.move.y *= fRatio;
		GLOBAL::originalCMD.move.z *= fRatio;
	}

	bool __fastcall HookedSetupBones(void* ECX, void* EDX, matrix3x4_t* matrix, int num_bones, int bone_mask, float current_time)
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		auto animstate = local_player->GetAnimState();
		auto old_absorigin = local_player->GetAbsOrigin();
		if (!ECX || !matrix || !local_player || local_player->GetHealth() <= 0 || !animstate)
			return original_setup_bones(ECX, matrix, num_bones, bone_mask, current_time);

		static auto last_networked_origin = Vector(0, 0, 0);
		if (GLOBAL::should_send_packet)
			last_networked_origin = local_player->GetVecOrigin();

		//if (SETTINGS::settings.lag_visual)
		//	local_player->SetAbsOrigin(last_networked_origin);

		return original_setup_bones(ECX, matrix, num_bones, bone_mask, current_time);
	}	

	int __fastcall HookedSendDatagram(SDK::NetChannel* ecx, void* edx, void* data)
	{
		if (!ecx)
			return 0;

		if (data || !INTERFACES::Engine->IsInGame())
			return original_send_datagram(ecx, data);

		backtracking->netchan = ecx;

		int in_reliable_state = ecx->m_nInReliableState;
		int in_sequence_num = ecx->m_nInSequenceNr;

		
		auto lag_s = SETTINGS::settings.FakeLatencyAmount / 1000.f;
		auto lag_delta = lag_s - INTERFACES::Engine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);

		if (SETTINGS::settings.fakelatency_enabled)
		{
			backtracking->AddLatency(ecx, lag_delta);
		}

		int ret = original_send_datagram(ecx, data);

		ecx->m_nInReliableState = in_reliable_state;
		ecx->m_nInSequenceNr = in_sequence_num;

		return ret;
	}

	/*void __fastcall SetChoked(SDK::NetChannel* ecx, void* edx, void* data)
	{
		if (data || !INTERFACES::Engine->IsInGame()) 
			return set_choked(ecx, edx, data);

		/*auto net_channel = INTERFACES::Engine->GetNetChannel();
		const auto current_choke = ecx->m_nChokedPackets;

		ecx->m_nChokedPackets = 0;
		ecx->send_datagram();
		--ecx->m_nOutSequenceNr;
		ecx->m_nChokedPackets = current_choke;*/

	//}

	/*void __fastcall DrawSetClr(SDK::ISurface * thisptr, void * edx, int r, int g, int b, int a)
	{
		const auto return_address = uintptr_t(_ReturnAddress());

		static auto return_to_scope_arc = UTILS::FindSignature("client_panorama.dll", "6A 00 FF 50 3C 8B 0D ? ? ? ? FF B7 ? ? ? ?") + 5;
		static auto return_to_scope_lens = UTILS::FindSignature("client_panorama.dll", "FF 50 3C 8B 4C 24 20") + 3;

		if (return_address == return_to_scope_arc
			|| return_address == return_to_scope_lens)
		{
			// We don't want this to draw, so we set the alpha to 0
			return orig_drawsetcolor(thisptr, edx, r, g, b, 0);
		}

		orig_drawsetcolor(thisptr, edx, r, g, b, a);
	}
	*/
	void setup_bones()
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player || local_player->GetHealth() <= 0) return;

		auto renderable = reinterpret_cast<void*>(uintptr_t(local_player) + 0x4);
		if (!renderable) return;

		static bool is_init = false;
		if (!is_init)
		{
			is_init = true;
			HOOKS::setup_bones_hook_manager.Init(renderable);
			HOOKS::original_setup_bones = HOOKS::setup_bones_hook_manager.HookFunction<HOOKS::SetupBonesFn>(13, HookedSetupBones);
		}
		if (!HOOKS::setup_bones_hook_manager.IsHooked(13))
			HOOKS::original_setup_bones = HOOKS::setup_bones_hook_manager.HookFunction<HOOKS::SetupBonesFn>(13, HookedSetupBones);
	}

	void NormalizeThing(Vector &vIn, Vector &vOut)
	{
		float flLen = vIn.Length();
		if (flLen == 0) {
			vOut.Init(0, 0, 1);
			return;
		}
		flLen = 1 / flLen;
		vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
	}
	float NormalizeVector(Vector& v) {
		float l = v.Length();
		if (l != 0.0f) {
			v /= l;
		}
		else {
			v.x = v.y = v.z = 0.0f;
		}
		return l;
	}

	

	
	void NormaliseViewAngleThing(Vector &angle)
	{
		while (angle.y <= -180) angle.y += 360;
		while (angle.y > 180) angle.y -= 360;
		while (angle.x <= -180) angle.x += 360;
		while (angle.x > 180) angle.x -= 360;


		if (angle.x > 89) angle.x = 89;
		if (angle.x < -89) angle.x = -89;
		if (angle.y < -180) angle.y = -179.999;
		if (angle.y > 180) angle.y = 179.999;

		angle.z = 0;
	}



	bool __stdcall HookedCreateMove(float sample_input_frametime, SDK::CUserCmd* cmd)
	{
		if (!cmd || cmd->command_number == 0)
			return false;


		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame() && INTERFACES::Engine->GetNetChannel() && !original_send_datagram)
		{
			net_channel_hook_manager.Init(INTERFACES::Engine->GetNetChannel());
			original_send_datagram = reinterpret_cast<SendDatagramFn>(net_channel_hook_manager.HookFunction<SendDatagramFn>(46, HookedSendDatagram)); //48
		}

		backtracking->UpdateIncomingSequences();

		uintptr_t* FPointer; __asm { MOV FPointer, EBP }
		byte* SendPacket = (byte*)(*FPointer - 0x1C);

		if (!SendPacket) return false;

		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player) return false;

	

		GLOBAL::should_send_packet = *SendPacket;
		GLOBAL::originalCMD = *cmd;
		GLOBAL::last_cmd = cmd;
		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			GrenadePrediction::instance().Tick(cmd->buttons);
			if (SETTINGS::settings.aim_type == 0 && SETTINGS::settings.slowalkbool && GetAsyncKeyState(VK_SHIFT)) {
				slowmogang(cmd, 24);
				GLOBAL::is_slowwalking = true;
			}
			else GLOBAL::is_slowwalking = false;

			SDK::CBaseEntity* entity = nullptr;

			// ok so first 2 are from anti fs, 3rd is from stickrpg and 4th is from "Yoinks" ( in desync correct.cpp
			resolver->DoCM();
			desync->Main(entity);
			resolver->AACMain();

			if (!INTERFACES::Engine->IsVoiceRecording())
				fakelag->do_fakelag(cmd);

			// getting that red, client side impact
			static SDK::ConVar* impacts = INTERFACES::cvar->FindVar("sv_showimpacts");
			if (SETTINGS::settings.impacts)
			{
				impacts->SetValue(1);
			}
			else {
				impacts->SetValue(0);
			}
			
			// so we can change our name through the console ( and utilize a name changer (or stealer))
			SDK::ConVar* nameVar = INTERFACES::cvar->FindVar("name");
			if (nameVar)
			{
				*(int*)((DWORD)&nameVar->fnChangeCallback + 0xC) = 0;
			}
	
			// infinite duck
			if (SETTINGS::settings.crouch_bool) 
			{
				cmd->buttons |= IN_BULLRUSH;
			}


			if (SETTINGS::settings.bhop_bool) movement->bunnyhop(cmd);
			if (SETTINGS::settings.duck_bool) movement->duckinair(cmd);
			if (SETTINGS::settings.strafe_bool) movement->autostrafer(cmd);
			if (SETTINGS::settings.misc_clantag) visuals->GameSense1();
			//if (SETTINGS::settings.skeetxd) visuals->GameSense1();
			//if (SETTINGS::settings.skeetxd2) visuals->GameSense2();
			//if (SETTINGS::settings.skeetxd1) visuals->fakeskeet();
			
			setup_bones(); // needed for baktrak
			
		
		
			Vector ViewAngle = cmd->viewangles;
			Vector OldViewAngle = ViewAngle; //aimstep related shit
			LegitAimbot lbot;
			//lbot.Update(cmd);

			prediction->run_prediction(cmd); // lets run engine prediction
			{
			    
				
				

				
				
			

				   
					if (SETTINGS::settings.aim_type == 0 && SETTINGS::settings.aim_bool)
					{
						//aimstep(cmd); thats not fucking right what is wrong with me

					    
					


						if (SETTINGS::settings.extrapolationbool)
						{
							linear_extraps.run(); // we predictt
						}
						
						aimbot->run_aimbot(cmd); // we aim

						int accuracyboost = SETTINGS::settings.Accuracyboost;

						switch (accuracyboost) // ez
						{
						case 0:
							break;
						case 1: backtracking->run_legit(cmd);
							break;
						case 2: backtracking->backtrack_player(cmd);
							break;
						}


					
						
						
						
					}

					if (SETTINGS::settings.aim_type == 1 && SETTINGS::settings.aim_bool)
					{

						//aimstep(cmd);
						if (GetAsyncKeyState(UTILS::INPUT::input_handler.keyBindings(SETTINGS::settings.legittrigger_key)) && SETTINGS::settings.legittrigger_bool)
							legitbot->triggerbot(cmd);

						if (SETTINGS::settings.legit_back)
							backtracking->run_legit(cmd);

						lbot.Run(local_player, cmd);
						//lbot.Do(cmd);
						

						if (SETTINGS::settings.rcsbool)
						{
							legitbot->RCS(cmd);
						}



					}

					if (SETTINGS::settings.aa_bool)
					{
						antiaim->do_antiaim(cmd);
						antiaim->fix_movement(cmd);
						//ground_ticks();
					}

			}
			prediction->end_prediction(cmd);
		
			
			Vector ViewAngleDelta = ViewAngle - OldViewAngle;

			
			if (SETTINGS::settings.aimstep_bool)
			{
				if (ViewAngleDelta.y > 37.f)
				{
					ViewAngleDelta.y = 37.f;
				}
				if (ViewAngleDelta.x > 37.f)
				{
					ViewAngleDelta.x = 37.f;
				}
			
				NormalizeVector(ViewAngleDelta);
				cmd->viewangles += ViewAngleDelta;
			} // ghetto aimstep, thanks uc 
			// doesnt even work ezez

			// gay fake duck 
			// credits to tommy
			if (SETTINGS::settings.fake_duck) 
			{
				bool do_once = false;
				bool crouch = false;
				auto nci = INTERFACES::Engine->GetNetChannel();
				cmd->buttons |= IN_BULLRUSH;
				crouch = nci->m_nChokedPackets > 7;
				if (GetAsyncKeyState(VK_LCONTROL))
				{
					local_player->GetAnimState()->m_fDuckAmount = 69696969.f; //b1g p2c "animfix" meme
					do_once = true;
					if (crouch)
						INTERFACES::Engine->ExecuteClientCmd("-duck");
					else
						INTERFACES::Engine->ExecuteClientCmd("+duck");
				}
				if (do_once && !(GetAsyncKeyState(VK_LCONTROL))) {
					local_player->GetAnimState()->m_fDuckAmount = 0.f;
					INTERFACES::Engine->ExecuteClientCmd("-duck");
					do_once = false;
				}
			}

	

			if (!GLOBAL::should_send_packet)
				GLOBAL::real_angles = cmd->viewangles;
			else
			{
				GLOBAL::FakePosition = local_player->GetAbsOrigin();
				GLOBAL::fake_angles = cmd->viewangles;

				//auto net_channel = INTERFACES::Engine->GetNetChannel();
				
				//net_channel->send_datagram();
			}
			

			
		}
		
	
	    
	
	
		
		*SendPacket = GLOBAL::should_send_packet;
		cmd->move = antiaim->fix_movement(cmd, GLOBAL::originalCMD);
		UTILS::ClampLemon(cmd->viewangles);
		


		return false;
	}

	void __stdcall HookedPaintTraverse(int VGUIPanel, bool ForceRepaint, bool AllowForce)
	{
		//SDK::IGameEvent* pEvent;
		std::string panel_name = INTERFACES::Panel->GetName(VGUIPanel);
		SDK::CBaseEntity* pPlayerEntity = nullptr;
		if (panel_name == "HudZoom" && SETTINGS::settings.scope_bool) return; 
		if (panel_name == "FocusOverlayPanel")
		{
			if (FONTS::ShouldReloadFonts())
				FONTS::InitFonts();

			if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
			{
				GrenadePrediction::instance().Paint();
				FEATURES::MISC::in_game_logger.Do();
				auto matpostprocess = INTERFACES::cvar->FindVar("mat_postprocess_enable");
				matpostprocess->fnChangeCallback = 0;
				matpostprocess->SetValue(SETTINGS::settings.matpostprocessenable);

				visuals->ModulateWorld();

				if (SETTINGS::settings.esp_bool)
				{
					visuals->Draw(); // esp
					pHitmarker->Paint(); // dynamic hitmarkers
					visuals->ClientDraw();
				
				}

			

				Flashlight.RunFrame();
			}

			MENU::PPGUI_PP_GUI::Begin();
			MENU::Do();
			MENU::PPGUI_PP_GUI::End();

			UTILS::INPUT::input_handler.Update();



			//	visuals->LogEvents();
		}


		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				viewMatrix[i][j] = INTERFACES::Engine->WorldToScreenMatrix()[i][j];
			}
		}
		original_paint_traverse(INTERFACES::Panel, VGUIPanel, ForceRepaint, AllowForce);

		const char* pszPanelName = INTERFACES::Panel->GetName(VGUIPanel);

		if (!strstr(pszPanelName, sPanel.data()))
			return;


		INTERFACES::Panel->SetMouseInputEnabled(VGUIPanel, menu_open);
	}
	

	/*void __fastcall DoExtraBonesProcessing(uintptr_t ecx, uintptr_t edx, SDK::studiohdr_t *hdr, Vector *pos, Quaternion *q, matrix3x4_t *matrix, SDK::CBoneBitList &bone_list, SDK::CIKContext *context) shitty meme i was trying
	{
		SDK::CBaseEntity* pPlayerEntity = (SDK::CBaseEntity*)ecx;
		
		auto oDoExtraBonesProcessing = HOOKS::Do_ExtraBonesProcessing;

		float Backup;
		if (pPlayerEntity->GetAnimState()) {
			Backup = pPlayerEntity->GetAnimState()->m_flUnknownFraction;
			pPlayerEntity->GetAnimState()->m_flUnknownFraction = 0.f;
		}

		oDoExtraBonesProcessing(ecx, hdr, pos, q, matrix, bone_list, context);

		if (pPlayerEntity->GetAnimState()) {
			pPlayerEntity->GetAnimState()->m_flUnknownFraction = Backup;
		} shit from tommy
	}*/

	/// actual [censored] animfix

	/*int animfix(SDK::CBaseEntity* entity)
	{
		int result; // eax
		int v3; // edi
		auto entity_1; // ecx
		auto animstate = entity->GetAnimState(); // esi
		float speed2d; // xmm0_4
		bool v7; // cf
		bool v8; // zf	
		float cur_time = INTERFACES::Globals->curtime; // xmm0_4
		float v10; // xmm0_4
		double v11; // st7
		float v12; // xmm1_4
		int v13; // eax
		int m_flEyeYaw; // xmm0_4
		int tickbase; // esi
		int v16; // ecx
		float float_negative_180_0;
		float float_180_0;
		float float_360_0;
		float float_1_1;
		float float_0_1;
		auto animstate_1; // [esp+8h] [ebp-Ch]
		float v18; // [esp+10h] [ebp-4h]

		result = *(float*)(0x38A0);
		//v3 = this;
		entity_1 = entity;
		//animstate = *float(offset_0x38A0 + entity);
		animstate_1 = animstate;
		if (animstate)
		{
			speed2d = *(animstate + 0xEC);
			v7 = speed2d < float_0_1;
			v8 = speed2d == float_0_1;
			cur_time = *(g_pGlobals + 0x10);
			if (v7 || v8)
			{
				if (cur_time <= (v3 + 4))
					goto LABEL_13;
				v11 = (animstate + 0x80 - animstate + 0x78);
				sub_56F7B2F6(v11);
				v18 = v11;
				v12 = v18;
				if (animstate + 0x80 <= animstate + 0x78)
				{
					if (float_negative_180_0 >= v18)
						v12 = v18 + float_360_0;
				}
				else if (v18 >= float_180_0)
				{
					v12 = v18 - float_360_0;
				}
				entity_1 = entity;
				if (COERCE_FLOAT(LODWORD(v12) & xmmword_3FBFF410) <= *&dword_3FBFF004)
					goto LABEL_13;
				v10 = cur_time + float_1_1;
			}
			else
			{
				v10 = cur_time + float_0_22;
			}
			v13 = dword_67A1ACF4;
			*(v3 + 4) = v10;
			m_flEyeYaw = *(animstate + 0x78);
			*v3 = m_flEyeYaw;
			*(v13 + entity_1) = m_flEyeYaw;
		LABEL_13:
			tickbase = *(m_nTickBase + entity_1);
			v16 = 3 * (*(m_nTickBase + entity_1) % 150 + 1);
			*(v3 + 4 * v16) = tickbase;
			*(v3 + 4 * v16 + 4) = *(animstate_1 + 0xEC);
			result = *v3;
			*(v3 + 4 * v16 + 8) = *v3;
		}
		return result;
	}*/

static void SkipAnimation(SDK::CBaseEntity *player) // gay tommy crap
	{
		static SDK::ConVar *sv_pvsskipanimation = INTERFACES::cvar->FindVar("sv_pvsskipanimation");
		int32_t backup_sv_pvsskipanimation = sv_pvsskipanimation->GetFloat();
		sv_pvsskipanimation->SetValue(0);
		*(int32_t*)((uintptr_t)player + 0xA30) = 0;
		*(int32_t*)((uintptr_t)player + 0x269C) = 0;
		int32_t backup_effects = *(int32_t*)((uintptr_t)player + 0xEC);
		*(int32_t*)((uintptr_t)player + 0xEC) |= 8;
		player->SetupBones(NULL, -1, 0x7FF00, INTERFACES::Globals->curtime);
		*(int32_t*)((uintptr_t)player + 0xEC) = backup_effects;
		sv_pvsskipanimation->SetValue(backup_sv_pvsskipanimation);
	};


void AnimFix(SDK::CBaseEntity* entity, bool BSlowwalk) // wtf
{
	static float storedSimtime[65];
	static SDK::CAnimationLayer StoredLayers[64][15];
	static float StoredPosParams[65][24];
	float* PosParams = (float*)((uintptr_t)entity + 0x2774);

	if (entity == nullptr) return;

	auto* AnimState = entity->GetAnimState();
	if (!AnimState || !entity->GetAnimOverlays() || !PosParams)
		return;

	if (storedSimtime[entity->GetIndex()] != entity->GetSimTime())
	{
		entity->GetClientSideAnimation2() = true;
		entity->UpdateClientSideAnimation();

		memcpy(StoredPosParams[entity->GetIndex()], PosParams, sizeof(float) * 24);
		memcpy(StoredLayers[entity->GetIndex()], entity->GetAnimOverlays(), (sizeof(SDK::CAnimationLayer) * 15));

		storedSimtime[entity->GetIndex()] = entity->GetSimTime();
	}

	entity->GetClientSideAnimation2() = false;

	memcpy((void*)PosParams, &StoredPosParams[entity->GetIndex()], (sizeof(float) * 24));
	memcpy(entity->GetAnimOverlays(), StoredLayers[entity->GetIndex()], (sizeof(SDK::CAnimationLayer) * 15));

	entity->SetAngle2(Vector(0, entity->GetAnimState()->m_flGoalFeetYaw, 0));
} /// bad animfix

/*void SDK::CBaseEntity::InvalidatePhysicsRecursive(int nChangeFlags) uwuwuwuwuwuw
{
	int nDirtyFlags = 0;

	if (nChangeFlags & VELOCITY_CHANGED)
	{
		nDirtyFlags |= EFL_DIRTY_ABSVELOCITY;
	}

	if (nChangeFlags & POSITION_CHANGED)
	{
		nDirtyFlags |= EFL_DIRTY_ABSTRANSFORM;
		NetworkProp()->MarkPVSInformationDirty();
		CollisionProp()->MarkPartitionHandleDirty();
	}
	if (nChangeFlags & ANGLES_CHANGED)
	{
		nDirtyFlags |= EFL_DIRTY_ABSTRANSFORM;
		if (CollisionProp()->DoesRotationInvalidateSurroundingBox())
		{
			CollisionProp()->MarkSurroundingBoundsDirty();
		}
		else
		{
			MarkRenderHandleDirty();
			g_pClientShadowMgr->AddToDirtyShadowList(this);
			g_pClientShadowMgr->MarkRenderToTextureShadowDirty(GetShadowHandle());
		}
		nChangeFlags |= POSITION_CHANGED | VELOCITY_CHANGED;
	}

	AddEFlags(nDirtyFlags);
	bool bOnlyDueToAttachment = false;
	if (nChangeFlags & ANIMATION_CHANGED)
	{
		g_pClientShadowMgr->MarkRenderToTextureShadowDirty(GetShadowHandle());
		if (!(nChangeFlags & (POSITION_CHANGED | VELOCITY_CHANGED | ANGLES_CHANGED)))
		{
			bOnlyDueToAttachment = true;
		}

		nChangeFlags = POSITION_CHANGED | ANGLES_CHANGED | VELOCITY_CHANGED;
	}

	for (CBaseEntity *pChild = FirstMoveChild(); pChild; pChild = pChild->NextMovePeer())
	{
		if (bOnlyDueToAttachment)
		{
			if ((pChild->GetParentAttachment() == 0) && !pChild->IsFollowingEntity())
				continue;
		}
		pChild->InvalidatePhysicsRecursive(nChangeFlags);
	}
}*/




	void __fastcall HookedFrameStageNotify(void* ecx, void* edx, int stage)
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player) return;
		Vector vecAimPunch, vecViewPunch;
		Vector* pAimPunch = nullptr; Vector* pViewPunch = nullptr;
		SDK::IGameEvent* pEvent = nullptr;
		

		switch (stage)
		{
		case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
			if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
			{
			        /// changers
					GloveChanger();
					xdSkinchanger();


					
					for (int i = 1; i <= 65; i++)
					{
						auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);

						if (entity == nullptr) continue;
						if (entity == local_player) continue;
						if (entity->GetHealth() <= 0) continue;
						//if (entity->GetIsDormant()) continue;

				
						////
						if (SETTINGS::settings.apply_freestand)
						{
							resolver->AntiAimCorrection(entity);
						}
					}

					if (SETTINGS::settings.bspottedradar)
					{
						for (int i = 1; i < 65; i++)
						{
							auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
							if (!entity || entity->GetHealth() <= 0 || entity->GetIsDormant())
								continue;

							if (entity == local_player) continue;
						
							entity->SetSpotted(true); /// cool radar thanks bamwar

							
						}
					}
					for (int i = 1; i <= 65; i++)
					{
						auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
						if (!entity) continue;

						bool is_local_player = entity == local_player;
						bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

						if (is_local_player) continue;
						if (is_teammate) continue;
						if (entity->GetHealth() <= 0) continue;
						if (entity->GetIsDormant()) continue;

						if (SETTINGS::settings.resolve2_bool)
						{
							int angle1[64];
							int angle2[64];
							int angle3[64];
							static float old_simtime[65];
							if (GLOBAL::should_send_packet)
								angle1[i] = entity->GetEyeAngles().y;
							else
								angle2[i] = entity->GetEyeAngles().y;

							if (angle1[i] != angle2[i])
								using_fake_angles[entity->GetIndex()] = true;
							else
								using_fake_angles[entity->GetIndex()] = false;

							if (using_fake_angles[entity->GetIndex()])
							{
								if (MATH::YawDistance(angle1[i], angle2[i]) < 59)
								{
									resolve_type[i] = 0;
									entity->GetEyeAnglesPtr()->y = entity->GetLowerBodyYaw() - MATH::YawDistance(angle1[i], angle2[i]);
								}
							}
						}
					}


			
			} break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
			if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
			{
				//FEATURESES::RAGEBOT::CBacktracking lag_compensation;
				//lag_compensation.Store();
				//backtracking->DisableInterpolation(entity);
			}
			break;
		case FRAME_RENDER_START:
			if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
			{
				
				
				//SDK::CBaseEntity* cum = nullptr;
				SDK::CBaseAnimState* animstate = local_player->GetAnimState();
				if (SETTINGS::settings.tp_bool)
				{
		
					*(Vector*)((DWORD)local_player + 0x31C8) = Vector(GLOBAL::real_angles.x, GLOBAL::real_angles.y, 0.f); // uwu

					if (SETTINGS::settings.aim_type == 0)
					{
					    if (animstate->m_bInHitGroundAnimation && animstate->m_flHeadHeightOrOffsetFromHittingGroundAnimation)
							INTERFACES::pPrediction->SetLocalViewAngles(Vector(-10.f, GLOBAL::real_angles.y, 0.f));
						else
							INTERFACES::pPrediction->SetLocalViewAngles(Vector(GLOBAL::real_angles.x, GLOBAL::real_angles.y, 0.f));

					}
					for (int i = 1; i <= 65; i++)
					{
						auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);

						if (entity != local_player) continue;
						if (local_player->GetHealth() <= 0) continue;
						
						if (entity == local_player)
						{
						//	AnimFix(local_player, false);
						}
					}

			

				}	

				for (int i = 1; i <= 65; i++)
				{
					auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);

					if (!entity) continue;
					if (entity == local_player) continue;
					if (entity->GetHealth() <= 0) continue;
					//if (entity->GetIsDormant()) continue;

					resolver->VisualResolver(entity); // just rip polaks feature names off ezezeze
				}
				
				
				/*for (int i = 1; i <= 65; i++)
				{
					auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);

					if (entity == nullptr) continue;
					if (entity == local_player) continue;	
					if (entity->GetHealth() <= 0) continue;
					if (entity->GetIsDormant()) continue;

					

					desync->HandleHits(entity);
					/////////////////////////////
					desync->AnimationFix(entity);
	
					
				}*/
				
		
			} break;

		case FRAME_NET_UPDATE_START:
			if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
			{
				//if (SETTINGS::settings.beam_bool)
			//	{
				//	visuals->DrawBulletBeams(); // bullet tracers
				//}

				if (SETTINGS::settings.preservekillfed)
				{
					visuals->preservefeed(pEvent); // broken preserve killfeed
				}

				

			} break;
		case FRAME_NET_UPDATE_END:
			if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
			{
				for (int i = 1; i < 65; i++)
				{
					auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);

					
					if (entity == nullptr) continue;

					if (!local_player)
						continue;

					bool is_local_player = entity == local_player;
					bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

					if (is_local_player)
						continue;

					if (is_teammate)
						continue;

					if (entity->GetHealth() <= 0)
						continue;

					//auto VarMap = reinterpret_cast<uintptr_t>(entity) + 36;
				//	auto VarMapSize = *reinterpret_cast<int*>(VarMap + 20);

					//for (auto index = 0; index < VarMapSize; index++)
					//	*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(VarMap) + index * 12) = 0; // bameware

					if (SETTINGS::settings.aim_type == 0)
					{
						backtracking->DisableInterpolation(entity); // you shouldnt do this >:(
				    }


					
				}

			}
			break;
		}
		original_frame_stage_notify(ecx, stage);

		

	}

	

	void __fastcall HookedDrawModelExecute(void* ecx, void* edx, SDK::IMatRenderContext* context, const SDK::DrawModelState_t& state, const SDK::ModelRenderInfo_t& render_info, matrix3x4_t* matrix)
	{
		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			

			
		   auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
				
				
		 
				

			//auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
			std::string ModelName = INTERFACES::ModelInfo->GetModelName(render_info.pModel);
			static SDK::IMaterial* material;
			if (SETTINGS::settings.noFlash)
			{
				SDK::IMaterial* Flash = INTERFACES::MaterialSystem->FindMaterial("effects\\flashbang", "ClientEffect textures");
				SDK::IMaterial* FlashWhite = INTERFACES::MaterialSystem->FindMaterial("effects\\flashbang_white", "ClientEffect textures");
				Flash->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true);
				FlashWhite->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true);
			}

			// idk 
			if (SETTINGS::settings.sleevebool)
			{
				if (ModelName.find("v_sleeve") != std::string::npos)
				{
					SDK::IMaterial* material = INTERFACES::MaterialSystem->FindMaterial(ModelName.c_str(), TEXTURE_GROUP_MODEL);
					if (!material) return;
					material->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true);
					INTERFACES::ModelRender->ForcedMaterialOverride(material);
				}
			}
		

			

			original_draw_model_execute(ecx, context, state, render_info, matrix);
		}
		
	}
	void __fastcall HookedSceneEnd(void* ecx, void* edx)
	{
		original_scene_end(ecx);
		

		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
			//CColor color = CColor(SETTINGS::settings.glow_col[0] * 255, SETTINGS::settings.glow_col[1] * 255, SETTINGS::settings.glow_col[2] * 255, SETTINGS::settings.glow_col[3] * 255), colorTeam = CColor(SETTINGS::settings.teamglow_color[0] * 255, SETTINGS::settings.teamglow_color[1] * 255, SETTINGS::settings.teamglow_color[2] * 255, SETTINGS::settings.teamglow_color[3] * 255), colorlocal = CColor(SETTINGS::settings.glowlocal_col[0] * 255, SETTINGS::settings.glowlocal_col[1] * 255, SETTINGS::settings.glowlocal_col[2] * 255, SETTINGS::settings.glowlocal_col[3] * 255);
			
			original_scene_end(ecx);
			static SDK::IMaterial* ignorez = chams->CreateMaterialBasic(true, true, false);
			static SDK::IMaterial* notignorez = chams->CreateMaterialBasic(false, true, false);
			static SDK::IMaterial* ignorez_metallic = chams->CreateMaterialMetallic(true, true, false);
			static SDK::IMaterial* notignorez_metallic = chams->CreateMaterialMetallic(false, true, false);

			if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
			{
				if (SETTINGS::settings.fakeaashit)
				{
					auto pLocal = reinterpret_cast<SDK::CBaseEntity*>(INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer()));
					if (pLocal)
					{

						static SDK::IMaterial* mat = chams->CreateMaterialBasic(false, true, true);
						if (mat)
						{
							typedef Vector Vector3D;
							Vector3D OrigAng;
							OrigAng = pLocal->GetEyeAngles();
							pLocal->SetAngle2(Vector(0, GLOBAL::fake_angles.y, 0)); // paste he  re ur AA.y value or pLocal->GetLby() (for example)
							bool LbyColor = false; // u can make LBY INDICATOR. When LbyColor is true. Color will be Green , if false it will be White
												   /*CColor NormalColor = CColor( 1, 1, 1 );
												   CColor lbyUpdateColor = CColor(0, 1, 0);*/
												   //	SetColorModulation(LbyColor ? lbyUpdateColor : NormalColor);
							//mat->ColorModulate(255, 255, 255);
							//INTERFACES::ModelRender->ForcedMaterialOverride(mat);
							//pLocal->DrawModel(0x1, 150);
							//INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
							//pLocal->SetAngle2(OrigAng);
						}
					}

				}
				auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
				if (!local_player) return;
				CColor color = SETTINGS::settings.glow_col, colorTeam = SETTINGS::settings.teamglow_color, colorlocal = SETTINGS::settings.glowlocal_col;
				for (int i = 1; i < 65; i++)
				{
					auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);

					if (!entity) continue;
					if (!local_player) continue;

					bool is_local_player = entity == local_player;
					bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;
					auto ignorezmaterial = SETTINGS::settings.chamstype == 0 ? ignorez_metallic : ignorez;
					auto notignorezmaterial = SETTINGS::settings.chamstype == 0 ? notignorez_metallic : notignorez;
					if (is_local_player)
					{
						if (SETTINGS::settings.localchams)
						{
							notignorezmaterial->ColorModulate(SETTINGS::settings.localchams_col);
							INTERFACES::ModelRender->ForcedMaterialOverride(notignorezmaterial);
							local_player->DrawModel(0x1, 255);
							INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
							continue;  break;
						}
					}

					if (entity->GetHealth() <= 0) continue;
					if (entity->GetIsDormant())	continue;
					if (entity->GetClientClass()->m_ClassID != 35) continue;

					if (is_teammate)
					{
						if (entity && SETTINGS::settings.chamsteam == 2)
						{
							ignorezmaterial->ColorModulate(SETTINGS::settings.teaminvis_color);
							INTERFACES::ModelRender->ForcedMaterialOverride(ignorezmaterial);
							entity->DrawModel(0x1, 255);

							notignorezmaterial->ColorModulate(SETTINGS::settings.teamvis_color);
							INTERFACES::ModelRender->ForcedMaterialOverride(notignorezmaterial);
							entity->DrawModel(0x1, 255);

							INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
						}
						else if (entity && SETTINGS::settings.chamsteam == 1)
						{
							notignorezmaterial->ColorModulate(SETTINGS::settings.teamvis_color);
							INTERFACES::ModelRender->ForcedMaterialOverride(notignorezmaterial);
							entity->DrawModel(0x1, 255);

							INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
						} continue;
					}
					else if (is_teammate && SETTINGS::settings.chamsteam)
						continue;


					if (entity && SETTINGS::settings.chams_type == 2)
					{
						ignorezmaterial->ColorModulate(SETTINGS::settings.imodel_col);
						INTERFACES::ModelRender->ForcedMaterialOverride(ignorezmaterial);
						entity->DrawModel(0x1, 255);

						notignorezmaterial->ColorModulate(SETTINGS::settings.vmodel_col);
						INTERFACES::ModelRender->ForcedMaterialOverride(notignorezmaterial);
						entity->DrawModel(0x1, 255);

						INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);	
					}
					else if (entity && SETTINGS::settings.chams_type == 1)
					{
						notignorezmaterial->ColorModulate(SETTINGS::settings.vmodel_col);
						INTERFACES::ModelRender->ForcedMaterialOverride(notignorezmaterial);
						entity->DrawModel(0x1, 255);

						INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
					}
				}

				for (auto i = 0; i < INTERFACES::GlowObjManager->GetSize(); i++)
				{
					auto& glowObject = INTERFACES::GlowObjManager->m_GlowObjectDefinitions[i];
					auto entity = reinterpret_cast<SDK::CBaseEntity*>(glowObject.m_pEntity);

					if (!entity) continue;
					if (!local_player) continue;

					if (glowObject.IsUnused()) continue;

					bool is_local_player = entity == local_player;
					bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

					if (is_local_player && in_tp && SETTINGS::settings.glowlocal)
					{
						if (SETTINGS::settings.gradientglow)
						{
							static float rainbow;
							rainbow += SETTINGS::settings.gradientspeed;
							if (rainbow > 1.f) rainbow = 0.f;
							colorlocal = CColor::FromHSB(rainbow, 1.f, 1.f);
						}
						glowObject.m_nGlowStyle = SETTINGS::settings.glowstylelocal;
						glowObject.m_flRed = colorlocal.RGBA[0] / 255.0f;
						glowObject.m_flGreen = colorlocal.RGBA[1] / 255.0f;
						glowObject.m_flBlue = colorlocal.RGBA[2] / 255.0f;
						glowObject.m_flAlpha = colorlocal.RGBA[3] / 255.0f;
						glowObject.m_bRenderWhenOccluded = true;
						glowObject.m_bRenderWhenUnoccluded = false;
						continue;
					}
					else if (!SETTINGS::settings.glowlocal && is_local_player)
						continue;

					if (entity->GetHealth() <= 0) continue;
					if (entity->GetIsDormant())	continue;
					if (entity->GetClientClass()->m_ClassID != 35) continue;

					if (is_teammate && SETTINGS::settings.glowteam)
					{
						glowObject.m_nGlowStyle = SETTINGS::settings.glowstyle; //0;
						glowObject.m_flRed = colorTeam.RGBA[0] / 255.0f;
						glowObject.m_flGreen = colorTeam.RGBA[1] / 255.0f;
						glowObject.m_flBlue = colorTeam.RGBA[2] / 255.0f;
						glowObject.m_flAlpha = colorTeam.RGBA[3] / 255.0f;
						glowObject.m_bRenderWhenOccluded = true;
						glowObject.m_bRenderWhenUnoccluded = false;
						continue;
					}
					else if (is_teammate && !SETTINGS::settings.glowteam)
						continue;

					if (SETTINGS::settings.glowenable)
					{
						glowObject.m_nGlowStyle = SETTINGS::settings.glowstyle;//0;
						glowObject.m_flRed = color.RGBA[0] / 255.0f;
						glowObject.m_flGreen = color.RGBA[1] / 255.0f;
						glowObject.m_flBlue = color.RGBA[2] / 255.0f;
						glowObject.m_flAlpha = color.RGBA[3] / 255.0f;
						glowObject.m_bRenderWhenOccluded = true;
						glowObject.m_bRenderWhenUnoccluded = false;
					}
				}


				if (SETTINGS::settings.smoke_bool)
				{
					std::vector<const char*> vistasmoke_wireframe = { "particle/vistasmokev1/vistasmokev1_smokegrenade" };

					std::vector<const char*> vistasmoke_nodraw =
					{
						"particle/vistasmokev1/vistasmokev1_fire",
						"particle/vistasmokev1/vistasmokev1_emods",
						"particle/vistasmokev1/vistasmokev1_emods_impactdust",
					};

					for (auto mat_s : vistasmoke_wireframe)
					{
						SDK::IMaterial* mat = INTERFACES::MaterialSystem->FindMaterial(mat_s, TEXTURE_GROUP_OTHER);
						mat->SetMaterialVarFlag(SDK::MATERIAL_VAR_WIREFRAME, true); //wireframe
					}

					for (auto mat_n : vistasmoke_nodraw)
					{
						SDK::IMaterial* mat = INTERFACES::MaterialSystem->FindMaterial(mat_n, TEXTURE_GROUP_OTHER);
						mat->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true);
					}

					static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);
					*(int*)(smokecout) = 0;
				}

		
			}
		}
	}

	void __fastcall HookedOverrideView(void* ecx, void* edx, SDK::CViewSetup* pSetup)
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player) return;

		auto animstate = local_player->GetAnimState();
		if (!animstate) return;



		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			GrenadePrediction::instance().View(pSetup);
			auto GetCorrectDistance = [&local_player](float ideal_distance) -> float //lambda //no dick
			{
				Vector inverse_angles;
				INTERFACES::Engine->GetViewAngles(inverse_angles);

				inverse_angles.x *= -1.f, inverse_angles.y += 180.f;

				Vector direction;
				MATH::AngleVectors(inverse_angles, &direction);

				SDK::CTraceWorldOnly filter;
				SDK::trace_t trace;
				SDK::Ray_t ray;

				ray.Init(local_player->GetVecOrigin() + local_player->GetViewOffset(), (local_player->GetVecOrigin() + local_player->GetViewOffset()) + (direction * (ideal_distance + 5.f)));
				INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);

				return ideal_distance * trace.flFraction;
			};


			if (SETTINGS::settings.tp_bool)
			{
				if (local_player->GetHealth() <= 0)
					local_player->SetObserverMode(5);

				if (!INTERFACES::Input->m_fCameraInThirdPerson)
				{
					INTERFACES::Input->m_fCameraInThirdPerson = true;
					INTERFACES::Input->m_vecCameraOffset = Vector(GLOBAL::real_angles.x, GLOBAL::real_angles.y, GetCorrectDistance(100));

					Vector camForward;
					MATH::AngleVectors(Vector(INTERFACES::Input->m_vecCameraOffset.x, INTERFACES::Input->m_vecCameraOffset.y, 0), &camForward);
				}
			}
			else
			{
				INTERFACES::Input->m_fCameraInThirdPerson = false;
				INTERFACES::Input->m_vecCameraOffset = Vector(GLOBAL::real_angles.x, GLOBAL::real_angles.y, 0);
			}
			auto zoomsensration = INTERFACES::cvar->FindVar("zoom_sensitivity_ratio_mouse");
			if (SETTINGS::settings.fixscopesens)
				zoomsensration->SetValue("0");
			else
				zoomsensration->SetValue("1");

			
			if (!local_player->GetIsScoped())
				pSetup->fov = SETTINGS::settings.fov_val;
			else if (local_player->GetIsScoped() && SETTINGS::settings.removescoping)
				pSetup->fov = SETTINGS::settings.fov_val;
		

			if (SETTINGS::settings.noviusalrecoil)
			{
				Vector aimPunch = local_player->GetPunchAngles();
				Vector viewPunch = local_player->GetViewPunchAngle();

				pSetup->angles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
				pSetup->angles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
				pSetup->angles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));

			}


			/*if (SETTINGS::settings.rcsbool)
			{
				Vector viewPunch = local_player->GetViewPunchAngle();
				Vector AimPunch = local_player->GetPunchAngles();

				if (local_player)
				{
					if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
					{
						pSetup->angles[0] -= viewPunch[0] * 2;
						pSetup->angles[1] -= viewPunch[1] * 2;
						pSetup->angles[2] -= viewPunch[2] * 2;
					}
				}
			}*/
			
		}


		original_override_view(ecx, pSetup);
	}
	void __fastcall HookedTraceRay(void *thisptr, void*, const SDK::Ray_t &ray, unsigned int fMask, SDK::ITraceFilter *pTraceFilter, SDK::trace_t *pTrace)
	{
		original_trace_ray(thisptr, ray, fMask, pTraceFilter, pTrace);
		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			if (SETTINGS::settings.autowall_optimization) // this disables csgos bullet holes (m3m3) and SLIGHTLY improves autowall (apparently)
			{
				pTrace->surface.flags |= SURF_SKY;
			}
		}
	}
	bool __fastcall HookedGetBool(void* pConVar, void* edx)
	{
		if ((uintptr_t)_ReturnAddress() == CAM_THINK)
			return true;

		return original_get_bool(pConVar);
	}
	float __fastcall GetViewmodelFOV()
	{
		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			float player_fov = original_viewmodel_fov();

			if (SETTINGS::settings.esp_bool)
				player_fov = SETTINGS::settings.viewfov_val;

			return player_fov;
		}
	}

	LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg) {
		case WM_LBUTTONDOWN:
			PressedKeys[VK_LBUTTON] = true;
			break;
		case WM_LBUTTONUP:
			PressedKeys[VK_LBUTTON] = false;
			break;
		case WM_RBUTTONDOWN:
			PressedKeys[VK_RBUTTON] = true;
			break;
		case WM_RBUTTONUP:
			PressedKeys[VK_RBUTTON] = false;
			break;
		case WM_MBUTTONDOWN:
			PressedKeys[VK_MBUTTON] = true;
			break;
		case WM_MBUTTONUP:
			PressedKeys[VK_MBUTTON] = false;
			break;
		case WM_XBUTTONDOWN:
		{
			UINT button = GET_XBUTTON_WPARAM(wParam);
			if (button == XBUTTON1)
			{
				PressedKeys[VK_XBUTTON1] = true;
			}
			else if (button == XBUTTON2)
			{
				PressedKeys[VK_XBUTTON2] = true;
			}
			break;
		}
		case WM_XBUTTONUP:
		{
			UINT button = GET_XBUTTON_WPARAM(wParam);
			if (button == XBUTTON1)
			{
				PressedKeys[VK_XBUTTON1] = false;
			}
			else if (button == XBUTTON2)
			{
				PressedKeys[VK_XBUTTON2] = false;
			}
			break;
		}
		case WM_KEYDOWN:
			PressedKeys[wParam] = true;
			break;
		case WM_KEYUP:
			PressedKeys[wParam] = false;
			break;
		default: break;
		}

		if (d3d_init && menu_open)
			return true;

		return CallWindowProc(INIT::OldWindow, hWnd, uMsg, wParam, lParam);
	}


	void GUI_Init(IDirect3DDevice9* pDevice)
	{
		
	}

	struct hud_weapons_t {
		std::int32_t* get_weapon_count() {
			return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
		}
	};
	template<class T>
	static T* FindHudElement(const char* name)
	{
		static auto pThis = *reinterpret_cast<DWORD**>(UTILS::FindSignature("client_panorama.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);

		static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(UTILS::FindSignature("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
		return (T*)find_hud_element(pThis, name);
	}
	void KnifeApplyCallbk()
	{

		static auto clear_hud_weapon_icon_fn =
			reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(
				UTILS::FindSignature("client_panorama.dll", "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C 89 5D FC"));

		auto element = FindHudElement<std::uintptr_t*>("CCSGO_HudWeaponSelection");

		auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xA0);
		if (hud_weapons == nullptr)
			return;

		if (!*hud_weapons->get_weapon_count())
			return;

		for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
			i = clear_hud_weapon_icon_fn(hud_weapons, i);

		static SDK::ConVar* Meme = INTERFACES::cvar->FindVar("cl_fullupdate");
		Meme->nFlags &= ~FCVAR_CHEAT;
		INTERFACES::Engine->ClientCmd_Unrestricted("cl_fullupdate");

	}
	SDK::CBaseWeapon* xd(SDK::CBaseEntity* xz)
	{
		if (!INTERFACES::Engine->IsConnected())
			return nullptr;
		if (!xz->IsAlive())
			return nullptr;

		HANDLE weaponData = *(HANDLE*)((DWORD)xz + OFFSETS::m_hActiveWeapon);
		return (SDK::CBaseWeapon*)INTERFACES::ClientEntityList->GetClientEntityFromHandle(weaponData);
	}

	short SafeWeaponID()
	{
		SDK::CBaseEntity* local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!(local_player))
			return 0;

		SDK::CBaseWeapon* WeaponC = xd(local_player);

		if (!(WeaponC))
			return 0;

		return WeaponC->GetItemDefenitionIndex();
	}
	long __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice)
	{
		static auto ofunc = directz.get_original<EndSceneFn>(42);

		D3DCOLOR rectColor = D3DCOLOR_XRGB(255, 0, 0);
		D3DRECT BarRect = { 1, 1, 1, 1 };
		
		pDevice->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, rectColor, 0, 0);

		if (!d3d_init)
			GUI_Init(pDevice);

		static const D3DRENDERSTATETYPE backupStates[] = { D3DRS_COLORWRITEENABLE, D3DRS_ALPHABLENDENABLE, D3DRS_SRCBLEND, D3DRS_DESTBLEND, D3DRS_BLENDOP, D3DRS_FOGENABLE };
		static const int size = sizeof(backupStates) / sizeof(DWORD);

		DWORD oldStates[size] = { 0 };

		for (int i = 0; i < size; i++)
			pDevice->GetRenderState(backupStates[i], &oldStates[i]);

		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);

		for (int i = 0; i < size; i++)
			pDevice->SetRenderState(backupStates[i], oldStates[i]);

		return ofunc(pDevice);
	}

	long __stdcall Hooked_EndScene_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		static auto ofunc = directz.get_original<EndSceneResetFn>(16);

		if (!d3d_init)
			return ofunc(pDevice, pPresentationParameters);


		auto hr = ofunc(pDevice, pPresentationParameters);



		return hr;
	}


	void draw_hitboxes(SDK::CBaseEntity* pEntity, int r, int g, int b, int a, float duration, float diameter) 
	{
		matrix3x4_t matrix[128];
		if (!pEntity->SetupBones(matrix, 128, 0x00000100, pEntity->GetSimTime()))
			return;
		SDK::studiohdr_t* hdr = INTERFACES::ModelInfo->GetStudioModel(pEntity->GetModel());
		SDK::mstudiohitboxset_t* set = hdr->pHitboxSet(0);
		for (int i = 0; i < set->numhitboxes; i++) {
			SDK::mstudiobbox_t* hitbox = set->GetHitbox(i);
			if (!hitbox)
				continue;
			Vector vMin, vMax;
			auto VectorTransform_Wrapperx = [](const Vector& in1, const matrix3x4_t &in2, Vector &out)
			{
				auto VectorTransform = [](const float *in1, const matrix3x4_t& in2, float *out)
				{
					auto DotProducts = [](const float *v1, const float *v2)
					{
						return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
					};
					out[0] = DotProducts(in1, in2[0]) + in2[0][3];
					out[1] = DotProducts(in1, in2[1]) + in2[1][3];
					out[2] = DotProducts(in1, in2[2]) + in2[2][3];
				};
				VectorTransform(&in1.x, in2, &out.x);
			};
			VectorTransform_Wrapperx(hitbox->bbmin, matrix[hitbox->bone], vMin);
			VectorTransform_Wrapperx(hitbox->bbmax, matrix[hitbox->bone], vMax);
			INTERFACES::DebugOverlay->DrawPill(vMin, vMax, hitbox->radius, r, g, b, a, duration);
		}
	}


	typedef bool(__thiscall *FireEventClientSide)(void*, SDK::IGameEvent*);

	bool __fastcall Hooked_FireEventClientSide(void *ecx, void* edx, SDK::IGameEvent* pEvent) {

		static auto ofunct = fireevent.get_original<FireEventClientSide>(9);

		if (!pEvent)
			return ofunct(ecx, pEvent);

		DamageESP::HandleGameEvent(pEvent);

		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			
			
			

			if (!strcmp(pEvent->GetName(), "player_hurt"))
			{
				int attackerid = pEvent->GetInt("attacker");
				int entityid = INTERFACES::Engine->GetPlayerForUserID(attackerid);
				SDK::CBaseEntity* hurt = (SDK::CBaseEntity*)INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(pEvent->GetInt("userid")));

				if (entityid == INTERFACES::Engine->GetLocalPlayer())
				{
					int nUserID = pEvent->GetInt("attacker");
					int nDead = pEvent->GetInt("userid");
					int gaylol = pEvent->GetInt("hitgroup");

					if (nUserID || nDead)
					{

						if (SETTINGS::settings.showplayerposonhit)
						{
							draw_hitboxes(hurt, 220, 220, 220, 255, 1, 0);
						}
					}
				}
			}

		
		}	



		return ofunct(ecx, pEvent);
	}




	void InitHooks()
	{
		iclient_hook_manager.Init(INTERFACES::Client);
		original_frame_stage_notify = reinterpret_cast<FrameStageNotifyFn>(iclient_hook_manager.HookFunction<FrameStageNotifyFn>(37, HookedFrameStageNotify));

		panel_hook_manager.Init(INTERFACES::Panel);
		original_paint_traverse = reinterpret_cast<PaintTraverseFn>(panel_hook_manager.HookFunction<PaintTraverseFn>(41, HookedPaintTraverse));

		model_render_hook_manager.Init(INTERFACES::ModelRender);
		original_draw_model_execute = reinterpret_cast<DrawModelExecuteFn>(model_render_hook_manager.HookFunction<DrawModelExecuteFn>(21, HookedDrawModelExecute));

		scene_end_hook_manager.Init(INTERFACES::RenderView);
		original_scene_end = reinterpret_cast<SceneEndFn>(scene_end_hook_manager.HookFunction<SceneEndFn>(9, HookedSceneEnd));

		trace_hook_manager.Init(INTERFACES::Trace);
		original_trace_ray = reinterpret_cast<TraceRayFn>(trace_hook_manager.HookFunction<TraceRayFn>(5, HookedTraceRay));

		override_view_hook_manager.Init(INTERFACES::ClientMode);
		original_override_view = reinterpret_cast<OverrideViewFn>(override_view_hook_manager.HookFunction<OverrideViewFn>(18, HookedOverrideView));
		original_create_move = reinterpret_cast<CreateMoveFn>(override_view_hook_manager.HookFunction<CreateMoveFn>(24, HookedCreateMove));
		original_viewmodel_fov = reinterpret_cast<GetViewmodelFOVFn>(override_view_hook_manager.HookFunction<GetViewmodelFOVFn>(35, GetViewmodelFOV));
		//Do_ExtraBonesProcessing = reinterpret_cast<ExtraBoneProcess_t>(Do_ExtraBonesProcessing.HookFunction<ExtraBoneProcess_t>(193, DoExtraBonesProcessing));
		auto sv_cheats = INTERFACES::cvar->FindVar("sv_cheats");
		get_bool_manager = VMT::VMTHookManager(reinterpret_cast<DWORD**>(sv_cheats));
		original_get_bool = reinterpret_cast<SvCheatsGetBoolFn>(get_bool_manager.HookFunction<SvCheatsGetBoolFn>(13, HookedGetBool));



		fireevent.setup(INTERFACES::GameEventManager);
		fireevent.hook_index(9, Hooked_FireEventClientSide);


		while (!(INIT::Window = FindWindowA("Valve001", nullptr)))
			Sleep(100);
		if (INIT::Window)
			INIT::OldWindow = (WNDPROC)SetWindowLongPtr(INIT::Window, GWL_WNDPROC, (LONG_PTR)Hooked_WndProc);
		DWORD DeviceStructureAddress = **(DWORD**)(UTILS::FindSignature("shaderapidx9.dll", "A1 ?? ?? ?? ?? 50 8B 08 FF 51 0C") + 1);
		if (DeviceStructureAddress) {
			directz.setup((DWORD**)DeviceStructureAddress);
			directz.hook_index(16, Hooked_EndScene_Reset);
			directz.hook_index(42, Hooked_EndScene);

		}
	}
	void EyeAnglesPitchHook(const SDK::CRecvProxyData* pData, void* pStruct, void* pOut)
	{
		*reinterpret_cast<float*>(pOut) = pData->m_Value.m_Float;

		auto entity = reinterpret_cast<SDK::CBaseEntity*>(pStruct);
		if (!entity)
			return;

		resolver->networked_angles.x = pData->m_Value.m_Float;
	}

	void EyeAnglesYawHook(const SDK::CRecvProxyData* pData, void* pStruct, void* pOut)
	{
		*reinterpret_cast<float*>(pOut) = pData->m_Value.m_Float;

		auto entity = reinterpret_cast<SDK::CBaseEntity*>(pStruct);
		if (!entity)
			return;

		resolver->networked_angles.y = pData->m_Value.m_Float;
	}
	void InitNetvarHooks()
	{
		UTILS::netvar_hook_manager.Hook("DT_CSPlayer", "m_angEyeAngles[0]", EyeAnglesPitchHook);
		UTILS::netvar_hook_manager.Hook("DT_CSPlayer", "m_angEyeAngles[1]", EyeAnglesYawHook);
	}
}









































































































































































































































































