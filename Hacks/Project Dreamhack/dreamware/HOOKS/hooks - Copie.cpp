#include "../MISC/includes.h"
#include "../MISC/xdxdxd.h"
#include "../FEATURES/skinchanger/skinchanger.h"
#include "../FEATURES/skinchanger/GloveChanger.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"
#include "../ImGui/dx9/imgui_impl_dx9.h"
#include "hooks.h"
#include "../UTILS/interfaces.h"
#include "../UTILS/offsets.h"
#include "../UTILS/NetvarHookManager.h"
#include "../UTILS/render.h"
#include "../SDK/CInput.h"
#include "../SDK/IClient.h"
#include "../SDK/CPanel.h"
#include "../SDK/ConVar.h"
#include "../SDK/CGlowObjectManager.h"
#include "../SDK/IEngine.h"
#include "../SDK/CTrace.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/RecvData.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/ModelInfo.h"
#include "../SDK/ModelRender.h"
#include "../SDK/RenderView.h"
#include "../SDK/CTrace.h"
#include "../SDK/CViewSetup.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/CPrediction.h"

#include "../FEATURES/rankchanger/Protobuffs.h"
#include "../FEATURES/rankchanger/IMemAlloc.h"
#include "../FEATURES/rankchanger/steam.h"
#include "../FEATURES/rankchanger/rankchanger.h"

#include "../FEATURES/Movement.h"
#include "../FEATURES/Visuals.h"
#include "../FEATURES/Chams.h"
#include "../FEATURES/AntiAim.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/Resolver.h"
#include "../FEATURES/Backtracking.h"
#include "../FEATURES/FakeLag.h"
#include "../FEATURES/EnginePred.h"
#include "../FEATURES/EventListener.h"
#include "../FEATURES/GrenadePrediction.h"
#include "../ImGUI/imgui.h"
#include "../ImGUI/imgui_internal.h"
#include "../MENU/menu_framework.h"
#include "../MISC/AWHitmarkers.h"
#include "../MISC/MISC.h"

#include "../MISC/Parser.h"

#include <intrin.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
namespace INIT
{
	HMODULE Dll;
	HWND Window;
	WNDPROC OldWindow;
}
CHackManager Hacks;
#define KEY_DOWN(VK_NNM) ((GetAsyncKeyState(VK_NNM) & 0x8000) ? 1:0)

ImFont* Tabs;
ImFont* Impact;
ImFont* Default;

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
const char* KeyStrings[] = {
	"",
	"Mouse 1",
	"Mouse 2",
	"Cancel",
	"Middle Mouse",
	"Mouse 4",
	"Mouse 5",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",

};


static char ConfigNamexd[64] = { 0 };

const char* GetWeaponNameById(int id)
{
	switch (id)
	{
	case 1:
		return "deagle";
	case 2:
		return "elite";
	case 3:
		return "fiveseven";
	case 4:
		return "glock";
	case 7:
		return "ak47";
	case 8:
		return "aug";
	case 9:
		return "awp";
	case 10:
		return "famas";
	case 11:
		return "g3sg1";
	case 13:
		return "galilar";
	case 14:
		return "m249";
	case 60:
		return "m4a1_silencer";
	case 16:
		return "m4a1";
	case 17:
		return "mac10";
	case 19:
		return "p90";
	case 23:
		return "mp5";
	case 24:
		return "ump45";
	case 25:
		return "xm1014";
	case 26:
		return "bizon";
	case 27:
		return "mag7";
	case 28:
		return "negev";
	case 29:
		return "sawedoff";
	case 30:
		return "tec9";
	case 32:
		return "hkp2000";
	case 33:
		return "mp7";
	case 34:
		return "mp9";
	case 35:
		return "nova";
	case 36:
		return "p250";
	case 38:
		return "scar20";
	case 39:
		return "sg556";
	case 40:
		return "ssg08";
	case 61:
		return "usp_silencer";
	case 63:
		return "cz75a";
	case 64:
		return "revolver";
	case 508:
		return "knife_m9_bayonet";
	case 500:
		return "bayonet";
	case 505:
		return "knife_flip";
	case 506:
		return "knife_gut";
	case 507:
		return "knife_karambit";
	case 509:
		return "knife_tactical";
	case 512:
		return "knife_falchion";
	case 514:
		return "knife_survival_bowie";
	case 515:
		return "knife_butterfly";
	case 516:
		return "knife_push";
	case 519:
		return "knife_ursus";
	case 520:
		return "knife_gypsy_jackknife";
	case 522:
		return "knife_stiletto";
	case 523:
		return "knife_widowmaker";

	default:
		return "";
	}
}

namespace ImGui
{

	static auto vector_getterxd = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};


}
ImFont* bigmenu_font;
ImFont* menu_font;
ImFont* smallmenu_font;
struct CUSTOMVERTEX {
	FLOAT x, y, z;
	FLOAT rhw;
	DWORD color;
	//  FLOAT tu, tv;   
};
typedef unsigned long CBaseHandle;
//--- Other Globally Used Variables ---///
static bool tick = false;
int ground_tick;
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
void slowwalkspeed(int speed)
{
	auto sidespeed = INTERFACES::cvar->FindVar("cl_sidespeed");
	auto forwardspeed = INTERFACES::cvar->FindVar("cl_forwardspeed");
	auto backspeed = INTERFACES::cvar->FindVar("cl_backspeed");
	if (sidespeed->GetInt() == 450 && speed == 450) return;
	sidespeed->SetValue(speed);
	forwardspeed->SetValue(speed);
	backspeed->SetValue(speed);
}

void SlowWalk(SDK::CUserCmd *userCMD)
{
	if (GetAsyncKeyState(SETTINGS::settings.Slowwalkkey))
	{
		GLOBAL::NormalSpeed = false;
		int finalspeed = 250 * SETTINGS::settings.Slowwalkspeed / 100;
		slowwalkspeed(finalspeed);
	}
	if (!GetAsyncKeyState(SETTINGS::settings.Slowwalkkey))
		GLOBAL::NormalSpeed = true;
	else
		GLOBAL::NormalSpeed = false;

	if (GLOBAL::NormalSpeed) slowwalkspeed(450);
}



namespace HOOKS
{
	CreateMoveFn original_create_move;
	PaintTraverseFn original_paint_traverse;
	PaintFn original_paint;
	FrameStageNotifyFn original_frame_stage_notify;
	DrawModelExecuteFn original_draw_model_execute;
	SceneEndFn original_scene_end;
	TraceRayFn original_trace_ray;
	OverrideViewFn original_override_view;
	RenderViewFn original_render_view;
	EndSceneResetFn oEndSceneReset;
	EndSceneFn oEndScene;
	SvCheatsGetBoolFn original_get_bool;
	GetViewmodelFOVFn original_viewmodel_fov;
	SendDatagramFn original_send_datagram = 0;
	PresentFn original_present;

	VMT::VMTHookManager iclient_hook_manager;
	VMT::VMTHookManager panel_hook_manager;
	VMT::VMTHookManager paint_hook_manager;
	VMT::VMTHookManager model_render_hook_manager;
	VMT::VMTHookManager scene_end_hook_manager;
	VMT::VMTHookManager render_view_hook_manager;
	VMT::VMTHookManager trace_hook_manager;
	VMT::VMTHookManager net_channel_hook_manager;
	VMT::VMTHookManager override_view_hook_manager;
	VMT::VMTHookManager input_table_manager;
	VMT::VMTHookManager get_bool_manager;
	VMT::VMTHookManager client_mode_hook_manager;
	VMT::VMTHookManager setup_bones_hook_manager;
	VMT::VMTHookManager direct;
	vfunc_hook directz;

	vfunc_hook gc_hook;

	Protobuffs write;


	CSX::Hook::VTable SurfaceTable;


	EGCResultxd __fastcall hkGCRetrieveMessage(void* ecx, void*, uint32_t *punMsgType, void *pubDest, uint32_t cubDest, uint32_t *pcubMsgSize)
	{
		static auto oGCRetrieveMessage = gc_hook.get_original<HOOKS::GCRetrieveMessage>(vtable_indexes::retrieve_message);
		auto status = oGCRetrieveMessage(ecx, punMsgType, pubDest, cubDest, pcubMsgSize);

		if (status == k_EGCResultOK)
		{

			void* thisPtr = nullptr;
			__asm mov thisPtr, ebx;
			auto oldEBP = *reinterpret_cast<void**>((uint32_t)_AddressOfReturnAddress() - 4);

			uint32_t messageType = *punMsgType & 0x7FFFFFFF;
			write.ReceiveMessage(thisPtr, oldEBP, messageType, pubDest, cubDest, pcubMsgSize);


		}


		return status;
	}



	EGCResultxd __fastcall hkGCSendMessage(void* ecx, void*, uint32_t unMsgType, const void* pubData, uint32_t cubData)
	{
		static auto oGCSendMessage = gc_hook.get_original<HOOKS::GCSendMessage>(vtable_indexes::send_message);

		EGCResult status;

		bool sendMessage = write.PreSendMessage(unMsgType, const_cast<void*>(pubData), cubData);

		if (!sendMessage)
			return k_EGCResultOKxd;

		return oGCSendMessage(ecx, unMsgType, const_cast<void*>(pubData), cubData);
	}

	void ParseSkins()
	{
		valve_parser::Document doc;
		auto r = doc.Load(".\\csgo\\scripts\\items\\items_game.txt", valve_parser::ENCODING::UTF8);


		valve_parser::Document english;
		r = english.Load(".\\csgo\\resource\\csgo_english.txt", valve_parser::ENCODING::UTF16_LE);


		auto weaponSkinCombo = doc.BreadthFirstSearch("weapon_icons");


		auto skinDataVec = doc.BreadthFirstSearchMultiple("paint_kits");


		auto PaintKitNames = english.BreadthFirstSearch("Tokens");



		std::vector<std::string> weaponNames = {
			"deagle",
			"elite",
			"fiveseven",
			"glock",
			"ak47",
			"aug",
			"awp",
			"famas",
			"g3sg1",
			"galilar",
			"m249",
			"m4a1_silencer", //needs to be before m4a1 else silencer doesnt get filtered out :D
			"m4a1",
			"mac10",
			"p90",
			"mp5",
			"ump45",
			"xm1014",
			"bizon",
			"mag7",
			"negev",
			"sawedoff",
			"tec9",
			"hkp2000",
			"mp7",
			"mp9",
			"nova",
			"p250",
			"scar20",
			"sg556",
			"ssg08",
			"usp_silencer",
			"cz75a",
			"revolver",
			"knife_m9_bayonet", //needs to be before bayonet else knife_m9_bayonet doesnt get filtered out :D
			"bayonet",
			"knife_flip",
			"knife_gut",
			"knife_karambit",
			"knife_tactical",
			"knife_falchion",
			"knife_survival_bowie",
			"knife_butterfly",
			"knife_push",
			"knife_ursus",
			"knife_gypsy_jackknife",
			"knife_stiletto",
			"knife_widowmaker"

		};

		//populate G::weaponSkins
		for (auto child : weaponSkinCombo->children)
		{
			if (child->ToObject())
			{
				for (auto weapon : weaponNames)
				{
					auto skinName = child->ToObject()->GetKeyByName("icon_path")->ToKeyValue()->Value.toString();
					auto pos = skinName.find(weapon);
					//filter out the skinname
					if (pos != std::string::npos)
					{
						auto pos2 = skinName.find_last_of('_');
						GLOBAL::weaponSkins[weapon].insert(
							skinName.substr(pos + weapon.length() + 1,
								pos2 - pos - weapon.length() - 1)
						);
						break;
					}
				}
			}
		}

		//populate skinData
		for (auto skinData : skinDataVec)
		{
			if (skinData->ToObject())
			{
				for (auto skin : skinData->children)
				{
					if (skin->ToObject())
					{
						skinInfo si;
						si.paintkit = skin->ToObject()->name.toInt();

						auto skinName = skin->ToObject()->GetKeyByName("name")->ToKeyValue()->Value.toString();
						auto tagNode = skin->ToObject()->GetKeyByName("description_tag");
						if (tagNode)
						{
							std::string tag = tagNode->ToKeyValue()->Value.toString();
							tag = tag.substr(1, std::string::npos); //skip #
							std::transform(tag.begin(), tag.end(), tag.begin(), towlower);
							si.tagName = tag;
						}

						auto keyVal = skin->ToObject()->GetKeyByName("seed");
						if (keyVal != nullptr)
							si.seed = keyVal->ToKeyValue()->Value.toInt();

						GLOBAL::skinMap[skinName] = si;
					}
				}
			}
		}

		//populate G::skinNames
		for (auto child : PaintKitNames->children)
		{
			if (child->ToKeyValue())
			{
				std::string key = child->ToKeyValue()->Key.toString();
				std::transform(key.begin(), key.end(), key.begin(), towlower);
				if (key.find("paintkit") != std::string::npos &&
					key.find("tag") != std::string::npos)
				{
					GLOBAL::skinNames[key] = child->ToKeyValue()->Value.toString();
				}
			}
		}



	}

	bool IsAbleToShoot()
	{
		
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player || local_player->GetHealth() <= 0)
			return;

		auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
		if (!weapon || weapon->GetItemDefenitionIndex() != SDK::WEAPON_REVOLVER)
			return;
		auto flServerTime = (float)local_player->GetTickBase() * INTERFACES::Globals->interval_per_tick;
		auto flNextPrimaryAttack = weapon->NextPrimaryAttack();

		return(!(flNextPrimaryAttack > flServerTime));
	}

	void auto_revolver(SDK::CUserCmd* cmd)
	{
		if (!SETTINGS::settings.auto_revolver_enabled)
			return;

		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player || local_player->GetHealth() <= 0)
			return;

		auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
		if (!weapon || weapon->GetItemDefenitionIndex() != SDK::WEAPON_REVOLVER)
			return;

		if (weapon->GetItemDefenitionIndex() != WEAPON_REVOLVER)
			return;

		if (IsAbleToShoot)
			return;

		static int delay = 0;
		delay--;

		if (delay <= 28)
			cmd->buttons |= IN_ATTACK;
		else
			delay = 0;
	}


	int __fastcall HookedSendDatagram(SDK::NetChannel* ecx, void* edx, void* data)
	{
		if (!ecx)
			return 0;

		if (data || !INTERFACES::Engine->IsInGame() || !SETTINGS::settings.fakelatency_enabled)
			return original_send_datagram(ecx, data);

		backtracking->netchan = ecx;

		int in_reliable_state = ecx->m_nInReliableState;
		int in_sequence_num = ecx->m_nInSequenceNr;

		auto lag_s = SETTINGS::settings.fakelatency_amount / 1000.f;
		auto lag_delta = lag_s - INTERFACES::Engine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);

		backtracking->AddLatency(ecx, lag_delta);
		int ret = original_send_datagram(ecx, data);

		ecx->m_nInReliableState = in_reliable_state;
		ecx->m_nInSequenceNr = in_sequence_num;

		return ret;
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
		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			GrenadePrediction::instance().Tick(cmd->buttons);

			if (!GetAsyncKeyState(0x56))
				fakelag->do_fakelag(cmd);

			static SDK::ConVar* ragdoll = INTERFACES::cvar->FindVar("cl_ragdoll_gravity");
			if (SETTINGS::settings.ragdoll_bool)
			{
				ragdoll->SetValue(-900);
			}
			else {
				ragdoll->SetValue(500);
			}

			static SDK::ConVar* impacts = INTERFACES::cvar->FindVar("sv_showimpacts");
			if (SETTINGS::settings.impacts)
			{
				impacts->SetValue(1);
			}
			else {
				impacts->SetValue(0);
			}

			if (SETTINGS::settings.bBlockBot && GetAsyncKeyState(SETTINGS::settings.iBlockBotKey))
			    Misc::AutoBlocker(cmd);

			if (SETTINGS::settings.Slowwalkenable)
				SlowWalk(cmd);

			Misc::FixCmd(cmd);
			Misc::edgejump(cmd);
		    Misc::g_bhop(cmd);
			Misc::g_strafer(cmd);

			if (SETTINGS::settings.auto_knife) {
				Misc::knife_Run(cmd);
			}
			if (SETTINGS::settings.antiafk)
			{
				if (INTERFACES::Engine->IsInGame() && local_player->IsAlive()) //&& !SETTINGS::settings.skinenabled)
				{

					static bool Jitter;
					Jitter = !Jitter;
					if (Jitter)
						cmd->sidemove += 450;
					else
						cmd->sidemove = -450;

					if (!Jitter)
						cmd->forwardmove = -450;
					else
						cmd->forwardmove = +450;

					cmd->buttons += IN_MOVELEFT;
				}
			}

			if (SETTINGS::settings.zbhob && GetAsyncKeyState(SETTINGS::settings.zbhop_key) & 0x8000)
			{
				static int tick = 0;
				tick += 0.5;
				int handler = tick / 3.14;
				cmd->sidemove = 450 * std::sin(handler);
			}

			static bool enabled = false;
			static bool check = false;
			int airstuckmode = SETTINGS::settings.iAirStuck;
			switch (airstuckmode)
			{
			case 0:
				break;
			case 1:
				if (GetAsyncKeyState(SETTINGS::settings.iAirStuckKey))
				{
					if (!check) {
						enabled = !enabled;
						check = true;
					}
				}
				else {
					check = false;
				}
				if (enabled) {
					auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));

					float ServerTime = local_player->GetTickBase() * INTERFACES::Globals->interval_per_tick;
					float NextPrimaryAttack = weapon->GetNextPrimaryAttack();

					bool Shooting = false;
					if (NextPrimaryAttack > ServerTime)
						Shooting = true;


					if (!Shooting) {
						cmd->tick_count = 9999999;
					}
				}
				break;
			case 2:
				if (KEY_DOWN(SETTINGS::settings.iAirStuckKey))
				{
					cmd->tick_count = 9999999;
				}
				break;
			}
			

			prediction->run_prediction(cmd);
			{

				if (SETTINGS::settings.aim_type == 0 && SETTINGS::settings.aim_bool)
				{
					aimbot->run_aimbot(cmd);
				}
				if (SETTINGS::settings.backtrack)
				{
					backtracking->backtrack_player(cmd);
				}


				if (SETTINGS::settings.aa_bool)
				{
					antiaim->do_antiaim(cmd);
					antiaim->fix_movement(cmd);
				}

			}
			prediction->end_prediction(cmd);

			if (SETTINGS::settings.auto_revolver_enabled)
			{
				auto_revolver(cmd);
			}


			if (!GLOBAL::should_send_packet)
				GLOBAL::real_angles = cmd->viewangles;
			else
			{
				GLOBAL::FakePosition = local_player->GetAbsOrigin();
				GLOBAL::fake_angles = cmd->viewangles;
			}

		}
		*SendPacket = GLOBAL::should_send_packet;
		UTILS::ClampLemon(cmd->viewangles);
		return false;
	}
	void __stdcall HookedPaintTraverse(int VGUIPanel, bool ForceRepaint, bool AllowForce)
	{
		std::string panel_name = INTERFACES::Panel->GetName(VGUIPanel);
		if (panel_name == "HudZoom" && SETTINGS::settings.scope_bool) return;
		if (panel_name == "FocusOverlayPanel")
		{
			if (FONTS::ShouldReloadFonts())
				FONTS::InitFonts();
			visuals->DrawMisc();

			if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
			{
				GrenadePrediction::instance().Paint();

				auto matpostprocess = INTERFACES::cvar->FindVar("mat_postprocess_enable");
				matpostprocess->fnChangeCallback = 0;
				matpostprocess->SetValue(SETTINGS::settings.matpostprocessenable);

				if (SETTINGS::settings.night_bool) visuals->ModulateWorld();


				if (SETTINGS::settings.esp_bool)
				{
					visuals->Draw();
					visuals->ClientDraw();
				}
				if (SETTINGS::settings.hitmarker)
				{
					pHitmarker->Paint();
				}

				auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
				if (!local_player) return;
				auto GetCorrectDistance = [&local_player](float ideal_distance) -> float
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

				
			}

			

			UTILS::INPUT::input_handler.Update();

			if (SETTINGS::settings.logs) visuals->LogEvents();

			static SDK::ConVar* fullbright = INTERFACES::cvar->FindVar("mat_fullbright");
			fullbright->nFlags &= ~FCVAR_CHEAT;
			if (SETTINGS::settings.fullbright)
			{
				fullbright->SetValue(1.00);
			}
			else
				fullbright->SetValue(0.00);
		}
		original_paint_traverse(INTERFACES::Panel, VGUIPanel, ForceRepaint, AllowForce);
	}
	void MessageSend()
	{
		write.SendClientHello();
		write.SendMatchmakingClient2GCHello();
	}
	void __fastcall HookedFrameStageNotify(void* ecx, void* edx, int stage)
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player) return;
		Vector vecAimPunch, vecViewPunch;
		Vector* pAimPunch = nullptr; Vector* pViewPunch = nullptr;
		visuals->ModulateSky();
		visuals->DoFSN();
		switch (stage)
		{
			case FRAME_NET_UPDATE_POSTDATAUPDATE_START:

				if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
				{
					g_run_gloves();
					g_run_skins();
					if (SETTINGS::settings.connectinventory)
						InventarNewSkinChanger(); //   <- connect shit for inventar ingame skins
					
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
						if (SETTINGS::settings.ResolverEnable == 0) return;

						if (SETTINGS::settings.ResolverEnable == 1) {
							resolver->resolve(entity);
						}
						if (SETTINGS::settings.ResolverEnable == 2) {
							resolver->resolve2(entity);
						}
					}
				} 
			    break;
			case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
				break;
			case FRAME_RENDER_START:
				if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
				{

					if (in_tp)
					{
						*(Vector*)((DWORD)local_player + 0x31C8) = Vector(GLOBAL::real_angles.x, GLOBAL::real_angles.y, 0.f);

						INTERFACES::pPrediction->SetLocalViewAngles(GLOBAL::real_angles);
						local_player->UpdateClientSideAnimation();
						INTERFACES::pPrediction->SetLocalViewAngles(GLOBAL::fake_angles);

					}
					for (int i = 1; i <= 65; i++)
					{
						auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
						if (!entity) continue;
						if (entity == local_player) continue;

						*(int*)((uintptr_t)entity + 0xA30) = INTERFACES::Globals->framecount;
						*(int*)((uintptr_t)entity + 0xA28) = 0;
					}
				} break;

			case FRAME_NET_UPDATE_START:
				if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
				{
					if (SETTINGS::settings.beam_bool)
						visuals->DrawBulletBeams();
				} break;
			case FRAME_NET_UPDATE_END:
				if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
				{
					for (int i = 1; i < 65; i++)
					{
						auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);

						if (!entity)
							continue;

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

						if (SETTINGS::settings.aim_type == 0)
							backtracking->DisableInterpolation(entity);
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
			std::string ModelName = INTERFACES::ModelInfo->GetModelName(render_info.pModel);

			if (SETTINGS::settings.misc_no_sleeves)
			{
				if (ModelName.find("v_sleeve") != std::string::npos)
				{
					SDK::IMaterial* material = INTERFACES::MaterialSystem->FindMaterial(ModelName.c_str(), TEXTURE_GROUP_MODEL);
					if (!material) return;
					material->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true);
					INTERFACES::ModelRender->ForcedMaterialOverride(material);
				}
			}
			auto entity = INTERFACES::ClientEntityList->GetClientEntity(render_info.entity_index);
			if (entity == local_player)
			{
				if (local_player && local_player->GetIsScoped())
					INTERFACES::RenderView->SetBlend(SETTINGS::settings.transparency_when_scoped * 0.01f);
			}

			if (SETTINGS::settings.noflash) {
				SDK::IMaterial* Flash = INTERFACES::MaterialSystem->FindMaterial("effects\\flashbang", "ClientEffect textures");
				SDK::IMaterial* FlashWhite = INTERFACES::MaterialSystem->FindMaterial("effects\\flashbang_white", "ClientEffect textures");
				Flash->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true);
				FlashWhite->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true);
				original_draw_model_execute(ecx, context, state, render_info, matrix);
			}

			

			
		}
		original_draw_model_execute(ecx, context, state, render_info, matrix);
	}
	void __fastcall HookedSceneEnd(void* ecx, void* edx)
	{
		original_scene_end(ecx);
		static SDK::IMaterial* ignorez = chams->CreateMaterialBasic(true, true, false);
		static SDK::IMaterial* notignorez = chams->CreateMaterialBasic(false, true, false);
		static SDK::IMaterial* ignorez_metallic = chams->CreateMaterialMetallic(true, true, false);
		static SDK::IMaterial* notignorez_metallic = chams->CreateMaterialMetallic(false, true, false);

		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
			if (!local_player) return;
			CColor color = CColor(SETTINGS::settings.glow_col), colorTeam = CColor(SETTINGS::settings.teamglow_color), colorlocal = CColor(SETTINGS::settings.glowlocal_col);
			for (int i = 1; i < 65; i++)
			{
				if (SETTINGS::settings.fakechams)
				{
					auto pLocal = reinterpret_cast<SDK::CBaseEntity*>(INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer()));
					if (pLocal && pLocal->IsAlive())
					{
						static SDK::IMaterial* mat = chams->CreateMaterialBasic(false, true, false);
						if (mat)
						{
							Vector OrigAng;
							OrigAng = pLocal->GetEyeAngles();
							pLocal->SetAngle2(Vector(0, GLOBAL::fake_angles.y, 0));
							bool LbyColor = false;
							mat->ColorModulate(CColor(SETTINGS::settings.fakechamscol));
							INTERFACES::ModelRender->ForcedMaterialOverride(mat);
							pLocal->DrawModel(0x1, 150);
							INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
							pLocal->SetAngle2(OrigAng);
						}
					}
				}

				auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);

				if (!entity) continue;
				if (!local_player) continue;

				bool is_local_player = entity == local_player;
				bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;
				auto ignorezmaterial = SETTINGS::settings.chamstype == 0 ? ignorez_metallic : ignorez;
				auto notignorezmaterial = SETTINGS::settings.chamstype == 0 ? notignorez_metallic : notignorez;
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))

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
					if (local_player->GetVelocity().Length2D() > 30 && local_player && SETTINGS::settings.localchams_fakelag && INTERFACES::Input->m_fCameraInThirdPerson)
					{
						static auto last_networked_origin = Vector(0, 0, 0);
						if (GLOBAL::should_send_packet)
							last_networked_origin = local_player->GetVecOrigin();
						local_player->SetAbsOrigin(last_networked_origin);
						//alpha
						static auto alpha = 0.f; static auto plus_or_minus = false;
						if (alpha <= 0.f || alpha >= 255.f)
							plus_or_minus = !plus_or_minus;
						alpha += plus_or_minus ? (255.f / 7 * 0.015) : -(255.f / 7 * 0.015);
						alpha = clamp(alpha, 0.f, 255.f);
						//alpha end
						notignorezmaterial->ColorModulate(SETTINGS::settings.localchams_col_lag);
						notignorezmaterial->AlphaModulate(alpha);
						INTERFACES::ModelRender->ForcedMaterialOverride(notignorezmaterial);
						local_player->DrawModel(0x1, 255);
						INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);

					}
				}

				if (entity->GetHealth() <= 0) continue;
				if (entity->GetIsDormant())	continue;
				if (entity->GetClientClass()->m_ClassID != 38) continue;

				if (is_teammate)
				{
					if (entity && SETTINGS::settings.chamsteam == 2)
					{
						ignorezmaterial->ColorModulate(CColor(SETTINGS::settings.teaminvis_color));
						INTERFACES::ModelRender->ForcedMaterialOverride(ignorezmaterial);
						entity->DrawModel(0x1, 255);

						notignorezmaterial->ColorModulate(CColor(SETTINGS::settings.teamvis_color));
						INTERFACES::ModelRender->ForcedMaterialOverride(notignorezmaterial);
						entity->DrawModel(0x1, 255);

						INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
					}
					else if (entity && SETTINGS::settings.chamsteam == 1)
					{
						notignorezmaterial->ColorModulate(CColor(SETTINGS::settings.teamvis_color));
						INTERFACES::ModelRender->ForcedMaterialOverride(notignorezmaterial);
						entity->DrawModel(0x1, 255);

						INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
					} continue;
				}
				else if (is_teammate && SETTINGS::settings.chamsteam)
					continue;

				if (entity && SETTINGS::settings.chams_type == 2)
				{
					ignorezmaterial->ColorModulate(CColor(SETTINGS::settings.imodel_col));
					INTERFACES::ModelRender->ForcedMaterialOverride(ignorezmaterial);
					entity->DrawModel(0x1, 255);

					notignorezmaterial->ColorModulate(CColor(SETTINGS::settings.vmodel_col));
					INTERFACES::ModelRender->ForcedMaterialOverride(notignorezmaterial);
					entity->DrawModel(0x1, 255);

					INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
				}
				else if (entity && SETTINGS::settings.chams_type == 1)
				{
					notignorezmaterial->ColorModulate(CColor(SETTINGS::settings.vmodel_col));
					INTERFACES::ModelRender->ForcedMaterialOverride(notignorezmaterial);
					entity->DrawModel(0x1, 255);

					INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
				}
			}

			for (auto i = 0; i < INTERFACES::GlowObjManager->GetSize(); i++)
			{
				auto &glowObject = INTERFACES::GlowObjManager->m_GlowObjectDefinitions[i];
				auto entity = reinterpret_cast<SDK::CBaseEntity*>(glowObject.m_pEntity);

				if (!entity) continue;
				if (!local_player) continue;

				if (glowObject.IsUnused()) continue;

				bool is_local_player = entity == local_player;
				bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

				if (is_local_player && in_tp && SETTINGS::settings.glowlocal)
				{
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
				if (entity->GetClientClass()->m_ClassID != 38) continue;

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
	void __fastcall HookedOverrideView(void* ecx, void* edx, SDK::CViewSetup* pSetup)
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player) return;

		auto animstate = local_player->GetAnimState();
		if (!animstate) return;

		if (GetAsyncKeyState(UTILS::INPUT::input_handler.keyBindings(SETTINGS::settings.thirdperson_int)) & 1)
			in_tp = !in_tp;

		if(GetAsyncKeyState((SETTINGS::settings.thirdpersonkey)) & 1)
			in_tp = !in_tp;
		GLOBAL::b_IsThirdPerson = true;

		if (SETTINGS::settings.norecoil)
		{
			pSetup->angles -= *local_player->GetAimPunchAngle() * 0.9f + *local_player->GetViewPunchAngle();
		}

		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			GrenadePrediction::instance().View(pSetup);
			auto GetCorrectDistance = [&local_player](float ideal_distance) -> float //lambda
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

			if (SETTINGS::settings.tp_bool && in_tp)
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

			if (SETTINGS::settings.aim_type == 0)
			{
				if (!local_player->GetIsScoped())
					pSetup->fov = SETTINGS::settings.fov_val;
				else if (local_player->GetIsScoped() && SETTINGS::settings.removescoping)
					pSetup->fov = SETTINGS::settings.fov_val;
			}
			else if (!(SETTINGS::settings.aim_type == 0) && !local_player->GetIsScoped())
				pSetup->fov = 90;
		}

		original_override_view(ecx, pSetup);
	}
	void __fastcall HookedTraceRay(void *thisptr, void*, const SDK::Ray_t &ray, unsigned int fMask, SDK::ITraceFilter *pTraceFilter, SDK::trace_t *pTrace)
	{
		original_trace_ray(thisptr, ray, fMask, pTraceFilter, pTrace);
		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
			pTrace->surface.flags |= SURF_SKY;
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
	void Hook_LockCursor(void* xd)
	{
		SurfaceTable.UnHook();
		INTERFACES::Surface->lockcursor();
		SurfaceTable.ReHook();
		if (menu_open)
			INTERFACES::Surface->unlockcursor();
	}

#define D3DX_PI    ((FLOAT)  3.141592654f)

	void CircleFilledRainbowColor(float x, float y, float rad, float rotate, int type, int resolution, IDirect3DDevice9* m_device)
	{
		LPDIRECT3DVERTEXBUFFER9 g_pVB2;

		std::vector<CUSTOMVERTEX> circle(resolution + 2);

		float angle = rotate * D3DX_PI / 180, pi = D3DX_PI;

		if (type == 1)
			pi = D3DX_PI; // Full circle
		if (type == 2)
			pi = D3DX_PI / 2; // 1/2 circle
		if (type == 3)
			pi = D3DX_PI / 4; // 1/4 circle

		pi = D3DX_PI / type; // 1/4 circle

		circle[0].x = x;
		circle[0].y = y;
		circle[0].z = 0;
		circle[0].rhw = 1;
		circle[0].color = D3DCOLOR_RGBA(0, 0, 0, 0);

		float hue = 0.f;

		for (int i = 1; i < resolution + 2; i++)
		{
			circle[i].x = (float)(x - rad * cos(pi*((i - 1) / (resolution / 2.0f))));
			circle[i].y = (float)(y - rad * sin(pi*((i - 1) / (resolution / 2.0f))));
			circle[i].z = 0;
			circle[i].rhw = 1;


			auto clr = CColor::HSBtoRGB(hue, 1.f, 1.f);
			circle[i].color = D3DCOLOR_RGBA(clr.RGBA[0], clr.RGBA[1], clr.RGBA[2], clr.RGBA[3]);
			hue += 0.02;
		}

		// Rotate matrix
		int _res = resolution + 2;
		for (int i = 0; i < _res; i++)
		{
			float Vx1 = x + (cosf(angle) * (circle[i].x - x) - sinf(angle) * (circle[i].y - y));
			float Vy1 = y + (sinf(angle) * (circle[i].x - x) + cosf(angle) * (circle[i].y - y));

			circle[i].x = Vx1;
			circle[i].y = Vy1;
		}

		m_device->CreateVertexBuffer((resolution + 2) * sizeof(CUSTOMVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB2, NULL);

		VOID* pVertices;
		g_pVB2->Lock(0, (resolution + 2) * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0);
		memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(CUSTOMVERTEX));
		g_pVB2->Unlock();

		m_device->SetTexture(0, NULL);
		m_device->SetPixelShader(NULL);
		m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_device->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX));
		m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		m_device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
		if (g_pVB2 != NULL)
			g_pVB2->Release();
	}
	long __stdcall Hooked_Present(IDirect3DDevice9* pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion)
	{
		auto oPresent = original_present;


		if (INTERFACES::Engine->IsInGame() && INTERFACES::Engine->IsConnected())
		{
			if (SETTINGS::settings.type_spread_circle == 2) {
				static float rot = 0.f;
				int w, h;
				INTERFACES::Engine->GetScreenSize(w, h); w /= 2; h /= 2;
				auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
				auto pWeapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
				if (pWeapon)
				{
					int Index = pWeapon->GetItemDefenitionIndex();
					if (local_player && Index != 42 && Index != 59 && Index != 500)
					{
						auto accuracy = pWeapon->GetInaccuracy() * 550.f;
						CircleFilledRainbowColor(w, h, accuracy, rot, 1, 50, pDevice);

						rot += 1.f;
						if (rot > 360.f) rot = 0.f;
					}
				}
			}
		}

		return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	}

	void OpenMenu()
	{
		static bool is_down = false;
		static bool is_clicked = false;
		if (GetAsyncKeyState(VK_INSERT))
		{
			is_clicked = false;
			is_down = true;
		}
		else if (!GetAsyncKeyState(VK_INSERT) && is_down)
		{
			is_clicked = true;
			is_down = false;
		}
		else
		{
			is_clicked = false;
			is_down = false;
		}

		if (is_clicked)
		{
			menu_open = !menu_open;

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

		OpenMenu();

		if (d3d_init && menu_open && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return true;

		return CallWindowProc(INIT::OldWindow, hWnd, uMsg, wParam, lParam);
	}
	void GUI_Init(IDirect3DDevice9* pDevice)
	{
		ImGui_ImplDX9_Init(INIT::Window, pDevice);




		ImGuiIO& io = ImGui::GetIO();

		auto& style = ImGui::GetStyle();


		style.Colors[ImGuiCol_Text] = ImColor(93, 89, 242, 255); // Text Color
		style.Colors[ImGuiCol_WindowBg] = ImVec4(.137f, .137f, .137f, 1.00f); //Menu background color
																			  //style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);	
		style.Colors[ImGuiCol_Border] = ImColor(93, 89, 242, 255); //Borders color of the menu
		style.Colors[ImGuiCol_Button] = ImColor(26, 28, 33, 180);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(93, 89, 242, 180); //Buton color when we go with the mouse
		style.Colors[ImGuiCol_ButtonActive] = ImColor(93, 89, 242, 180); //Button color when we press
		style.Colors[ImGuiCol_TitleBg] = ImColor(93, 89, 242, 255); //Title of the menu background color
		style.Colors[ImGuiCol_TitleBgActive] = ImColor(93, 89, 242, 255); //Title of the active menu background color
																			   //style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);

		style.Colors[ImGuiCol_FrameBg] = ImColor(26, 28, 33, 180); //Background color of the checkbox ...
		style.Colors[ImGuiCol_FrameBgHovered] = ImColor(93, 89, 242, 255); //Background of the item when hovered
		style.Colors[ImGuiCol_FrameBgActive] = ImColor(93, 89, 242, 255); //Background color of the active item

		style.Colors[ImGuiCol_ComboBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.8f); //Combobox background color
		style.Colors[ImGuiCol_CheckMark] = ImColor(93, 89, 242, 255); //Radiobutton or checkbox mark color

		style.Colors[ImGuiCol_MenuBarBg] = ImColor(93, 89, 242, 255);
		style.Colors[ImGuiCol_ScrollbarBg] = ImColor(93, 89, 242, 255); //Scrollbar background color
		style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(93, 89, 242, 255); //Scrollbar color
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(93, 89, 242, 255); //Scrollbar color when hovered
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(93, 89, 242, 255); //Scrollbar color when grabed
		style.Colors[ImGuiCol_SliderGrab] = ImColor(93, 89, 242, 255); //Slider color 
		style.Colors[ImGuiCol_SliderGrabActive] = ImColor(93, 89, 242, 255);//Slider color when grabed
		style.Colors[ImGuiCol_Header] = ImColor(93, 89, 242, 255);//Color of the selected item in a combolist
		style.Colors[ImGuiCol_HeaderHovered] = ImColor(93, 89, 242, 255);//Color of the hovered item in a combolist
		style.Colors[ImGuiCol_HeaderActive] = ImColor(93, 89, 242, 255);//Color of the selected item in a combolist when pressed


		style.WindowRounding = 0.f;
		style.FramePadding = ImVec2(0, 0);
		style.ScrollbarSize = 6.f;
		style.ScrollbarRounding = 0.f;
		style.GrabMinSize = 6.f;

		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.WindowMinSize = ImVec2(200, 200);
		style.Alpha = 1.f;
		style.FrameRounding = 0.0f;
		style.IndentSpacing = 6.0f;
		style.ColumnsMinSpacing = 50.0f;
		style.GrabRounding = 3.0f;
		style.AntiAliasedLines = true;

		Default = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Ruda-Bold.ttf", 13.0f);
		Tabs = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Ruda-Bold.ttf", 15.0f);
		GLOBAL::Text = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 14.0f); // Fontsize is now ok

		d3d_init = true;

	}
	void BtnMenuTab()
	{
		auto& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_Button] = ImColor(40, 42, 47, 255);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(40, 42, 47, 255);
		style.Colors[ImGuiCol_ButtonActive] = ImColor(40, 42, 47, 255);
		style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);

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
	bool ColorsTab = false;
#define XorStr( s ) ( s )

	int aSubTab = 0;
	int aCSubTab = 0;
	bool ConfisgTab = false;
	void DrawRageTab()
	{
		auto& styler = ImGui::GetStyle();
		static int rageuo = 1;
		//BtnMenuTab();

		if (ImGui::Button("aimbot", ImVec2(654 / 2, 30)))
			rageuo = 1;

		ImGui::SameLine();

		if (ImGui::Button("anti-aim", ImVec2(656 / 2, 30)))
			rageuo = 2;

		styler.Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
		styler.Colors[ImGuiCol_Border] = ImColor(60, 60, 60, 175);
		if (rageuo == 1)
		{
			ImGui::NewLine();
			ImGui::BeginChild("main", ImVec2(179, 343), true);
			{
				ImGui::Checkbox(("enable esp"), &SETTINGS::settings.esp_bool);
				ImGui::Checkbox(("indicator"), &SETTINGS::settings.autoindicator);
				ImGui::Checkbox(("open color"), &SETTINGS::settings.opencolor);
				if (!SETTINGS::settings.Antiuntrusted) {
					ImGui::Checkbox("No Spread", &SETTINGS::settings.NoSpread);
				}
			}ImGui::EndChild();

			ImGui::SameLine();

			ImGui::BeginChild("MultiPoint", ImVec2(179, 343), true);
			{
				ImGui::Checkbox(("enable esp"), &SETTINGS::settings.esp_bool);
				ImGui::Checkbox(("indicator"), &SETTINGS::settings.autoindicator);
				ImGui::Checkbox(("open color"), &SETTINGS::settings.opencolor);

			}ImGui::EndChild();

			ImGui::SameLine();

			ImGui::BeginChild("HitScan", ImVec2(179, 343), true);
			{
				ImGui::Checkbox(("enable esp"), &SETTINGS::settings.esp_bool);
				ImGui::Checkbox(("indicator"), &SETTINGS::settings.autoindicator);
				ImGui::Checkbox(("open color"), &SETTINGS::settings.opencolor);

			}ImGui::EndChild();
		}
		if (rageuo == 2)
		{
			ImGui::NewLine();

			ImGui::BeginChild("jump", ImVec2(179, 170), true);
			{
				ImGui::Checkbox(("enable esp"), &SETTINGS::settings.esp_bool);
				ImGui::Checkbox(("indicator"), &SETTINGS::settings.autoindicator);
				ImGui::Checkbox(("open color"), &SETTINGS::settings.opencolor);
			}ImGui::EndChild();

			ImGui::SameLine();

			ImGui::BeginChild("move", ImVec2(179, 170), true);
			{
				ImGui::Checkbox(("enable esp"), &SETTINGS::settings.esp_bool);
				ImGui::Checkbox(("indicator"), &SETTINGS::settings.autoindicator);
				ImGui::Checkbox(("open color"), &SETTINGS::settings.opencolor);

			}ImGui::EndChild();
		}

	}

	int vSubTab = 0;
	int cSubTab = 0;
	int cSubTab2 = 0;
	int cSubTab3 = 0;
	void DrawVisualsTab()
	{
		if (vSubTab == 0)
		{
			ImGui::BeginChild("##player", ImVec2(190, 360), true);
			{
				ImGui::RadioButton("player esp", &cSubTab3, 0);
				ImGui::SameLine();
				ImGui::RadioButton("other esp", &cSubTab3, 1);

				if (cSubTab3 == 0)
				{
					ImGui::Checkbox(("enable esp"), &SETTINGS::settings.esp_bool);
					ImGui::Checkbox(("indicator"), &SETTINGS::settings.autoindicator);
					ImGui::Checkbox(("open color"), &SETTINGS::settings.opencolor);
					ImGui::Checkbox(("open cfg"), &SETTINGS::settings.opencfg);
					ImGui::Checkbox(("enable legit"), &SETTINGS::settings.esp_bool);
					
					ImGui::Checkbox("thirdperson", &SETTINGS::settings.tp_bool);
					ImGui::Hotkey("key", &SETTINGS::settings.thirdpersonkey, ImVec2(40, 25));

					
					
					const char* weptype[] =
					{
						"type1",
						"type2"

					};
					ImGui::Text("box type");
					ImGui::Checkbox(("enable legit"), &SETTINGS::settings.esp_bool);

					const char* healthtype[] =
					{
						"type1",
						"type2"

					};
				}
				if (cSubTab3 == 1)
				{
					ImGui::Checkbox(("enable legit"), &SETTINGS::settings.esp_bool);

					const char* healthtype[] =
					{
						"type1",
						"type2"

					};
				}
			}
			ImGui::EndChild();

			ImGui::SameLine();
			ImGui::BeginChild("##models", ImVec2(190, 360), true);
			{
				ImGui::RadioButton("esp", &cSubTab, 0);
				ImGui::SameLine();
				ImGui::RadioButton("misc", &cSubTab, 1);
				if (cSubTab == 0)
				{
					ImGui::Checkbox(("enable legit"), &SETTINGS::settings.esp_bool);
					ImGui::Checkbox(("enable legit"), &SETTINGS::settings.esp_bool);
					const char* weptype[] =
					{
						"type1",
						"type2"

					};
					ImGui::Text("box type");

				}
				if (cSubTab == 1)
				{
					ImGui::Checkbox(("enable legit"), &SETTINGS::settings.esp_bool);
					ImGui::Checkbox(("enable legit"), &SETTINGS::settings.esp_bool);
					const char* weptype[] =
					{
						"type1",
						"type2"

					};
					ImGui::Text("box type");
					ImGui::Checkbox(("enable legit"), &SETTINGS::settings.esp_bool);
					ImGui::Checkbox(("enable legit"), &SETTINGS::settings.esp_bool);
					ImGui::Checkbox(("enable legit"), &SETTINGS::settings.esp_bool);
					const char* healthtype[] =
					{
						"type1",
						"type2"

					};
				}
			}
			ImGui::EndChild();
		}
	}
	void DrawColorsTab()
	{
		//ImGui::PushFont(Default);
		ImGui::SetNextWindowSize(ImVec2(300, 300));
		if (ImGui::Begin("colors", &ColorsTab, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize));
		{
			ImGui::BeginChild("colors", ImVec2(280, 270), true);
			{
				ImGui::Text(("Box Colors"));
				ImGui::Checkbox(("enable legit"), &SETTINGS::settings.esp_bool);


				ImGui::Text(("Glows"));
				ImGui::Checkbox(("enable legit"), &SETTINGS::settings.esp_bool);

				ImGui::Text(("Bullet Tracers"));
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}
	void ConfigTab()
	{
		//ImGui::PushFont(Default);
		ImGui::SetNextWindowSize(ImVec2(300, 300));
		if (ImGui::Begin("configs", &ConfisgTab, ImVec2(300, 300), 0.98f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar))
		{
			ImGui::TextColored(ImColor(93, 89, 242, 255), "configuration");
			ImGui::BeginChild("configuration", ImVec2(280, 270), true);
			{
				static const char* configs[] = { "auto", "scout", "awp", "pistol", "nospread", "legit" };

				std::string config;

				switch (ImGui::Combo("configurations", &SETTINGS::settings.config_sel, configs, ARRAYSIZE(configs)))
				{

				case 0: config = "auto"; break;
				case 1: config = "scout"; break;
				case 2: config = "awp"; break;
				case 3: config = "pistol"; break;
				case 4: config = "nospread"; break;
				case 5: config = "legit"; break;
				}

				if (ImGui::Button("Load Config", ImVec2(120, 30)))

				{
					SETTINGS::settings.Load(config);

					GLOBAL::Msg("Configuration loaded.    \n");
				}

				if (ImGui::Button("Save Config", ImVec2(120, 30)))
				{
					SETTINGS::settings.Save(config);

					GLOBAL::Msg("Configuration saved.    \n");
				}
			}
			ImGui::EndChild();
		}
		//ImGui::PopFont();

		ImGui::End();
	}
	void DrawMiscTab()
	{
		ImGui::BeginChild("misc", ImVec2(225, 495), true);
		{
			ImGui::Checkbox("Anti-untrusted", &SETTINGS::settings.Antiuntrusted);

			const char* key_binds[] = { "none", "mouse1", "mouse2", "mouse3", "mouse4", "mouse5", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12" };
			const char* iAutostrafer[] =
			{
				"Silent",
				"Normal",
				"Sideways",
				"On Move"
			};

			ImGui::Checkbox("Auto knife", &SETTINGS::settings.auto_knife);
			ImGui::Checkbox("Auto bhop", &SETTINGS::settings.AutoJump);
			ImGui::Checkbox("Auto strafer", &SETTINGS::settings.AutoStrafe);
			ImGui::Checkbox("(WASD) keys", &SETTINGS::settings.wasd);

			ImGui::Text("Edge Jump");
			ImGui::Combo("###Edge Jump", &SETTINGS::settings.edgejumpkey, KeyStrings, ARRAYSIZE(KeyStrings));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Jump at the edge of a surface");
			if (SETTINGS::settings.edgejumpkey > 0)
			{
				ImGui::Checkbox("strafe in air", &SETTINGS::settings.strafeedgejump);
			}


			ImGui::Checkbox(("anti-afk"), &SETTINGS::settings.antiafk);

			ImGui::Checkbox(("thirdperson"), &SETTINGS::settings.tp_bool);
			ImGui::Combo(("thirdperson key"), &SETTINGS::settings.thirdperson_int, key_binds, ARRAYSIZE(key_binds));
		
			ImGui::Checkbox("Blockbot", &SETTINGS::settings.bBlockBot);
			ImGui::Hotkey("##blockbotkey", &SETTINGS::settings.iBlockBotKey, ImVec2(100, 20));
			const char* iAirstuck[] =
			{
				"Off",
				"Toggle",
				"Hold"
			};
			ImGui::Text("Airstuck");
			ImGui::Combo("##airstuck", &SETTINGS::settings.iAirStuck, iAirstuck, ARRAYSIZE(iAirstuck));
			ImGui::Hotkey("##key", &SETTINGS::settings.iAirStuckKey, ImVec2(100, 20));
			ImGui::Checkbox("Z-hop", &SETTINGS::settings.zbhob);
			ImGui::Hotkey("##Z-hop-key", &SETTINGS::settings.zbhop_key, ImVec2(100, 20));

		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("settings", ImVec2(225, 495), true);
		{
			ImGui::Checkbox("Slow Walk", &SETTINGS::settings.Slowwalkenable);
			ImGui::Combo("###Slow Walk", &SETTINGS::settings.Slowwalkkey, KeyStrings, ARRAYSIZE(KeyStrings));
			ImGui::Text("Slow Walk Speed");
			ImGui::SliderFloat("###Slowwalkspeed", &SETTINGS::settings.Slowwalkspeed, 0, 100);
			

		}
		ImGui::EndChild();
	}

	/*struct hud_weapons_t
	{
		std::int32_t* get_weapon_count()
		{
			return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
		}
	};
	template<class T>
	static T* FindHudElement(const char* name)
	{
		static auto pThis = *reinterpret_cast<DWORD**>(UTILS::FindSignature(("client_panorama.dll"), ("B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08")) + 1);

		static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(UTILS::FindSignature(("client_panorama.dll"), ("55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28")));
		return (T*)find_hud_element(pThis, name);
	}*/
	/*void KnifeApplyCallbk()
	{

		static auto clear_hud_weapon_icon_fn = reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(UTILS::FindSignature(("client_panorama.dll"), ("55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C")));

		auto element = FindHudElement<std::uintptr_t*>(("CCSGO_HudWeaponSelection"));

		auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xA0);
		if (hud_weapons == nullptr)
			return;

		if (!*hud_weapons->get_weapon_count())
			return;

		for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
			i = clear_hud_weapon_icon_fn(hud_weapons, i);

		INTERFACES::Globals->ForceFullUpdate();
	}*/
	const char* knifemodels[]{ "Bayonet",
	"Flip Knife",
	"Gut Knife",
	"Karambit",
	"M9 Bayonet",
	"Huntsman Knife",
	"Butterfly Knife",
	"Falchion Knife",
	"Shadow Daggers",
	"Bowie Knife",
	"Navaja Knife",
	"Stiletto Knife",
	"Ursus Knife",
	"Talon Knife" };


	void DrawSkinTab()
	{
		ImGui::BeginChild("skinchanger", ImVec2(213, 495), true);
		{

			/*const char* knifeskins[] =
			{
				"None",
				"Crimson Web",
				"Bone Mask",
				"Fade",
				"Night",
				"Blue Steel",
				"Stained",
				"Case Hardened",
				"Slaughter",
				"Safari Mesh",
				"Boreal Forest",
				"Ultraviolet",
				"Urban Masked",
				"Scorched",
				"Rust Coat",
				"Tiger Tooth",
				"Damascus Steel",
				"Damascus Steel",
				"Marble Fade",
				"Rust Coat",
				"Doppler Ruby",
				"Doppler Sapphire",
				"Doppler Blackpearl",
				"Doppler Phase 1",
				"Doppler Phase 2",
				"Doppler Phase 3",
				"Doppler Phase 4",
				"Gamma Doppler Phase 1",
				"Gamma Doppler Phase 2",
				"Gamma Doppler Phase 3",
				"Gamma Doppler Phase 4",
				"Gamma Doppler Emerald",
				"Lore",
				"Black Laminate",
				"Autotronic",
				"Freehand"
			};
			if (ImGui::Checkbox("Skinchanger", &SETTINGS::settings.skinenabled)) KnifeApplyCallbk();
			ImGui::Combo(("Knife Model"), &SETTINGS::settings.Knife, knifemodels, ARRAYSIZE(knifemodels));
			ImGui::Combo(("Knife Skin"), &SETTINGS::settings.KnifeSkin, knifeskins, ARRAYSIZE(knifeskins));

			const char* gloves[] =
			{
				"Off",
				"Bloodhound - Bronzed",
				"Bloodhound - Charred",
				"Bloodhound - Snakebite",
				"Bloodhound - Guerrilla",
				"Sporty - Pandora's Box",
				"Sporty - Arid",
				"Sporty - Supercondutor",
				"Sporty - Hedge Maze",
				"Driver - Crimson Weave",
				"Driver - Lunar Weave",
				"Driver - Diamondback",
				"Driver - Convoy",
				"Wraps - Leather",
				"Wraps - Badlands",
				"Wraps - Spruce DDPAT",
				"Wraps - Slaughter",
				"Motorcycle - Spearmint",
				"Motorcycle - Cool Mint",
				"Motorcycle - BOOM!",
				"Motorcycle - Eclipse",
				"Specialist - Emerald Web",
				"Specialist - Foundation",
				"Specialist - Forest DDPAT",
				"Specialist - Crimson Kimono",
				"Hydra - Case Hardened",
				"Hydra - Emerald",
				"Hydra - Rattler",
				"Hydra - Mangrove",
				"Specialist - Fade",
				"Specialist - Crimson Web",
				"Specialist - Mogul",
				"Specialist - Buckshot",
				"Sport - Omega",
				"Sport - Vice",
				"Sport - Amphibious",
				"Sport - Bronze Morph",
				"Wraps - Cobalt Skulls",
				"Wraps - Overprint",
				"Wraps - Duct Tape",
				"Wraps - Arboreal",
				"Motorcycle - POW!",
				"Motorcycle - Turtle",
				"Motorcycle - Transport",
				"Motorcycle - Polygon",
				"Driver - King Snake",
				"Driver - Imperial Plaid",
				"Driver - Overtake",
				"Driver - Racing Green"
			};
			const char* GloveModel[] =
			{
				"None",
				"Bloodhound",
				"Sporty",
				"Slick",
				"Handwrap leathery",
				"Motorcycle",
				"specialist",
				"Bloodhound hydra"
			};
			ImGui::Checkbox("glovechanger", &SETTINGS::settings.glovesenabled);
			ImGui::Combo(("glove model"), &SETTINGS::settings.gloves, GloveModel, ARRAYSIZE(GloveModel));
			if (SETTINGS::settings.gloves == 1)
			{

				const char* glovesskins[] =
				{
					"charred",
					"snakebite",
					"bronzed",
					"guerilla"
				};

				ImGui::Combo(("glove skin"), &SETTINGS::settings.skingloves, glovesskins, ARRAYSIZE(glovesskins));
			}
			else
				if (SETTINGS::settings.gloves == 2)
				{
					const char* glovesskins[] =
					{
						"hedge maze",
						"andoras box",
						"superconductor",
						"arid",
						"omega",
						"amphibious",
						"bronze morph"
					};

					ImGui::Combo(("glove skin"), &SETTINGS::settings.skingloves, glovesskins, ARRAYSIZE(glovesskins));
				}
				else
					if (SETTINGS::settings.gloves == 3)
					{
						const char* glovesskins[] =
						{
							"lunar weave",
							"convoy",
							"crimson weave",
							"diamondback",
							"overtake",
							"racing green",
							"king snake",
							"imperial plaid"
						};

						ImGui::Combo(("glove skin"), &SETTINGS::settings.skingloves, glovesskins, ARRAYSIZE(glovesskins));

					}
					else
						if (SETTINGS::settings.gloves == 4)
						{
							const char* glovesskins[] =
							{
								"leather",
								"spruce ddpat",
								"slaughter",
								"cobalt skulls",
								"overprint",
								"duct tape",
								"arboreal"
							};

							ImGui::Combo(("glove skin"), &SETTINGS::settings.skingloves, glovesskins, ARRAYSIZE(glovesskins));

						}
						else
							if (SETTINGS::settings.gloves == 5)
							{
								const char* glovesskins[] =
								{
									"eclipse",
									"spearmint",
									"boom",
									"cool mint",
									"turtle",
									"transport",
									"polygon",
									"pow"
								};

								ImGui::Combo(("glove skin"), &SETTINGS::settings.skingloves, glovesskins, ARRAYSIZE(glovesskins));

							}
							else
								if (SETTINGS::settings.gloves == 6)
								{
									const char* glovesskins[] =
									{
										"forest ddpat",
										"crimson kimono",
										"emerald web",
										"foundation",
										"crimson web",
										"buckshot",
										"fade",
										"mogul"
									};

									ImGui::Combo(("glove skin"), &SETTINGS::settings.skingloves, glovesskins, ARRAYSIZE(glovesskins));

								}
								else
									if (SETTINGS::settings.gloves == 7)
									{
										const char* glovesskins[] =
										{
											"emerald",
											"mangrove",
											"rattler",
											"case hardened"
										};

										ImGui::Combo(("glove skin"), &SETTINGS::settings.skingloves, glovesskins, ARRAYSIZE(glovesskins));

									}
									else
										if (SETTINGS::settings.gloves == 0)
										{
											const char* glovesskins[] = { "" };

											ImGui::Combo(("glove skin"), &SETTINGS::settings.skingloves, glovesskins, ARRAYSIZE(glovesskins));

										}

			
			if (ImGui::Button("Force Update", ImVec2(-1, 30)))
			{
				KnifeApplyCallbk();

			}*/
              if (ImGui::Checkbox("skinchanger", &SETTINGS::settings.skinenabled)) KnifeApplyCallbk();

               ImGui::Checkbox("glovechanger", &SETTINGS::settings.glovesenabled);

			   if (ImGui::Button("Force Update", ImVec2(-1, 30)))
			   {
	              KnifeApplyCallbk();
               }

			   static bool Main = true;
			   static bool Colors = false;

			   static int Page = 0;


			   const char* meme = "page : error";
			   switch (Page)
			   {
			   case 0: meme = "page : 1";  break;
			   case 1: meme = "page : 2";  break;
			   default: break;
			   }

			   ImGui::Text(meme); ImGui::SameLine(); ImGui::Text(("                  ")); ImGui::SameLine();
			   if (ImGui::Button(("-"), ImVec2(22, 22)))
			   {
				   if (Page != 0)
					   Page--;
			   };
			   ImGui::SameLine();
			   if (ImGui::Button(("+"), ImVec2(22, 22)))
			   {
				   if (Page != 3)
					   Page++;
			   };

			   ImGui::Text(("        "));
			   const char* KnifeModel[] =
			   {
				   "Bayonet",
				   "Flip Knife",
				   "Gut Knife",
				   "Karambit",
				   "M9 Bayonet",
				   "Huntsman Knife",
				   "Butterfly Knife",
				   "Falchion Knife",
				   "Shadow Daggers",
				   "Bowie Knife",
				   "Navaja Knife",
				   "Stiletto Knife",
				   "Ursus Knife",
				   "Talon Knife"
			   };
			   const char* knifeskins[] =
			   {
				   "None",
				   "Crimson Web",
				   "Bone Mask",
				   "Fade",
				   "Night",
				   "Blue Steel",
				   "Stained",
				   "Case Hardened",
				   "Slaughter",
				   "Safari Mesh",
				   "Boreal Forest",
				   "Ultraviolet",
				   "Urban Masked",
				   "Scorched",
				   "Rust Coat",
				   "Tiger Tooth",
				   "Damascus Steel",
				   "Damascus Steel",
				   "Marble Fade",
				   "Rust Coat",
				   "Doppler Ruby",
				   "Doppler Sapphire",
				   "Doppler Blackpearl",
				   "Doppler Phase 1",
				   "Doppler Phase 2",
				   "Doppler Phase 3",
				   "Doppler Phase 4",
				   "Gamma Doppler Phase 1",
				   "Gamma Doppler Phase 2",
				   "Gamma Doppler Phase 3",
				   "Gamma Doppler Phase 4",
				   "Gamma Doppler Emerald",
				   "Lore",
				   "Black Laminate",
				   "Autotronic",
				   "Freehand"
			   };
			   switch (Page)
			   {
			   case 0:
			   {
				   const char* GloveModel[] =
				   {
					   "None",
					   "Bloodhound",
					   "Sporty",
					   "Slick",
					   "Handwrap leathery",
					   "Motorcycle",
					   "specialist",
					   "Bloodhound hydra"
				   };
				   ImGui::PushItemWidth(150.0f);
				   ImGui::Combo(("Knife Model"), &SETTINGS::settings.Knife, KnifeModel, ARRAYSIZE(KnifeModel));
				   ImGui::PushItemWidth(150.0f);
				   ImGui::Combo(("Knife Skin"), &SETTINGS::settings.KnifeSkin, knifeskins, ARRAYSIZE(knifeskins));
				   ImGui::PushItemWidth(150.0f);
				   ImGui::Combo(("glove model"), &SETTINGS::settings.gloves, GloveModel, ARRAYSIZE(GloveModel));
				   ImGui::PushItemWidth(150.0f);
				   if (SETTINGS::settings.gloves == 1)
				   {

					   const char* glovesskins[] =
					   {
						   "charred",
						   "snakebite",
						   "bronzed",
						   "guerilla"
					   };

					   ImGui::Combo(("glove skin"), &SETTINGS::settings.skingloves, glovesskins, ARRAYSIZE(glovesskins));
				   }
				   else
					   if (SETTINGS::settings.gloves == 2)
					   {
						   const char* glovesskins[] =
						   {
							   "hedge maze",
							   "andoras box",
							   "superconductor",
							   "arid",
							   "omega",
							   "amphibious",
							   "bronze morph"
						   };

						   ImGui::Combo(("glove skin"), &SETTINGS::settings.skingloves, glovesskins, ARRAYSIZE(glovesskins));
					   }
					   else
						   if (SETTINGS::settings.gloves == 3)
						   {
							   const char* glovesskins[] =
							   {
								   "lunar weave",
								   "convoy",
								   "crimson weave",
								   "diamondback",
								   "overtake",
								   "racing green",
								   "king snake",
								   "imperial plaid"
							   };

							   ImGui::Combo(("glove skin"), &SETTINGS::settings.skingloves, glovesskins, ARRAYSIZE(glovesskins));

						   }
						   else
							   if (SETTINGS::settings.gloves == 4)
							   {
								   const char* glovesskins[] =
								   {
									   "leather",
									   "spruce ddpat",
									   "slaughter",
									   "cobalt skulls",
									   "overprint",
									   "duct tape",
									   "arboreal"
								   };

								   ImGui::Combo(("glove skin"), &SETTINGS::settings.skingloves, glovesskins, ARRAYSIZE(glovesskins));

							   }
							   else
								   if (SETTINGS::settings.gloves == 5)
								   {
									   const char* glovesskins[] =
									   {
										   "eclipse",
										   "spearmint",
										   "boom",
										   "cool mint",
										   "turtle",
										   "transport",
										   "polygon",
										   "pow"
									   };

									   ImGui::Combo(("glove skin"), &SETTINGS::settings.skingloves, glovesskins, ARRAYSIZE(glovesskins));

								   }
								   else
									   if (SETTINGS::settings.gloves == 6)
									   {
										   const char* glovesskins[] =
										   {
											   "forest ddpat",
											   "crimson kimono",
											   "emerald web",
											   "foundation",
											   "crimson web",
											   "buckshot",
											   "fade",
											   "mogul"
										   };

										   ImGui::Combo(("glove skin"), &SETTINGS::settings.skingloves, glovesskins, ARRAYSIZE(glovesskins));

									   }
									   else
										   if (SETTINGS::settings.gloves == 7)
										   {
											   const char* glovesskins[] =
											   {
												   "emerald",
												   "mangrove",
												   "rattler",
												   "case hardened"
											   };

											   ImGui::Combo(("glove skin"), &SETTINGS::settings.skingloves, glovesskins, ARRAYSIZE(glovesskins));

										   }
										   else
											   if (SETTINGS::settings.gloves == 0)
											   {
												   const char* glovesskins[] = { "" };

												   ImGui::Combo(("glove skin"), &SETTINGS::settings.skingloves, glovesskins, ARRAYSIZE(glovesskins));

											   }




			   }break;
			   case 1:
			   {



			   }break;
			
			   default:break;
			   }


		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("inventorys", ImVec2(225, 495), true);
		{
			ImGui::Checkbox(XorStr("Inventchanger"), &SETTINGS::settings.Inventarchanger.enable);
			static int itemDefinitionIndex = 0;
			static int paintKit = 0;
			static int rarity = 0;
			static int seed = 0;
			static float wear = 0.f;

			static int gmodel = 0;
			static int gskin = 0.f;

			static int kmodel = 0.f;

			static int type = 0.f;

			const char* KnifeModel[]{ "Bayonet",
				"Flip Knife",
				"Gut Knife",
				"Karambit",
				"M9 Bayonet",
				"Huntsman Knife",
				"Butterfly Knife",
				"Falchion Knife",
				"Shadow Daggers",
				"Bowie Knife",
				"Navaja Knife",
				"Stiletto Knife",
				"Ursus Knife",
				"Talon Knife" };

			static const char* types[] = {
				"Weapon",
				"Glove",
				"Knife"
			};

			static const char* weapons[] = {
				"None",
				"Desert Eagle",
				"Dual Berettas",
				"Five SeveN",
				"Glock",
				"",
				"",
				"AK47",
				"AUG",
				"AWP",
				"AMAS",
				"G3SG1",
				"",
				"Galil AR",
				"M249",
				"",
				"M4A4",
				"MAC10",
				"",
				"P90",
				"",
				"",
				"",
				"",
				"UMP45",
				"XM1014",
				"PPBizon",
				"MAG7",
				"Negev",
				"SawedOff",
				"Tec9",
				" ",
				"P2000",
				"MP7",
				"MP9",
				"Nova",
				"P250",
				"SCAR20",
				"SG553",
				"SCOUT",
			};

			ImGui::PushItemWidth(130);
			ImGui::Combo(XorStr("Item Type"), &type, types, ARRAYSIZE(types));


			if (type == 0)
			{

				ImGui::PushItemWidth(130);	ImGui::Combo(XorStr("Weapon"), &itemDefinitionIndex, weapons, ARRAYSIZE(weapons));

			}
			//			ImGui::InputInt("Weapon ID", &itemDefinitionIndex);

			if (type == 2)
			{
				/*const char* knifemodels[]{ "Bayonet",
					"Flip Knife",
					"Gut Knife",
					"Karambit",
					"M9 Bayonet",
					"Huntsman Knife",
					"Butterfly Knife",
					"Falchion Knife",
					"Shadow Daggers",
					"Bowie Knife",
					"Navaja Knife",
					"Stiletto Knife",
					"Ursus Knife",
					"Talon Knife" };*/

				ImGui::PushItemWidth(130);	ImGui::Combo(XorStr("Model##knife"), &kmodel, knifemodels, ARRAYSIZE(knifemodels));

			}


			if (type == 2)
			{
				if (kmodel == 0)
					itemDefinitionIndex = 500;
				if (kmodel == 9)
					itemDefinitionIndex = 514;
				if (kmodel == 6)
					itemDefinitionIndex = 515;
				if (kmodel == 7)
					itemDefinitionIndex = 512;
				if (kmodel == 1)
					itemDefinitionIndex = 505;
				if (kmodel == 2)
					itemDefinitionIndex = 506;
				if (kmodel == 5)
					itemDefinitionIndex = 509;
				if (kmodel == 3)
					itemDefinitionIndex = 507;
				if (kmodel == 4)
					itemDefinitionIndex = 508;
				if (kmodel == 8)
					itemDefinitionIndex = 516;
				if (kmodel == 10)
					itemDefinitionIndex = 520;
				if (kmodel == 11)
					itemDefinitionIndex = 522;
				if (kmodel == 12)
					itemDefinitionIndex = 519;
				if (kmodel == 13)
					itemDefinitionIndex = 523;
			}

			if (type == 1)
			{

				ImGui::PushItemWidth(130);	ImGui::Combo(XorStr("Model##glove"), &gmodel, "Bloodhound\0Sport\0Driver\0Wraps\0Moto\0Specialist\0Hyndra\0\0", ARRAYSIZE("Bloodhound\0Sport\0Driver\0Wraps\0Moto\0Specialist\0Hyndra\0\0"));


				const char* gstr;
				if (gmodel == 0)
				{
					gstr = "Charred\0\rSnakebite\0\rBronzed\0\rGuerilla\0\0";
				}
				else if (gmodel == 1)
				{
					gstr = "Hedge Maze\0\rPandoras Box\0\rSuperconductor\0\rArid\0\rAmphibious\0\rBronze Morph\0\rOmega\0\rVice\0\0";
				}
				else if (gmodel == 2)
				{
					gstr = "Lunar Weave\0\rConvoy\0\rCrimson Weave\0\rDiamondback\0\King Snake\0\Imperial Plaid\0\Overtake\0\Racing Green\0\0";
				}
				else if (gmodel == 3)
				{
					gstr = "Leather\0\rSpruce DDPAT\0\rSlaughter\0\rBadlands\0\rCobalt Skulls\0\rOverprint\0\rDuct Tape\0\rArboreal\0\0";
				}
				else if (gmodel == 4)
				{
					gstr = "Eclipse\0\rSpearmint\0\rBoom!\0\rCool Mint\0\rPOW!\0\rTurtle\0\rTransport\0\rPolygon\0\0";
				}
				else if (gmodel == 5)
				{
					gstr = "Forest DDPAT\0\rCrimson Kimono\0\rEmerald Web\0\rFoundation\0\rCrimson Web\0\rBuckshot\0\rFade\0\rMogul\0\0";
				}
				else if (gmodel == 6)
				{
					gstr = "Emerald\0\rMangrove\0\rRattler\0\rCase Hardened\0\0";
				}
				else
					gstr = "";


				ImGui::PushItemWidth(130);	ImGui::Combo(XorStr("Skin##glove"), &gskin, gstr, -1);

			}

			if (type == 1)
			{
				if (gmodel == 1)
				{
					itemDefinitionIndex = 5027;
				}
				else if (gmodel == 2)
				{
					itemDefinitionIndex = 5030;
				}
				else if (gmodel == 3)
				{
					itemDefinitionIndex = 5031;
				}
				else if (gmodel == 4)
				{
					itemDefinitionIndex = 5032;
				}
				else if (gmodel == 5)
				{
					itemDefinitionIndex = 5033;
				}
				else if (gmodel == 6)
				{
					itemDefinitionIndex = 5034;
				}
				else if (gmodel == 7)
				{
					itemDefinitionIndex = 5035;
				}

				if (gmodel == 1)
				{
					switch (gskin)
					{
					case 0:
						paintKit = 10006;
						break;
					case 1:
						paintKit = 10007;
						break;
					case 2:
						paintKit = 10008;
						break;
					case 3:
						paintKit = 10039;
						break;
					}
				}
				else if (gmodel == 2)
				{
					switch (gskin)
					{
					case 0:
						paintKit = 10018;
						break;
					case 1:
						paintKit = 10037;
						break;
					case 2:
						paintKit = 10038;
						break;
					case 3:
						paintKit = 10019;
						break;

					case 4:
						paintKit = 10045;
						break;
					case 5:
						paintKit = 10046;
						break;
					case 6:
						paintKit = 10047;
						break;
					case 7:
						paintKit = 10048;
						break;
					}
				}
				else if (gmodel == 3)
				{
					switch (gskin)
					{
					case 0:
						paintKit = 10013;
						break;
					case 1:
						paintKit = 10015;
						break;
					case 2:
						paintKit = 10016;
						break;
					case 3:
						paintKit = 10040;
						break;

					case 4:
						paintKit = 10041;
						break;
					case 5:
						paintKit = 10042;
						break;
					case 6:
						paintKit = 10043;
						break;
					case 7:
						paintKit = 10044;
						break;
					}
				}
				else if (gmodel == 4)
				{
					switch (gskin)
					{
					case 0:
						paintKit = 10009;
						break;
					case 1:
						paintKit = 10010;
						break;
					case 2:
						paintKit = 10021;
						break;
					case 3:
						paintKit = 10036;
						break;
					case 4:
						paintKit = 10053;
						break;
					case 5:
						paintKit = 10054;
						break;
					case 6:
						paintKit = 10055;
						break;
					case 7:
						paintKit = 10056;
						break;
					}
				}
				else if (gmodel == 5)
				{
					switch (gskin)
					{
					case 0:
						paintKit = 10024;
						break;
					case 1:
						paintKit = 10026;
						break;
					case 2:
						paintKit = 10027;
						break;
					case 3:
						paintKit = 10028;
						break;
					case 4:
						paintKit = 10049;
						break;
					case 5:
						paintKit = 10050;
						break;
					case 6:
						paintKit = 10051;
						break;
					case 7:
						paintKit = 10052;
						break;
					}
				}
				else if (gmodel == 6)
				{
					switch (gskin)
					{
					case 0:
						paintKit = 10030;
						break;
					case 1:
						paintKit = 10033;
						break;
					case 2:
						paintKit = 10034;
						break;
					case 3:
						paintKit = 10035;
						break;
					case 4:
						paintKit = 10061;
						break;
					case 5:
						paintKit = 10062;
						break;
					case 6:
						paintKit = 10063;
						break;
					case 7:
						paintKit = 10064;
						break;
					}
				}
				else if (gmodel == 7)
				{
					switch (gskin)
					{
					case 0:
						paintKit = 10057;
						break;
					case 1:
						paintKit = 10058;
						break;
					case 2:
						paintKit = 10059;
						break;
					case 3:
						paintKit = 10060;
						break;
					}
				}
			}




			if (type == 0 || type == 2)
			{

				std::string skinName1 = GetWeaponNameById(itemDefinitionIndex);
				std::string skinStr1 = "";
				int curItem = -1;
				int s = 0;
				for (auto skin : GLOBAL::weaponSkins[skinName1])
				{
					int pk = GLOBAL::skinMap[skin].paintkit;
					if (pk == paintKit)
						curItem = s;

					skinStr1 += GLOBAL::skinNames[GLOBAL::skinMap[skin].tagName].c_str();
					skinStr1.push_back('\0');
					s++;
				}

				skinStr1.push_back('\0');


				ImGui::PushItemWidth(130); if (ImGui::Combo(XorStr("Skin"), &curItem, skinStr1.c_str()))
				{
					int pk1 = 0;
					int c1 = 0;
					for (auto skin : GLOBAL::weaponSkins[skinName1])
					{
						if (curItem == c1)
						{
							pk1 = GLOBAL::skinMap[skin].paintkit;
							break;
						}

						c1++;
					}
					paintKit = pk1;
				}
			}
			{

				if (ImGui::Button(XorStr("Add"))) {
					SETTINGS::settings.Inventarchanger.g_skins.insert(SETTINGS::settings.Inventarchanger.g_skins.end(), { itemDefinitionIndex , paintKit });
				} ImGui::SameLine();
				if (ImGui::Button(XorStr("Apply##Skin"))) {

					write.SendClientHello();
					write.SendMatchmakingClient2GCHello();

				}

			}
			int weapon_index = 0;
			for (auto weapon : SETTINGS::settings.Inventarchanger.g_skins) {

				std::string weap = GetWeaponNameById(weapon.wId);


				if (ImGui::Selectable(std::string("Weap: " + weap + " | Skin Id: " + std::to_string(weapon.paintkit)).c_str())) {
					SETTINGS::settings.Inventarchanger.g_skins.erase(SETTINGS::settings.Inventarchanger.g_skins.begin() + weapon_index);
				}
				weapon_index++;
			}



		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("rankchanger", ImVec2(210, 285), true);
		{
			ImGui::Checkbox("Rackchanger", &SETTINGS::settings.RankChanger.enabled);
			const char* xdxdranks[] = {
				"off",
				"silver 1",
				"silver 2",
				"silver 3",
				"silver 4",
				"silver elite",
				"silver elite master",
				"gold nova 1",
				"gold nova 2",
				"gold nova 3",
				"gold nova master",
				"master guardian 1",
				"master guardian 2",
				"master guardian elite",
				"distinguished master guardian",
				"legendary eagle",
				"legendary eagle master",
				"supreme master first class",
				"the global elite"
			};
			ImGui::Combo(("Rank"), &SETTINGS::settings.RankChanger.rank_id, xdxdranks, ARRAYSIZE(xdxdranks));
			ImGui::PushItemWidth(150.0f);
			ImGui::SliderInt("Level", &SETTINGS::settings.RankChanger.player_level, 0, 40);
			ImGui::InputInt("Xp##level", &SETTINGS::settings.RankChanger.player_level_xp);
			ImGui::PushItemWidth(150.0f);
			ImGui::InputInt("Wins", &SETTINGS::settings.RankChanger.wins);
			ImGui::PushItemWidth(150.0f);
			ImGui::InputInt("Friend", &SETTINGS::settings.RankChanger.cmd_friendly);
			ImGui::PushItemWidth(150.0f);
			ImGui::InputInt("Teach", &SETTINGS::settings.RankChanger.cmd_teaching);
			ImGui::PushItemWidth(150.0f);
			ImGui::InputInt("Leader", &SETTINGS::settings.RankChanger.cmd_leader);
			static const char* bans[] =
			{
				"none",
				"you were kicked from the last match (competitive cooldown)",
				"you killed too many teammates (competitive cooldown)",
				"you killed a teammate at round start (competitive cooldown)",
				"you failed to reconnect to the last match (competitive cooldown)",
				"you abandoned the last match (competitive cooldown)",
				"you did too much damage to your teammates (competitive cooldown)",
				"you did too much damage to your teammates at round start (competitive cooldown)",
				"this account is permanently untrusted (global cooldown)",
				"you were kicked from too many recent matches (competitive cooldown)",
				"convicted by overwatch - majorly disruptive (global cooldown)",
				"convicted by overwatch - minorly disruptive (global cooldown)",
				"resolving matchmaking state for your account (temporary cooldown)",
				"resolving matchmaking state after the last match (temporary cooldown)",
				"this account is permanently untrusted (global cooldown)",
				"(global cooldown)",
				"you failed to connect by match start. (competitive cooldown)",
				"you have kicked too many teammates in recent matches (competitive cooldown)",
				"congratulations on your recent competitive wins! before you play competitive matches further please wait for matchmaking servers to calibrate your skill group placement based on your lastest performance. (temporary cooldown)",
				"a server using your game server login token has been banned. your account is now permanently banned from operating game servers, and you have a cooldown from connecting to game servers. (global cooldown)"
			};
			ImGui::Combo("Ban", &SETTINGS::settings.RankChanger.ban, bans, IM_ARRAYSIZE(bans));
			ImGui::InputInt("Time##ban", &SETTINGS::settings.RankChanger.time);
			if (ImGui::Button(("Update##egfa")))
			{
				write.SendClientHello();
				write.SendMatchmakingClient2GCHello();

			}
			
		}
		ImGui::EndChild();
		
		
	}


	long __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice)
	{
		static auto ofunc = directz.get_original<EndSceneFn>(42);

		D3DCOLOR rectColor = D3DCOLOR_XRGB(255, 0, 0);
		D3DRECT BarRect = { 1, 1, 1, 1 };
		ImGuiStyle& style = ImGui::GetStyle();
		pDevice->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, rectColor, 0, 0);

		if (!d3d_init)
			GUI_Init(pDevice);


		static const D3DRENDERSTATETYPE backupStates[] = { D3DRS_COLORWRITEENABLE, D3DRS_ALPHABLENDENABLE, D3DRS_SRCBLEND, D3DRS_DESTBLEND, D3DRS_BLENDOP, D3DRS_FOGENABLE };
		static const int size = sizeof(backupStates) / sizeof(DWORD);

		DWORD oldStates[size] = { 0 };

		for (int i = 0; i < size; i++)
			pDevice->GetRenderState(backupStates[i], &oldStates[i]);

		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);

		ImGui_ImplDX9_NewFrame();

		if (SETTINGS::settings.opencolor)
			DrawColorsTab();

		if (SETTINGS::settings.opencfg)
			ConfigTab();

		static int tab;

		if (menu_open)
		{


			ImVec2 mainWindowPos;
			ImGui::PushFont(Tabs);
			ImGui::SetNextWindowSize(ImVec2(680, 580));
			ImGui::Begin("DreamHack", &menu_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
			{
				//ImGui::TextColored(ImColor(93, 89, 242, 255), "DreamHack");
				static char buttons[][16] = { "rage", "visual", "misc", "skins"};

				static int iTab = 0;
				for (int i = 0; i < 4; i++)
				{
					if (i > 0) ImGui::SameLine(0.0f, -0.3f);

					//ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00f, 1.00f, 1.00f, 1.00f));

					if (ImGui::Button(buttons[i], ImVec2(160, 30)))
						iTab = i;
				}
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();

				switch (iTab)
				{

				case 0: DrawRageTab(); break;
				case 1: DrawVisualsTab(); break;
				case 2: DrawMiscTab(); break;
				case 3: DrawSkinTab(); break;
				}

				ImGui::End();
			}
			ImGui::PopFont();

		}
		ImGui::Render();


		

		//SpectatorList();




		for (int i = 0; i < size; i++)
			pDevice->SetRenderState(backupStates[i], oldStates[i]);

		return ofunc(pDevice);
	}

	long __stdcall Hooked_EndScene_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		static auto ofunc = directz.get_original<EndSceneResetFn>(16);

		if (!d3d_init)
			return ofunc(pDevice, pPresentationParameters);

		ImGui_ImplDX9_InvalidateDeviceObjects();

		auto hr = ofunc(pDevice, pPresentationParameters);

		ImGui_ImplDX9_CreateDeviceObjects();

		return hr;
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

		auto sv_cheats = INTERFACES::cvar->FindVar("sv_cheats");
		get_bool_manager = VMT::VMTHookManager(reinterpret_cast<DWORD**>(sv_cheats));
		original_get_bool = reinterpret_cast<SvCheatsGetBoolFn>(get_bool_manager.HookFunction<SvCheatsGetBoolFn>(13, HookedGetBool));
		SurfaceTable.InitTable(INTERFACES::Surface);
		SurfaceTable.HookIndex(67, Hook_LockCursor);
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
		gc_hook.setup(INTERFACES::g_SteamGameCoordinator);
		gc_hook.hook_index(vtable_indexes::send_message, hkGCSendMessage);
		gc_hook.hook_index(vtable_indexes::retrieve_message, hkGCRetrieveMessage);
		ParseSkins();

	}
	void EyeAnglesPitchHook(const SDK::CRecvProxyData *pData, void *pStruct, void *pOut)
	{
		*reinterpret_cast<float*>(pOut) = pData->m_Value.m_Float;

		auto entity = reinterpret_cast<SDK::CBaseEntity*>(pStruct);
		if (!entity)
			return;

	}
	void EyeAnglesYawHook(const SDK::CRecvProxyData *pData, void *pStruct, void *pOut)
	{
		*reinterpret_cast<float*>(pOut) = pData->m_Value.m_Float;

		auto entity = reinterpret_cast<SDK::CBaseEntity*>(pStruct);
		if (!entity)
			return;
	}

	void InitNetvarHooks()
	{
		UTILS::netvar_hook_manager.Hook("DT_CSPlayer", "m_angEyeAngles[0]", EyeAnglesPitchHook);
		UTILS::netvar_hook_manager.Hook("DT_CSPlayer", "m_angEyeAngles[1]", EyeAnglesYawHook);
	}
}