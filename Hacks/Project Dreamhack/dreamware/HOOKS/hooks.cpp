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
#include "../SDK/CInputSystem.h"


#include "../FEATURES/rankchanger/Protobuffs.h"
#include "../FEATURES/rankchanger/IMemAlloc.h"
#include "../FEATURES/rankchanger/steam.h"
#include "../FEATURES/rankchanger/rankchanger.h"
#include "../FEATURES/sound_esp.h"

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

static char ConfigNamexd[70] = { 0 };
namespace ImGui
{

	static auto vector_getterxd = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	IMGUI_API bool ComboBoxArrayxd(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getterxd,
			static_cast<void*>(&values), values.size());
	}
}
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
		

		auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
		
		auto flServerTime = (float)local_player->GetTickBase() * INTERFACES::Globals->interval_per_tick;
		auto flNextPrimaryAttack = weapon->NextPrimaryAttack();

		return(!(flNextPrimaryAttack > flServerTime));
	}

	void auto_revolver(SDK::CUserCmd* cmd)
	{

		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player || local_player->GetHealth() <= 0)
			return;

		auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
		if (!weapon || weapon->GetItemDefenitionIndex() != SDK::WEAPON_REVOLVER)
			return;

		static float delay = 0.f;
		if (delay < 0.15f)
		{
			delay += INTERFACES::Globals->interval_per_tick;
			cmd->buttons |= IN_ATTACK;
		}
		else
			delay = 0.f;
		
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

	typedef void(__fastcall* ClanTagFn)(const char*, const char*);
	ClanTagFn dw_ClanTag;
	void SetClanTag(const char* tag, const char* name)
	{

		if (!dw_ClanTag)
			dw_ClanTag = reinterpret_cast<ClanTagFn>(((DWORD)UTILS::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30", "xxxxxxxxx????xxxxxx")));

		if (dw_ClanTag)
			dw_ClanTag(tag, name);
	}
	void Clan_Tag()
	{
		if (!SETTINGS::settings.clantags == 1)
			return;
		static int iLastTime;

		float latency = INTERFACES::Engine->GetNetChannelInfo()->GetLatency(FLOW_INCOMING) + INTERFACES::Engine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);
		static int timer = 0;
		if (int(INTERFACES::Globals->curtime * 2 + latency) != iLastTime)
		{
			//but case clantag is broken tho (this not, i tested)
			switch (timer)
			{
			case 0: SetClanTag("D", "D"); break;
			case 1: SetClanTag("Dr", "Dre"); break;
			case 2: SetClanTag("Dre", "dea"); break;
			case 3: SetClanTag("Drea", "Drea"); break;
			case 4: SetClanTag("Dream", "Dream"); break;
			case 5: SetClanTag("DreamH", "DreamH"); break;
			case 6: SetClanTag("DreamHa", "DreamHa"); break;
			case 7: SetClanTag("DreamHac", "DreamHac"); break;
			case 8: SetClanTag("DreamHack", "DreamHack"); break;
			case 9: SetClanTag("DreamHack", "DreamHack"); break;
			case 10: SetClanTag("DreamHack", "DreamHack"); break;
			case 11: SetClanTag("DreamHack", "DreamHack"); break;
			case 12: SetClanTag("DreamHack", "DreamHack"); break;
			case 13: SetClanTag("DreamHack", "DreamHack"); break;
			case 14: SetClanTag("DreamHack", "DreamHack"); break;


			}
			timer++;
			iLastTime = int(INTERFACES::Globals->curtime * 2 + latency);
		}
		if (timer > 15)
			timer = 0;

	}
	void set_name(const char* name)
	{
		SDK::ConVar* nameConvar = INTERFACES::cvar->FindVar(("name"));
		*(int*)((DWORD)&nameConvar->fnChangeCallback + 0xC) = NULL;
		nameConvar->SetValueChar(name);
	}
	void namespam()
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player)
			return;

		static clock_t start_t = clock();
		double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
		if (timeSoFar < .5)
			return;
		const char* result;
		std::vector <std::string> names;
		if (INTERFACES::Engine->IsInGame() && INTERFACES::Engine->IsConnected()) {
			for (int i = 1; i < INTERFACES::Globals->maxclients; i++)
			{
				SDK::CBaseEntity *entity = INTERFACES::ClientEntityList->GetClientEntity(i);

				SDK::player_info_t pInfo;

				if (entity && local_player->GetTeam() == entity->GetTeam())
				{
					SDK::ClientClass* cClass = (SDK::ClientClass*)entity->GetClientClass();

					if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
					{
						if (INTERFACES::Engine->GetPlayerInfo(i, &pInfo))
						{
							if (!strstr(pInfo.name, "GOTV"))
								names.push_back(pInfo.name);
						}
					}
				}
			}
		}

		set_name("\n\xAD\xAD\xAD");
		int randomIndex = rand() % names.size();
		char buffer[128];
		sprintf_s(buffer, "%s ", names[randomIndex].c_str());
		result = buffer;



		set_name(result);
		start_t = clock();

	}

	void setviewmovelscallback()
	{
		INTERFACES::cvar->get_convar("viewmodel_offset_x")->callbacks.SetSize(0);
		INTERFACES::cvar->get_convar("viewmodel_offset_y")->callbacks.SetSize(0);
		INTERFACES::cvar->get_convar("viewmodel_offset_z")->callbacks.SetSize(0);
	}


	char* const chatspam[] =
	{
		"DreamHack > ALL",
		"DreamHack dominates all",
		"Keep getting tapped? Get DreamHack nigga!",
		"Get Good Get DreamHack",
		"DreamHack > ALL",
		"DreamHack best free hvh cheat!!!",
		"DreamHack best free hvh cheat!!!",
		"DreamHack best free hvh cheat!!!",
		"DreamHack best free hvh cheat!!!",
		"DreamHack best free hvh cheat!!!",
		"DreamHack best free hvh cheat!!!",
		"DreamHack best free hvh cheat!!!",
		"DreamHack > ALL",
		"DreamHack dominates all",
		"Keep getting tapped? Get DreamHack nigga!",
		"Get Good Get DreamHack",

		
	};

	template<class T, class U>
	T fine(T in, U low, U high)
	{
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}

	void SayInChat(const char *text)
	{
		char buffer[250];
		sprintf_s(buffer, "say \"%s\"", text);
		INTERFACES::Engine->ClientCmd_Unrestricted(buffer);
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

			if (!INTERFACES::Engine->IsVoiceRecording())
				fakelag->do_fakelag(cmd);

			static SDK::ConVar* ragdoll = INTERFACES::cvar->FindVar("cl_ragdoll_gravity");
			if (SETTINGS::settings.ragdoll_bool)
			{
				ragdoll->SetValue(-900);
			}
			else {
				ragdoll->SetValue(500);
			}
			if (SETTINGS::settings.clantags == 1) {
				Clan_Tag();
			}

			static SDK::ConVar* impacts = INTERFACES::cvar->FindVar("sv_showimpacts");
			if (SETTINGS::settings.impacts)
			{
				impacts->SetValue(1);
			}
			else {
				impacts->SetValue(0);
			}
			if (SETTINGS::settings.autozeus_bool)
				aimbot->autozeus(cmd);

			if (SETTINGS::settings.autoknife_bool)
				aimbot->autoknife(cmd);

			if (SETTINGS::settings.bBlockBot && GetAsyncKeyState(SETTINGS::settings.iBlockBotKey))
			    Misc::AutoBlocker(cmd);

			auto_revolver(cmd);

			if (SETTINGS::settings.infinityduck) { cmd->buttons |= IN_BULLRUSH; }

			if (SETTINGS::settings.Slowwalkenable)
				SlowWalk(cmd);

			if (SETTINGS::settings.namespam)
				namespam();

			if (SETTINGS::settings.chatspam)
			{
				static size_t lastTime = 0;
				static int counter = 0;

				if (GetTickCount() > lastTime)
				{
					counter = counter + 1;
					if (counter > 5)
						counter = 0;

					SayInChat(chatspam[counter]);

					lastTime = GetTickCount() + 850;
				}
			}


			Misc::FixCmd(cmd);
			Misc::edgejump(cmd);
		    Misc::g_bhop(cmd);
			Misc::g_strafer(cmd);
			Misc::fake_crouch(cmd);

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
			if (SETTINGS::settings.removesky)
			{
				auto skybox = INTERFACES::cvar->FindVar("sv_skyname");
				if (skybox) skybox->SetValue("sky_l4d_rural02_ldr");
			}
			else {
				static SDK::ConVar* sv_skyname = INTERFACES::cvar->FindVar("sv_skyname");
				sv_skyname->nFlags &= ~FCVAR_CHEAT;
				sv_skyname->SetValue("vertigoblue_hdr");
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
			
			if (SETTINGS::settings.silentaim && SETTINGS::settings.aim_bool)
			{
				INTERFACES::Engine->SetViewAngles(cmd->viewangles);
			};

			prediction->run_prediction(cmd);
			{

				if (SETTINGS::settings.aim_bool)
				{
					if (SETTINGS::settings.fakelagpredicion)
					{
						if (local_player->IsAlive())
						{
							for (int i = 1; i < INTERFACES::Globals->maxclients; i++)
							{
								auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
								if (!entity || entity == local_player || entity->GetClientClass()->m_ClassID != !entity->IsAlive()) continue;



								float simtime_delta = entity->GetSimTime() - *reinterpret_cast<float*>(uintptr_t(entity) + OFFSETS::m_flSimulationTime) + 0x4;

								int choked_ticks = HOOKS::fine(TIME_TO_TICKS(simtime_delta), 1, 15);
								Vector lastOrig;

								if (lastOrig.Length() != entity->GetVecOrigin().Length())
									lastOrig = entity->GetVecOrigin();

								float delta_distance = (entity->GetVecOrigin() - lastOrig).LengthSqr();
								if (delta_distance > 4096.f)
								{
									Vector velocity_per_tick = entity->GetVelocity() * INTERFACES::Globals->interval_per_tick;
									auto new_origin = entity->GetVecOrigin() + (velocity_per_tick * choked_ticks);
									entity->SetAbsOrigin(new_origin);
								}
							}
						}
					}

					aimbot->run_aimbot(cmd);
				}
				
				int accuracyboost = SETTINGS::settings.Accuracyboost;

				switch (accuracyboost)
				{
				case 0:
					break;
				case 1: backtracking->backtrack_player(cmd);
					break;
				case 2: backtracking->run_legit(cmd);
					break;
				}
				if (SETTINGS::settings.extrapolation)
					aimbot->extrapolation();


				if (SETTINGS::settings.aa_bool)
				{
					antiaim->do_antiaim(cmd);
					antiaim->fix_movement(cmd);
				}
				SDK::CBaseAnimState* animstate = local_player->GetAnimState();

				if (!animstate)
					return;

				if (!GLOBAL::should_send_packet) {

					if (animstate->m_bInHitGroundAnimation && SETTINGS::settings.Changepitchonland) /// change pitch while we supress
						cmd->viewangles.x = -12.0f;

				}
			}
			prediction->end_prediction(cmd);

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
		if (panel_name == "HudZoom" && SETTINGS::settings.fixscopesens) return;
		if (panel_name == "MatSystemTopPanel")
		{
			visuals->watermark();
		}
		if (panel_name == "FocusOverlayPanel")
		{
			if (FONTS::ShouldReloadFonts())
				FONTS::InitFonts();
			visuals->DrawMisc();

			if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
			{
				GrenadePrediction::instance().Paint();
				g_soundesp.listener();

				auto matpostprocess = INTERFACES::cvar->FindVar("mat_postprocess_enable");
				matpostprocess->fnChangeCallback = 0;
				matpostprocess->SetValue(SETTINGS::settings.matpostprocessenable);

				 visuals->ApplyNightmode();


				if (SETTINGS::settings.esp_bool)
				{
					visuals->Draw();
					visuals->ClientDraw();
				}
				/*if (SETTINGS::settings.hitmarker)
				{
					pHitmarker->Paint();
				}*/

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
			if (SETTINGS::settings.night_bool == 2)
			{
				fullbright->SetValue(1.00);
			}
			else
				fullbright->SetValue(0.00);

			SDK::ConVar* type2 = INTERFACES::cvar->FindVar("cl_weapon_debug_show_accuracy");
			if (SETTINGS::settings.dreamhackmarkers)
			{
				type2->SetValue(1);
			}
			else
				type2->SetValue(0);

		}
		original_paint_traverse(INTERFACES::Panel, VGUIPanel, ForceRepaint, AllowForce);
	}
	void MessageSend()
	{
		write.SendClientHello();
		write.SendMatchmakingClient2GCHello();
	}

	void FixThemAnim(SDK::CBaseEntity* entity)
	{
		if (!entity)
			return;

		auto old_curtime = INTERFACES::Globals->curtime;
		auto old_frametime = INTERFACES::Globals->frametime;
		auto old_fraction = entity->GetAnimState()->m_flUnknownFraction = 0.f;

		INTERFACES::Globals->curtime = entity->GetSimTime();
		INTERFACES::Globals->frametime = INTERFACES::Globals->interval_per_tick;

		SDK::CAnimationLayer backup_layers[15];
		std::memcpy(backup_layers, entity->GetAnimOverlays(), (sizeof(SDK::CAnimationLayer) * 15));

		if (entity->GetAnimState())
			entity->GetAnimState()->m_iLastClientSideAnimationUpdateFramecount = INTERFACES::Globals->framecount - 1;

		entity->GetClientSideAnimation2() = true;
		entity->UpdateClientSideAnimation();
		entity->GetClientSideAnimation2() = false;

		std::memcpy(entity->GetAnimOverlays(), backup_layers, (sizeof(SDK::CAnimationLayer) * 15));

		INTERFACES::Globals->curtime = old_curtime;
		INTERFACES::Globals->frametime = old_frametime;


		entity->GetAnimState()->m_flUnknownFraction = old_fraction; //fix them legs

		entity->SetAngle2(Vector(0.f, entity->GetAnimState()->m_flGoalFeetYaw, 0.f));
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

						if (SETTINGS::settings.resolvedesync) {
							resolver->g_desync_correction(entity);
						}
					}
				} 
			    break;
			case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
				break;
			case FRAME_RENDER_START:
				if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
				{
					g_soundesp.draw();

					if (in_tp)
					{

						SDK::CBaseAnimState* animstate = local_player->GetAnimState();

						if (!animstate)
							return;


						if (animstate->m_bInHitGroundAnimation && ground_tick > 1)
							*reinterpret_cast<Vector*>(reinterpret_cast<DWORD>(local_player) + 0x31D8) = Vector(-10, GLOBAL::angles.y, 0.f);
						else
							*reinterpret_cast<Vector*>(reinterpret_cast<DWORD>(local_player) + 0x31D8) = Vector(GLOBAL::angles.x, GLOBAL::angles.y, 0.f);

						INTERFACES::pPrediction->SetLocalViewAngles(GLOBAL::real_angles);
						if (GetAsyncKeyState((SETTINGS::settings.fakewalkspeed))) {
							local_player->UpdateClientSideAnimation(); //update our client side animation
							local_player->GetAnimState()->m_flUnknownFraction = 0.f; //replace leg shuffling with leg sliding
						}
						else
							FixThemAnim(local_player); //do that premium anim fix m8


					}
					for (int i = 1; i <= 65; i++)
					{
						auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
						if (!entity) continue;
						if (entity == local_player) continue;
						if (entity)
						{
							if (entity->GetHealth() > 0 && !entity->GetIsDormant())
							{
								backtracking->UpdateBacktrackRecords(entity);
							}
						}

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
			/*if (SETTINGS::settings.asus_props)
			{
				INTERFACES::RenderView->SetBlend(1 - (SETTINGS::settings.asus_props_o / 100));
			}*/
			auto entity = INTERFACES::ClientEntityList->GetClientEntity(render_info.entity_index);
			if (entity == local_player)
			{
				if (local_player && local_player->GetIsScoped())
					INTERFACES::RenderView->SetBlend(SETTINGS::settings.transparency_when_scoped * 0.01f);
			}

			if (SETTINGS::settings.removeflash) {
				SDK::IMaterial* Flash = INTERFACES::MaterialSystem->FindMaterial("effects\\flashbang", "ClientEffect textures");
				SDK::IMaterial* FlashWhite = INTERFACES::MaterialSystem->FindMaterial("effects\\flashbang_white", "ClientEffect textures");
				Flash->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true);
				FlashWhite->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true);
				original_draw_model_execute(ecx, context, state, render_info, matrix);
			}

			if (SETTINGS::settings.fpsboost)
			{
				auto da = INTERFACES::cvar->FindVar("cl_interp");
				da->SetValue(0);
				auto da1 = INTERFACES::cvar->FindVar("cl_interp_ratio");
				da1->SetValue(1);
				auto da2 = INTERFACES::cvar->FindVar("cl_disable_ragdolls");
				da2->SetValue(1);
				auto da3 = INTERFACES::cvar->FindVar("dsp_slow_cpu");
				da3->SetValue(1);
				auto da5 = INTERFACES::cvar->FindVar("mat_disable_bloom");
				da5->SetValue(1);
				auto da6 = INTERFACES::cvar->FindVar("r_drawparticles");
				da6->SetValue(0);
				auto da7 = INTERFACES::cvar->FindVar("func_break_max_pieces");
				da7->SetValue(0);
				auto da8 = INTERFACES::cvar->FindVar("muzzleflash_light");
				da8->SetValue(0);
				auto da9 = INTERFACES::cvar->FindVar("r_eyemove");
				da9->SetValue(0);
				auto da10 = INTERFACES::cvar->FindVar("r_eyegloss");
				da10->SetValue(0);
				auto da11 = INTERFACES::cvar->FindVar("mat_queue_mode");
				da11->SetValue(2);
				auto xd80 = INTERFACES::cvar->FindVar("r_showenvcubemap");
				xd80->SetValue(0);


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
			CColor color = CColor(SETTINGS::settings.glow_col[0] * 255, SETTINGS::settings.glow_col[1] * 255, SETTINGS::settings.glow_col[2] * 255, SETTINGS::settings.glow_col[3] * 255), colorTeam = CColor(SETTINGS::settings.teamglow_color[0] * 255, SETTINGS::settings.teamglow_color[1] * 255, SETTINGS::settings.teamglow_color[2] * 255, SETTINGS::settings.teamglow_color[3] * 255), colorlocal = CColor(SETTINGS::settings.glowlocal_col[0] * 255, SETTINGS::settings.glowlocal_col[1] * 255, SETTINGS::settings.glowlocal_col[2] * 255, SETTINGS::settings.glowlocal_col[3] * 255), colorweapon = CColor(SETTINGS::settings.gloweapon[0] * 255, SETTINGS::settings.gloweapon[1] * 255, SETTINGS::settings.gloweapon[2] * 255, SETTINGS::settings.gloweapon[3] * 255);

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
							mat->ColorModulate(CColor(SETTINGS::settings.fakechamscol[0] * 255, SETTINGS::settings.fakechamscol[1] * 255, SETTINGS::settings.fakechamscol[2] * 255, SETTINGS::settings.fakechamscol[3] * 255));
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

						notignorezmaterial->ColorModulate(CColor(SETTINGS::settings.localchams_col[0] * 255, SETTINGS::settings.localchams_col[1] * 255, SETTINGS::settings.localchams_col[2] * 255, SETTINGS::settings.localchams_col[3] * 255));
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
					if (entity && SETTINGS::settings.teambehindwall)
					{
						ignorezmaterial->ColorModulate(CColor(SETTINGS::settings.teaminvis_color[0] * 255, SETTINGS::settings.teaminvis_color[1] * 255, SETTINGS::settings.teaminvis_color[2] * 255, SETTINGS::settings.teaminvis_color[3] * 255));
						INTERFACES::ModelRender->ForcedMaterialOverride(ignorezmaterial);
						entity->DrawModel(0x1, 255);

						notignorezmaterial->ColorModulate(CColor(SETTINGS::settings.teamvis_color[0] * 255, SETTINGS::settings.teamvis_color[1] * 255, SETTINGS::settings.teamvis_color[2] * 255, SETTINGS::settings.teamvis_color[3] * 255));
						INTERFACES::ModelRender->ForcedMaterialOverride(notignorezmaterial);
						entity->DrawModel(0x1, 255);

						INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
					}
					else if (entity && SETTINGS::settings.teammodel)
					{
						notignorezmaterial->ColorModulate(CColor(SETTINGS::settings.teamvis_color[0] * 255, SETTINGS::settings.teamvis_color[1] * 255, SETTINGS::settings.teamvis_color[2] * 255, SETTINGS::settings.teamvis_color[3] * 255));
						INTERFACES::ModelRender->ForcedMaterialOverride(notignorezmaterial);
						entity->DrawModel(0x1, 255);

						INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
					} continue;
				}
				else if (is_teammate && SETTINGS::settings.chamsteam)
					continue;

				if (entity && SETTINGS::settings.playerbehindwall)
				{
					ignorezmaterial->ColorModulate(CColor(SETTINGS::settings.imodel_col[0] * 255, SETTINGS::settings.imodel_col[1] * 255, SETTINGS::settings.imodel_col[2] * 255, SETTINGS::settings.imodel_col[3] * 255));
					INTERFACES::ModelRender->ForcedMaterialOverride(ignorezmaterial);
					entity->DrawModel(0x1, 255);

					notignorezmaterial->ColorModulate(CColor(SETTINGS::settings.vmodel_col[0] * 255, SETTINGS::settings.vmodel_col[1] * 255, SETTINGS::settings.vmodel_col[2] * 255, SETTINGS::settings.vmodel_col[3] * 255));
					INTERFACES::ModelRender->ForcedMaterialOverride(notignorezmaterial);
					entity->DrawModel(0x1, 255);

					INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
				}
				else if (entity && SETTINGS::settings.playerchams == 1)
				{
					notignorezmaterial->ColorModulate(CColor(SETTINGS::settings.vmodel_col[0] * 255, SETTINGS::settings.vmodel_col[1] * 255, SETTINGS::settings.vmodel_col[2] * 255, SETTINGS::settings.vmodel_col[3] * 255));
					INTERFACES::ModelRender->ForcedMaterialOverride(notignorezmaterial);
					entity->DrawModel(0x1, 255);

					INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
				}

				if (SETTINGS::settings.showhistorychams)
				{
					Vector orig = entity->GetAbsOrigin();

					if (entity && backtracking->PlayerRecord[i].records.size() > 0)
					{
						tick_record record = backtracking->PlayerRecord[i].records.at(0);

						if (backtracking->IsTickValid(record) && orig != record.m_vecAbsOrigin)
						{
							ignorezmaterial->ColorModulate(CColor(SETTINGS::settings.historychamscolor[0] * 255, SETTINGS::settings.historychamscolor[1] * 255, SETTINGS::settings.historychamscolor[2] * 255, SETTINGS::settings.historychamscolor[3] * 255));
							entity->SetAbsOrigin(record.m_vecAbsOrigin);

							INTERFACES::ModelRender->ForcedMaterialOverride(ignorezmaterial);
							entity->DrawModel(0x1, 255);

							INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);

							entity->SetAbsOrigin(orig);
						}
					}

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

				if (entity->GetClientClass()->m_ClassID == 1 || entity->GetClientClass()->m_ClassID == 66 || entity->GetClientClass()->m_ClassID == 87 || entity->GetClientClass()->m_ClassID == 98 || entity->GetClientClass()->m_ClassID == 108 || entity->GetClientClass()->m_ClassID == 134 || entity->GetClientClass()->m_ClassID == 1 || entity->GetClientClass()->m_ClassID == 9 || entity->GetClientClass()->m_ClassID == 29 || entity->GetClientClass()->m_ClassID == 39 || entity->GetClientClass()->m_ClassID == 41 || entity->GetClientClass()->m_ClassID == WEAPON_CPlantedC4 || entity->GetClientClass()->m_ClassID == WEAPON_C4 || entity->GetClientClass()->m_ClassID == WEAPON_MOLOTOV || entity->GetClientClass()->m_ClassID == WEAPON_INCGRENADE || entity->GetClientClass()->m_ClassID == WEAPON_HEGRENADE || entity->GetClientClass()->m_ClassID == WEAPON_SMOKEGRENADE || entity->GetClientClass()->m_ClassID == WEAPON_FLASHBANG || entity->GetClientClass()->m_ClassID == WEAPON_CC4 || entity->GetClientClass()->m_ClassID == CMolotovGrenade || entity->GetClientClass()->m_ClassID == CSmokeGrenade ||  entity->GetClientClass()->m_ClassID == WEAPON_DECOY || strstr(entity->GetClientClass()->m_pNetworkName, "Weapon"))
				{
					if (SETTINGS::settings.weapongglows) {
						//bool m_teammate = entity->GetTeam() == entity->GetTeam();
						glowObject.m_flRed = colorweapon.RGBA[0] / 255.0f;
						glowObject.m_flGreen = colorweapon.RGBA[1] / 255.0f;
						glowObject.m_flBlue = colorweapon.RGBA[2] / 255.0f;
						glowObject.m_flAlpha = colorweapon.RGBA[3] / 255.0f;
						glowObject.m_bRenderWhenOccluded = true;
						glowObject.m_bRenderWhenUnoccluded = false;
						//; glow weapons

					}
				}
				

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
					mat->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true); //wireframe
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
		if (GetAsyncKeyState(SETTINGS::settings.Fakecrouchkey))
		{
			pSetup->origin.z = local_player->GetAbsOrigin().z + 64.f;
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


			//INTERFACES::cvar->FindVar("viewmodel_fov")->SetValue(SETTINGS::settings.viewmodel);
		//	INTERFACES::cvar->FindVar("viewmodel_offset_x")->SetValue(SETTINGS::settings.viewmodel_x);
			//INTERFACES::cvar->FindVar("viewmodel_offset_y")->SetValue(SETTINGS::settings.viewmodel_y);
			//INTERFACES::cvar->FindVar("viewmodel_offset_z")->SetValue(SETTINGS::settings.viewmodel_z - 10);

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
				else if (local_player->GetIsScoped() && SETTINGS::settings.fixscopesens)
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


	auto Uninject = []() -> void
	{
		HMODULE h_module;
		if (GetModuleHandleEx(NULL, NULL, &h_module) != 0)
		{
			//INTERFACES::Engine->ClientCmd("cl_mouseenable 1");
			INTERFACES::InputSystem->EnableInput(true);
			HOOKS::UnHook();

			DllMain(h_module, DLL_PROCESS_DETACH, NULL);
		}
	};

	void GUI_Init(IDirect3DDevice9* pDevice)
	{
		ImGui_ImplDX9_Init(INIT::Window, pDevice);




		ImGuiIO& io = ImGui::GetIO();

		auto& style = ImGui::GetStyle();


		//style.Colors[ImGuiCol_Text] = ImColor(93, 89, 242, 255); // Text Color
		style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255); // Text Color
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
		smallmenu_font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(smalll_compressed_data_base85, 13);

		d3d_init = true;

	}
	void BtnMenuTab()
	{
		auto& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_Button] = ImColor(40, 42, 47, 255);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(40, 42, 47, 255);
		style.Colors[ImGuiCol_ButtonActive] = ImColor(40, 42, 47, 255);
		style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
		//style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);

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


	Vector2D RotatePoint(Vector EntityPos, Vector LocalPlayerPos, int posX, int posY, int sizeX, int sizeY, float angle, float zoom, bool* viewCheck, bool angleInRadians = false)
	{
		float r_1, r_2;
		float x_1, y_1;
		r_1 = -(EntityPos.y - LocalPlayerPos.y);
		r_2 = EntityPos.x - LocalPlayerPos.x;
		float Yaw = angle - 90.0f;
		float yawToRadian = Yaw * (float)(M_PI / 180.0F);
		x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20;
		y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20;
		*viewCheck = y_1 < 0;
		x_1 *= zoom;
		y_1 *= zoom;
		int sizX = sizeX / 2;
		int sizY = sizeY / 2;
		x_1 += sizX;
		y_1 += sizY;
		if (x_1 < 5)
			x_1 = 5;
		if (x_1 > sizeX - 5)
			x_1 = sizeX - 5;
		if (y_1 < 5)
			y_1 = 5;
		if (y_1 > sizeY - 5)
			y_1 = sizeY - 5;
		x_1 += posX;
		y_1 += posY;
		return Vector2D(x_1, y_1);
	}
	bool EntityIsInvalid(SDK::CBaseEntity* Entity)
	{
		if (!Entity)
			return true;
		if (Entity->GetHealth() <= 0)
			return true;
		if (Entity->GetIsDormant())
			return true;
		return false;
	}
	void DrawRadar()
	{
		if (SETTINGS::settings.Radargui.Enabled)
		{
			ImGuiStyle& style = ImGui::GetStyle();
			ImVec2 oldPadding = style.WindowPadding;
			float oldAlpha = style.Colors[ImGuiCol_WindowBg].w;
			style.WindowPadding = ImVec2(0, 0);
			style.Colors[ImGuiCol_WindowBg].w = 1.f;
			if (ImGui::Begin("Radar", &SETTINGS::settings.Radargui.Enabled, ImVec2(200, 200), 0.4F, ImGuiWindowFlags_NoTitleBar |/*ImGuiWindowFlags_NoResize | */ImGuiWindowFlags_NoCollapse))
			{
				ImVec2 siz = ImGui::GetWindowSize();
				ImVec2 pos = ImGui::GetWindowPos();
				ImDrawList* DrawList = ImGui::GetWindowDrawList();
				DrawList->AddRect(ImVec2(pos.x - 6, pos.y - 6), ImVec2(pos.x + siz.x + 6, pos.y + siz.y + 6), CColor::Black().GetD3DColor(), 0.0F, -1, 1.5f);
				ImDrawList* windowDrawList = ImGui::GetWindowDrawList();
				windowDrawList->AddLine(ImVec2(pos.x + (siz.x / 2), pos.y + 0), ImVec2(pos.x + (siz.x / 2), pos.y + siz.y), CColor::Black().GetD3DColor(), 1.5f);
				windowDrawList->AddLine(ImVec2(pos.x + 0, pos.y + (siz.y / 2)), ImVec2(pos.x + siz.x, pos.y + (siz.y / 2)), CColor::Black().GetD3DColor(), 1.5f);
				if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
				{
					auto LocalPlayer = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
					if (LocalPlayer)
					{
						Vector LocalPos = LocalPlayer->GetEyePosition();
						Vector ang;
						INTERFACES::Engine->GetViewAngles(ang);
						for (int i = 0; i < INTERFACES::Engine->GetMaxClients(); i++) {
							SDK::CBaseEntity* pBaseEntity = INTERFACES::ClientEntityList->GetClientEntity(i);
							SDK::player_info_t pInfo;
							INTERFACES::Engine->GetPlayerInfo(i, &pInfo);
							if (EntityIsInvalid(pBaseEntity))
								continue;
							SDK::CBaseEntity* observerTarget = INTERFACES::ClientEntityList->GetClientEntityFromHandle(LocalPlayer->GetObserverTargetHandle());
							bool bIsEnemy = (LocalPlayer->GetTeam() != pBaseEntity->GetTeam() || pBaseEntity == observerTarget || pBaseEntity == LocalPlayer) ? true : false;
							if (SETTINGS::settings.Radargui.OnlyEnemy && !bIsEnemy)
								continue;
							bool viewCheck = false;
							Vector2D EntityPos = RotatePoint(pBaseEntity->GetVecOrigin(), LocalPos, pos.x, pos.y, siz.x, siz.y, ang.y, SETTINGS::settings.Radargui.Range, &viewCheck);
							//ImU32 clr = (bIsEnemy ? (isVisibled ? Color::LightGreen() : Color::Blue()) : Color::White()).GetU32();
							ImU32 clr = (bIsEnemy ? CColor::Red() : CColor::LightBlue()).GetD3DColor();
							static bool drawname = true;
							if (pBaseEntity == observerTarget || pBaseEntity == LocalPlayer)
							{
								clr = CColor::White().GetD3DColor();
								drawname = false;
							}
							else
								drawname = true;
							int s = 2;
							windowDrawList->AddRect(ImVec2(EntityPos.x - s, EntityPos.y - s),
								ImVec2(EntityPos.x + s, EntityPos.y + s),
								clr);
							//		RECT TextSize = D::GetTextSize(F::ESP, pInfo.name);
							//	if (drawname && Vars.Misc.Radar.Nicks)
							//		windowDrawList->AddText(ImVec2(EntityPos.x - (TextSize.left / 2), EntityPos.y - s), CColor::White().GetU32(), pInfo.name);
						}
					}
				}
			}
			ImGui::End();
			style.WindowPadding = oldPadding;
			style.Colors[ImGuiCol_WindowBg].w = oldAlpha;
		}

	}

	std::vector<int> GetObservervators(int playerId)
	{
		std::vector<int> SpectatorList;

		SDK::CBaseEntity* pPlayer = (SDK::CBaseEntity*)INTERFACES::ClientEntityList->GetClientEntity(playerId);

		if (!pPlayer)
			return SpectatorList;

		if (!pPlayer->IsAlive())
		{
			SDK::CBaseEntity* pObserverTarget = (SDK::CBaseEntity*)INTERFACES::ClientEntityList->GetClientEntityFromHandle(pPlayer->GetObserverTargetHandle());

			if (!pObserverTarget)
				return SpectatorList;

			pPlayer = pObserverTarget;
		}

		for (int PlayerIndex = 0; PlayerIndex < 65; PlayerIndex++)
		{
			SDK::CBaseEntity* pCheckPlayer = (SDK::CBaseEntity*)INTERFACES::ClientEntityList->GetClientEntity(PlayerIndex);

			if (!pCheckPlayer)
				continue;

			if (pCheckPlayer->GetIsDormant() || pCheckPlayer->IsAlive())
				continue;

			SDK::CBaseEntity* pObserverTarget = (SDK::CBaseEntity*)INTERFACES::ClientEntityList->GetClientEntityFromHandle(pCheckPlayer->GetObserverTargetHandle
());

			if (!pObserverTarget)
				continue;

			if (pPlayer != pObserverTarget)
				continue;

			SpectatorList.push_back(PlayerIndex);
		}

		return SpectatorList;
	}

	void SpectatorList()
	{
		if (!SETTINGS::settings.spectators)
			return;

		if (!INTERFACES::Engine->IsInGame() && !INTERFACES::Engine->IsConnected())
			return;

		int specs = 0;
		int modes = 0;
		std::string spect = "";
		std::string mode = "";
		int DrawIndex = 1;

		for (int playerId : GetObservervators(INTERFACES::Engine->GetLocalPlayer()))
		{
			if (playerId == INTERFACES::Engine->GetLocalPlayer())
				continue;

			SDK::CBaseEntity* pPlayer = (SDK::CBaseEntity*)INTERFACES::ClientEntityList->GetClientEntity(playerId);

			if (!pPlayer)
				continue;

			SDK::player_info_t Pinfo;
			INTERFACES::Engine->GetPlayerInfo(playerId, &Pinfo);

			//if (Pinfo.fakeplayer)
			//	continue;

			spect += Pinfo.name;
			spect += "\n";
			specs++;

			if (spect != "")
			{
				CColor PlayerObsColor;
				switch (pPlayer->GetObserverMode())
				{
				case 4:
					mode += "Perspective";
					break;
				case 5:
					mode += "3rd Person";
					break;
				case 6:
					mode += "Free look";
					break;
				case 1:
					mode += "Deathcam";
					break;
				case 2:
					mode += "Freezecam";
					break;
				case 3:
					mode += "Fixed";
					break;
				default:
					break;
				}

				mode += "\n";
				modes++;
			}
		}
		bool Spectators = true;
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_WindowBg] = ImVec4(.137f, .137f, .137f, 1.00f);
		//ImGui::PushFont(Main2);
		if (ImGui::Begin("Spectator List", &Spectators, ImVec2(200, 250), 1.0F, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar))
		{

			if (specs > 0) spect += "\n";
			if (modes > 0) mode += "\n";
			ImVec2 size = ImGui::CalcTextSize(spect.c_str());

			ImGui::SetWindowSize(ImVec2(200, 40 + size.y));
			ImGui::Columns(2);

			ImGui::Text("Name");
			ImGui::NextColumn();

			ImGui::Text("Mode");
			ImGui::NextColumn();
			ImGui::Separator();

			ImGui::Text(spect.c_str());
			ImGui::NextColumn();

			ImGui::Text(mode.c_str());
			ImGui::Columns(1);
			DrawIndex++;

		}
		//ImGui::PopFont();
		ImGui::End();
	}


	void DrawRageTab()
	{
		ImGui::BeginChild("Aimbot Main", ImVec2(226, 495), true);
		{
			ImGui::Checkbox("Enable aim", &SETTINGS::settings.aim_bool);
			ImGui::Spacing();
			

			static string enemy_preview = "";
			static const char* Hitboxes[] =
			{
				"Head",
				"Chest",
				"Stomach",
				"Arms",
				"Legs",
			};
			ImGui::Text("Target hitbox");
			if (ImGui::BeginCombo("##multibox", enemy_preview.c_str()))
			{
				enemy_preview = "";
				vector<string> item;
				for (auto i = 0; i < ARRAYSIZE(Hitboxes); ++i) {
					ImGui::Selectable(Hitboxes[i], &SETTINGS::settings.targethitbox[i], ImGuiSelectableFlags_DontClosePopups);
					if (SETTINGS::settings.targethitbox[i])
						item.push_back(Hitboxes[i]);
				}

				for (auto i = 0; i < item.size(); ++i) {
					if (item.empty())
						enemy_preview += "";
					else if (item.size() == 1)
						enemy_preview += item[i];
					else if (!(i == item.size()))
						enemy_preview += item[i] + ", ";
					else
						enemy_preview += item[i];
				}

				ImGui::EndCombo();
			}
			ImGui::Spacing();
			ImGui::Checkbox("Avoid limbs if moving", &SETTINGS::settings.IgnoreLimbs);
			ImGui::Spacing();
			ImGui::Checkbox("Avoid head if jumbing", &SETTINGS::settings.ignoreheads);
			ImGui::Spacing();
			
			ImGui::Checkbox("Multi-point scale", &SETTINGS::settings.multi_bool);
			ImGui::Spacing();
			ImGui::Text("Head scale");
			ImGui::SliderFloat("##head", &SETTINGS::settings.point_val, 0, 1);
			ImGui::Spacing();
			ImGui::Text("body scale");
			ImGui::SliderFloat("##body", &SETTINGS::settings.body_val, 0, 1);
			ImGui::Spacing();
			ImGui::Checkbox("Auto scope", &SETTINGS::settings.automaticscope);
			ImGui::Spacing();
			ImGui::Checkbox("Silent aim", &SETTINGS::settings.silentaim);
			ImGui::Spacing();
			ImGui::Text("Minimum hit chance");
			ImGui::SliderFloat("##chance", &SETTINGS::settings.chance_val, 0, 100);
			ImGui::Spacing();
			ImGui::Text("Minimum Damage");
			ImGui::SliderFloat("##damage", &SETTINGS::settings.damage_val, 0, 100);
			ImGui::Spacing();
			ImGui::Checkbox("Override awp", &SETTINGS::settings.awpoverride);
			ImGui::Spacing();
			ImGui::Checkbox("Engine prediction", &SETTINGS::settings.vecvelocityprediction);
			ImGui::Spacing();
			ImGui::Checkbox("Rmove recoil", &SETTINGS::settings.removerecoil);
			ImGui::Spacing();
			ImGui::Checkbox(("Lag Prediction"), &SETTINGS::settings.fakelagpredicion);
			ImGui::Checkbox(("Auto zeus"), &SETTINGS::settings.autozeus_bool);
			ImGui::Spacing();
			ImGui::Checkbox(("Auto knife"), &SETTINGS::settings.autoknife_bool);

		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Aimbot Other", ImVec2(210, 495), true);
		{
			ImGui::Checkbox("Resolver", &SETTINGS::settings.resolvedesync);
			ImGui::Spacing();
			static const char* accuracyboost[] =
			{
				"Off",
				"Default",
				"Testing(!)"

			};
			ImGui::Text("Accuracy boost");
			ImGui::Combo(XorStr("##boost"), &SETTINGS::settings.Accuracyboost, accuracyboost, ARRAYSIZE(accuracyboost));
			ImGui::Spacing();

			static const char* autostops[] =
			{
				        "Off",
						"Quick",
						"Full",
						"Slide"

			};
			ImGui::Text("Auto stop mode");
			ImGui::Combo(XorStr("##autostop"), &SETTINGS::settings.autostopmethod, autostops, ARRAYSIZE(autostops));
			ImGui::Spacing();
			ImGui::Checkbox("Extrapolation", &SETTINGS::settings.extrapolation);
			ImGui::Spacing();

			static const char* bodyaim[] =
			{
				"Off",
				"Normal",
				"Adabtive"

			};
			ImGui::Text("Prefer body aim");
			ImGui::Combo(XorStr("##Bodyaim"), &SETTINGS::settings.bodyiaimtype, bodyaim, ARRAYSIZE(bodyaim));
			ImGui::Spacing();
			ImGui::Hotkey("Force baim key", &SETTINGS::settings.baimonkey, ImVec2(40, 25));
			ImGui::Spacing();
			ImGui::Checkbox("Baim if lethal", &SETTINGS::settings.baimiflethal);
			ImGui::Spacing();
			ImGui::Checkbox("Baim if slowwalking", &SETTINGS::settings.baimifslow);
			ImGui::Spacing();
			ImGui::Checkbox("Baim if moving", &SETTINGS::settings.baimifmove);

		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Anti-Aim", ImVec2(230, 495), true);
		{
			ImGui::Checkbox("Anti-Aim", &SETTINGS::settings.aa_bool);
			static const char* aa_pitch[] =
			{
				"off",
				"Down",
				"Fake down",
				"fake up",
				"Minimal",
				"Random",
				"Costume",

			};
			ImGui::Spacing();
			ImGui::Text("Anti-aim pitch");
			ImGui::Combo(XorStr("##r pitch"), &SETTINGS::settings.standpitch, aa_pitch, ARRAYSIZE(aa_pitch));
			if (SETTINGS::settings.standpitch == 6)
			{
				ImGui::Text("Costume pitch");
				ImGui::SliderFloat("##pustom pitch", &SETTINGS::settings.custompitch, -180, 180);
			}
			ImGui::Spacing();
			static const char* yawbase[] =
			{
				"Off",
				"Local view",
				"Static",
				"At target",
			};
			ImGui::Text("Yaw base");
			ImGui::Combo(XorStr("##r yaw base"), &SETTINGS::settings.yawbaseangle, yawbase, ARRAYSIZE(yawbase));
			ImGui::Spacing();
			ImGui::Text("Yaw add");
			ImGui::SliderFloat("##add", &SETTINGS::settings.yawadd, -180, 180);
			ImGui::Spacing();
			static const char* yawangle[] =
			{
				"off",
				"180z",
				"Jitter",
				"180 Jitter",
				"Rotate",

			};
			ImGui::Text("Yaw angle");
			ImGui::Combo(XorStr("##r yaw angle"), &SETTINGS::settings.yawangle, yawangle, ARRAYSIZE(yawangle));
			if (SETTINGS::settings.yawangle == 2)
			{
				ImGui::Text("Jitter range");
				ImGui::SliderFloat("##rangee", &SETTINGS::settings.jitterrange, -260, 260);
			}
			else if (SETTINGS::settings.yawangle == 4)
			{
				ImGui::Text("Rotaion speed ");
				ImGui::SliderFloat("##rotation", &SETTINGS::settings.antiaim.spinspeed, -60, 60);
			}
			ImGui::Spacing();
			static const char* freestand[] =
			{
				"off",
				"Freestanding default",
				"freestand jitter",
				"Manual",
			};
			ImGui::Checkbox("Freestand", &SETTINGS::settings.enablefreestanding);
			if (SETTINGS::settings.enablefreestanding)
			{
				ImGui::Text("Freestanding type");
				ImGui::Combo(XorStr("##r yaw freestand"), &SETTINGS::settings.freestand_type, freestand, ARRAYSIZE(freestand));
			    if (SETTINGS::settings.freestand_type == 2)	
				{
					ImGui::Text("Freestanding jitter range");
					ImGui::SliderFloat("##freerange", &SETTINGS::settings.frestandingjitter, -160, 160);
				}
				else if (SETTINGS::settings.freestand_type == 3)
				{
					ImGui::Hotkey("manual right", &SETTINGS::settings.manualright, ImVec2(40, 25));
					ImGui::Hotkey("manual left", &SETTINGS::settings.manualleft, ImVec2(40, 25));
					//ImGui::Hotkey("manual back", &SETTINGS::settings.manualback, ImVec2(40, 25));
				}
			}
			ImGui::Spacing();
			ImGui::Text("Fake add");
			ImGui::SliderFloat("##fakeadd", &SETTINGS::settings.desynoth, -58, 58);
			ImGui::Spacing();

			 ImGui::Checkbox("Fake lag", &SETTINGS::settings.lag_bool);
			 ImGui::Spacing();
			 ImGui::Text("Lag amount");
			 ImGui::SliderInt("##amounation", &SETTINGS::settings.Fakelagamount, 0, 14);
			 ImGui::Spacing();
			 ImGui::Checkbox("Adabtive lag", &SETTINGS::settings.addabtivelag);
			 ImGui::Spacing();
			 ImGui::Checkbox("Break LC", &SETTINGS::settings.breaklagcomp);
			 ImGui::Spacing();
			 ImGui::Checkbox("Infinity duck", &SETTINGS::settings.infinityduck);
			 ImGui::Spacing();
			 ImGui::Checkbox("Fake crouch", &SETTINGS::settings.fakecrouch);
			 ImGui::Hotkey("Fake crouch key", &SETTINGS::settings.Fakecrouchkey, ImVec2(40, 25));
			 ImGui::Spacing();
			 ImGui::Checkbox("Slow walk", &SETTINGS::settings.Slowwalkenable);
			 ImGui::Spacing();
			 ImGui::Hotkey("Slow walk key", &SETTINGS::settings.Slowwalkkey, ImVec2(40, 25));
			 ImGui::Spacing();
			 ImGui::Text("Slow Walk Speed");
			 ImGui::SliderFloat("###Slowwalkspeed", &SETTINGS::settings.Slowwalkspeed, 0, 100);
			 ImGui::Spacing();
			 ImGui::Checkbox("0 Pitch on land", &SETTINGS::settings.Changepitchonland);

}
		ImGui::EndChild();

	}

	int vSubTab = 0;
	int cSubTab = 0;
	int cSubTab2 = 0;
	int cSubTab3 = 0;
	
	const char* key_binds[] = { "none", "mouse1", "mouse2", "mouse3", "mouse4", "mouse5", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12" };

	void DrawVisualsTab()
	{

		ImGui::BeginChild("Player Esp", ImVec2(228, 495), true);
		{
			ImGui::Checkbox("Enable esp", &SETTINGS::settings.esp_bool);
			ImGui::Spacing();
			ImGui::Checkbox("Teammates", &SETTINGS::settings.EspTeam);
			ImGui::Spacing();
			ImGui::Checkbox("Bounding box", &SETTINGS::settings.box_bool);
			ImGui::SameLine();
			ImGui::ColorEdit4(("###boxcolor"), SETTINGS::settings.box_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			ImGui::Checkbox("Health bar", &SETTINGS::settings.health_bool);
			ImGui::Spacing();
			ImGui::Checkbox("Name", &SETTINGS::settings.name_bool);
			ImGui::SameLine();
			ImGui::ColorEdit4(("###namecolor"), SETTINGS::settings.name_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			const char* weapontype[]{
	"Off",
	"Text",
	"Icone",
			};
			ImGui::Checkbox("Flags", &SETTINGS::settings.info_bool);
			ImGui::Spacing();
			ImGui::Text("Weapon"),
			ImGui::Combo("###Weapon", &SETTINGS::settings.weaponstype, weapontype, ARRAYSIZE(weapontype));
			ImGui::SameLine();
			ImGui::ColorEdit4(("###weaponcolor"), SETTINGS::settings.weapon_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			ImGui::Checkbox("Ammo", &SETTINGS::settings.ammo_bool);
			ImGui::Spacing();
			ImGui::Checkbox("Money", &SETTINGS::settings.money_bool);
			ImGui::Spacing();
			ImGui::Checkbox("Distance", &SETTINGS::settings.drawdistance);
			ImGui::Spacing();
			ImGui::Checkbox("Glow", &SETTINGS::settings.glowenable);
			ImGui::SameLine();
			ImGui::ColorEdit4(("###enemyglow"), SETTINGS::settings.glow_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			ImGui::Checkbox("Hit marker", &SETTINGS::settings.hitmarker_screen);
			ImGui::SameLine();
			ImGui::Checkbox("DreamHack markers", &SETTINGS::settings.dreamhackmarkers);
			ImGui::Spacing();
			ImGui::Checkbox("Footsteps", &SETTINGS::settings.vis_sound);
			ImGui::SameLine();
			ImGui::ColorEdit4(("###footsteps"), SETTINGS::settings.vis_soundcolor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			ImGui::Checkbox("Line of sight", &SETTINGS::settings.snaplines);
			ImGui::SameLine();
			ImGui::ColorEdit4(("###sightline"), SETTINGS::settings.snaplines_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();

			//ImGui::Checkbox("Skeleton", &SETTINGS::settings.bones);
			//ImGui::SameLine();
			//ImGui::ColorEdit4(("###colorskeleton"), SETTINGS::settings.skeletoncolor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			
			ImGui::Checkbox("Lag hitbox", &SETTINGS::settings.lagcompensatedhitboxes_bool);
			ImGui::SliderFloat("Duration", &SETTINGS::settings.lag_amount, 0, 10);
			ImGui::Spacing();

			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Text("Colored models");
			ImGui::Spacing();
			const char* chamstype[] =
			{
				"Default",
				"Flat"
			};
			ImGui::Text("Type");
			ImGui::Combo(("###Type"), &SETTINGS::settings.chamstype, chamstype, ARRAYSIZE(chamstype));
			ImGui::Spacing();
			ImGui::Checkbox("Player", &SETTINGS::settings.playerchams);
			ImGui::SameLine();
			ImGui::ColorEdit4(("###playerchams"), SETTINGS::settings.vmodel_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			ImGui::Checkbox("Player (behind wall)", &SETTINGS::settings.playerbehindwall);
			ImGui::SameLine();
			ImGui::ColorEdit4(("###playerbehindwall"), SETTINGS::settings.imodel_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			ImGui::Checkbox("History chams", &SETTINGS::settings.showhistorychams);
			ImGui::SameLine();
			ImGui::ColorEdit4(("###historyColor"), SETTINGS::settings.historychamscolor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			

		}
		ImGui::EndChild();
		ImGui::SameLine();
		
		ImGui::BeginChild("Local Esp", ImVec2(226, 495), true);
		{
			ImGui::Checkbox("Local model", &SETTINGS::settings.localchams);
			ImGui::SameLine();
			ImGui::ColorEdit4(("###modelcolor"), SETTINGS::settings.localchams_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			//ImGui::Checkbox(("Desync shadow"), &SETTINGS::settings.fakechams);
			//ImGui::Spacing();
			ImGui::Checkbox("Teammates model", &SETTINGS::settings.teammodel);
			ImGui::SameLine();
			ImGui::ColorEdit4(("###teammodelcolor"), SETTINGS::settings.teamvis_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			ImGui::Checkbox("Teammates behind wall", &SETTINGS::settings.teambehindwall);
			ImGui::SameLine();
			ImGui::ColorEdit4(("###behindwallmodelcolor"), SETTINGS::settings.teaminvis_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			ImGui::Checkbox("Teammates glow", &SETTINGS::settings.glowteam);
			ImGui::SameLine();
			ImGui::ColorEdit4(("###Teammatesgcolor"), SETTINGS::settings.teamglow_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			ImGui::Checkbox("Dropped weapons", &SETTINGS::settings.weapongglows);
			ImGui::SameLine();
			ImGui::ColorEdit4(("###glow items"), SETTINGS::settings.gloweapon, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			ImGui::Checkbox("Grenades", &SETTINGS::settings.grennades);
			ImGui::SameLine();
			ImGui::ColorEdit4(("###nades items"), SETTINGS::settings.grenadepredline_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			ImGui::Checkbox("Radar", &SETTINGS::settings.fov_bool);
			ImGui::SameLine();
			ImGui::ColorEdit4(("###radarcol"), SETTINGS::settings.fov_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			ImGui::Checkbox("Enable Radar", &SETTINGS::settings.Radargui.Enabled);
			ImGui::Text("Range");
			ImGui::SliderFloat("##Range", &SETTINGS::settings.Radargui.Range, 1, 5);
			ImGui::Checkbox("Only Enemy", &SETTINGS::settings.Radargui.OnlyEnemy);
			ImGui::Spacing();
			ImGui::Checkbox("Recoil Indicator", &SETTINGS::settings.recoilindic);
			ImGui::SameLine();
			ImGui::ColorEdit4(("###recoil indicator"), SETTINGS::settings.recoilindicolor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			ImGui::Checkbox("Spectators", &SETTINGS::settings.spectators);
			ImGui::Spacing();
			ImGui::Checkbox("Remove flashbang effects", &SETTINGS::settings.removeflash);
			ImGui::Spacing();
			ImGui::Checkbox("Remove smoke grenades", &SETTINGS::settings.smoke_bool);
			ImGui::Spacing();
			ImGui::Checkbox("Visual recoil adjusment", &SETTINGS::settings.norecoil);
			ImGui::Spacing();
			ImGui::Text("Transparent walls");
			ImGui::SliderFloat("##Transparent World", &SETTINGS::settings.asus_props_o, 0, 100);
			ImGui::Spacing();
			ImGui::Spacing();
			const char* brithnaess[] =
			{
				"Default",
				"Night mode",
				"Full bright"
			};
			ImGui::Text("Brightness adjusment");
			ImGui::Combo(("###Brightness"), &SETTINGS::settings.night_bool, brithnaess, ARRAYSIZE(brithnaess));
			ImGui::Spacing();
			ImGui::Checkbox("Remove scope", &SETTINGS::settings.fixscopesens);
			ImGui::Spacing();
			ImGui::Checkbox("Remove sky", &SETTINGS::settings.removesky);
			ImGui::Spacing();
			ImGui::Checkbox("Disable post processing", &SETTINGS::settings.matpostprocessenable);
			ImGui::Checkbox(("Force thirdperson"), &SETTINGS::settings.tp_bool);
			ImGui::Spacing();
			ImGui::Text("Key");
			ImGui::Combo(("##key"), &SETTINGS::settings.thirdperson_int, key_binds, ARRAYSIZE(key_binds));
			ImGui::Spacing();
			ImGui::Text("Scope blend");
			ImGui::SliderFloat("##scope blend", &SETTINGS::settings.transparency_when_scoped, 0, 50);
			

		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Other Esp", ImVec2(200, 280), true);
		{
			ImGui::Checkbox("Bullets beams", &SETTINGS::settings.beam_bool);
			ImGui::Spacing();
			ImGui::Text("Entity");
			ImGui::SameLine();
			ImGui::ColorEdit4(("###entitycolor"), SETTINGS::settings.bulletenemy_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			ImGui::Text("Teammates");
			ImGui::SameLine();
			ImGui::ColorEdit4(("###teamscolor"), SETTINGS::settings.bulletteam_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			ImGui::Checkbox("Bullets impacts", &SETTINGS::settings.impacts);
			ImGui::Spacing();
			ImGui::Checkbox("Damages", &SETTINGS::settings.dmg_bool);
			ImGui::SameLine();
			ImGui::ColorEdit4(("###damages"), SETTINGS::settings.dmg_ccolor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			ImGui::Text("View model fov");
			ImGui::SliderInt("###Fov weapon", &SETTINGS::settings.viewfov_val, 0, 150);
			ImGui::Text("Fov X");
			ImGui::SliderInt("###Fov x", &SETTINGS::settings.viewmodel_x, 0, 10);
			ImGui::Text("Fov Y");
			ImGui::SliderInt("###Fov y", &SETTINGS::settings.viewmodel_y, 0, 10);
			ImGui::Text("Fov Z");
			ImGui::SliderInt("###Fov z", &SETTINGS::settings.viewmodel_z, 0, 20);
			ImGui::Spacing();
			ImGui::Checkbox("Hit sound", &SETTINGS::settings.hitmarker_val);
			ImGui::Spacing();
			ImGui::Checkbox("Remove sleeves", &SETTINGS::settings.misc_no_sleeves);
			ImGui::Spacing();
			ImGui::Checkbox("Penetration crosshair", &SETTINGS::settings.xhair_bool);
		}
		ImGui::EndChild();

		
	}
	/*void DrawColorsTab()
	{
		//ImGui::PushFont(Default);
		ImGui::SetNextWindowSize(ImVec2(300, 300));
		if (ImGui::Begin("colors", &ColorsTab, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize));
		{
			ImGui::BeginChild("colors", ImVec2(280, 270), true);
			{
				
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}*/
	

	

	void DrawMiscTab()
	{
		ImGui::BeginChild("misc", ImVec2(250, 510), true);
		{
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Text("Override FOV");
			ImGui::SliderFloat("##worldview", &SETTINGS::settings.fov_val, 0, 180);
			ImGui::Spacing();

			const char* iAutostrafer[] =
			{
				"Silent",
				"Normal",
				"Sideways",
				"On Move"
			};
			ImGui::Checkbox("Auto bhop", &SETTINGS::settings.AutoJump);
			ImGui::Spacing();
			ImGui::Checkbox("Z-hop", &SETTINGS::settings.zbhob);
			ImGui::Spacing();
			ImGui::Checkbox("Auto strafer", &SETTINGS::settings.AutoStrafe);
			ImGui::Spacing();
			ImGui::Checkbox("(WASD) keys", &SETTINGS::settings.wasd);
			ImGui::Spacing();
			ImGui::Text("Edge Jump");
			ImGui::Combo("###Edge Jump", &SETTINGS::settings.edgejumpkey, KeyStrings, ARRAYSIZE(KeyStrings));
			ImGui::Spacing();
			if (SETTINGS::settings.edgejumpkey > 0)
			{
				ImGui::Checkbox("strafe in air", &SETTINGS::settings.strafeedgejump);
			}
			ImGui::Spacing();
			ImGui::Checkbox(("anti-afk"), &SETTINGS::settings.antiafk);
			ImGui::Spacing();
			ImGui::Checkbox(("Fps boost"), &SETTINGS::settings.fpsboost);
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Checkbox("Blockbot", &SETTINGS::settings.bBlockBot);
			ImGui::Spacing();
			ImGui::Hotkey("##blockbotkey", &SETTINGS::settings.iBlockBotKey, ImVec2(100, 20));
			const char* iAirstuck[] =
			{
				"Off",
				"Toggle",
				"Hold"
			};
			ImGui::Text("Airstuck");
			ImGui::Combo("##airstuck", &SETTINGS::settings.iAirStuck, iAirstuck, ARRAYSIZE(iAirstuck));
			ImGui::Spacing();
			ImGui::Hotkey("##key", &SETTINGS::settings.iAirStuckKey, ImVec2(100, 20));
			ImGui::Spacing();
			ImGui::Checkbox("Log weapon purchases", &SETTINGS::settings.logweaponpurchase);
			ImGui::Spacing();
			ImGui::Checkbox("Log damage dealt", &SETTINGS::settings.Logdamage);
			ImGui::Spacing();
			ImGui::Checkbox("Clanttag spammer", &SETTINGS::settings.misc_clantag);
			{
				const char* clantag[] = { "Off", "DreamHack", "Costume"};
				ImGui::Combo("Type", &SETTINGS::settings.clantags, clantag, ARRAYSIZE(clantag));
				auto applyxd = [](const char * name) -> void {
					using Fn = int(__fastcall *)(const char *, const char *);
					static auto fn = reinterpret_cast<Fn>(UTILS::FindSignature("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));
					fn(name, name);
				};
				if (SETTINGS::settings.clantags == 2)
				{
					static char buffer[16];
					ImGui::InputText("custom", buffer, IM_ARRAYSIZE(buffer));
					if (ImGui::Button("Set clantag")) {
						applyxd(buffer);

					}
				}
			}
			

		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::Spacing();
		ImGui::SameLine();
		ImGui::BeginChild("settings", ImVec2(230, 400), true);
		{
			ImGui::Checkbox("Anti-untrusted", &SETTINGS::settings.Antiuntrusted);
			ImGui::Spacing();
			ImGui::Text("Menu color");
			ImGui::SameLine();
			ImGui::ColorEdit4(("##Menu color"), SETTINGS::settings.Checkmark, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::Spacing();
			ImGui::Checkbox("Name spam", &SETTINGS::settings.namespam);
			ImGui::Spacing();
			ImGui::Checkbox("Chat spam", &SETTINGS::settings.chatspam);
			ImGui::Spacing();
			ImGui::PushFont(smallmenu_font);
			{
				ImGui::InputText("##CFG", ConfigNamexd, 70);
				static int sel;
				std::string config;
				std::vector<std::string> configs = SETTINGS::settings.GetConfigs();
				if (configs.size() > 0) {
					ImGui::ComboBoxArrayxd("Configs", &sel, configs);
					ImGui::Spacing();
					//ImGui::Separator();
					ImGui::Spacing();
					ImGui::PushItemWidth(220.f);
					config = configs[SETTINGS::settings.config_sel];
				}
				SETTINGS::settings.config_sel = sel;
				ImGui::Spacing();

				if (configs.size() > 0) {
					if (ImGui::Button("Load", ImVec2(130, 40)))
					{

						SETTINGS::settings.Load(config);
						GLOBAL::Msg("[Config] Configuration loaded.    \n");
					}
				}
				ImGui::Spacing();

				if (configs.size() >= 1) {
					if (ImGui::Button("Save", ImVec2(130, 40)))
					{
						SETTINGS::settings.Save(config);
						GLOBAL::Msg("[Config] Configuration saved.    \n");
					}
				}
				ImGui::Spacing();

				if (ImGui::Button("Create", ImVec2(130, 40)))
				{
					std::string ConfigFileName = ConfigNamexd;
					if (ConfigFileName.size() < 1)
					{
						ConfigFileName = "settings";
					}
					SETTINGS::settings.CreateConfig(ConfigFileName);
					GLOBAL::Msg("[Config] Configuration created.    \n");
				}
				ImGui::Spacing();

				if (config.size() >= 1) {
					if (ImGui::Button("Delete", ImVec2(130, 40)))
					{
						SETTINGS::settings.Remove(config);
						GLOBAL::Msg("[Config] Configuration removed.    \n");
					}
				}
				ImGui::Spacing();

				if (ImGui::Button("Shutdown", ImVec2(130, 40)))
				{
					Uninject();
				}
				

			}
			ImGui::PopFont();
		}
		ImGui::EndChild();
		
	}

	
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

		
              if (ImGui::Checkbox("skinchanger", &SETTINGS::settings.skinenabled)) KnifeApplyCallbk();
			  ImGui::Spacing();
			  ImGui::Checkbox("glovechanger", &SETTINGS::settings.glovesenabled);
			   ImGui::Spacing();
			   ImGui::Text("Knife wear");
			   ImGui::SliderFloat("##wear", &SETTINGS::settings.Knifwear, 0, 1000);
			   ImGui::Spacing();
			   if (ImGui::Button("Force Update##egfa", ImVec2(100, 25)))
			 //  if (ImGui::Button("Force Update", ImVec2(-1, 25)))
			   {
	              KnifeApplyCallbk();
               }
			   ImGui::Spacing();

			   static bool Main = true;
			   static bool Colors = false;

			   static int Page = 0;


			   /*const char* meme = "page : error";
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
			   };*/

			   //ImGui::Text(("        "));
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
				   ImGui::Text("Knife Model");
				   ImGui::Combo(("###Knife Model"), &SETTINGS::settings.Knife, KnifeModel, ARRAYSIZE(KnifeModel));
				   ImGui::PushItemWidth(150.0f);
				   ImGui::Text("Knife Skin");
				   ImGui::Combo(("###Knife Skin"), &SETTINGS::settings.KnifeSkin, knifeskins, ARRAYSIZE(knifeskins));
				   ImGui::PushItemWidth(150.0f);
				   ImGui::Text("Glove Model");
				   ImGui::Combo(("###glove model"), &SETTINGS::settings.gloves, GloveModel, ARRAYSIZE(GloveModel));
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
			ImGui::Spacing();
			{
				if (ImGui::Button("Add##egfa", ImVec2(60, 20)))
				//if (ImGui::Button(XorStr("Add")))
				{
					SETTINGS::settings.Inventarchanger.g_skins.insert(SETTINGS::settings.Inventarchanger.g_skins.end(), { itemDefinitionIndex , paintKit });
				} ImGui::SameLine();
				if (ImGui::Button("Apply##Skin", ImVec2(60, 20)))
				//if (ImGui::Button(XorStr("Apply##Skin"))) 
				{

					write.SendClientHello();
					write.SendMatchmakingClient2GCHello();

				}

			}
			ImGui::Spacing();
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
			ImGui::Spacing();
			if (ImGui::Button("Update##egfa", ImVec2(90, 25)))
			//if (ImGui::Button(("Update##egfa")))
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
		if (INTERFACES::Engine->IsConnected()) DrawRadar();

		

		//if (SETTINGS::settings.opencfg)
		//	ConfigTab();

		static int tab;

		SpectatorList();

		if (menu_open)
		{

			ImVec4* colors = ImGui::GetStyle().Colors;
			float r = SETTINGS::settings.Checkmark[0], g = SETTINGS::settings.Checkmark[1], b = SETTINGS::settings.Checkmark[2], a = SETTINGS::settings.Checkmark[3];
			colors[ImGuiCol_CheckMark] = ImVec4(r, g, b, a);

			colors[ImGuiCol_SliderGrab] = ImVec4(r, g, b, a);

			colors[ImGuiCol_SliderGrabActive] = ImVec4(r, g, b, a);

			colors[ImGuiCol_ComboBg] = ImVec4(r, g, b, a);
			colors[ImGuiCol_ButtonHovered] = ImVec4(r, g, b, a);
			colors[ImGuiCol_ButtonActive] = ImVec4(r, g, b, a);
			colors[ImGuiCol_Button] = ImVec4(r, g, b, a);
			colors[ImGuiCol_MenuBarBg] = ImVec4(r, g, b, a);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(r, g, b, a);
			colors[ImGuiCol_Border] = ImVec4(r, g, b, a);
			colors[ImGuiCol_TitleBgActive] = ImVec4(r, g, b, a);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(r, g, b, a);
			colors[ImGuiCol_FrameBgActive] = ImVec4(r, g, b, a);
			colors[ImGuiCol_Header] = ImVec4(r, g, b, a);
			colors[ImGuiCol_HeaderHovered] = ImVec4(r, g, b, a);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(r, g, b, a);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(r, g, b, a);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(r, g, b, a);
			colors[ImGuiCol_HeaderActive] = ImVec4(r, g, b, a);

			ImVec2 mainWindowPos;
			ImGui::PushFont(Tabs);
			ImGui::SetNextWindowSize(ImVec2(700, 600));
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

	void UnHook()
	{
		iclient_hook_manager.Restore();
		panel_hook_manager.Restore();
		model_render_hook_manager.Restore();
		render_view_hook_manager.Restore();
		trace_hook_manager.Restore();
		client_mode_hook_manager.Restore();
		setup_bones_hook_manager.Restore();
		net_channel_hook_manager.Restore();

		CGameEvents::RemoveEventListeners();


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

		setviewmovelscallback();

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