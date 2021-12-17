#include "hooks.hpp"
#include <intrin.h>

#pragma intrinsic(_ReturnAddress)

void Hooks::Initialize()
{
	ConVar* sv_cheats_con = g_CVar->FindVar("sv_cheats");

	hlclient_hook.setup(g_CHLClient);
	direct3d_hook.setup(g_D3DDevice9);
	vguipanel_hook.setup(g_VGuiPanel);
	sound_hook.setup(g_EngineSound);
	stdrender_hook.setup(g_StudioRender);
	viewrender_hook.setup(g_ViewRender);
	clientmode_hook.setup(g_ClientMode);
	clientstate_hook.setup((void*)((uintptr_t)g_ClientState + 0x8));
	partition_hook.setup(g_SpatialPartition);
	bspquery_hook.setup(g_EngineClient->GetBSPTreeQuery());
	sv_cheats.setup(sv_cheats_con);
	steamapi_hook.setup(g_SteamGameCoordinator);

	direct3d_hook.hook_index(index::EndScene, EndScene);
	direct3d_hook.hook_index(index::Reset, Reset);

	vguipanel_hook.hook_index(index::PaintTraverse, PaintTraverse);

	sound_hook.hook_index(index::EmitSound, EmitSound);

	stdrender_hook.hook_index(index::DrawModelExecute, DrawModelExecute);

	viewrender_hook.hook_index(index::RenderSmokeOverlay, RenderSmokeOverlay);
	hlclient_hook.hook_index(index::FrameStageNotify, FrameStageNotify);
	clientmode_hook.hook_index(index::CreateMove, CreateMove);
	clientmode_hook.hook_index(index::DoPostScreenSpaceEffects, DoPostScreenEffects);
	clientmode_hook.hook_index(index::OverrideView, OverrideView);
	clientmode_hook.hook_index(index::GetViewModelFov, GetViewModelFov);
	clientmode_hook.hook_index(index::ShouldDrawFog, ShouldDrawFog);

	clientstate_hook.hook_index(index::ReadPacketEntities, ReadPacketEntities);

	partition_hook.hook_index(index::SuppressLists, SuppressLists);

	bspquery_hook.hook_index(index::ListLeavesInBox, ListLeavesInBox);

	sv_cheats.hook_index(index::GetBool, SvCheatsGetBool);

	steamapi_hook.hook_index(index::SendMsg, GCSendMessage);
	steamapi_hook.hook_index(index::RetrieveMsg, GCRetrieveMsg);

	sequence_hook = new recv_prop_hook(C_BaseViewModel::m_nSequence(), RecvProxy);

	listener = new CGameEventListener();
	g_GameEvents->AddListener(listener, "item_purchase", false);
}

void Hooks::Shutdown()
{
	direct3d_hook.unhook_all();
	vguipanel_hook.unhook_all();
	stdrender_hook.unhook_all();
	viewrender_hook.unhook_all();
	hlclient_hook.unhook_all();
	clientmode_hook.unhook_all();
	clientstate_hook.unhook_all();
	sound_hook.unhook_all();
	bspquery_hook.unhook_all();
	sv_cheats.unhook_all();
	steamapi_hook.unhook_all();

	g_GameEvents->RemoveListener(listener);

	g_Glow->Shutdown();
	sequence_hook->~recv_prop_hook();
}